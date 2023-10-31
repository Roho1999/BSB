#ifndef __psf_font_include__
#define __psf_font_include__

/*****************************************************************************/
/* Operating System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                               P S F _ F O N T                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* As a screen font for printing characters on the UEFI framebuffer we are   */
/* a Linux console font that is statically linked as a binary 'blob' into    */
/* the OOStuBS kernel. Linux console fonts are using the PSF2 format.        */
/*****************************************************************************/

#define PSF_FONT_MAGIC 0x864ab572
 
struct PSF_Font {
    unsigned int magic;         /* magic bytes to identify PSF */
    unsigned int version;       /* zero */
    unsigned int headersize;    /* offset of bitmaps in file, 32 */
    unsigned int flags;         /* 0 if there's no unicode table */
    unsigned int numglyph;      /* number of glyphs */
    unsigned int bytesperglyph; /* size of each glyph */
    unsigned int height;        /* height in pixels */
    unsigned int width;         /* width in pixels */
};

#endif // __psf_font_include__
