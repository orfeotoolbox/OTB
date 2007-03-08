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
// Revision      : $Revision: 9097 $
// Revision_date : $Date: 2006-06-13 22:57:27 +0200 (mar, 13 jun 2006) $
// Author(s)     : Deepak Bandyopadhyay, Lutz Kettner
// 
// Standard streambuf implementation following Nicolai Josuttis, "The 
// Standard C++ Library".
// ============================================================================

#ifndef ossimGzStream_HEADER
#define ossimGzStream_HEADER 1

// standard C++ with new header file names and std:: namespace
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/ossimConfig.h>
#include <iostream>
#include <fstream>
#include <ossim/base/ossimIoStream.h>

#if OSSIM_HAS_LIBZ
#include <zlib.h>
// ----------------------------------------------------------------------------
// Internal classes to implement gzstream. See below for user classes.
// ----------------------------------------------------------------------------

class OSSIM_DLL ossimGzStreamBuf : public std::streambuf {
private:
    static const int bufferSize = 47+256;    // size of data buff
    // totals 512 bytes under g++ for igzstream at the end.

    gzFile           file;               // file handle for compressed file
    char             buffer[bufferSize]; // data buffer
    bool             opened;             // open/close state of stream
    int              mode;               // I/O mode

    int flush_buffer();
public:
    ossimGzStreamBuf() : opened(false) {
        setp( buffer, buffer + (bufferSize-1));
        setg( buffer + 4,     // beginning of putback area
              buffer + 4,     // read position
              buffer + 4);    // end position      
        // ASSERT: both input & output capabilities will not be used together
    }
    bool is_open()const { return opened; }
    ossimGzStreamBuf* open( const char* name, int open_mode);
    ossimGzStreamBuf* close();
    ~ossimGzStreamBuf() { close(); }
    
    virtual int overflow( int c = EOF);

    // will not use  buffer for get.
    virtual std::streamsize xsgetn(char_type* __s,
                                   std::streamsize n);
//    virtual int     underflow();
    virtual int     sync();
    virtual pos_type seekoff(off_type t, std::ios_base::seekdir dir,
                             std::ios_base::openmode omode = std::ios_base::in |
                             std::ios_base::out);
/*     virtual pos_type seekpos(pos_type posType,  */
/*                              std::ios_base::openmode __mode = std::ios_base::in | */
/*                              std::ios_base::out); */
};

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
class OSSIM_DLL ossimIgzStream : public ossimIStream
{
protected:
    ossimGzStreamBuf buf;
    
public:
    ossimIgzStream() :ossimIStream(&buf){} 
    ossimIgzStream( const char* name, int openMode = std::ios::in)
       : ossimIStream(&buf)
    {
       open(name, openMode);
    }
    virtual ~ossimIgzStream()
    {
       buf.close();
    }
    ossimGzStreamBuf* rdbuf() { return &buf; }
    
    virtual void open( const char* name, int open_mode = std::ios::in);
    virtual void close();
    virtual bool is_open()const
    {
       return buf.is_open();
    }
   virtual bool isCompressed()const
   {
      return true;
   }
TYPE_DATA    
};

class OSSIM_DLL ossimOgzStream : public ossimOStream
{
protected:
    ossimGzStreamBuf buf;
    
public:
    ossimOgzStream(){init(&buf);}
    ossimOgzStream( const char* name, int mode = std::ios::out)
        : ossimOStream(&buf) {}  
    virtual ~ossimOgzStream()
    {
       buf.close();
    }
    ossimGzStreamBuf* rdbuf() { return &buf; }
    void open( const char* name, int openMode = std::ios::out);
    virtual void close();
    virtual bool is_open()const
    {
       return buf.is_open();
    }
   virtual bool isCompressed()const
   {
      return true;
   }
TYPE_DATA    
};

#endif
   
#endif // GZSTREAM_H

