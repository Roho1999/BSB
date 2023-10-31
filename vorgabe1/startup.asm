
;******************************************************************************
;* OO-StuBS                                                                   *
;*----------------------------------------------------------------------------*
;*                                                                            *
;*                        S T A R T U P . A S M                               *
;*                                                                            *
;*----------------------------------------------------------------------------*
;* The 'startup' function is the entry point of the system. It has to be      *
;* linked to the fixed address 0xffff800000000000. i.e. the start address of  *
;* operating system. As the function is called by the UEFI bootloader, we     *
;* alread run in 64-bit 'long mode' and the pagetable has already been        *
;* initialized by the bootloader.                                             *
;******************************************************************************

;
;   Constants
;

; Start address of the kernel, i.e. the virtual address of 'startup'
KERNEL_VADDR equ 0xffff800000000000

; Stack fuer die main-Funktion
STACKSIZE: equ 64*1024

;
;   System
;

; Exported functions
[GLOBAL startup]
[GLOBAL idt]
; [GLOBAL __cxa_pure_virtual]
; [GLOBAL _ZdlPv]
; [GLOBAL _ZdlPvj]
[GLOBAL _ZdlPvm]
; [GLOBAL atexit]

; Exported global variables
[GLOBAL bootloader_info]

; Functions from the C++-part of the operating system
[EXTERN main]
[EXTERN guardian]

; Global addresses that are provided by the linker (script)
[EXTERN __bss_start]
[EXTERN __bss_end]
[EXTERN __init_array_start]
[EXTERN __init_array_end]
[EXTERN __fini_array_start]
[EXTERN __fini_array_end]

[SECTION .text]

;
;   System Start (in 64-bit Long Mode)
;
;   We set up the GDT and IDT, which is important for interrupt processing.
;   Interrupts are disabled and the PICs are configured. The stack pointer
;   is moved so that our own stack of well-defined size is used. Furthermore,
;   the BSS segment is initialized and global constructor/destructors are
;   executed. In between we call the system's main() function.
;

startup:
[BITS 64]
	; save the argument (pointer to bootloader info) on the in a global variable
	mov		[rel bootloader_info], rdi

	; initialize BSS with 0; must be done *before* switching to new stack (in BSS)
	call	init_bss

	; use our own GDT to be on the safe side
	lgdt   [rel gdt_80]

	; far jump into the new code segment by using retfq
	push	1 * 0x8 ; new CS value
	lea     rax, [rel setup_data_segments]
	push	rax
	retfq

	; initialize data segment registers
setup_data_segments:
	mov    ax, 2 * 0x8 ; new value for all data segments
	mov    ds, ax
	mov    es, ax
	mov    fs, ax
	mov    gs, ax

	; setup stack segment and stack pointer
	mov    ss, ax
	lea    rsp, [rel init_stack+STACKSIZE]

	; initialize IDT and PICs
	cli
	lidt   [rel idt_descr]
	call   reprogram_pics

	fninit         ; activate FPU
	call   _init   ; execute constructors of globale objects
	call   main    ; call the kernel's main function
	call   _fini   ; run destructors (should normally not happen)
	cli            ; we shouldn't come here
	hlt            ; to halt

;
; Initialize the BSS with 0
;

init_bss:
	lea   rbx, [rel __bss_start]
	lea   r12, [rel __bss_end]
.loop:
	cmp    rbx, r12
	je     .done
	mov    qword [rbx], 0
	add    rbx, 8
	ja     .loop
.done:
	ret

;
;   interrupt handling: low-level wrapper functions
;

; specific start of the ISRs
%macro wrapper 1
wrapper_%1:
	%if %1 != 8 && %1 != 10 && %1 != 11 && %1 != 12 && %1 != 13 && %1 != 14 && %1 != 17 && %1 != 21 && %1 != 29 && %1 != 30
	push   -1; exception error code -1 if there is none pushed by the CPU
	%endif
	push   rax ; needed for first argument for guardian
	mov    al, %1
	jmp    wrapper_body
%endmacro

; ... is automatically generated
%assign i 0
%rep 256
wrapper i
%assign i i+1
%endrep

; common body of the ISRs
wrapper_body:
	; this is expected by gcc-generated code
	cld
	; save volatile registers
	push   rcx
	push   rdx
	push   rdi
	push   rsi
	push   r8
	push   r9
	push   r10
	push   r11

	; pass the interrupt number and the exception stack frame address to the guardian function
	and    rax, 0xff  ; clear unused bits
	mov    rdi, rax   ; first argument in rdi
	mov    rsi, rsp
	add    rsi, 9*8   ; compensate for 9 push operations
	call   guardian

	; restore volatile registers
	pop    r11
	pop    r10
	pop    r9
	pop    r8
	pop    rsi
	pop    rdi
	pop    rdx
	pop    rcx
	pop    rax

	; finally remove the true or injected exception code from the stack
	add	   rsp, 8

	; done!
	iretq

;
; Neuprogrammierung der PICs (Programmierbare Interrupt-Controller), damit
; alle 15 Hardware-Interrupts nacheinander in der idt liegen.
;

reprogram_pics:
	mov    al, 0x11   ; ICW1: 8086-Modus mit ICW4
	out    0x20, al
	call   delay
	out    0xa0, al
	call   delay
	mov    al, 0x20   ; ICW2 Master: IRQ # Offset (32)
	out    0x21, al
	call   delay
	mov    al, 0x28   ; ICW2 Slave: IRQ # Offset (40)
	out    0xa1, al
	call   delay
	mov    al, 0x04   ; ICW3 Master: Slaves an IRQs
	out    0x21, al
	call   delay
	mov    al, 0x02   ; ICW3 Slave: Verbunden mit IRQ2 des Masters
	out    0xa1, al
	call   delay
	mov    al, 0x03   ; ICW4: 8086-Modus und automatischer EOI
	out    0x21, al
	call   delay
	out    0xa1, al
	call   delay

	mov    al, 0xff   ; Hardware-Interrupts durch PICs
	out    0xa1, al   ; ausmaskieren. Nur der Interrupt 2,
	call   delay      ; der der Kaskadierung der beiden
	mov    al, 0xfb   ; PICs dient, ist erlaubt.
	out    0x21, al

	ret

;
; Konstruktoren globaler Objekte ausführen
;

_init:
	lea   rbx, [rel __init_array_start]
	lea   r12, [rel __init_array_end]
;	mov    rbx, __init_array_start
_init_loop:
	cmp    rbx, r12
	je     _init_done
	mov    rax, [rbx]
	call   rax
	add    rbx, 8
	ja     _init_loop
_init_done:
	ret

;
; Destruktoren globaler Objekte ausführen
;

_fini:
	lea    rbx, [rel __fini_array_start]
	lea    r12, [rel __init_array_end]
;	mov    rbx, __fini_array_start
_fini_loop:
	cmp    rbx, r12
	je     _fini_done
	mov    rax, [rbx]
	call   rax
	add    rbx, 8
	ja     _fini_loop
_fini_done:
	ret

;
; Kurze Verzögerung für in/out-Befehle
;

delay:
	jmp    .L2
.L2:
	ret

;
; Funktionen für den C++ Compiler. Diese Label müssen für den Linker
; definiert sein; da bei OOStuBS keine Freigabe des Speichers erfolgt, können
; die Funktionen aber leer sein.
;

; __cxa_pure_virtual: ; "virtual" Methode ohne Implementierung aufgerufen
; _ZdlPv:             ; void operator delete(void*)
; _ZdlPvj:            ; void operator delete(void*, unsigned int) fuer g++ 6.x
_ZdlPvm:            ; void operator delete(void*, unsigned long) fuer g++ 6.x
; atexit:             ; void atexit(void (*arg1)(void*), void* arg2, void* arg3)
 	ret

[SECTION .data]

;
; Segment-Deskriptoren
;

gdt:
	dw  0,0,0,0   ; NULL-Deskriptor

	; 64-Bit-Codesegment-Deskriptor
	dw  0xFFFF    ; 4Gb - (0x100000*0x1000 = 4Gb)
	dw  0x0000    ; base address=0
	dw  0x9A00    ; code read/exec
	dw  0x00AF    ; granularity=4096, 386 (+5th nibble of limit), Long-Mode

	; Datensegment-Deskriptor
	dw  0xFFFF    ; 4Gb - (0x100000*0x1000 = 4Gb)
	dw  0x0000    ; base address=0
	dw  0x9200    ; data read/write
	dw  0x00CF    ; granularity=4096, 386 (+5th nibble of limit)

gdt_80:
	dw  gdt_80 - gdt - 1   ; GDT Limit=3 GDT Eintraege - 1
	dq  gdt       ; Adresse der GDT

;
; Interrupt Descriptor Table mit 256 Einträgen
;

idt:
%macro idt_entry 1
	dw  (wrapper_%1 - startup + KERNEL_VADDR) & 0xffff ; Offset 0 .. 15
	dw  0x0000 | 0x8 * 1 ; Selector zeigt auf den 64-Bit-Codesegment-Deskriptor der GDT
	dw  0x8e00 ; 8 -> interrupt is present, e -> 80386 32-bit interrupt gate
	dw  ((wrapper_%1 - startup + KERNEL_VADDR) & 0xffff0000) >> 16 ; Offset 16 .. 31
	dd  ((wrapper_%1 - startup + KERNEL_VADDR) & 0xffffffff00000000) >> 32 ; Offset 32..63
	dd  0x00000000 ; Reserviert
%endmacro

%assign i 0
%rep 256
idt_entry i
%assign i i+1
%endrep

idt_descr:
	dw  idt_descr - idt - 1 ; size of the IDT
	dq idt                  ; address of the IDT

;
; Bootloader Info Pointer
;
; The pointer is stored in .data, because .bss is being overwritten with 0

bootloader_info:
	dq 0

[SECTION .bss]

;
; The stack to be used by the kernel. Of course UEFI has already allocated a stack,
; but we don't know its size.
;

global init_stack:data (init_stack.end - init_stack)
init_stack:
	resb STACKSIZE
.end:
