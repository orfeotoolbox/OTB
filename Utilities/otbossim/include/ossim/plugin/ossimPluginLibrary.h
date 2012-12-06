//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimPluginLibrary.h 20694 2012-03-19 12:22:05Z dburken $
#ifndef ossimPluginLibrary_HEADER
#define ossimPluginLibrary_HEADER 1
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
};

#endif 
