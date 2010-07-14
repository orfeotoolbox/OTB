#include <ossim/base/ossimEnvironmentUtility.h>
#include <cstdlib>

#if defined(_WIN32) && !defined(__CYGWIN__)
#  define OSSIM_ENVIRONMENT_UTILITY_UNIX 0
#  include <direct.h>
#else
#  define OSSIM_ENVIRONMENT_UTILITY_UNIX 1
#endif

ossimEnvironmentUtility* ossimEnvironmentUtility::theInstance=0;

ossimEnvironmentUtility::ossimEnvironmentUtility()
{
   ossimFilename dir = getUserOssimPluginDir();
   
   if(!dir.empty())
   {
      thePluginSearchPath.push_back(dir);
   }
   
   dir = getInstalledOssimPluginDir();
   if(!dir.empty())
   {   
      thePluginSearchPath.push_back(dir);
   }

   
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
   ossimString result;
   char* lookup = std::getenv(variable.c_str());
   // getenv returns NULL if not found.
   if (lookup)
   {
      result = (const char*)lookup;
   }
   return result;
}

ossimFilename ossimEnvironmentUtility::getUserOssimSupportDir()const
{
   ossimFilename result = getUserDir();
   
#if OSSIM_ENVIRONMENT_UTILITY_UNIX
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

ossimString   ossimEnvironmentUtility::getUserName()const
{
#if OSSIM_ENVIRONMENT_UTILITY_UNIX
   return getEnvironmentVariable("USER");
#else
   return getEnvironmentVariable("USERNAME");
#endif
}

ossimFilename ossimEnvironmentUtility::getUserDir()const
{
   ossimFilename result;

#if OSSIM_ENVIRONMENT_UTILITY_UNIX
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
#if OSSIM_ENVIRONMENT_UTILITY_UNIX
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
#if OSSIM_ENVIRONMENT_UTILITY_UNIX
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

ossimFilename ossimEnvironmentUtility::getCurrentWorkingDir()const
{
   ossimFilename result;

#if OSSIM_ENVIRONMENT_UTILITY_UNIX
   result = getEnvironmentVariable("PWD");
#else
   char buf[512];
   _getcwd(buf, 512);
   result = buf;
#endif

   return result;
}

ossimFilename ossimEnvironmentUtility::searchAllPaths(const ossimFilename& file)const
{
   ossimFilename result;

   result = findPlugin(file);
   if(!result.empty()) return result;

   result = findData(file);


   return result;
}

void ossimEnvironmentUtility::addDataSearchPath(const ossimFilename& path)
{
   theDataSearchPath.push_back(path);
}

void ossimEnvironmentUtility::addDataSearchPathToFront(const ossimFilename& path)
{
   theDataSearchPath.insert(theDataSearchPath.begin(), path);
}

void ossimEnvironmentUtility::addPluginSearchPath(const ossimFilename& path)
{
   thePluginSearchPath.push_back(path);
}

void ossimEnvironmentUtility::addPluginSearchPathToFront(const ossimFilename& path)
{
   thePluginSearchPath.insert(thePluginSearchPath.begin(), path);
}

ossimFilename ossimEnvironmentUtility::findPlugin(const ossimFilename& plugin)const
{
   for(ossimEnvironmentUtility::FilenameListType::const_iterator iter = thePluginSearchPath.begin();
       iter != thePluginSearchPath.end();
       ++iter)
   {
      ossimFilename temp = iter->dirCat(plugin);
      if(temp.exists())
      {
         return temp;
      }
   }

   return "";
}

ossimFilename ossimEnvironmentUtility::findData(const ossimFilename& data)const
{
   for(ossimEnvironmentUtility::FilenameListType::const_iterator iter = theDataSearchPath.begin();
       iter != theDataSearchPath.end();
       ++iter)
   {
      ossimFilename temp = iter->dirCat(data);
      if(temp.exists())
      {
         return temp;
      }
   }

   return "";
}


ossimEnvironmentUtility::FilenameListType& ossimEnvironmentUtility::getPluginSearchPath()
{
   return thePluginSearchPath;
}

const ossimEnvironmentUtility::FilenameListType& ossimEnvironmentUtility::getPluginSearchPath()const
{
   return thePluginSearchPath;
}

ossimEnvironmentUtility::FilenameListType& ossimEnvironmentUtility::getDataSearchPath()
{
   return theDataSearchPath;
}

const ossimEnvironmentUtility::FilenameListType& ossimEnvironmentUtility::getDataSearchPath()const
{
   return theDataSearchPath;
}

// Hidden copy constructor.
ossimEnvironmentUtility::ossimEnvironmentUtility(
   const ossimEnvironmentUtility& /* obj */)
{
}

// Hidden operator=
const ossimEnvironmentUtility& ossimEnvironmentUtility::operator=(
   const ossimEnvironmentUtility& /* rhs */)
{
   return *this;
}
