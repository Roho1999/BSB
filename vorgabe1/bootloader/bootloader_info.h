#ifndef __bootloader_info_include__
#define __bootloader_info_include__

#include <stdint.h>

struct BootloaderInfo {
        void *framebuffer_pointer;      // pixel framebuffer for graphics output
        uint32_t horizontal_resolution; // screen width
        uint32_t vertical_resolution;   // screen height
        uint32_t pixels_per_scanline;   // value >= horizontal_resolution!
        void *acpi_table;               // needed to detect system configuration
};

#endif
