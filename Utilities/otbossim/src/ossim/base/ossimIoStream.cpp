//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
// 
// Class definitiaons for:
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
//  $Id: ossimIoStream.cpp 11206 2007-06-13 13:11:35Z gpotts $
#include <ossim/base/ossimIoStream.h>

ossimIStream::ossimIStream()
   : ossimStreamBase(),
#ifdef _MSC_VER
   std::istream((std::_Uninitialized)0)
#else
   std::istream()
#endif
{}

ossimIStream::ossimIStream(std::streambuf* sb)
   : ossimStreamBase(),
     std::istream(sb)
{}

ossimIStream::~ossimIStream()
{}

ossimOStream::ossimOStream()
   : ossimStreamBase(),
#ifdef _MSC_VER
   std::ostream((std::_Uninitialized)0)
#else
   std::ostream()
#endif
{}

ossimOStream::ossimOStream(std::streambuf* sb)
   : ossimStreamBase(),
     std::ostream(sb)
{}

ossimOStream::~ossimOStream()
{}

ossimIOStream::ossimIOStream()
   : ossimStreamBase(),
#ifdef _MSC_VER
   std::iostream((std::_Uninitialized)0)
#else
   std::iostream()
#endif
{}

ossimIOStream::~ossimIOStream()
{}

ossimIOMemoryStream::ossimIOMemoryStream()
   : ossimIOStream(),
     theBuf(std::ios::in|std::ios::out)
{
   ossimIOStream::init(&theBuf);
}

ossimIOMemoryStream::~ossimIOMemoryStream()
{
}

bool ossimIOMemoryStream::is_open()const
{
   return true;
}

void ossimIOMemoryStream::open(const char* /* protocolString */,
                               int /* openMode */)
{
}

ossimString ossimIOMemoryStream::str()
{
   return theBuf.str();
}

void ossimIOMemoryStream::close()
{}

ossim_uint64 ossimIOMemoryStream::size()const
{
   ossimIOMemoryStream*  thisPtr = const_cast<ossimIOMemoryStream*>(this);
   std::streampos pos = thisPtr->tellg();
   thisPtr->seekg(0, std::ios::end);
   std::streampos endPos = thisPtr->tellg();
   thisPtr->seekg(pos, std::ios::beg);
   
   return (ossim_uint64)(endPos);
}

ossimIMemoryStream::ossimIMemoryStream(const ossimString& inputBuf)
   
   : ossimIStream(),
     theBuf(inputBuf.c_str(), std::ios::in)
{
   ossimIStream::init(&theBuf);
}

ossimIMemoryStream::~ossimIMemoryStream()
{
}

bool ossimIMemoryStream::is_open()const
{
   return true;
}

ossim_uint64 ossimIMemoryStream::size()const
{
   ossimIMemoryStream*  thisPtr = const_cast<ossimIMemoryStream*>(this);
   std::streampos pos = thisPtr->tellg();
   thisPtr->seekg(0, std::ios::end);
   std::streampos endPos = thisPtr->tellg();
   thisPtr->seekg(pos, std::ios::beg);
   return (ossim_uint64)(endPos);
}

void ossimIMemoryStream::open(const char* /* protocolString */,
                              int /* openMode */ )
{
}

void ossimIMemoryStream::close()
{}

ossimString ossimIMemoryStream::str()
{
   return theBuf.str();
}

ossimOMemoryStream::ossimOMemoryStream()
   : ossimOStream(),
     theBuf(std::ios::out)
{
   ossimOStream::init(&theBuf);
}

ossimOMemoryStream::~ossimOMemoryStream()
{
}

bool ossimOMemoryStream::is_open()const
{
   return true;
}

ossim_uint64 ossimOMemoryStream::size()const
{
   ossimOMemoryStream*  thisPtr = const_cast<ossimOMemoryStream*>(this);
   std::streampos pos = thisPtr->tellp();
   thisPtr->seekp(0, std::ios::end);
   std::streampos endPos = thisPtr->tellp();
   thisPtr->seekp(pos, std::ios::beg);
   return (ossim_uint64)(endPos);
}

void ossimOMemoryStream::open(const char* /* protocolString */,
                              int /* openMode */ )
{
}

void ossimOMemoryStream::close()
{}

ossimString ossimOMemoryStream::str()
{
   return theBuf.str();
}

ossimIOFStream::ossimIOFStream()
   : ossimStreamBase(),
     std::fstream()
{
}

ossimIOFStream::ossimIOFStream(const char* name,
                               std::ios_base::openmode mode)
   : ossimStreamBase(),
     std::fstream(name, mode)
{
}

ossimIOFStream::~ossimIOFStream()
{
}

ossimIFStream::ossimIFStream()
   : ossimStreamBase(),
     std::ifstream()
{
}
ossimIFStream::ossimIFStream(const char* file, std::ios_base::openmode mode)
   : ossimStreamBase(),
     std::ifstream(file, mode)
{
}

ossimIFStream::~ossimIFStream()
{
}

ossimOFStream::ossimOFStream()
   : ossimStreamBase(),
     std::ofstream()
{
}

ossimOFStream::ossimOFStream(const char* name, std::ios_base::openmode mode)
   : ossimStreamBase(),
     std::ofstream(name, mode)
{
}

ossimOFStream::~ossimOFStream()
{
}

void operator >> (ossimIStream& in,ossimOStream& out)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }
}

ossimIOStream& operator >> (ossimIStream& in,ossimIOStream& out)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }
   
   return out;
}

void operator >> (ossimIOStream& in,ossimOStream& out)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }
}

ossimIOStream& operator >> (ossimIOStream& in,ossimIOStream& out)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }

   return out;
}

void operator << (ossimOStream& out, ossimIStream& in)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }
}

void operator << (ossimOStream& out, ossimIOStream& in)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }
}

ossimIOStream& operator << (ossimIOStream& out, ossimIStream& in)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }

   return out;
}

ossimIOStream& operator << (ossimIOStream& out, ossimIOStream& in)
{
   char buf[1024];
   bool done = false;

   while(!done&&!in.fail())
   {
      in.read(buf, 1024);
      if(in.gcount() < 1024)
      {
         done = true;
      }
      if(in.gcount() > 0)
      {
         out.write(buf, in.gcount());
      }
   }

   return out;
}
