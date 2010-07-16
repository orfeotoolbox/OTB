//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimCustomEditorWindowRegistry.h 17108 2010-04-15 21:08:06Z dburken $
#ifndef ossimCustomEditorWindowRegistry_HEADER
#define ossimCustomEditorWindowRegistry_HEADER
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimCustomEditorWindow.h>
#include <ossim/base/ossimCustomEditorWindowFactoryBase.h>

#include <vector>
class ossimCustomEditorWindowFactoryBase;

class OSSIMDLLEXPORT ossimCustomEditorWindowRegistry : public ossimCustomEditorWindowFactoryBase
{
public:
   virtual ~ossimCustomEditorWindowRegistry();
   static ossimCustomEditorWindowRegistry* instance();
   bool registerFactory(ossimCustomEditorWindowFactoryBase* factory);

   virtual ossimCustomEditorWindow* createCustomEditor(ossimObject* obj,
                                                       void* parent=NULL)const;
   virtual ossimCustomEditorWindow* createCustomEditor(const ossimString& classType,
                                                       void* parent=NULL)const;

   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   /*!
    * Returns a native GUI Editor with popup
    */
   virtual void* createPopupEditor(ossimObject* obj,
                                   void* parent=NULL)const;
   
   /*!
    * Returns a native GUI child window without a popup.
    */
   virtual void* createPanelEditor(ossimObject* obj,
                                   void* parent=NULL)const;
   
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

protected:
   ossimCustomEditorWindowRegistry(){}
   ossimCustomEditorWindowRegistry(const ossimCustomEditorWindowRegistry&){}
   void operator =(const ossimCustomEditorWindowRegistry& /*rhs*/){}
   
   std::vector<ossimCustomEditorWindowFactoryBase*> theFactoryList;
   
   static ossimCustomEditorWindowRegistry*                 theInstance;

TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimCustomEditorWindowRegistryGetInstance();
}
#endif
