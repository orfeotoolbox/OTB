#ifndef ossimEnvironmentUtility_HEADER
#define ossimEnvironmentUtility_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>

class OSSIM_DLL ossimEnvironmentUtility
{
public:
   ossimEnvironmentUtility();
   static ossimEnvironmentUtility* instance();

   ossimString getEnvironmentVariable(const ossimString& variable)const;

   ossimFilename getUserDir()const;
   
   ossimFilename getUserOssimSupportDir()const;
   ossimFilename getUserOssimPreferences()const;
   ossimFilename getUserOssimPluginDir()const;
   
   ossimFilename getInstalledOssimSupportDir()const;
   ossimFilename getInstalledOssimPluginDir()const;
   ossimFilename getInstalledOssimPreferences()const;
   
protected:
   static ossimEnvironmentUtility* theInstance;
};


#endif
