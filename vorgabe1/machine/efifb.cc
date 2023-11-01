#include "efifb.h"
#include "bootloader/bootloader_info.h"
#include "object/fb.h"

extern BootloaderInfo *bootloader_info;

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 


EFI_Framebuffer::EFI_Framebuffer()
{
    this->width = bootloader_info->pixels_per_scanline;
    this->height = bootloader_info->vertical_resolution;
    this->buffer_ptr = (unsigned int*) bootloader_info->framebuffer_pointer;
    
    unsigned int *pixelptr;
    for (unsigned int y = 0; y < this->height; y++)
    {
        for (unsigned int x = 0; x < this->width; x++)
        {
            unsigned int value = 0x111111;
            pixelptr = this->buffer_ptr + y * this->width + x;
            *pixelptr = value;
        }
    }
}

void EFI_Framebuffer::clear(){
    unsigned int *pixelptr;
    for (unsigned int y = 0; y < this->height; y++)
    {
        for (unsigned int x = 0; x < this->width; x++)
        {
            unsigned int value = 0x000000;
            pixelptr = this->buffer_ptr + y * this->width + x;
            *pixelptr = value;
        }
    }
}


void EFI_Framebuffer::blit(const Framebuffer &fb,
                            unsigned int x,
                            unsigned int y)
{
    unsigned int *efifb = (unsigned int*) bootloader_info->framebuffer_pointer;
    unsigned int *pixelptr_new = efifb;

    for (unsigned int y_new = y; y_new < this->height; y_new++)
    {
        for (unsigned int x_new = x; x_new < this->width; x_new++)
        {
            if (x <= this->width && y <= this->height)
            {
                Pixel pixel_old = fb.get_pixel(x_new, y_new);
                unsigned int value = pixel_old.get_value();

                pixelptr_new = efifb + y_new * this->width + x_new;
                *pixelptr_new = value;
            }
        }
    }
}


void EFI_Framebuffer::blit_rect(const Framebuffer &fb,
                                unsigned int xr, // x-Koordinate von wo wir nehmen
                                unsigned int yr, // y-Koordinate von wo wir nehmen
                                unsigned int wr, // breite
                                unsigned int hr, // höhe
                                unsigned int x = 0, // neue Stelle
                                unsigned int y = 0) // neue Stelle
{
    unsigned int *efifb = (unsigned int*) bootloader_info->framebuffer_pointer;
    unsigned int *pixelptr_new = efifb;

    if (xr + wr < fb.get_width() && yr + hr < fb.get_width())
    {
        for (unsigned int y_new = y; y_new < y + hr; y_new++)
        {
            for (unsigned int x_new = x; x_new < x + wr; x_new++)
            {
                if (x_new <= this->width || y_new <= this->height)
                {
                    Pixel pixel_old = fb.get_pixel(xr + x_new - x, yr + y_new - y);

                    unsigned int value = pixel_old.get_value();

                    pixelptr_new = efifb + y_new * this->width + x_new;
                    *pixelptr_new = value;
                }
            }
        }
    }
}


unsigned int EFI_Framebuffer::get_width() const{
    return this->width;
}
unsigned int EFI_Framebuffer::get_height() const{

    return this->height;
}