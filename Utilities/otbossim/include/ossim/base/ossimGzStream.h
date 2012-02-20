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
// File          : gzstream.h
// Revision      : $Revision: 13050 $
// Revision_date : $Date: 2008-06-19 20:07:35 +0200 (jeu., 19 juin 2008) $
// Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
// 
// Standard streambuf implementation following Nicolai Josuttis, "The 
// Standard C++ Library".
// ============================================================================

#ifndef ossimGzStream_HEADER
#define ossimGzStream_HEADER

// standard C++ with new header file names and std:: namespace
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/ossimConfig.h>
#include <iostream>
#include <fstream>
#include <ossim/base/ossimIoStream.h>

#if OSSIM_HAS_LIBZ
// ----------------------------------------------------------------------------
// Internal classes to implement gzstream. See below for user classes.
// ----------------------------------------------------------------------------

class OSSIM_DLL ossimGzStreamBuf : public std::streambuf
{

public:
   ossimGzStreamBuf();

   virtual ~ossimGzStreamBuf();
   
   bool is_open()const;
   ossimGzStreamBuf* open( const char* name, int open_mode);
   ossimGzStreamBuf* close();
      
   virtual int overflow( int c = EOF);
   
   // will not use  buffer for get.
   virtual std::streamsize xsgetn(char_type* __s, std::streamsize n);
   virtual int     underflow();
   virtual int     sync();
   virtual pos_type seekoff(off_type t, std::ios_base::seekdir dir,
                            std::ios_base::openmode omode = std::ios_base::in |
                            std::ios_base::out);
/*     virtual pos_type seekpos(pos_type posType,  */
/*                              std::ios_base::openmode __mode = std::ios_base::in | */
/*                              std::ios_base::out); */
   
private:
	struct PrivateData;
   int flush_buffer();

   static const int bufferSize = 303; // 47+256 size of data buff
   // totals 512 bytes under g++ for igzstream at the end.
   PrivateData*     prvtData;
   char             buffer[bufferSize]; // data buffer
   bool             opened;             // open/close state of stream
   int              mode;               // I/O mode

}; // End of class ossimGzStreamBuf

/* class OSSIM_DLL ossimGzStreamBase : virtual public ossimProtocolStream */
/* { */
/* protected: */
/*     ossimGzStreamBuf buf; */
    
/* public: */
/*     ossimGzStreamBase():ossimProtocolStream(&buf) {  } */
/*     ossimGzStreamBase( const char* name, int open_mode); */
/*     virtual ~ossimGzStreamBase(); */
/*     virtual void open( const char* name, int open_mode); */
/*     virtual void close(); */
/*     ossimGzStreamBuf* rdbuf() { return &buf; } */
    
/* TYPE_DATA     */
/* }; */

//class OSSIM_DLL ossimIgzStream : public ossimGzStreamBase, public std::istream
class OSSIM_DLL ossimIgzStream : public ossimIFStream
{
public:
   ossimIgzStream();
   ossimIgzStream( const char* name,
                   std::ios_base::openmode mode = std::ios_base::in);
   virtual ~ossimIgzStream();
   ossimGzStreamBuf* rdbuf();
   
   virtual void open( const char* name,
                      std::ios_base::openmode mode = std::ios_base::in);
   
   virtual void close();
   virtual bool is_open()const;
   virtual bool isCompressed()const;

protected:
    ossimGzStreamBuf buf;
   
}; // End of class ossimIgzStream

class OSSIM_DLL ossimOgzStream : public ossimOFStream
{
public:
   ossimOgzStream();
   ossimOgzStream( const char* name,
                   std::ios_base::openmode mode =
                   std::ios_base::out|std::ios_base::trunc );
   virtual ~ossimOgzStream();
   
   ossimGzStreamBuf* rdbuf();
   void open( const char* name,
              std::ios_base::openmode mode =
              std::ios_base::out|std::ios_base::trunc );
   virtual void close();
   virtual bool is_open()const;
   virtual bool isCompressed()const;

protected:
   ossimGzStreamBuf buf;

}; // End of class ossimOgzStream

#endif /* #if OSSIM_HAS_LIBZ */
   
#endif /* #define ossimGzStream_HEADER */
