#ifndef ossimIoStream_HEADER
#define ossimIoStream_HEADER
#include "ossimProtocolStream.h"
#include <iostream>
#include <fstream>
#include <sstream>


class OSSIM_DLL ossimIStream : public ossimProtocolStream, public std::istream
{
public:
   ossimIStream():std::istream((std::streambuf*)0){}
   ossimIStream(std::streambuf* buf):ossimProtocolStream(buf),std::istream(buf)
   {}
   void init(std::streambuf* buf)
   {
      ossimProtocolStream::init(buf);
      std::istream::init(buf);
   }
TYPE_DATA
};

class OSSIM_DLL ossimOStream : public ossimProtocolStream, public std::ostream
{
public:
   ossimOStream():std::ostream((std::streambuf*)0){}
   ossimOStream(std::streambuf* buf):ossimProtocolStream(buf),std::ostream(buf)
   {}
   void init(std::streambuf* buf)
   {
      ossimProtocolStream::init(buf);
      std::ostream::init(buf);
   }
   
TYPE_DATA
};

class OSSIM_DLL ossimIOStream : public ossimProtocolStream, public std::iostream
{
public:
   ossimIOStream():ossimProtocolStream((std::streambuf*)0),std::iostream((std::streambuf*)0){}
   ossimIOStream(std::streambuf* buf):ossimProtocolStream(buf),std::iostream(buf)
   {}
   void init(std::streambuf* buf)
   {
      ossimProtocolStream::init(buf);
      std::iostream::init(buf);
   }

TYPE_DATA      
};

class OSSIM_DLL ossimIOMemoryStream : public ossimIOStream
{
protected:
   std::stringbuf theBuf;
   
public:
   ossimIOMemoryStream()
      :theBuf(std::ios::in|std::ios::out)
   {
      ossimIOStream::init(&theBuf);
   }
   bool is_open()const
   {
      return true;
   }
   virtual void open(const char* protocolString,
                     int openMode)
   {
   }
   ossimString str()
   {
      return theBuf.str();
   }
   virtual void close()
   {}

   ossim_uint32 size()const
   {
      ossimIOMemoryStream*  thisPtr = const_cast<ossimIOMemoryStream*>(this);
      std::streampos pos = thisPtr->tellg();
      thisPtr->seekg(0, std::ios::end);
      std::streampos endPos = thisPtr->tellg();
      thisPtr->seekg(pos, std::ios::beg);
      
      return (ossim_uint32)(endPos);
   }
   virtual bool isCompressed()const
   {
      return false;
   }
};

class OSSIM_DLL ossimIMemoryStream : public ossimIStream
{
protected:
   std::stringbuf theBuf;
   
public:
   ossimIMemoryStream(const ossimString& inputBuf)
      :theBuf(inputBuf.c_str(),
              std::ios::in)
   {
      ossimIStream::init(&theBuf);
   }
   bool is_open()const
   {
      return true;
   }
   ossim_uint32 size()const
   {
      ossimIMemoryStream*  thisPtr = const_cast<ossimIMemoryStream*>(this);
      std::streampos pos = thisPtr->tellg();
      thisPtr->seekg(0, std::ios::end);
      std::streampos endPos = thisPtr->tellg();
      thisPtr->seekg(pos, std::ios::beg);
      return (ossim_uint32)(endPos);
   }
   virtual void open(const char* protocolString,
                     int openMode)
   {
   }
   virtual void close(){}
   ossimString str()
   {
      return theBuf.str();
   }
   virtual bool isCompressed()const
   {
      return false;
   }
};

class OSSIM_DLL ossimOMemoryStream : public ossimOStream
{
protected:
   std::stringbuf theBuf;
   
public:
   ossimOMemoryStream()
      :theBuf(std::ios::out)
   {
      ossimOStream::init(&theBuf);
   }
   bool is_open()const
   {
      return true;
   }
   ossim_uint32 size()const
   {
      ossimOMemoryStream*  thisPtr = const_cast<ossimOMemoryStream*>(this);
      std::streampos pos = thisPtr->tellp();
      thisPtr->seekp(0, std::ios::end);
      std::streampos endPos = thisPtr->tellp();
      thisPtr->seekp(pos, std::ios::beg);
      return (ossim_uint32)(endPos);
   }
   virtual void open(const char* protocolString,
                     int openMode)
   {
   }
   virtual void close()
   {}

   ossimString str()
   {
      return theBuf.str();
   }
   virtual bool isCompressed()const
   {
      return false;
   }
};

class OSSIM_DLL ossimIOFStream : public ossimIOStream
{
protected:
   std::filebuf theBuf;
   
public:
   ossimIOFStream()
   {
      ossimIOStream::init(&theBuf);
   }
   ossimIOFStream(const char* name,
                 int openMode=std::ios::in|std::ios::out|std::ios::binary)
   {
      ossimIOStream::init(&theBuf);
      open(name, openMode);
   }
   virtual ~ossimIOFStream()
   {
      if(is_open())
      {
         close();
      }
   }
   
   std::filebuf* rdbuf()
   {
      return &theBuf;
   }

   virtual void open(const char* name,
                 int openMode=std::ios::in|std::ios::out|std::ios::binary)
   {
      if(!theBuf.open(name, (std::ios::openmode)openMode))
      {
         this->setstate(std::ios::failbit);
      }
   }
   virtual void close()
   {
      if(!theBuf.close())
      {
         this->setstate(std::ios::failbit);
      }
   }
   virtual bool is_open()const
   {
      return theBuf.is_open();
   }
   virtual bool isCompressed()const
   {
      return false;
   }
TYPE_DATA   
};

class OSSIM_DLL ossimIFStream : public ossimIStream
{
protected:
   std::filebuf theBuf;
   
public:
   ossimIFStream()
   {
      ossimIStream::init(&theBuf);
   }
   ossimIFStream(const char* name,
                 int openMode=std::ios::in|std::ios::binary)
   {
      ossimIStream::init(&theBuf);
      open(name, openMode);
   }
   virtual ~ossimIFStream()
   {
      if(is_open())
      {
         close();
      }
   }
   
   std::filebuf* rdbuf() { return &theBuf; }
   virtual void open(const char* name,
                     int openMode = std::ios::in|std::ios::binary)
   {
      if(!theBuf.open(name, (std::ios::openmode)openMode))
      {
         this->setstate(std::ios::failbit);
      }
   }
   virtual void close()
   {
      if(!theBuf.close())
      {
         this->setstate(std::ios::failbit);
      }
   }
   virtual bool is_open()const
   {
      return theBuf.is_open();
   }
   virtual bool isCompressed()const
   {
      return false;
   }
TYPE_DATA   
};

class OSSIM_DLL ossimOFStream : public ossimOStream
{
protected:
   std::filebuf theBuf;
   
public:
   ossimOFStream()
   {
      ossimOStream::init(&theBuf);
   }
   ossimOFStream(const char* name,
                 int openMode=std::ios::out)
   {
      ossimOStream::init(&theBuf);
      open(name, openMode);
   }
   virtual ~ossimOFStream()
   {
      if(is_open())
      {
         close();
      }
   }
   std::filebuf* rdbuf() { return &theBuf; }
   virtual void open(const char* name,
                     int openMode = std::ios::out)
   {
      if(!theBuf.open(name, (std::ios::openmode)openMode))
      {
         this->setstate(std::ios::failbit);
      }
   }
   virtual void close()
   {
      if(!theBuf.close())
      {
         this->setstate(std::ios::failbit);
      }
   }
   virtual bool is_open()const
   {
      return theBuf.is_open();
   }
   virtual bool isCompressed()const
   {
      return false;
   }
TYPE_DATA   
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
