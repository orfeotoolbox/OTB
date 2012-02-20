#ifndef ossimWebRequest_HEADER
#define ossimWebRequest_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimUrl.h>
#include <ossim/base/ossimWebResponse.h>

class OSSIM_DLL ossimWebRequest : public ossimObject
{
public:
   ossimWebRequest(){}
   ossimWebRequest(const ossimUrl& url)
   :m_url(url)
   {}
   const ossimUrl& getUrl()const{return m_url;}
   virtual bool setUrl(const ossimUrl& url){m_url = url; return true;}
   void clearLastError(){m_lastError = "";}
   const ossimString getLastError(){return m_lastError;}
   virtual ossimWebResponse* getResponse(){return 0;}
   
protected:
   ossimUrl m_url;
   mutable ossimString m_lastError;
   
   TYPE_DATA;
};
#endif
