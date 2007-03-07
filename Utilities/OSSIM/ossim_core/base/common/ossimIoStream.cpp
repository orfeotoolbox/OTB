#include "ossimIoStream.h"

RTTI_DEF1(ossimIStream, "ossimIProtocolStream", ossimProtocolStream);
RTTI_DEF1(ossimOStream, "ossimOProtocolStream", ossimProtocolStream);
RTTI_DEF1(ossimIOStream, "ossimOProtocolStream", ossimProtocolStream);

RTTI_DEF1(ossimIFStream, "ossimIFStream", ossimIStream);
RTTI_DEF1(ossimOFStream, "ossimOFStream", ossimOStream);
RTTI_DEF1(ossimIOFStream, "ossimIOFStream", ossimIOStream);

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
