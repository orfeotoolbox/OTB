//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
// 
// Class declarations for:
//
// ossimIStream
// ossimOStream
// ossimIOStream
// ossimIOMemoryStream
// ossimIMemoryStream
// ossimOMemoryStream
// ossimIOFStream
// ossimIFStream
// ossimOFStream
//
//*******************************************************************
//  $Id: ossimIoStream.h 11176 2007-06-07 19:45:56Z dburken $
#ifndef ossimIoStream_HEADER
#define ossimIoStream_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimStreamBase.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ossim/base/ossimString.h>


class OSSIM_DLL ossimIStream : public ossimStreamBase, public std::istream   
{
public:
   ossimIStream();
   ossimIStream(std::streambuf* sb);
   virtual ~ossimIStream();
};


class OSSIM_DLL ossimOStream : public ossimStreamBase, public std::ostream
{
public:
   ossimOStream();
   ossimOStream(std::streambuf* sb);   
   virtual ~ossimOStream();
};

class OSSIM_DLL ossimIOStream : public ossimStreamBase, public std::iostream
{
public:
   ossimIOStream();
   virtual ~ossimIOStream();
};

class OSSIM_DLL ossimIOMemoryStream : public ossimIOStream
{
public:
   ossimIOMemoryStream();

   virtual ~ossimIOMemoryStream();

   // ??? (drb)
   bool is_open()const;

   // ??? (drb)
   virtual void open(const char* /* protocolString */,
                     int /* openMode */);

   ossimString str();

   // ??? (drb)
   virtual void close();

   // ??? (drb) std::streamsize
   ossim_uint64 size()const;

protected:
   std::stringbuf theBuf;
};

class OSSIM_DLL ossimIMemoryStream : public ossimIStream
{
public:
   
   ossimIMemoryStream(const ossimString& inputBuf);
   
   virtual ~ossimIMemoryStream();
   
   bool is_open()const;
   
   ossim_uint64 size()const;
   
   virtual void open(const char* /* protocolString */,
                     int /* openMode */ );

   virtual void close();
   
   ossimString str();

protected:
   std::stringbuf theBuf;
   
};

class OSSIM_DLL ossimOMemoryStream : public ossimOStream
{
public:
   ossimOMemoryStream();
   virtual ~ossimOMemoryStream();   

   bool is_open()const;

   ossim_uint64 size()const;

   virtual void open(const char* /* protocolString */,
                     int /* openMode */ );

   virtual void close();

   ossimString str();

protected:
   std::stringbuf theBuf;
};

class OSSIM_DLL ossimIOFStream : public ossimStreamBase, public std::fstream
{
public:
   ossimIOFStream();

   ossimIOFStream(const char* name,
                  std::ios_base::openmode mode =
                  std::ios_base::in | std::ios_base::out);

   virtual ~ossimIOFStream();
};

class OSSIM_DLL ossimIFStream : public ossimStreamBase, public std::ifstream
{
public:
   ossimIFStream();
   
   ossimIFStream(const char* file,
                 std::ios_base::openmode mode = std::ios_base::in);

   virtual ~ossimIFStream();

};

class OSSIM_DLL ossimOFStream : public ossimStreamBase, public std::ofstream
{
public:
   ossimOFStream();

   ossimOFStream(const char* name,
                 std::ios_base::openmode mode =
                 std::ios_base::out|std::ios_base::trunc);

   virtual ~ossimOFStream();

};

OSSIM_DLL void operator >> (ossimIStream& in,ossimOStream& out);
OSSIM_DLL ossimIOStream& operator >> (ossimIStream& in,ossimIOStream& out);
OSSIM_DLL void operator >> (ossimIOStream& in,ossimOStream& out);
OSSIM_DLL ossimIOStream& operator >> (ossimIOStream& in,ossimIOStream& out);
OSSIM_DLL void operator << (ossimOStream& out, ossimIStream& in);
OSSIM_DLL void operator << (ossimOStream& out, ossimIOStream& in);
OSSIM_DLL ossimIOStream& operator << (ossimIOStream& out, ossimIStream& in);
OSSIM_DLL ossimIOStream& operator << (ossimIOStream& out, ossimIOStream& in);


#endif
