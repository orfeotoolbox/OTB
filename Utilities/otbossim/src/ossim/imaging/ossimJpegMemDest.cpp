//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Most of code and comments below are from jpeg-6b "example.c" file. See 
// http://www4.cs.fau.de/Services/Doc/graphics/doc/jpeg/libjpeg.html
//
// Author:  Oscar Kramer (From example by Thomas Lane)
//----------------------------------------------------------------------------
// $Id$

#include <ossim/imaging/ossimJpegMemDest.h>
#include <cstdlib> /* free, malloc */

/* *** Custom destination manager for JPEG writer *** */

typedef struct 
{
   struct jpeg_destination_mgr pub; /* public fields */
   std::ostream* stream;		/* target stream */
   JOCTET* buffer;		/* start of buffer */
} cpp_dest_mgr;

#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */

void init_destination (j_compress_ptr cinfo)
{
   cpp_dest_mgr* dest = (cpp_dest_mgr*) cinfo->dest;

   /* Allocate the output buffer --- it will be released when done with image */
   dest->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));

   dest->pub.next_output_byte = dest->buffer;
   dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

boolean empty_output_buffer (j_compress_ptr cinfo)
{
   cpp_dest_mgr* dest = (cpp_dest_mgr*) cinfo->dest;
   dest->stream->write ((char*)dest->buffer, OUTPUT_BUF_SIZE);

   dest->pub.next_output_byte = dest->buffer;
   dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

   return TRUE;
}


void term_destination (j_compress_ptr cinfo)
{
   cpp_dest_mgr* dest = (cpp_dest_mgr*) cinfo->dest;
   size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

   /* Write any data remaining in the buffer */
   if (datacount > 0) 
      dest->stream->write ((char*)dest->buffer, datacount);

   dest->stream->flush();
   free (cinfo->dest);
}

void jpeg_cpp_stream_dest (j_compress_ptr cinfo, std::ostream& stream)
{
   cpp_dest_mgr* dest;

   /* first time for this JPEG object? */
   if (cinfo->dest == NULL) 
      cinfo->dest = (struct jpeg_destination_mgr *) malloc (sizeof(cpp_dest_mgr));

   dest = (cpp_dest_mgr*) cinfo->dest;
   dest->pub.init_destination = init_destination;
   dest->pub.empty_output_buffer = empty_output_buffer;
   dest->pub.term_destination = term_destination;
   dest->stream = &stream;
}
