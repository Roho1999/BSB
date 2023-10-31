/*****************************************************************************/
/* Operating System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                             E F I _ M A I N                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* This file implements a minimal UEFI bootlader for OO-StuBS. The Makefile  */
/* has a special handling for all files in the bootloader directory.         */
/* The .text and .data sections of the OO-StuBS kernel are linked as a       */
/* binary blob to the bootloader. Hence, no explicit loading is necessary.   */
/* The loader initialized the 'Graphics Output Protocol' in the default mode */
/* and allocates sufficient memory for the kernel (including .bss) and a new */
/* pagetable. The pagetable is necessary to map the kernel and the GOP-      */
/* framebuffer to the fixed virtual addresses 0xffff800000000000 and         */
/* 0xffff800040000000, respectively. This allows for linking the kernel      */
/* statically and, thus, easy debugging in QEMU. Mapping the framebuffer is  */
/* necessary to enable 'WRITE COMBINING' on the memory region. Otherwise,    */
/* performance would be unacceptable on real hardware. For this purpose, the */
/* PAT-bits in the page table entries have to be changed and the PAT itself, */
/* too. The implementation is based on the GNU-EFI library to interface with */
/* UEFI functions.                                                           */
/*****************************************************************************/

#include <efi.h>
#include <efilib.h>

#include "bootloader_info.h"

struct BootloaderInfo bootloader_info;

typedef UINT64 *PageTable; /* an array of 0x200 64-bit entries */
#define PTBL_ENTRIES 0x200
typedef uint8_t Page[4096];

// Symbols provided by the linker:
extern UINT8 _binary_system_bin_start[];      // physical text segment start address
extern char _binary_system_bin_size;       // text + alignment + data
const UINT64 kernel_size = (UINT64)&_binary_system_bin_size;
extern char __KERNEL_SIZE__[];             // text + alignment + data + alignment + bss
const UINT64 kernel_total_size = (UINT64)__KERNEL_SIZE__;

#define PTBL_BIT_PRESENT  (1 << 0)
#define PTBL_BIT_WRITABLE (1 << 1)
#define PTBL_BIT_USER     (1 << 2)
#define PTBL_BIT_ACCESSED (1 << 5)
#define PTBL_BIT_DIRTY    (1 << 6)
#define PTBL_BIT_HUGE     (1 << 7)
#define PAGE_XD_NX        (1 << 63)

#define PTBL_ADDR_MASK 0x000ffffffffff000  // 52 bit address mask

static void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
 
static void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi) {
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

static UINT64 cpuGetCR3() {
  UINT64 cr3;
  __asm__ __volatile__ (
      "mov %%cr3, %%rax\n\t"
      "mov %%rax, %0\n\t"
  : "=m" (cr3)
  : /* no input */
  : "%rax"
  );
  return cr3;
}

static void cpuSetCR3(UINT64 cr3) {
  __asm__ __volatile__(
      "movq %0, %%cr3;"::"r"((UINT64)cr3));
}

VOID efi_pause () {
	EFI_INPUT_KEY Key;

	WaitForSingleEvent (ST->ConIn->WaitForKey, 0);
	uefi_call_wrapper((void*)ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &Key);
}

BOOLEAN pt_lookup(UINT64 *pml4, EFI_PHYSICAL_ADDRESS virt, UINT64 *entry, UINT64 *page_size) {
  int pml4_index = (virt >> 39) & 0x1ff;
  int pdpt_index = (virt >> 30) & 0x1ff;
  int pdt_index = (virt >> 21) & 0x1ff;
  int pt_index = (virt >> 12) & 0x1ff;

  *entry = pml4[pml4_index];
  if (!(*entry & PTBL_BIT_PRESENT))
    return FALSE;

  UINT64 *pdpt = (UINT64*)(*entry & PTBL_ADDR_MASK);
  *entry = pdpt[pdpt_index];
  if (!(*entry & PTBL_BIT_PRESENT))
    return FALSE;
  if (*entry & PTBL_BIT_HUGE) {
    *page_size = 0x40000000; // 1GB
    return TRUE;
  }

  UINT64 *pdt = (UINT64*)(*entry & PTBL_ADDR_MASK);
  *entry = pdt[pdt_index];
  if (!(*entry & PTBL_BIT_PRESENT))
    return FALSE;
  if (*entry & PTBL_BIT_HUGE) {
    *page_size = 0x200000; // 2 MiB
    return TRUE;
  }

  UINT64 *pt = (UINT64*)(*entry & PTBL_ADDR_MASK);
  *entry = pt[pt_index];
  if (!(*entry & PTBL_BIT_PRESENT))
    return FALSE;
  *page_size = 4096;
  return TRUE;
}

// Calculate the size of additional page table structures to map the kernel and the framebuffer.
// Parameters are given in *pages*, not in bytes.
UINT64 calculate_ptbl_size(UINT64 kernel_pages, UINT64 fb_pages) {
  // We assume 4 KiB pages and the typical 4-level page table hierarchy.
  // Each PT covers 2 MiB of address space (512 * 4 KiB)
  // Each PDT covers 1 GiB of address space (512 * 2 MiB)
  //  => upper limit for kernel and framebuffer size
  // Each PDPT covers 512 GiB (512 * 1 GiB) => one PDPT per region
  // PML4 covers 256 TiB (512 * 0.5 TiB)

  // Virtual address:
  //   kernel: ffff800000000000
  //   fb    : ffff800040000000  <= bit 30 set (lowest bit of PDPT index)
  UINT64 result = 4; // we need at least one shared PML4, one shared PDPT, and two PDTs
  result += ((kernel_pages + PTBL_ENTRIES - 1) >> 9);  // devide by 2 MiB
  result += ((fb_pages + PTBL_ENTRIES - 1) >> 9);      //  => number of needed PTs
  return result;
}

// Initialize the additional page table structures to map the kernel and the framebuffer.
// Parameters are given in *pages*, not in bytes.
// The virtual addresses will become 0xffff800000000000 and 0xffff800040000000.
void init_ptbl(PageTable pml4, EFI_PHYSICAL_ADDRESS kernel_addr, UINT64 kernel_pages,
  EFI_PHYSICAL_ADDRESS fb_addr, UINT64 fb_pages) {

  PageTable orig_pml4 = (PageTable)cpuGetCR3();
  PageTable pdpt = pml4 + PTBL_ENTRIES; // common PDPT is located directly after PML4
  PageTable pdt_kernel = pdpt + PTBL_ENTRIES; // the two PDTs follow
  PageTable pdt_fb = pdt_kernel + PTBL_ENTRIES;
  PageTable pt = pdt_fb + PTBL_ENTRIES; // finally all page tables (number depends on kernel and fb sizes)

  // initialize the page table entries

  // first copy the original PML4 into the newly allocate memory
  //  => the original table might be mapped read-only (qemu!)
  for (int i = 0; i < PTBL_ENTRIES; i++) pml4[i] = orig_pml4[i];

  // now the new structure
  pml4[0x100] = (((UINT64)pdpt) & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
  pdpt[0x0] = (((UINT64)pdt_kernel) & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
  pdpt[0x1] = (((UINT64)pdt_fb) & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
  for (int i = 2; i < PTBL_ENTRIES; i++) pdpt[i] = 0;

  // fill the kernel page tables
  int page = 0;
  while (page < kernel_pages) {
    pt[page++] = (kernel_addr & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
    kernel_addr += 4096;
  }
  while (page % PTBL_ENTRIES != 0) // fill up the last page table
    pt[page++] = 0;
  for (int i = 0; i < (page >> 9); i++) {
    pdt_kernel[i] = (((UINT64)pt) & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
    pt += PTBL_ENTRIES;
  }

  // the same for the framebuffer
  page = 0;
  while (page < fb_pages) {
    pt[page] = (fb_addr & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
    pt[page++] |= ((1 << 7) | (1 << 4) | (1 << 3)); // PAT bit to enable WRITE COMBINING mode
    fb_addr += 4096;
  }
  while (page % PTBL_ENTRIES != 0) // fill up the last page table
    pt[page++] = 0;
  for (int i = 0; i < (page >> 9); i++) {
    pdt_fb[i] = (((UINT64)pt) & PTBL_ADDR_MASK) | PTBL_BIT_PRESENT | PTBL_BIT_WRITABLE | PTBL_BIT_USER;
    pt += PTBL_ENTRIES;
  }

  UINT64 pt_entry = 0, page_size = 0;
  BOOLEAN result = pt_lookup(pml4, 0xffff800000000000ull, &pt_entry, &page_size);
  Print((CHAR16*)L"    new kernel %d 0x%llx 0x%llx\n", (int)result, pt_entry, page_size);
  result = pt_lookup(pml4, 0xffff800040000000ull, &pt_entry, &page_size);
  Print((CHAR16*)L"    new fb %d 0x%llx 0x%llx\n", (int)result, pt_entry, page_size);

}

/**
 * get_memory_map
 */
EFI_STATUS get_memory_map(OUT VOID** memory_map, OUT UINTN* memory_map_size,
                          OUT UINTN* memory_map_key, OUT UINTN* descriptor_size,
                          OUT UINT32* descriptor_version) {

  /** Program status. */
  EFI_STATUS status;
  /** Input key type used to capture user input. */
  //EFI_INPUT_KEY input_key;

  Print((CHAR16*)L" * Allocating memory map %llx.\n", memory_map_size);
  status = uefi_call_wrapper((void*)gBS->GetMemoryMap, 5,
          memory_map_size, *memory_map, memory_map_key,
          descriptor_size, descriptor_version);

  // This will always fail on the first attempt, this call will return the
  // required buffer size.
  if(status != EFI_BUFFER_TOO_SMALL) {
    Print((CHAR16*)L"### Error getting memory map size!\n");
    return status;
  }

  // According to: https://stackoverflow.com/a/39674958/5931673
  // Up to two new descriptors may be created in the process of allocating the
  // new pool memory.
  *memory_map_size += 2 * (*descriptor_size);

  status = uefi_call_wrapper((void*)gBS->AllocatePool, 3,
          EfiLoaderData, *memory_map_size, (VOID**)memory_map);
  if(status != EFI_SUCCESS) {
    return status;
  }

  status = uefi_call_wrapper((void*)gBS->GetMemoryMap, 5,
          memory_map_size, *memory_map, memory_map_key,
          descriptor_size, descriptor_version);
  if(status != EFI_SUCCESS) {
    return status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

  EFI_STATUS status; // result of various UEFI calls
  /** The EFI memory map descriptor. */
  EFI_MEMORY_DESCRIPTOR* memory_map = NULL;
  /** The memory map key. */
  UINTN memory_map_key = 0;
  /** The size of the memory map buffer. */
  UINTN memory_map_size = 0;
  /** The memory map descriptor size. */
  UINTN descriptor_size;
  /** The memory map descriptor. */
  UINT32 descriptor_version;

  EFI_PHYSICAL_ADDRESS page_table_start = 0; // to be initialized before being used!
  EFI_PHYSICAL_ADDRESS kernel_start = 0;

  InitializeLib(ImageHandle, SystemTable);
  Print((CHAR16*)L"OO-StuBS UEFI startup code running.\n");
  Print((CHAR16*)L" * TEXT+DATA size is %ld.\n", kernel_size);
  Print((CHAR16*)L" * Total kernel space is 0x%llx.\n", kernel_total_size);

  // Disable the watchdog timer.
#define DISABLE_UEFI_WATCHDOG_TIMER
#ifdef DISABLE_UEFI_WATCHDOG_TIMER
  // Disable watchdog timer for debugging
  status = uefi_call_wrapper((void*)BS->SetWatchdogTimer, 4, 0, 0, 0, NULL);
  if(EFI_ERROR(status))
  {
    Print((CHAR16*)L"### Error stopping watchdog, timeout still counting down...\n");
  }
#endif

  EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
 
  status = uefi_call_wrapper((void*)BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
  if(EFI_ERROR(status)) {
    Print((CHAR16*)L"### Unable to locate GOP\n");
    efi_pause();
    return status;
  }

  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
  UINTN SizeOfInfo, numModes, nativeMode;
 
  status = uefi_call_wrapper((void*)gop->QueryMode, 4, gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
  // this is needed to get the current video mode
  if (status == EFI_NOT_STARTED) {
    Print((CHAR16*)L" * GOP not started,yet. Trying now.\n");
    status = uefi_call_wrapper((void*)gop->SetMode, 2, gop, 0);
  }
  if(EFI_ERROR(status)) {
    Print((CHAR16*)L"### Unable to get native GOP mode\n");
    efi_pause();
    return status;
  }
  nativeMode = gop->Mode->Mode;
  numModes = gop->Mode->MaxMode;
  Print((CHAR16*)L" * GOP located: Got native mode %d (of %d)\n", nativeMode, numModes);

    // get framebuffer
  Print((CHAR16*)L" * Framebuffer: 0x%llx (size %d, res. %dx%d, per line %d)\n",
    gop->Mode->FrameBufferBase,
    gop->Mode->FrameBufferSize,
    gop->Mode->Info->HorizontalResolution,
    gop->Mode->Info->VerticalResolution,
    gop->Mode->Info->PixelsPerScanLine
  );

  // save relevant framebuffer information for OO-StuBS
  bootloader_info.framebuffer_pointer   = (void*)0xffff800040000000ull;
  bootloader_info.vertical_resolution   = gop->Mode->Info->VerticalResolution;
  bootloader_info.horizontal_resolution = gop->Mode->Info->HorizontalResolution;
  bootloader_info.pixels_per_scanline   = gop->Mode->Info->PixelsPerScanLine;
  bootloader_info.acpi_table            = 0; // for now ...

  // Analyze 'SystemTable' and find ACPI 2.0 table
  EFI_GUID acpi2 = ACPI_20_TABLE_GUID;
  EFI_CONFIGURATION_TABLE *conf = SystemTable->ConfigurationTable;
  for (int i = 0; i < SystemTable->NumberOfTableEntries; i++) {
    if (CompareGuid (&(conf[i].VendorGuid), &acpi2) == 0) {
      Print((CHAR16*)L" * ACPI 2.0 table found at 0x%llx\n", conf[i].VendorTable);
      bootloader_info.acpi_table = conf[i].VendorTable;
      break;
    }
  }
  if (!bootloader_info.acpi_table) {
    Print((CHAR16*)L"### System has no ACPI 2.0 table.\n");
    efi_pause();
    return EFI_ABORTED;
  }

  // Allocate memory for the system and the extended page table structures
  UINT64 fb_pages = (gop->Mode->FrameBufferSize + 4095) / 4096;
  UINT64 kernel_pages = (kernel_total_size + 4095) / 4096;
  UINT64 page_table_pages = calculate_ptbl_size(kernel_pages, fb_pages); // in pages
  status = uefi_call_wrapper(gBS->AllocatePages, 4, AllocateAnyPages, EfiLoaderData,
    page_table_pages + kernel_pages , &page_table_start);
  if(EFI_ERROR(status)) {
    Print((CHAR16*)L"### Error allocating 0x%llx pages.\n", page_table_pages + kernel_pages);
    efi_pause();
    return status;
  }

  kernel_start = page_table_start + (page_table_pages << 12);
  Print((CHAR16*)L" * Allocated 0x%llx pages for page table at 0x%llx.\n", page_table_pages, page_table_start);
  Print((CHAR16*)L" * Allocated 0x%llx pages for kernel at 0x%llx.\n", kernel_pages, kernel_start);

  // Copy the loaded kernel image to the allocated memory region
  CopyMem((void*)kernel_start, (const void *)_binary_system_bin_start, kernel_size);

  // Create a new page table at 'page_table_start', which maps the kernel and the framebuffer
  // to predefined virtual addresses.
  init_ptbl((PageTable)page_table_start, (EFI_PHYSICAL_ADDRESS)kernel_start, kernel_pages,
    (EFI_PHYSICAL_ADDRESS)gop->Mode->FrameBufferBase, fb_pages);

 // Set PAT7 to 0x01 (=write combining)
  UINT32 lo, hi;
  cpuGetMSR(0x277, &lo, &hi);
  hi = (hi & 0x00ffffff) | 0x01000000;
  cpuSetMSR(0x277, lo, hi);

  Print((CHAR16*)L" * Exit boot services and run OOStuBS...\n");

  // Get the memory map prior to exiting the boot service.
  memory_map_size = 0;
  memory_map_key = 0;
  status = get_memory_map((VOID**)&memory_map, &memory_map_size,
          &memory_map_key, &descriptor_size, &descriptor_version);
  if(EFI_ERROR(status)) {
          // Error has already been printed.
          return status;
  }

  // No 'Print' or 'efi_pause' after this point! It affects the validity of the memory map key.

  status = uefi_call_wrapper((void*)gBS->ExitBootServices, 2, ImageHandle, memory_map_key);
  if (EFI_ERROR(status))
    return status;

  // Switch to new page table
  cpuSetCR3((UINT64)page_table_start);

  // Now jump into the kernel at its virtual address and pass the bootloader info.
  void (*startup_func)(struct BootloaderInfo *) = (void (*)(struct BootloaderInfo *))0xffff800000000000ull;
  startup_func(&bootloader_info);

  while (TRUE);
  return EFI_SUCCESS;
}
