//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPluginLibrary.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimPluginLibrary_HEADER
#define ossimPluginLibrary_HEADER
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/plugin/ossimDynamicLibrary.h>

class OSSIMDLLEXPORT ossimPluginLibrary : public ossimDynamicLibrary
{
public:
   ossimPluginLibrary();
   ossimPluginLibrary(const ossimString& name);
   virtual ~ossimPluginLibrary();
   void initialize();
   void finalize();
   ossimString getDescription()const;
   void getClassNames(std::vector<ossimString>& classNames)const;
   
protected:
   
   ossimSharedObjectInfo* theInfo;

TYPE_DATA
};

#endif 
