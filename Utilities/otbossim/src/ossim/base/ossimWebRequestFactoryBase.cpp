#include <ossim/base/ossimWebRequestFactoryBase.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimHttpRequest.h>

#if 0
ossimHttpRequest* ossimWebRequestFactoryBase::createHttpRequest(const ossimUrl& url,
                                            const ossimString& method)
{
   ossimKeywordlist kwl;
   kwl["type"] = url.getProtocol().c_str();
   kwl["method"] = method.c_str();
   ossimRefPtr<ossimObject> httpRequestResult = createObject(kwl);
   ossimRefPtr<ossimHttpRequest> result = dynamic_cast<ossimHttpRequest*>(httpRequestResult.get());

   return result.release();
}
#endif
