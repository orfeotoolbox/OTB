#include <ossim/base/ossimHttpRequest.h>

RTTI_DEF1(ossimHttpRequest, "ossimHttpRequest", ossimWebRequest);

bool ossimHttpRequest::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   bool result = true;
   ossimObject::loadState(kwl, prefix);
   ossimString url = kwl.find(prefix, "url");
   ossimString method = kwl.find(prefix, "method");
   method = method.upcase();
   
   if(method == "GET")
   {
      m_methodType = HTTP_METHOD_GET;
   }
   else if(method == "POST")
   {
      m_methodType = HTTP_METHOD_POST;
   }
   else
   {
      m_methodType = HTTP_METHOD_UNKNOWN;
   }
   
   
   clearHeaderOptions();
   
   return result;
}
