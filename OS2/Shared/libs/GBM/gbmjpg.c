/*

gbmjpg.c - JPEG File Interchange Format

Credit for writing this module must go to Martin Lisowski,
and Heiko Nitzsche (enhancements for CMYK import, gray export).

This file is just as public domain as the rest of GBM.

This module makes use of the work of the Independent JPEG group version 6d,

Reads JPEG-FIF images with 8 Bit per component (YUV, YCCK, CMYK)
Also reads progressive JPEG images
Also reads greyscale JPEG images (Y only)
Writes JPEG-FIF images with 8 Bit per component (YUV)

Can specify to write a simple progressive JPEG, default is not to
    Output Option: prog (write a simple progressive JPEG, default is not to)

Can specify the compression quality
    Output Option: quality=# (0 to 100, default 75)

Can specify the smoothing factor
    Output Option: smooth=# (0 to 100, default 0 - no smoothing)

Can specify to write an optimized JPEG
    Output Option: optimize (write optimized JPEG, default is not to)

Can specify the colour channel the output grey values are based on
    Output option: r,g,b,k (default: k, combine color channels and write grey equivalent)

Write additonal comment
    Output option: comment=text

Errors returned by JPEGv6d are passed through. Warnings are ignored
and GBM_ERR_OK is passed to the application. If DEBUG is set to
true, the Warnings are printed to stdout. Errors are not printed
to stdout. See my_output_message().

Since receiving the initial version from Martin, I've :-
    1. Incorporated it into the overall GBM source structure.
    2. Compiled with VisualAge C++ (OS/2), xlc (AIX), Visual C++ (Win32).
    3. Eliminated the fdopen problem via source/destination managers.
    4. Extended the file extensions list to include others I've seen.
    5. Made it a conditional part of the GBM build.
    6. Comply with GBM source conventions.
    7. Folded the source.
    8. Avoided placing IJG datastructures in the gbm.priv as this causes
       problems as to when the IJG datastructure should be cleaned up.
       Side effect: header is scanned twice.
    9. Fixed jpg_qft to return correct structure.

History:
--------
(Heiko Nitzsche)

22-Feb-2006: Move format description strings to gbmdesc.h
08-Sep-2006: * Add support for reading YCCK and CMYK encoded JPEGs
               (simple color space conversion only)
             * Add support for writing grayscale JPEGs (8bit) with r,g,b,k options
             * Add support for smoothing and optimized writing
             * Add support for writing a user comment
             * Undo hack in jmorecfg.h (changing oder of RGB out put from RGB to BGR),
               it is now back to the original order as this is required by TIFF decoder.
               The color component order is now swapped in the jpg codec.
15-Aug-2008: Integrate new GBM types
*/

/* ---------------------------------------- */

#ifdef ENABLE_IJG

#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "gbm.h"
#include "gbmhelp.h"
#include "gbmdesc.h"
#include "gbmmap.h"
#include "gbmmem.h"

#include "jpeglib.h"
/* #include "jinclude.h" */
#include "jerror.h"

/* Note: We include more than just jpeglib.h because we are implementing
   a source and a destination manager, which need to include the others. */

/* Error and warning messages generated by JPEGv6d are printf-like
   strings, thus one also needs the parameters to display the message
   correctly. The GBM-functions however only can return error-numbers
   (GBM_ERR). To be able to give a complete error message to the user,
   this module uses the following approach :-

   When an error occurs, the printf-like message and its paramters
   are expanded to a normal string. This string is saved in a
   array called "message buffer" that can hold a certain amount of
   such messages. Each array index in the message buffer corresponds
   to an error code of type GBM_ERR:
   error code = GBM_ERR_JPG_MESSAGE_0 + array index
   This error code is returned to the application. The application
   can obtain the message text by calling gbm_err() which in turn
   calls jpg_err(). [ jpg_err() calculates the message buffer array
   index by subracting GBM_ERR_JPG_MESSAGE_0 from the error code
   and returns the message text. ]
   This however can lead to a wrong message text if in the meantime
   a new JPEGv6 error message has been inserted to the message
   buffer at that same position.
   To avoid this as much as possible, new messages always replace
   the oldest message of the message buffer (see add_msg()). */

/* max. number of messages stored in the
   message buffer jpeg_error_messages[] */
#define JMSG_MAX 20

#define GBM_ERR_JPG_BAD_QUALITY   ((GBM_ERR) 1900)
#define GBM_ERR_JPG_BAD_BPP       ((GBM_ERR) 1901)
#define GBM_ERR_JPG_BAD_SMOOTH    ((GBM_ERR) 1902)
#define GBM_ERR_JPG_BAD_COMMENT   ((GBM_ERR) 1903)
#define GBM_ERR_JPG_MESSAGE_0     ((GBM_ERR) 1910)
#define GBM_ERR_JPG_MESSAGE_LAST  ((GBM_ERR) GBM_ERR_JPG_MESSAGE_0 + JMSG_MAX - 1)

typedef struct
{
    unsigned int age;
    char str[JMSG_LENGTH_MAX];
} MSG;

/* the message buffer: */
static MSG msgs[JMSG_MAX];

/* replace oldest message with new message */
static GBM_ERR add_msg(const char *msg)
{
    unsigned int max_age = 0;
    int i, max_idx = 0;

    /* search for the oldest message */
    for ( i = 0; i < JMSG_MAX; i++ )
        if ( ++msgs[i].age > max_age )
        {
            max_age = msgs[i].age;
            max_idx = i;
        }

    /* replace with new message */
    msgs[max_idx].age = 0;
    strncpy(msgs[max_idx].str, msg, JMSG_LENGTH_MAX);
    msgs[max_idx].str[JMSG_LENGTH_MAX-1] = '\0';

    return GBM_ERR_JPG_MESSAGE_0 + max_idx;
}

/* ---------------------------------------- */

typedef struct
{
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
} ERR;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
    ERR *e = (ERR *) cinfo->err;
    char buffer[JMSG_LENGTH_MAX];

    /* Create the message-string */
    (*cinfo->err->format_message)(cinfo, buffer);

    /* Return control to the setjmp point.
       add_msg() saves the message text in the message buffer.
       setjmp() will return apropriate GBM_ERR-code generated by add_msg().
       Later, jpg_err() will return the message text, if given that
       GBM_ERR-code. */
    longjmp(e->setjmp_buffer, add_msg(buffer));
}

/* ---------------------------------------- */

/*
 * Actual output of a warning or trace message.
 */

METHODDEF(void) my_output_message(j_common_ptr cinfo)
{
    char buffer[JMSG_LENGTH_MAX];

    /* Create the message */
    (*cinfo->err->format_message) (cinfo, buffer);
#if DEBUG
    /* Send it to stdout, adding a newline */
    printf("%s\n", buffer);
#endif
}

/* ---------------------------------------- */

/* GBM uses file descriptors not streams for data output,
   so implement a destination manager for use by the JPEG library. */

#define DBUFSIZE 4096

typedef struct
{
    struct jpeg_destination_mgr pub;
    int fd;
    JOCTET buf[DBUFSIZE];
} DST;

METHODDEF(void) init_destination(j_compress_ptr cinfo)
{
    DST *d = (DST *) cinfo->dest;
    d->pub.next_output_byte = d->buf;
    d->pub.free_in_buffer = DBUFSIZE;
}

METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo)
{
    DST *d = (DST *) cinfo->dest;
    if ( gbm_file_write(d->fd, d->buf, DBUFSIZE) != DBUFSIZE )
        ERREXIT(cinfo, JERR_FILE_WRITE);
    d->pub.next_output_byte = d->buf;
    d->pub.free_in_buffer = DBUFSIZE;
    return GBM_TRUE;
}

METHODDEF(void) term_destination(j_compress_ptr cinfo)
{
    DST *d = (DST *) cinfo->dest;
    int bytes = (int)(DBUFSIZE-d->pub.free_in_buffer);
    if ( bytes > 0 )
        if ( gbm_file_write(d->fd, d->buf, bytes) != bytes )
            ERREXIT(cinfo, JERR_FILE_WRITE);
    }

static void init_fd_dest(j_compress_ptr cinfo, int fd, DST *d)
{
    cinfo->dest = (struct jpeg_destination_mgr *) d;
    d->pub.init_destination    = init_destination;
    d->pub.empty_output_buffer = empty_output_buffer;
    d->pub.term_destination    = term_destination;
    d->fd                      = fd;
}

/* ---------------------------------------- */

/* GBM uses file descriptors not streams for data input,
   so implement a source manager for use by the JPEG library. */

#define    SBUFSIZE 4096

typedef struct
{
    struct      jpeg_source_mgr pub;
    int         fd;
    JOCTET      buf[SBUFSIZE];
    gbm_boolean start_of_file;
} SRC;

METHODDEF(void) init_source(j_decompress_ptr dinfo)
{
    SRC *s = (SRC *) dinfo->src;
    s->start_of_file = GBM_TRUE;
}

METHODDEF(boolean) fill_input_buffer(j_decompress_ptr dinfo)
{
    SRC *s = (SRC *) dinfo->src;
    int bytes;
    if ( (bytes = gbm_file_read(s->fd, s->buf, SBUFSIZE)) <= 0 )
    {
        if ( s->start_of_file )
            ERREXIT(dinfo, JERR_INPUT_EMPTY);
        s->buf[0] = (JOCTET) 0xff;
        s->buf[1] = (JOCTET) JPEG_EOI;
        bytes = 2;
    }
    s->pub.next_input_byte = s->buf;
    s->pub.bytes_in_buffer = bytes;
    s->start_of_file = GBM_FALSE;
    return GBM_TRUE;
}

METHODDEF(void) skip_input_data(j_decompress_ptr dinfo, long num_bytes)
{
    SRC *s = (SRC *) dinfo->src;
    if ( num_bytes > 0 )
    {
        while ( num_bytes > (long) s->pub.bytes_in_buffer )
        {
            num_bytes -= (long) s->pub.bytes_in_buffer;
            fill_input_buffer(dinfo);
        }
        s->pub.next_input_byte += (size_t) num_bytes;
        s->pub.bytes_in_buffer -= (size_t) num_bytes;
    }
}

METHODDEF(void) term_source(j_decompress_ptr dinfo)
{
    dinfo=dinfo; /* Suppress compiler warning */
}

static void init_fd_src(j_decompress_ptr dinfo, int fd, SRC *s)
{
    dinfo->src = (struct jpeg_source_mgr *) s;
    s->pub.init_source       = init_source;
    s->pub.fill_input_buffer = fill_input_buffer;
    s->pub.skip_input_data   = skip_input_data;
    s->pub.resync_to_restart = jpeg_resync_to_restart;
    s->pub.term_source       = term_source;
    s->pub.bytes_in_buffer   = 0;
    s->pub.next_input_byte   = NULL;
    s->fd                    = fd;
}

/* ---------------------------------------- */

static GBMFT jpg_gbmft =
{
        GBM_FMT_DESC_SHORT_JPG,
        GBM_FMT_DESC_LONG_JPG,
        GBM_FMT_DESC_EXT_JPG,
    GBM_FT_R8 | GBM_FT_R24 | GBM_FT_W8 | GBM_FT_W24,
};

GBM_ERR jpg_qft(GBMFT *gbmft)
{
    *gbmft = jpg_gbmft;
    return GBM_ERR_OK;
}

/* ---------------------------------------- */
/* ---------------------------------------- */

GBM_ERR jpg_rhdr(const char *fn, int fd, GBM *gbm, const char *opt)
{
    int jrc;
    struct jpeg_decompress_struct dinfo;
    ERR err;
    SRC src;
    gbm_boolean map_CMYK_to_BGR = GBM_FALSE;

    fn=fn; opt=opt; /* Suppress compiler warnings */

    /* Initialize the JPEG decompression object with default error handling. */
    dinfo.err = jpeg_std_error((struct jpeg_error_mgr *)&err);
    dinfo.err->output_message = my_output_message;
    dinfo.err->error_exit = my_error_exit;
    if ( (jrc = setjmp(err.setjmp_buffer)) != 0 )
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object and return.
         */
        jpeg_destroy_decompress(&dinfo);
        return jrc;
    }

    jpeg_create_decompress(&dinfo);

    /* Use a file descriptor based source manager */
    init_fd_src(&dinfo, fd, &src);

    /* Read file header, set default decompression parameters */
    (void) jpeg_read_header(&dinfo, GBM_TRUE);
    /* We can ignore the return value from jpeg_read_header since
     *   (a) suspension is not possible with the stdio data source, and
     *   (b) we passed GBM_TRUE to reject a tables-only JPEG file as an error.
     * See libjpeg.doc for more info.
     */

    /* fill in GBM structure */
#if DEBUG
    printf("image color-space = %d\n", dinfo.jpeg_color_space);
    printf("image components = %d\n", dinfo.num_components);
    printf("output color-space = %d\n", dinfo.out_color_space);
#endif

    /* check for supported color-spaces */
    map_CMYK_to_BGR = GBM_FALSE;
    switch(dinfo.out_color_space)
    {
        case JCS_RGB:
        case JCS_GRAYSCALE:
            /* we can handle these directly */
            break;

        case JCS_YCbCr:
            /* Request output as RGB */
            dinfo.out_color_space = JCS_RGB;
            break;

        case JCS_YCCK:
            /* Request output as CMYK, we do a simple conversion to BGR then. */
            dinfo.out_color_space = JCS_CMYK;
            map_CMYK_to_BGR = GBM_TRUE;
            break;

        case JCS_CMYK:
            /* We do a simple conversion to BGR then. */
            map_CMYK_to_BGR = GBM_TRUE;
            break;

        default:
            /* Request output as RGB. IJG will complain if it cannot convert. */
            dinfo.out_color_space = JCS_RGB;
            break;
    }

    /* Start decompressor */
    (void) jpeg_start_decompress(&dinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    /* We may need to do some setup of our own at this point before reading
     * the data.  After jpeg_start_decompress() we have the correct scaled
     * output image dimensions available, as well as the output colormap
     * if we asked for color quantization.
     */
    gbm->w   = dinfo.output_width;
    gbm->h   = dinfo.output_height;
    gbm->bpp = (map_CMYK_to_BGR ? 3 : dinfo.output_components) * 8;
#if DEBUG
    printf("output components = %d\n", dinfo.output_components);
    printf("gbm bpp           = %d\n", gbm->bpp);
#endif
    jpeg_destroy_decompress(&dinfo);
    return GBM_ERR_OK;
}

/* ---------------------------------------- */
/* ---------------------------------------- */

/* If there is only one component (i.e. 8bpp), we have a greyscale picture */

GBM_ERR jpg_rpal(int fd, GBM *gbm, GBMRGB *gbmrgb)
{
    fd=fd; /* Suppress compiler warning */
    if ( gbm->bpp == 8 )
    {
        int p;
        for ( p = 0; p < 0x100; p++ )
            gbmrgb[p].r =
            gbmrgb[p].g =
            gbmrgb[p].b = (gbm_u8) p;
    }
    return GBM_ERR_OK;
}

/* ---------------------------------------- */
/* ---------------------------------------- */

/* We re-read the header in order that the dinfo be set up correctly again.
   We can't expect it in the gbm.priv, as jpg_rhdr may be called without
   ever calling this routine. Who would clean up the dinfo in that case? */

GBM_ERR jpg_rdata(int fd, GBM *gbm, gbm_u8 *data)
{
    struct jpeg_decompress_struct dinfo;
    ERR err;
    SRC src;
    int jrc;
    gbm_boolean map_CMYK_to_BGR = GBM_FALSE;
    gbm_u8 * cmykData = NULL;

    gbm_file_lseek(fd, 0L, GBM_SEEK_SET);

    dinfo.err = jpeg_std_error((struct jpeg_error_mgr *)&err);
    dinfo.err->output_message = my_output_message;
    dinfo.err->error_exit = my_error_exit;
    if ( (jrc = setjmp(err.setjmp_buffer)) != 0 )
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object and return.
         */
        jpeg_destroy_decompress(&dinfo);

        /* Don't miss to free CMYK data buffer if there is one used! */
        gbmmem_free(cmykData);
        cmykData = NULL;

        return jrc;
    }

    jpeg_create_decompress(&dinfo);

    /* Use a file descriptor based source manager */
    init_fd_src(&dinfo, fd, &src);

    /* Read file header, set default decompression parameters */
    (void) jpeg_read_header(&dinfo, GBM_TRUE);

    /* check for supported color-spaces */
    map_CMYK_to_BGR = GBM_FALSE;
    switch(dinfo.out_color_space)
    {
        case JCS_RGB:
        case JCS_GRAYSCALE:
            /* we can handle these directly */
            break;

        case JCS_YCbCr:
            /* Request output as RGB */
            dinfo.out_color_space = JCS_RGB;
            break;

        case JCS_YCCK:
            /* Request output as CMYK, we do a simple conversion to BGR then. */
            dinfo.out_color_space = JCS_CMYK;
            map_CMYK_to_BGR = GBM_TRUE;
            break;

        case JCS_CMYK:
            /* We do a simple conversion to BGR then. */
            map_CMYK_to_BGR = GBM_TRUE;
            break;

        default:
            /* Request output as RGB. IJG will complain if it cannot convert. */
            dinfo.out_color_space = JCS_RGB;
            break;
    }

    /* Start decompressor */
    (void) jpeg_start_decompress(&dinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    /* Do we need a larger buffer for reading? */
    if (map_CMYK_to_BGR)
    {
        const size_t stride_src = dinfo.output_width * dinfo.output_components;
        const size_t stride_dst = (dinfo.output_width * 3 + 3) & ~3;
        GBM     gbm_src      = *gbm;
        gbm_u8 *c_data       = NULL;

        if (dinfo.output_components != 4)
        {
           /* This is an important step since it will release a good deal of memory. */
           jpeg_destroy_decompress(&dinfo);
           return GBM_ERR_JPG_BAD_BPP;
        }

        gbm_src.bpp = dinfo.output_components * 8;

        /* Create a local buffer for the CMYK data */
        cmykData = (gbm_u8 *) gbmmem_malloc(stride_src);
        if (cmykData == NULL)
        {
           /* This is an important step since it will release a good deal of memory. */
           jpeg_destroy_decompress(&dinfo);
           return GBM_ERR_MEM;
        }

        /* Process data */
        c_data = data + stride_dst * (dinfo.output_height - 1);

        /* Here we use the library's state variable dinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        while ( dinfo.output_scanline < dinfo.output_height )
        {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            int num_scanlines;
            JSAMPROW sarray[1];  /* array of pointers to rows */
            sarray[0] = cmykData;
            num_scanlines = jpeg_read_scanlines(&dinfo, sarray, 1);

            /* convert CMYK data to BGR */
            if (! gbm_map_row_CMYK_to_BGR(cmykData, &gbm_src,
                                          c_data  ,  gbm    , GBM_FALSE))
            {
                /* This is an important step since it will release a good deal of memory. */
                jpeg_destroy_decompress(&dinfo);

                /* Don't miss to free CMYK data buffer! */
                gbmmem_free(cmykData);
                cmykData = NULL;

                return GBM_ERR_JPG_BAD_BPP;
            }

            c_data -= stride_dst * num_scanlines;
        }

        /* Don't miss to free CMYK data buffer! */
        gbmmem_free(cmykData);
        cmykData = NULL;
    }
    else
    {
        const size_t stride = ((dinfo.output_width * dinfo.output_components + 3) & ~3);

        /* Process data */
        gbm_u8 * c_data = data + stride * (dinfo.output_height - 1);

        if (dinfo.output_components > 3)
        {
            /* This is an important step since it will release a good deal of memory. */
            jpeg_destroy_decompress(&dinfo);
            return GBM_ERR_JPG_BAD_BPP;
        }

        /* Here we use the library's state variable dinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        while ( dinfo.output_scanline < dinfo.output_height )
        {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            int num_scanlines;
            JSAMPROW sarray[1];  /* array of pointers to rows */
            sarray[0] = c_data;
            num_scanlines = jpeg_read_scanlines(&dinfo, sarray, 1);

            /* convert RGB data to BGR */
            if (dinfo.output_components == 3)
            {
                if (! gbm_map_row_RGBx_BGRx(c_data, gbm,
                                            c_data, gbm,
                                            NULL, GBM_FALSE))
                {
                    /* This is an important step since it will release a good deal of memory. */
                    jpeg_destroy_decompress(&dinfo);
                    return GBM_ERR_JPG_BAD_BPP;
                }
            }

            c_data -= stride * num_scanlines;
        }
    }

    (void) jpeg_finish_decompress(&dinfo);
    /* We can ignore the return value since suspension is not possible
     * with the file descriptor data source.
     */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&dinfo);

    /* At this point you may want to check to see whether any corrupt-data
     * warnings occurred (test whether err.pub.num_warnings is nonzero).
     */
#if DEBUG
    printf("jpg_rdata: num_warnings=%ld\n",err.pub.num_warnings);
#endif
    return GBM_ERR_OK;
}

/* ---------------------------------------- */
/* ---------------------------------------- */

static gbm_boolean make_output_palette(const GBMRGB gbmrgb[], gbm_u8 grey[], const char *opt)
{
  const gbm_boolean  k = ( gbm_find_word(opt, "k") != NULL );
  const gbm_boolean  r = ( gbm_find_word(opt, "r") != NULL );
  const gbm_boolean  g = ( gbm_find_word(opt, "g") != NULL );
  const gbm_boolean  b = ( gbm_find_word(opt, "b") != NULL );
  int i;

#define  SW4(a,b,c,d)   ((a)*8+(b)*4+(c)*2+(d))

  switch ( SW4(k,r,g,b) )
  {
    case SW4(0,0,0,0):
      /* Default is the same as "k" */
    case SW4(1,0,0,0):
      for ( i = 0; i < 0x100; i++ )
      {
        grey[i] = (gbm_u8) ( ((gbm_u16) gbmrgb[i].r *  77U +
                              (gbm_u16) gbmrgb[i].g * 150U +
                              (gbm_u16) gbmrgb[i].b *  29U) >> 8 );
      }
      return GBM_TRUE;

    case SW4(0,1,0,0):
      for ( i = 0; i < 0x100; i++ )
      {
        grey[i] = gbmrgb[i].r;
      }
      return GBM_TRUE;

    case SW4(0,0,1,0):
      for ( i = 0; i < 0x100; i++ )
      {
        grey[i] = gbmrgb[i].g;
      }
      return GBM_TRUE;

    case SW4(0,0,0,1):
      for ( i = 0; i < 0x100; i++ )
      {
        grey[i] = gbmrgb[i].b;
      }
      return GBM_TRUE;
  }
  return GBM_FALSE;
}

/* ---------------------------------------- */

GBM_ERR jpg_w(const char *fn, int fd, const GBM *gbm, const GBMRGB *gbmrgb, const gbm_u8 *data, const char *opt)
{
    int jrc;
    int quality = 75;
    int smooth  = 0;
    size_t stride  = 0;
    const char *s      = NULL;
    const char *index  = NULL;
    const gbm_u8 *c_data = NULL;
          gbm_u8 *t_data = NULL;
    struct jpeg_compress_struct cinfo;
    ERR err;
    DST dst;

    fn=fn; gbmrgb=gbmrgb; /* Suppress compiler warnings */

    switch(gbm->bpp)
    {
        case 24:
        case 8:
           stride = (gbm->w * (gbm->bpp/8) + 3) & ~3;
           break;

        default:
           return GBM_ERR_NOT_SUPP;
    }

    c_data = data + stride * (gbm->h - 1);

    if ( (index = gbm_find_word_prefix(opt, "quality=")) != NULL )
    {
        sscanf(index + 8, "%d", &quality);
        if ( quality < 0 || quality > 100 )
        {
            return GBM_ERR_JPG_BAD_QUALITY;
        }
    }

    if ( (index = gbm_find_word_prefix(opt, "smooth=")) != NULL )
    {
        sscanf(index + 7, "%d", &smooth);
        if ( smooth < 0 || smooth > 100 )
        {
            return GBM_ERR_JPG_BAD_SMOOTH;
        }
    }

    /* Initialize the JPEG compression object with default error handling. */
    cinfo.err = jpeg_std_error((struct jpeg_error_mgr *)&err);
    cinfo.err->output_message = my_output_message;
    cinfo.err->error_exit = my_error_exit;
    if ( (jrc = setjmp(err.setjmp_buffer)) != 0 )
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object and return.
         */
        jpeg_destroy_compress(&cinfo);

        /* Don't forget to cleanup temporary conversion buffer. */
        gbmmem_free(t_data);
        t_data = NULL;

        return jrc;
    }

    jpeg_create_compress(&cinfo);

    init_fd_dest(&cinfo, fd, &dst);

    /* First we supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    cinfo.image_width      = gbm->w;                       /* image width and height, in pixels */
    cinfo.image_height     = gbm->h;
    cinfo.input_components = gbm->bpp / 8;                 /* # of color components per pixel */
    cinfo.in_color_space   = (cinfo.input_components == 3) /* colorspace of input image       */
                             ? JCS_RGB : JCS_GRAYSCALE;

    /* Now use the library's routine to set default compression parameters.
     * (You must set at least cinfo.in_color_space before calling this,
     * since the defaults depend on the source color space.)
     */
    jpeg_set_defaults(&cinfo);

    /* Now you can set any non-default parameters you wish to.
     * Here we just illustrate the use of quality (quantization table) scaling:
     */
    jpeg_set_quality(&cinfo, quality, GBM_TRUE /* limit to baseline-JPEG values */);

    /* Optionally allow huffman table optimization (needs a second pass -> slower). */
    if ( gbm_find_word(opt, "optimize") != NULL )
    {
        cinfo.optimize_coding = GBM_TRUE;
    }

    /* smoothing factor: 0 is off */
    cinfo.smoothing_factor = smooth;

    /* Optionally allow simple progressive output. */
    if ( gbm_find_word(opt, "prog") != NULL )
    {
        jpeg_simple_progression(&cinfo);
    }

    /* GBM_TRUE ensures that we will write a complete interchange-JPEG file.
     * Pass GBM_TRUE unless you are very sure of what you're doing.
     */
    jpeg_start_compress(&cinfo, GBM_TRUE);

    /* Write a comment to the "COM" marker */
    if ((s = gbm_find_word_prefix(opt, "comment=")) != NULL)
    {
       char comment_text[1000+1] = { 0 };

       if (sscanf(s + 8, "%1000[^\"]", comment_text) != 1)
       {
          if (sscanf(s + 8, "%1000[^ ]", comment_text) != 1)
          {
             jpeg_destroy_compress(&cinfo);
             return GBM_ERR_JPG_BAD_COMMENT;
          }
       }

       jpeg_write_marker(&cinfo, JPEG_COM,
                        (const JOCTET *)comment_text, (unsigned int)strlen(comment_text));
    }

    /* We need to convert BGR to RGB data */
    if (cinfo.input_components == 3)
    {
       /* Create a local buffer for the BGR->RGB data conversion */
       t_data = (gbm_u8 *) gbmmem_malloc(stride);
       if (t_data == NULL)
       {
          jpeg_destroy_compress(&cinfo);
          return GBM_ERR_MEM;
       }

       /* Here we use the library's state variable cinfo.next_scanline as the
        * loop counter, so that we don't have to keep track ourselves.
        * To keep things simple, we pass one scanline per call; you can pass
        * more if you wish, though.
        */
       while ( cinfo.next_scanline < cinfo.image_height)
       {
           /* jpeg_write_scanlines expects an array of pointers to scanlines.
            * Here the array is only one element long, but you could pass
            * more than one scanline at a time if that's more convenient.
            */
           int num_scanlines;
           JSAMPROW row_pointer[1];  /* pointer to JSAMPLE row[s] */

           /* convert BGR data to RGB */
           if (! gbm_map_row_RGBx_BGRx(c_data, gbm,
                                       t_data, gbm,
                                       NULL, GBM_FALSE))
           {
               jpeg_destroy_compress(&cinfo);
               /* Don't forget to cleanup temporary conversion buffer. */
               gbmmem_free(t_data);
               t_data = NULL;
               return GBM_ERR_WRITE;
           }

           row_pointer[0] = (gbm_u8 *) t_data;
           num_scanlines  = jpeg_write_scanlines(&cinfo, row_pointer, 1);
           c_data        -= stride * num_scanlines;
       }

       /* Don't forget to cleanup temporary conversion buffer. */
       gbmmem_free(t_data);
       t_data = NULL;
    }
    else /* Grayscale */
    {
       int  j;
       gbm_u8 grey[0x100];

       /* Create a local buffer for the data conversion to gray */
       t_data = (gbm_u8 *) gbmmem_malloc(stride);
       if (t_data == NULL)
       {
          jpeg_destroy_compress(&cinfo);
          return GBM_ERR_MEM;
       }

       if (! make_output_palette(gbmrgb, grey, opt))
       {
          gbmmem_free(t_data);
          t_data = NULL;
          jpeg_destroy_compress(&cinfo);
          return GBM_ERR_BAD_OPTION;
       }

       /* Here we use the library's state variable cinfo.next_scanline as the
        * loop counter, so that we don't have to keep track ourselves.
        * To keep things simple, we pass one scanline per call; you can pass
        * more if you wish, though.
        */
       while ( cinfo.next_scanline < cinfo.image_height)
       {
           int num_scanlines;
           JSAMPROW row_pointer[1];  /* pointer to JSAMPLE row[s] */

           /* convert to gray or extract the color component requested */
           for ( j = 0; j < gbm->w; j++ )
           {
              t_data[j] = grey[c_data[j]];
           }

           /* jpeg_write_scanlines expects an array of pointers to scanlines.
            * Here the array is only one element long, but you could pass
            * more than one scanline at a time if that's more convenient.
            */
           row_pointer[0] = (gbm_u8 *) t_data;
           num_scanlines  = jpeg_write_scanlines(&cinfo, row_pointer, 1);
           c_data        -= stride * num_scanlines;
       }

       /* Don't forget to cleanup temporary conversion buffer. */
       gbmmem_free(t_data);
       t_data = NULL;
    }

    jpeg_finish_compress(&cinfo);

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    return GBM_ERR_OK;
}

/* ---------------------------------------- */

const char *jpg_err(GBM_ERR rc)
{
    switch ( rc )
    {
        case GBM_ERR_JPG_BAD_QUALITY:
            return "quality is not in 0..100";

        case GBM_ERR_JPG_BAD_SMOOTH:
            return "smooth is not in 0..100";

        case GBM_ERR_JPG_BAD_BPP:
            return "unsupported bpp";

        case GBM_ERR_JPG_BAD_COMMENT:
            return "comment could not be parsed";
    }
    if ( rc >= GBM_ERR_JPG_MESSAGE_0 &&
         rc <= GBM_ERR_JPG_MESSAGE_LAST )
        return msgs[rc-GBM_ERR_JPG_MESSAGE_0].str;
    else
        return NULL;
}

#else

char jpg_missing;

#endif

