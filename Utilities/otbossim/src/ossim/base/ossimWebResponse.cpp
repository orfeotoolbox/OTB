#include <ossim/base/ossimWebResponse.h>

RTTI_DEF1(ossimWebResponse, "ossimWebResponse" , ossimObject);


void ossimWebResponse::copyAllDataFromInputStream(ByteBuffer& buffer)
{
   std::istream* input = getInputStream();
   
   if(input)
   {
      input->seekg(0, std::ios::beg);
      char buf[1024];
      while(!input->eof()&&input->good())
      {
         input->read(buf, 1024);
         int gcount = input->gcount();
         if(gcount)
         {
            buffer.insert(buffer.end(), buf , buf + gcount);
         }
      }
   }
}
