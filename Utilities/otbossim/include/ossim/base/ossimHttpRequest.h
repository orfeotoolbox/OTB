#ifndef ossimHttpRequest_HEADER
#define ossimHttpRequest_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimWebRequest.h>
#include <ossim/base/ossimHttpResponse.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>

class OSSIM_DLL ossimHttpRequest :public ossimWebRequest
{
public:
   enum HttpMethodType
   {
      HTTP_METHOD_UNKNOWN = 0,
      HTTP_METHOD_GET     = 1,
      HTTP_METHOD_POST    = 2
      // later we will add support for post
   };
   ossimHttpRequest()
   :m_methodType(HTTP_METHOD_UNKNOWN)
   {
   }
   bool set(const ossimUrl& url, 
                         const ossimKeywordlist& headerOptions, 
                         HttpMethodType methodType=HTTP_METHOD_GET)
   {
      bool result = setUrl(url);
      m_headerOptions = headerOptions;
      m_methodType    = methodType;
      
      return result;
   }
   virtual ~ossimHttpRequest()
   {
   }
   void setHeaderOptions(const ossimKeywordlist& options)
   {
      m_headerOptions = options;
   }
   void addHeaderOption(const ossimString& name, const ossimString& value)
   {
      m_headerOptions.add(name, value);
   }
   void clearHeaderOptions(){m_headerOptions.clear();}
   ossimKeywordlist& getHeaderOptions(){return m_headerOptions;}
   const ossimKeywordlist& getHeaderOptions()const{return m_headerOptions;}
   
   
   virtual ossimString getLastError()const{return m_lastError;}
   
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   
protected:
   ossimKeywordlist m_headerOptions;
   HttpMethodType   m_methodType;
   TYPE_DATA;
};
#endif
