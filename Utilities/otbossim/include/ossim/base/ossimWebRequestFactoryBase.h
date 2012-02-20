#ifndef ossimWebRequestFactoryBase_HEADER
#define ossimWebRequestFactoryBase_HEADER
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/base/ossimHttpRequest.h>
#include <ossim/base/ossimKeywordlist.h>
/**
 * We will for now hard code to be ossimHttpRequest but later
 * we will refactor and add a base ossimWebRequest so we can handle
 * other schema definitions such as ftp, scp, smtp, ... etc.
 */
class OSSIM_DLL ossimWebRequestFactoryBase : public ossimBaseObjectFactory
{
public:
   virtual ossimWebRequest* create(const ossimUrl& /*url*/)
   {
      return 0;
   }
};
#endif
