// ============================================================================
// gzstream, C++ iostream classes wrapping the zlib compression library.
// Copyright (C) 2001  Deepak Bandyopadhyay, Lutz Kettner
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ============================================================================
//
// File          : gzstream.C
// Revision      : $Revision: 17195 $
// Revision_date : $Date: 2010-04-23 19:32:18 +0200 (ven. 23 avril 2010) $
// Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
// 
// Standard streambuf implementation following Nicolai Josuttis, "The 
// Standard C++ Library".
// ============================================================================

#include <ossim/base/ossimGzStream.h>

#if OSSIM_HAS_LIBZ
#include <zlib.h>

#include <iostream>
#include <fstream>
#include <cstring>  // for memcpy


// --------------------------------------
// class ossimGzStreamBuf:
// --------------------------------------


struct ossimGzStreamBuf::PrivateData
{
	gzFile           file;               // file handle for compressed file
};
// ----------------------------------------------------------------------------
// Internal classes to implement gzstream. See header file for user classes.
// ----------------------------------------------------------------------------
ossimGzStreamBuf::ossimGzStreamBuf()
   : prvtData(new PrivateData()),
     opened(false),
     mode(0) // ??? (drb)
{

   setp( buffer, buffer + (bufferSize-1));
   setg( buffer + 4,     // beginning of putback area
         buffer + 4,     // read position
         buffer + 4);    // end position      
   // ASSERT: both input & output capabilities will not be used together
}

ossimGzStreamBuf::~ossimGzStreamBuf()
{
   close();
   if(prvtData)
   {
	   delete prvtData;
	   prvtData = 0;
   }
}

bool ossimGzStreamBuf::is_open() const
{
   return opened;
}

ossimGzStreamBuf* ossimGzStreamBuf::open( const char* name, int open_mode)
{
    if ( is_open())
    {
        return (ossimGzStreamBuf*)0;
    }
    mode = open_mode;
    // no append nor read/write mode
    if ((mode & std::ios::ate) || (mode & std::ios::app)
        || ((mode & std::ios::in) && (mode & std::ios::out)))
    {
        return (ossimGzStreamBuf*)0;
    }
    
    char  fmode[10];
    char* fmodeptr = fmode;
    if ( mode & std::ios::in)
    {
       *fmodeptr++ = 'r';
    }
    else if ( mode & std::ios::out)
    {
        *fmodeptr++ = 'w';
    }
    *fmodeptr++ = 'b';
    *fmodeptr = '\0';
    prvtData->file = gzopen( name, fmode);
    if (prvtData->file == 0)
    {
       return (ossimGzStreamBuf*)0;
    }
    opened = true;
    return this;
}

ossimGzStreamBuf * ossimGzStreamBuf::close()
{
   if ( is_open())
   {
      sync();
      opened = false;
      if ( gzclose( prvtData->file) == Z_OK)
      {
         return this;
      }
    }
    return (ossimGzStreamBuf*)0;
}

std::streamsize ossimGzStreamBuf::xsgetn(char_type* __s,
                                         std::streamsize n)
{
   int num = gzread( prvtData->file, __s, n);

   if (num <= 0) // ERROR or EOF
      return EOF;
   
   return num;
}

int ossimGzStreamBuf::underflow()
{
   // used for input buffer only
   if ( gptr() && ( gptr() < egptr()))
      return * reinterpret_cast<unsigned char *>( gptr());

   if ( ! (mode & std::ios::in) || ! opened)
        return EOF;
    // Josuttis' implementation of inbuf
    int n_putback = gptr() - eback();
    if ( n_putback > 4)
        n_putback = 4;
    memcpy( buffer + (4 - n_putback), gptr() - n_putback, n_putback);

    int num = gzread( prvtData->file, buffer+4, bufferSize-4);
    if (num <= 0) // ERROR or EOF
        return EOF;

    // reset buffer pointers
    setg( buffer + (4 - n_putback),   // beginning of putback area
          buffer + 4,                 // read position
          buffer + 4 + num);          // end of buffer

    // return next character
    return * reinterpret_cast<unsigned char *>( gptr());
}

int ossimGzStreamBuf::flush_buffer()
{
    // Separate the writing of the buffer from overflow() and
    // sync() operation.
    int w = pptr() - pbase();
    if ( gzwrite( prvtData->file, pbase(), w) != w)
        return EOF;
    pbump( -w);
    return w;
}

int ossimGzStreamBuf::overflow( int c) { // used for output buffer only
    if ( ! ( mode & std::ios::out) || ! opened)
        return EOF;
    if (c != EOF) {
        *pptr() = c;
        pbump(1);
    }
    if ( flush_buffer() == EOF)
        return EOF;
    return c;
}

int ossimGzStreamBuf::sync() {
    // Changed to use flush_buffer() instead of overflow( EOF)
    // which caused improper behavior with std::endl and flush(),
    // bug reported by Vincent Ricard.
    if ( pptr() && pptr() > pbase()) {
        if ( flush_buffer() == EOF)
            return -1;
    }
    return 0;
}

ossimGzStreamBuf::pos_type ossimGzStreamBuf::seekoff(off_type t,
                                                     std::ios_base::seekdir dir,
                                                     std::ios_base::openmode /* omode */)
{
   int whence = 0;
   switch(dir)
   {
      case std::ios::beg:
      {
         whence = SEEK_SET;
         break;
      }
      case std::ios::end:
      {
         whence = SEEK_END;
         break;
      }
      case std::ios::cur:
      {
         whence = SEEK_CUR;
         break;
      }
      default:
      {
         whence = SEEK_CUR;
         break;
      }
   }

   return gzseek(prvtData->file, t, whence);
}

// ossimGzStreamBuf::pos_type ossimGzStreamBuf::seekpos(pos_type posType, 
//                                                      std::ios_base::openmode)
// {
//    int whence = 0;
//    switch(posType)
//    {
//       case std::ios::beg:
//       {
//          whence = SEEK_SET;
//          break;
//       }
//       case std::ios::end:
//       {
//          whence = SEEK_END;
//          break;
//       }
//       case std::ios::cur:
//       {
//          whence = SEEK_CUR;
//          break;
//       }
//       default:
//       {
//          whence = SEEK_CUR;
//          break;
//       }
//    }
   
//    return gzseek(file, t, whence);
// }

// --------------------------------------
// class ossimGzStreamBase:
// --------------------------------------


ossimIgzStream::ossimIgzStream()
   : ossimIFStream()
{
   init(&buf);
}

ossimIgzStream::ossimIgzStream( const char* name,
                                std::ios_base::openmode mode )
   : ossimIFStream()
{
   init(&buf);
   open(name, mode);
}

ossimIgzStream::~ossimIgzStream()
{
   buf.close();
}

ossimGzStreamBuf* ossimIgzStream::rdbuf()
{
   return &buf;
}

void ossimIgzStream::open( const char* name,
                           std::ios_base::openmode mode )
{
   if ( ! buf.open( name, mode))
   {
      clear( rdstate() | std::ios::badbit);
   }
}

void ossimIgzStream::close()
{
   if ( buf.is_open())
   {
      if ( !buf.close())
      {
         clear( rdstate() | std::ios::badbit);
      }
   }
}

bool ossimIgzStream::is_open()const
{
   return buf.is_open();
}

bool ossimIgzStream::isCompressed()const
{
   return true;
}

ossimOgzStream::ossimOgzStream()
   : ossimOFStream()
{
   init(&buf);
}

ossimOgzStream::ossimOgzStream( const char* /* name */,
                                std::ios_base::openmode /* mode */)
   : ossimOFStream()
{
   init(&buf);
}

ossimOgzStream::~ossimOgzStream()
{
   buf.close();
}

ossimGzStreamBuf* ossimOgzStream::rdbuf()
{
   return &buf;
}

void ossimOgzStream::open( const char* name,
                           std::ios_base::openmode mode )
{
   if ( ! buf.open( name, mode))
   {
      clear( rdstate() | std::ios::badbit);
   }
}

void ossimOgzStream::close()
{
   if ( buf.is_open())
   {
      if ( !buf.close())
      {
         clear( rdstate() | std::ios::badbit);
      }
   }
}

bool ossimOgzStream::is_open()const
{
   return buf.is_open();
}

bool ossimOgzStream::isCompressed()const
{
   return true;
}

// ============================================================================
// EOF //
#endif // OSSIM_HAS_LIBZ
