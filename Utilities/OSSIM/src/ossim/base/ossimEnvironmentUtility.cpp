#include <ossim/base/ossimEnvironmentUtility.h>
#include <stdlib.h>

#if defined(WIN32) && !defined(__CYGWIN__)
#define OSSIM_ENVIRONEMENT_UTILITY_UNIX 0
#else
#define OSSIM_ENVIRONEMENT_UTILITY_UNIX 1
#endif

ossimEnvironmentUtility* ossimEnvironmentUtility::theInstance=0;

ossimEnvironmentUtility::ossimEnvironmentUtility()
{
   theInstance = this;
}

ossimEnvironmentUtility* ossimEnvironmentUtility::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimEnvironmentUtility;
   }

   return theInstance;
}

ossimString ossimEnvironmentUtility::getEnvironmentVariable(const ossimString& variable)const
{
   return ossimString(getenv(variable.c_str()));
}

ossimFilename ossimEnvironmentUtility::getUserOssimSupportDir()const
{
   ossimFilename result = getUserDir();
   
#if OSSIM_ENVIRONEMENT_UTILITY_UNIX
#   ifdef __APPLE__
   result = result.dirCat("Library/Application Support/ossim");
   
#   else
   result = result.dirCat(".ossim");
#   endif
#else
   result = result.dirCat("Application Data\\ossim");
#endif
   
   return result;
}

ossimFilename ossimEnvironmentUtility::getUserDir()const
{
   ossimFilename result;

#if OSSIM_ENVIRONEMENT_UTILITY_UNIX
   result = ossimFilename(getEnvironmentVariable("HOME"));
#else
   result =ossimFilename(getEnvironmentVariable("USERPROFILE"));
#endif

   return result;
}

ossimFilename ossimEnvironmentUtility::getUserOssimPreferences()const
{
   ossimFilename result = getUserOssimSupportDir();

   if(result == "")
   {
      return "";
   }

   result = result.dirCat("preferences");
   
   return result;
}

ossimFilename ossimEnvironmentUtility::getUserOssimPluginDir()const
{
   ossimFilename result = getUserOssimSupportDir();

   if(result != "")
   {
      result = result.dirCat("plugins");
   }
   
   return result;
}

ossimFilename ossimEnvironmentUtility::getInstalledOssimSupportDir()const
{
   ossimFilename result;
#if OSSIM_ENVIRONEMENT_UTILITY_UNIX
#   ifdef __APPLE__
   result = "/Library/Application Support/ossim";
#   else
   result = "/usr/share/ossim";
   if(!result.exists())
   {
      result = "/usr/local/share/ossim";
   }
#   endif
#else
   // NEED the windows test here.
#endif
   if(result != "")
   {
      if(!result.exists())
      {
         result = "";
      }
   }

   return result;
}

ossimFilename ossimEnvironmentUtility::getInstalledOssimPluginDir()const
{
   ossimFilename result = getInstalledOssimSupportDir();

   //Need generic unix plugin location
#if OSSIM_ENVIRONEMENT_UTILITY_UNIX
#   ifndef __APPLE__
   return "";
#   endif
#endif
   if((result!="")&&result.exists())
   {
      result = result.dirCat("plugins");
   }
   else
   {
      result = "";
   }

   return result;
}

ossimFilename ossimEnvironmentUtility::getInstalledOssimPreferences()const
{
   ossimFilename result = getInstalledOssimSupportDir();

   
   if((result!="")&&result.exists())
   {
      result = result.dirCat("preferences");
   }
   else
   {
      result = "";
   }

   if(!result.exists())
   {
      result = "";
   }
   
   return result;
   
}
