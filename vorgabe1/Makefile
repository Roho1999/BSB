# -----------------------------------------------------------------------------
# 
#                              M A K E F I L E
# 
# zum Uebungsbetriebssystem OOStuBS der Lehrveranstaltung 'Betriebssystembau'.
# -----------------------------------------------------------------------------
#
# Durch Eingabe von 'make' werden die Quelltexte des Systems kompiliert. 'make
# bootdisk' schreibt das System-Image auf einen USB-Stick, von dem das System
# dann gebootet werden kann. Mit 'make clean' wird im Verzeichnis aufgeraeumt.
# Die Targets 'qemu', 'qemu-gdb', 'gdb', 'ddd' starten den QEMU-Emulator mit
# bzw. ohne GDB-Stub und starten den GDB- bzw. den DDD-Debugger, um sich dann
# mit dem GDB-Stub zu verbinden. Auf Poolrechnern koennen mit 'vorgabe2' bis
# 'vorgabe7' die Vorgaben fuer eine neue Aufgabe in das aktuelle
# Arbeitsverzeichnis eingepflegt werden.
#
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Liste der Quelltexte:

STARTUP_SOURCE = ./startup.asm
CC_SOURCES = $(shell find . -name "*.cc")
C_SOURCES = $(shell find . -name "*.c")
ASM_SOURCES = $(shell find ./machine -name "*.asm")
FONT_SOURCES = $(shell find . -name "*.psf")
BOOTLOADER_SOURCES = $(shell find ./bootloader -name "*.c")

# Einstellungen in Abhaengigkeit vom Generierungssystem:
#
#    ASMOBJFORMAT: Ausgabeformat fuer den Assembler. Das Format muss dem
#                  jeweiligen Format des verwendeten C++ Compilers angepasst
#                  werde, damit gemischte Objektdateien gelinkt werden koennen.
#    OBJDIR:       Verzeichnis, in dem die Objektdateien abgelegt werden
#                  sollen.
#    DEPDIR:       Verzeichnis, in dem die Abhaengigkeitsdateien abgelegt werden
#                  sollen
#    DRIVE_HD:     Festplatten-/USB-Device, auf das das System-Image
#                  geschrieben wird (Target: bootdisk)
#    DELETE:       Name des Kommandos zum Loeschen von Dateien
#    ASM:          Zu benutzender Assembler
#    CC/CXX:       Zu benutzender C/C++-Compiler
#    CFLAGS:       Flags fuer den C-Compileraufruf
#    CXXFLAGS:     Flags fuer den C++-Compileraufruf
#    QEMU:         Architekturspezifischer Name des QEMU-Programms

VERBOSE = @
OBJDIR = ./build
DEPDIR = ./dep
# ACHTUNG: ein falsch angegebenes Laufwerk kann dazu fuehren, dass Daten auf dem
# spezifizierten Laufwerk verloren gehen! Nicht mit root-Rechten ausfuehren!
DRIVE_HD = /dev/sdz
DRIVE_HD_PARTITION = $(DRIVE_HD)1
FNAME = aufgabe
DELETE = rm
ASM = nasm
ASMFLAGS := -f elf64 -g -F dwarf
QEMU ?= qemu-system-x86_64
CC ?= gcc
CXX ?= g++
CFLAGS := $(CFLAGS) -O2 -m64 -fno-pie -no-pie -mcmodel=large -mno-red-zone -Wall -Wno-write-strings -fno-stack-protector -nostdlib -I. -g #-DDEBUG
BOOTLOADER_CFLAGS := -fpic -fshort-wchar -I/usr/include/efi -DEFI_FUNCTION_WRAPPER
CXXFLAGS := $(CFLAGS) -Wno-non-virtual-dtor -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti -fno-exceptions

EFI_LIB      := /usr/lib
EFI_CRT_OBJS := ${EFI_LIB}/crt0-efi-x86_64.o
EFI_LDS      := ${EFI_LIB}/elf_x86_64_efi.lds

# -------------------------------------------------------------------------
# Namen der Unterverzeichnisse mit den Quelltexten

VPATH = $(sort $(dir $(STARTUP_SOURCE) $(CC_SOURCES) $(C_SOURCES) $(ASM_SOURCES) $(FONT_SOURCES)))

# -------------------------------------------------------------------------
# Listen mit den Objektdateien, die beim Kompilieren entstehen:

FIRST_OBJECT = $(addprefix $(OBJDIR)/,$(patsubst %.asm,_%.o, $(notdir $(STARTUP_SOURCE))))
C_OBJECTS = $(notdir $(C_SOURCES:.c=.o))
CC_OBJECTS = $(notdir $(CC_SOURCES:.cc=.o))
BOOTLOADER_OBJECTS = $(notdir $(BOOTLOADER_SOURCES:.c=.o))

DEP_FILES = $(patsubst %.o,$(DEPDIR)/%.d,$(C_OBJECTS))
DEP_FILES += $(patsubst %.o,$(DEPDIR)/%.d,$(CC_OBJECTS))

ASM_OBJECTS = $(patsubst %.asm,_%.o, $(notdir $(ASM_SOURCES)))
FONT_OBJECTS = $(notdir $(FONT_SOURCES:.psf=.o))
OBJPRE = $(addprefix $(OBJDIR)/,$(ASM_OBJECTS) $(C_OBJECTS) $(CC_OBJECTS))
OBJ_SYSTEM = $(addprefix $(OBJDIR)/,$(filter-out $(BOOTLOADER_OBJECTS), $(ASM_OBJECTS) $(C_OBJECTS) $(CC_OBJECTS) $(FONT_OBJECTS)))
OBJ_BOOTLOADER = $(addprefix $(OBJDIR)/, $(BOOTLOADER_OBJECTS))

# --------------------------------------------------------------------------
# Default target: Das Image fuer USB-Sticks und QEMU

all: $(OBJDIR)/bootdisk.iso

# --------------------------------------------------------------------------
# Regeln zur Erzeugung der Abhaengigkeitsdateien

$(DEPDIR)/%.d : %.c
	@echo "DEP		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CC) $(CFLAGS) -MM -MT $(OBJDIR)/$*.o -MF $@ $<

$(DEPDIR)/%.d : %.cc
	@echo "DEP		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) $(CXXFLAGS) -MM -MT $(OBJDIR)/$*.o -MF $@ $<

# --------------------------------------------------------------------------
# Regeln zur Erzeugung der Objektdateien

$(OBJDIR)/%.o : ./bootloader/%.c
	@echo "CC*		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CC) -c $(BOOTLOADER_CFLAGS) -o $@ $<

$(OBJDIR)/%.o : %.c
	@echo "CC		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o : %.cc
	@echo "CXX		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) -c $(CXXFLAGS) -o $@ $<

$(OBJDIR)/_%.o : %.asm
	@echo "ASM		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(ASM) $(ASMFLAGS) -o $@ $<

$(OBJDIR)/%.o : %.psf
	@echo "FONT		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) objcopy -O elf64-x86-64 -B i386 -I binary $< $@

# --------------------------------------------------------------------------
# Gelinktes System

$(OBJDIR)/system: $(FIRST_OBJECT) $(OBJ_SYSTEM)
	@echo "LD		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) $(CXX) $(CXXFLAGS) -z max-page-size=0x1000 -e startup -T sections -o $@ $(FIRST_OBJECT) $(OBJ_SYSTEM)

# Gelinkter Bootloader

$(OBJDIR)/bootx64.so: $(OBJDIR)/system $(OBJ_BOOTLOADER)
	@echo "LD*		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) objcopy -O binary $< system.bin
	$(VERBOSE) objcopy -O elf64-x86-64 -B i386 -I binary --add-symbol __KERNEL_SIZE__=0x$(firstword $(shell nm $< | grep __KERNEL_SIZE__)) system.bin system.bin.o
	$(VERBOSE) ld $(EFI_CRT_OBJS) $(OBJ_BOOTLOADER) system.bin.o -fpic -znocombreloc -T$(EFI_LDS) -shared -Bsymbolic -L $(EFI_LIB) -l:libgnuefi.a -l:libefi.a -o $@
	$(VERBOSE) rm -f system.bin system.bin.o

$(OBJDIR)/bootx64.efi: $(OBJDIR)/bootx64.so
	@echo "EFI		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	$(VERBOSE) objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 $< $@

# --------------------------------------------------------------------------
# 'bootdisk.iso' besteht aus dem System und einem Bootloader (EFI) mit
# Bootblock und Setup-Code. Der Bootloader wird mit dem System gelinkt und
# anschliessend in eine ISO-Datei geschrieben.

$(OBJDIR)/bootdisk.iso : $(OBJDIR)/bootx64.efi
	@echo "BUILD		$@"
	@if test \( ! \( -d $(@D) \) \) ;then mkdir -p $(@D);fi
	dd if=/dev/zero of=$@ bs=512 count=93750 status=none
	parted $@ -s -a minimal mklabel gpt
	parted $@ -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted $@ -s -a minimal toggle 1 boot
	dd if=/dev/zero of=$(OBJDIR)/part.img bs=512 count=91669 status=none
	mformat -i $(OBJDIR)/part.img -h 32 -t 32 -n 64 -c 1
	mmd -i $(OBJDIR)/part.img ::/EFI
	mmd -i $(OBJDIR)/part.img ::/EFI/BOOT
	mcopy -i $(OBJDIR)/part.img $< ::/EFI/BOOT/BOOTX64.EFI
	dd if=$(OBJDIR)/part.img of=$@ bs=512 count=91669 seek=2048 conv=notrunc status=none
	rm $(OBJDIR)/part.img

# --------------------------------------------------------------------------
# 'clean' loescht das generierte System, die Objektdateien und die
# Abhaengigkeitsdateien

clean:	
	@echo "RM		$(OBJDIR)"
	$(VERBOSE) rm -rf $(OBJDIR)
	@echo "RM		$(DEPDIR)"
	$(VERBOSE) rm -rf $(DEPDIR)

# --------------------------------------------------------------------------
# 'vorgabe%' importiert neue Dateien aus einer Vorgabe (2 bis 7) in das
# Arbeitsverzeichnis, so dass mit der naechsten Aufgabe begonnen werden kann.
vorgabe%:
	@echo "EXTRACT         /home/bsb/vorgaben/$@.tar.gz"
	$(VERBOSE) tar --strip-components=1 --keep-old-files -xvf /home/bsb/vorgaben/$@.tar.gz

# --------------------------------------------------------------------------
# 'bootdisk_efi' erzeugt zunaechst das System, falls das noch nicht geschehen ist.
# Danach wird die erste Partition (DRIVE_HD_PARTITION) gemounted und das System wird
# unter EFI/BOOT/BOOTX64.EFI auf das Laufwerk geschrieben. Dieses Target kann genutzt
# werden, wenn das Laufwerk nicht jedes mal neu formatiert werden soll. Das Laufwerk
# wird am Ende nicht ausgeworfen um erneute Schreibzugriffe zu ermoeglichen.

bootdisk_efi: $(OBJDIR)/bootx64.efi
	-$(VERBOSE) udisksctl mount -b $(DRIVE_HD_PARTITION)
	MP=$$(lsblk -o MOUNTPOINT $(DRIVE_HD_PARTITION) | tail -1) && mkdir -p $$MP/EFI/BOOT && cp $< $$MP/EFI/BOOT/BOOTX64.EFI
	$(VERBOSE) sync
	$(VERBOSE) udisksctl unmount -b $(DRIVE_HD_PARTITION)

# --------------------------------------------------------------------------
# 'bootdisk' erzeugt zunaechst das System, falls das noch nicht geschehen ist.
# Danach wird das System auf das spezifizierte Laufwerk geschrieben, welches
# sowohl eine Festplatte als auch ein USB-Stick sein kann. Das Laufwerk wird
# am Ende sicher ausgeworfen (keine weitere Nutzung ohne erneutes Einstecken mehr möglich).
# ACHTUNG: ein falsch angegebenes Laufwerk kann dazu fuehren, dass Daten auf dem
# spezifizierten Laufwerk verloren gehen! Nicht mit root-Rechten ausfuehren!

bootdisk: $(OBJDIR)/bootdisk.iso
	@echo "CP		$<"
	$(VERBOSE) cp $< $(DRIVE_HD)
	$(VERBOSE) eject $(DRIVE_HD)

# --------------------------------------------------------------------------
# 'qemu' ruft den qemu-Emulator mit dem System auf.

qemu: $(OBJDIR)/bootdisk.iso
	$(QEMU) -cpu qemu64 -m 512 -bios /usr/share/ovmf/OVMF.fd -drive file=$<,format=raw

# --------------------------------------------------------------------------
# 'qemu-smp' ruft den qemu-Emulator mit SMP-Support und 2 CPUs mit dem System auf.

qemu-smp: $(OBJDIR)/bootdisk.iso
	$(QEMU) -cpu qemu64 -m 512 -bios /usr/share/ovmf/OVMF.fd -drive file=$<,format=raw -smp 2

# --------------------------------------------------------------------------
# 'qemu-gdb' starts the emulator with activated gdb stub. The gdb command to connect with the
# stub is stored in a user-specific file under /tmp and qemu waits after its window has been
# opened. Run 'make gdb' to start a gdb instance that automatically connects to qemu. Then use
# 'break <function-name>' to set a breakpoint and 'cont' to proceed with the boot process.

qemu-gdb: $(OBJDIR)/bootdisk.iso
	$(VERBOSE) echo "target remote localhost:$(shell echo $$(( $$(id -u) + 1023 )))" > /tmp/gdbcommands.$(shell id -u)
	$(QEMU) -cpu qemu64 -m 512 -bios /usr/share/ovmf/OVMF.fd -drive file=$<,format=raw -S -gdb tcp::$(shell echo $$(( $$(id -u) + 1023 )))

# --------------------------------------------------------------------------
# 'gdb' startet den GDB-Debugger und verbindet sich mit dem GDB-Stub des vorher
# gestarteten QEMU.

gdb:
	gdb -x /tmp/gdbcommands.$(shell id -u) $(OBJDIR)/system

# --------------------------------------------------------------------------
# 'ddd' startet den DDD-Debugger und verbindet sich mit dem GDB-Stub des vorher
# gestarteten QEMU.

ddd:
	ddd --gdb -x /tmp/gdbcommands.$(shell id -u) $(OBJDIR)/system

# --------------------------------------------------------------------------
# Einbindung der Abhaengigkeitsdateien

ifneq ($(MAKECMDGOALS),clean)
-include $(DEP_FILES)
endif

.PHONY: clean bootdisk gdb ddd qemu qemu-smp qemu-gdb
