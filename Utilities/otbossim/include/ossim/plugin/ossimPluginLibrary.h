//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPluginLibrary.h 18967 2011-02-25 19:40:48Z gpotts $
#ifndef ossimPluginLibrary_HEADER
#define ossimPluginLibrary_HEADER
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/plugin/ossimDynamicLibrary.h>

class OSSIMDLLEXPORT ossimPluginLibrary : public ossimDynamicLibrary
{
public:
   ossimPluginLibrary();
   ossimPluginLibrary(const ossimString& name, const ossimString& options="");
   virtual ~ossimPluginLibrary();
   void initialize();
   void finalize();
   ossimString getDescription()const;
   void getClassNames(std::vector<ossimString>& classNames)const;
   void setOptions(const ossimString& options);
protected:
   ossimString m_options;
   ossimSharedObjectInfo* m_info;

TYPE_DATA
};

#endif 
