#include <ossim/base/ossimWebRequestFactoryRegistry.h>
#include <ossim/base/ossimHttpRequest.h>

ossimWebRequestFactoryRegistry* ossimWebRequestFactoryRegistry::m_instance = 0;

ossimWebRequestFactoryRegistry::ossimWebRequestFactoryRegistry()
{
   m_instance = this;
}

ossimWebRequestFactoryRegistry* ossimWebRequestFactoryRegistry::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimWebRequestFactoryRegistry();
   }
   
   return m_instance;
}


ossimObject* ossimWebRequestFactoryRegistry::createObject(const ossimKeywordlist& kwl,
                                                          const char* prefix)const
{
  return createObjectFromRegistry(kwl, prefix);
}

ossimWebRequest* ossimWebRequestFactoryRegistry::create(const ossimUrl& url)
{
   ossim_uint32 idx = 0;
   ossimWebRequest* result = 0;
   for(idx = 0; ((idx < m_factoryList.size())&&!result); ++idx)
   {
      result = m_factoryList[idx]->create(url);
   }
   
   return result;
}

ossimHttpRequest* ossimWebRequestFactoryRegistry::createHttp(const ossimUrl& url)
{
   ossimRefPtr<ossimWebRequest> request = create(url);
   ossimRefPtr<ossimHttpRequest> result;
   if(request.valid())
   {
      result = dynamic_cast<ossimHttpRequest*>(request.get());
      request = 0;
   }
   
   return result.release();
}