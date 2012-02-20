#include <ossim/base/ossimHttpResponse.h>

RTTI_DEF1(ossimHttpResponse, "ossimHttpResponse", ossimWebResponse);

void ossimHttpResponse::convertHeaderStreamToKeywordlist()
{
   m_headerKwl.clear();
   ossimByteStreamBuffer buf(m_headerBuffer);
   
   std::istream in(&buf);
   in.seekg(0); // make sure we are at the begining
   // skip method type
   //
   std::string statusLine;
   std::getline(in, statusLine);
   m_statusLine = statusLine;
   
   m_headerKwl.parseStream(in);
   std::vector<ossimString> statusLineArray;
   m_statusLine.split(statusLineArray, " ");
   if(statusLineArray.size() > 1)
   {
      m_statusCode = statusLineArray[1].toUInt32();
   }
   else
   {
      m_statusCode = 0;
   }
}
