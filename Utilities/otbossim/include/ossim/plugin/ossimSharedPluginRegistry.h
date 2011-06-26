#ifndef ossimSharedPluginRegistry_HEADER
#define ossimSharedPluginRegistry_HEADER
#include <iostream>
#include <vector>
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/plugin/ossimPluginLibrary.h>

class OSSIMDLLEXPORT ossimSharedPluginRegistry
{
public:
   virtual ~ossimSharedPluginRegistry();
   
   static ossimSharedPluginRegistry* instance();

   bool registerPlugin(const ossimFilename& filename, const ossimString& options="");//, bool insertFrontFlag=false);
   bool unregisterPlugin(int idx);
   bool unregisterPlugin(ossimPluginLibrary* plugin);
   const ossimPluginLibrary* getPlugin(const ossimFilename& filename)const;
   ossimPluginLibrary* getPlugin(const ossimFilename& filename);

   ossim_uint32 getIndex(const ossimPluginLibrary* lib)const;
   ossimPluginLibrary* getPlugin(ossim_uint32 idx);
   const ossimPluginLibrary* getPlugin(ossim_uint32 idx)const;
   ossim_uint32 getNumberOfPlugins()const;

   /**
    * Checks if filename is already loaded to avoid duplication.
    * 
    * @param filename The file to check.
    *
    * @return true if any of the plugins match file name, false if not.
    */
   bool isLoaded(const ossimFilename& filename) const;
   
   void printAllPluginInformation(std::ostream& out);
   
protected:
   ossimSharedPluginRegistry();
   ossimSharedPluginRegistry(const ossimSharedPluginRegistry&){}
   void operator = (const ossimSharedPluginRegistry&){}

   //static ossimSharedPluginRegistry* theInstance;   
   std::vector<ossimRefPtr<ossimPluginLibrary> > theLibraryList;
};

#endif
