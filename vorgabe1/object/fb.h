#ifndef __framebuffer_include__
#define __framebuffer_include__

#include "object/heap.h"

class Pixel {
   unsigned int value;
public:
   Pixel(const Pixel &pixel) : value(pixel.get_value()) {}

   // Pixel: Constructs a pixel object in RGB encoding. The transparency 'a' is
   //        options (255=non-transparent, 0=transparent).
   Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) :
      value(((unsigned)a << 24) | ((unsigned)r << 16) | ((unsigned)g << 8) | (unsigned)b) {}

   Pixel(unsigned int v) : value(v) {}
   
   // GET_VALUE: Getter function for returning the composed RGBA value.
   unsigned int get_value() const { return value; }

};
static_assert(sizeof(Pixel) == sizeof(unsigned int), "Unexpected Pixel size in memory!");

#define BLACK_PIXEL Pixel(0,0,0)
#define LIGHTGRAY_PIXEL Pixel(0xcc,0xcc,0xcc)
#define RED_PIXEL Pixel(0xff,0,0)

class Framebuffer {
private:
   Framebuffer(const Framebuffer &copy) = delete; // Prohibit copying

   Pixel *buffer;     // the buffer is an array of Pixels
   unsigned int w, h; // having the dimention w * h

public:
   // FRAMEBUFFER: Constructs a framebuffer object.
   Framebuffer(int width, int height) :
      buffer((Pixel*)Heap::allocate(width * height * sizeof(Pixel))), w(width), h(height) {}

   // ~FRAMEBUFFER: Make sure the allocated memory is freed!
   ~Framebuffer() { Heap::free(buffer); }

   // GET_PIXEL_PTR: Get a pointer to the pixel at position 'x', 'y'.
   Pixel *get_pixel_ptr (unsigned int x, unsigned int y) const { return &buffer[y * w + x]; }

   // GET_PIXEL: Get the pixel at position 'x', 'y'.
   Pixel get_pixel (unsigned int x, unsigned int y) const { return *get_pixel_ptr(x, y); }

   // SET_PIXEL: Set the pixel, i.e. a specific RGBA color, at position 'x', 'y'.
   void set_pixel (unsigned int x, unsigned int y, Pixel p) const { *get_pixel_ptr(x, y) = p; }

   // GET_WIDTH: Get the width of the framebuffer.
   unsigned int get_width() const { return w; }

   // GET_HEIGHT: Get the height of the framebuffer.
   unsigned int get_height() const { return h; }
};
#endif // __framebuffer_include__
