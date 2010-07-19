//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken, original code from Thomas G. Lane (memsrc.c)
//
// Description:
// Code to use jpeg-6b library to read jpeg image from memory.
//----------------------------------------------------------------------------
// $Id$

//---
// Original code from Thomas G. Lane, header comment follows.
//---

/*
 * memsrc.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains decompression data source routines for the case of
 * reading JPEG data from a memory buffer that is preloaded with the entire
 * JPEG file.  This would not seem especially useful at first sight, but
 * a number of people have asked for it.
 * This is really just a stripped-down version of jdatasrc.c.  Comparison
 * of this code with jdatasrc.c may be helpful in seeing how to make
 * custom source managers for other purposes.
 */

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
/* #include <jinclude.h> */

#include <ossim/imaging/ossimJpegMemSrc.h>
#include <jerror.h>
extern "C"
{
void ossimJpegErrorExit (j_common_ptr cinfo)
{
   /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
   ossimJpegErrorPtr myerr = (ossimJpegErrorPtr) cinfo->err;
   
   /* Always display the message. */
   /* We could postpone this until after returning, if we chose. */
   (*cinfo->err->output_message) (cinfo);
   
   /* Return control to the setjmp point */
   longjmp(myerr->setjmp_buffer, 1);
}


/* Expanded data source object for memory input */

struct ossimJpegSourceMgr
{
   struct jpeg_source_mgr pub;	/* public fields */
   JOCTET eoi_buffer[2];	/* a place to put a dummy EOI */
};

typedef ossimJpegSourceMgr* ossimJpegSourceMgrPtr;


/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

void init_source (j_decompress_ptr /* cinfo */)
{
  /* No work, since jpeg_memory_src set up the buffer pointer and count.
   * Indeed, if we want to read multiple JPEG images from one buffer,
   * this *must* not do anything to the pointer.
   */
}


/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In this application, this routine should never be called; if it is called,
 * the decompressor has overrun the end of the input buffer, implying we
 * supplied an incomplete or corrupt JPEG datastream.  A simple error exit
 * might be the most appropriate response.
 *
 * But what we choose to do in this code is to supply dummy EOI markers
 * in order to force the decompressor to finish processing and supply
 * some sort of output image, no matter how corrupted.
 */
boolean fill_input_buffer (j_decompress_ptr cinfo)
{
  ossimJpegSourceMgrPtr src = (ossimJpegSourceMgrPtr) cinfo->src;

  WARNMS(cinfo, JWRN_JPEG_EOF);

  /* Create a fake EOI marker */
  src->eoi_buffer[0] = (JOCTET) 0xFF;
  src->eoi_buffer[1] = (JOCTET) JPEG_EOI;
  src->pub.next_input_byte = src->eoi_buffer;
  src->pub.bytes_in_buffer = 2;

  return TRUE;
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * If we overrun the end of the buffer, we let fill_input_buffer deal with
 * it.  An extremely large skip could cause some time-wasting here, but
 * it really isn't supposed to happen ... and the decompressor will never
 * skip more than 64K anyway.
 */
void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  ossimJpegSourceMgrPtr src = (ossimJpegSourceMgrPtr) cinfo->src;

  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}


/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */
void term_source (j_decompress_ptr /* cinfo */)
{
  /* no work necessary here */
}


/*
 * Prepare for input from a memory buffer.
 */
void ossimJpegMemorySrc (j_decompress_ptr cinfo,
                         const JOCTET * buffer,
                         std::size_t bufsize)
{
  ossimJpegSourceMgrPtr src;

  /* The source object is made permanent so that a series of JPEG images
   * can be read from a single buffer by calling jpeg_memory_src
   * only before the first one.
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(ossimJpegSourceMgr));
  }

  src = (ossimJpegSourceMgrPtr) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;

  src->pub.next_input_byte = buffer;
  src->pub.bytes_in_buffer = bufsize;
}
}

