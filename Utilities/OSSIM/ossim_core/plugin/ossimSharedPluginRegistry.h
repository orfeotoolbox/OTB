#ifndef ossimSharedPluginRegistry_HEADER
#define ossimSharedPluginRegistry_HEADER
#include <vector>
#include <plugin/ossimSharedObjectBridge.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/ossimRefPtr.h>
#include <plugin/ossimPluginLibrary.h>

class OSSIMDLLEXPORT ossimSharedPluginRegistry
{
public:
   virtual ~ossimSharedPluginRegistry();
   
   static ossimSharedPluginRegistry* instance();

   bool registerPlugin(const ossimFilename& filename);
   bool unregisterPlugin(int idx);
   const ossimPluginLibrary* getPlugin(const ossimFilename& filename)const;
   ossimPluginLibrary* getPlugin(const ossimFilename& filename);

   ossim_uint32 getIndex(const ossimPluginLibrary* lib)const;
   ossimPluginLibrary* getPlugin(ossim_uint32 idx);
   const ossimPluginLibrary* getPlugin(ossim_uint32 idx)const;
   ossim_uint32 getNumberOfPlugins()const;
   
protected:
   ossimSharedPluginRegistry(){}
   ossimSharedPluginRegistry(const ossimSharedPluginRegistry&){}
   void operator = (const ossimSharedPluginRegistry&){}

   static ossimSharedPluginRegistry* theInstance;
  
   std::vector<ossimRefPtr<ossimPluginLibrary> > theLibraryList;
};

#endif
