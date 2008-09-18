#ifndef ossimEnvironmentUtility_HEADER
#define ossimEnvironmentUtility_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>

class OSSIM_DLL ossimEnvironmentUtility
{
public:
   typedef std::vector<ossimFilename> FilenameListType;
   
   ossimEnvironmentUtility();
   static ossimEnvironmentUtility* instance();

   ossimString getEnvironmentVariable(const ossimString& variable)const;

   ossimFilename getUserDir()const;
   ossimString   getUserName()const;
   ossimFilename getUserOssimSupportDir()const;
   ossimFilename getUserOssimPreferences()const;
   ossimFilename getUserOssimPluginDir()const;
   
   ossimFilename getInstalledOssimSupportDir()const;
   ossimFilename getInstalledOssimPluginDir()const;
   ossimFilename getInstalledOssimPreferences()const;

   ossimFilename searchAllPaths(const ossimFilename& file)const;
   
   ossimFilename findPlugin(const ossimFilename& plugin)const;
   ossimFilename findData(const ossimFilename& data)const;

   void addDataSearchPath(const ossimFilename& path);
   void addDataSearchPathToFront(const ossimFilename& path);
   void addPluginSearchPath(const ossimFilename& path);
   void addPluginSearchPathToFront(const ossimFilename& path);
 
   ossimEnvironmentUtility::FilenameListType& getPluginSearchPath();
   const ossimEnvironmentUtility::FilenameListType& getPluginSearchPath()const;
   ossimEnvironmentUtility::FilenameListType& getDataSearchPath();
   const ossimEnvironmentUtility::FilenameListType& getDataSearchPath()const;
   
protected:
   static ossimEnvironmentUtility* theInstance;

   ossimEnvironmentUtility::FilenameListType thePluginSearchPath;
   ossimEnvironmentUtility::FilenameListType theDataSearchPath;
};


#endif
