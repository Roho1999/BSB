/*****************************************************************************/
/* Operating System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                             E F I _ S C R E E N                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* By using this class you can access the screen of the PC. Access is        */
/* performed directly in the hardware level by writing to the graphic card's */
/* framebuffer, which is mapped into the kernel's address space.             */
/* In order to improve the performance, the screen content is stored in a    */
/* fully-cached local buffer. Whenever changes in the local buffer must      */
/* become visible on the screen, the hardware frame buffer is updated.       */
/* The class supports printing ASCII-encoded text and a visible text cursor. */
/*****************************************************************************/

#include "machine/efiscr.h"
#include "machine/efifb.h"
#include "object/psffont.h"

#define FONT _binary___fonts_Lat15_Terminus28x14_psf_start

extern PSF_Font FONT; // Symbol wird beim Linken bereitgestellt.

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 
EFI_Screen::EFI_Screen() :
    efifb(),
    fb(efifb.get_width(), efifb.get_height()),
    rows{efifb.get_height() / FONT.height},
    columns{efifb.get_width() / ((FONT.bytesperglyph /FONT.height)*8)}
{

}

void EFI_Screen::clear()
{
    efifb.clear();
}

int EFI_Screen::getrows () const
{
    return this->fb.get_height() / FONT.height;
}

int EFI_Screen::getcolumns() const
{
    unsigned int bytesperline = FONT.bytesperglyph / FONT.height;
    return this->fb.get_width() / (bytesperline*8); 
}

void EFI_Screen::show(int x, int y, unsigned char c, Pixel fg_color = Pixel(0xffffff), Pixel bg_color = Pixel(0x000000))
{
    unsigned char *glyph = (unsigned char*)&FONT + FONT.headersize + (int) c * FONT.bytesperglyph;
    
    unsigned int bytesperline = FONT.bytesperglyph / FONT.height;

    for(unsigned int y_n = 0; y_n < FONT.height; y_n++)
    {
        for(unsigned int x_n = 0; x_n < bytesperline; x_n++)
        {
            for (unsigned int bit = 0; bit < 8; bit++)
            {
                if((glyph[y_n*bytesperline + x_n] >> (7-bit)) & 0x1)
                {
                    fb.set_pixel(x*bytesperline*8 + x_n*8 + bit, y*FONT.height + y_n, Pixel(fg_color));
                }
                else
                {
                    fb.set_pixel(x*bytesperline*8 + x_n*8 + bit, y*FONT.height + y_n, Pixel(bg_color));
                }
            } 
        }
    }
}

void EFI_Screen::setpos (int x, int y)
{
    this->posx = x;
    this->posy = y;
}


void EFI_Screen::getpos (int &x, int &y) const
{
    x = this->posx;
    y = this->posy;
}


// Mit dieser Methode kann eine Zeichenkette text ausgegeben werden,
// wobei ab der aktuellen Position des Cursors geschrieben wird.

// Da die Zeichenkette anders als sonst bei C üblich keine Nullterminierung zu enthalten braucht,
// wird der Parameter length benötigt, der angeben muss aus wie vielen Zeichen text besteht.

// Nach Abschluss der Ausgabe soll der Cursor hinter dem zuletzt ausgegebenen Zeichen stehen.

// Der gesamte text soll einheitlich mit den durch fg_color und bg_color 
// gewählten Farben dargestellt werden, wobei wieder mit Default-Werten zu arbeiten ist.

// Wenn bis zum Ende der Zeile nicht mehr genügend Platz ist,
// soll die Ausgabe auf der folgenden Zeile fortgesetzt werden.

// Sobald die letzte Bildschirmzeile gefüllt ist, soll der gesamte
// Bildschirmbereich um eine Zeile nach oben geschoben werden ("scrolling").

// Dadurch verschwindet die erste Zeile. Dafür kann nun die letzte Zeile erst gelöscht
// und dann die Ausgabe dort fortgesetzt werden.

// Ein Zeilenumbruch muss auch erfolgen, wann immer das Zeichen '\n' im auszugebenen Text enthalten ist. 

void EFI_Screen::print (const char* text,
                        Pixel fg_color = Pixel(0xffffff),
                        Pixel bg_color = Pixel(0x000000))
{
    int x = 0; // Spalte
    int y = 0; // Zeile
    getpos(x, y); 

    for (int i = 0; text[i] != '\0'; i++)
    {
        // Absatz ist gewünscht
        if(text[i] == '\n')
        {
            x = 0;
            y += 1;
        }
        else
        {
            // Ende einer Zeile
            if (x >= getcolumns())
            {
                x = 0;
                y += 1;
            }

            // Wann wollen wir scrollen?
            if (y >= getrows())
            {
                scroll_up();
                y -= 1;
            }

            // Zeige das Zeichen an und packe Cursor weiter
            show(x, y, text[i], fg_color, bg_color);
            x += 1;
        }
    }

    show(x, y, '|', fg_color, bg_color);
    //x+= 1;
    setpos(x, y);

    efifb.blit(fb, 0, 0);
}


void EFI_Screen::scroll_up()
{
    for (unsigned int x = 0; x < this->fb.get_width(); x++)
    {
        for (unsigned int y = 0; y < this->fb.get_height(); y++)
        {
            this->fb.set_pixel(x, y, this->fb.get_pixel(x, y + FONT.height));
        }
    }
}