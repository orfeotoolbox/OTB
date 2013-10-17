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
//  $Id: ossimIoStream.cpp 22423 2013-09-27 17:36:26Z gpotts $
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
#ifdef _MSC_VER

  ossimIFStream64::ossimIFStream64(const char* pFilename, std::ios_base::openmode mode, int prot) :
      std::ifstream(theFile = std::_Fiopen(pFilename, mode, prot))
   {
   }

   ossimIFStream64::~ossimIFStream64()
   {
      if(is_open())
      {
         close();
      }
   }

   void ossimIFStream64::seekg64(off_type off, ios_base::seekdir way)
   {
      _fseeki64(theFile, off, way);
   }

   void ossimIFStream64::seekg64(streampos pos, ios_base::seekdir way)
   {
      // Undo the potentially bad typecast done by _FPOSOFF when fpos is > max long
      const off_type off(pos);
      const fpos_t fpos = pos.seekpos();
      seekg64(off - _FPOSOFF(fpos) + fpos, way);
   }

   void ossimIFStream64::seekg64(std::istream& str, off_type off, 
                                 ios_base::seekdir way)
   {
      ossimIFStream64* pStream = dynamic_cast<ossimIFStream64*>(&str);
      if (pStream != NULL)
      {
         pStream->seekg64(off, way);
      }
      else
      {
         str.seekg(off, way);
      }
   }

   void ossimIFStream64::seekg64(std::istream& str, 
                                 std::streampos pos, 
                                 ios_base::seekdir way)
   {
      ossimIFStream64* pStream = dynamic_cast<ossimIFStream64*>(&str);
      if (pStream != NULL)
      {
         pStream->seekg64(pos, way);
      }
      else
      {
         str.seekg(pos, way);
      }
   }

   ossimOFStream64::ossimOFStream64(const char* pFilename, 
      std::ios_base::openmode mode, 
      int prot) :
      std::ofstream(pFilename, mode, prot)
   {
   }

   ossimOFStream64::~ossimOFStream64()
   {
      if(is_open())
      {
         close();
      }
   }

   ossim_uint64 ossimOFStream64::tellp64()
   {
      // Undo the potentially bad typecast done by _FPOSOFF when fpos is > max long
      const pos_type pos = tellp();
      const off_type off(pos);
      const fpos_t fpos = pos.seekpos();
      return off - _FPOSOFF(fpos) + fpos;
   }

#else

   ossimIFStream64::ossimIFStream64(const char* pFilename, 
      std::ios_base::openmode mode, long prot) :
      std::ifstream(pFilename, mode)
   {
   }

   ossimIFStream64::~ossimIFStream64()
   {
      if(is_open())
      {
         close();
      }
   }

   void ossimIFStream64::seekg64(off_type off, ios_base::seekdir way)
   {
      std::ifstream::seekg(off, way);
   }

   void ossimIFStream64::seekg64(std::istream& str, 
                                 off_type off, ios_base::seekdir way)
   {
      str.seekg(off, way);
   }

   ossimOFStream64::ossimOFStream64(const char* pFilename, 
      std::ios_base::openmode mode, long prot) :
      std::ofstream(pFilename, mode)
   {
   }

   ossimOFStream64::~ossimOFStream64()
   {
      if(is_open())
      {
         close();
      }
   }

   ossim_uint64 ossimOFStream64::tellp64()
   {
      return tellp();
   }

#endif // _MSC_VER

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
