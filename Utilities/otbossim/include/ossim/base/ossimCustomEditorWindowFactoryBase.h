//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimCustomEditorWindowFactoryBase.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimCustomEditorWindowFactoryBase_HEADER
#define ossimCustomEditorWindowFactoryBase_HEADER
#include <ossim/base/ossimObjectFactory.h>

class OSSIMDLLEXPORT ossimCustomEditorWindow;

class OSSIMDLLEXPORT ossimCustomEditorWindowFactoryBase : public ossimObjectFactory
{
public:
   virtual ossimCustomEditorWindow* createCustomEditor(ossimObject* obj,
                                                       void* parent=NULL)const=0;
   virtual ossimCustomEditorWindow* createCustomEditor(const ossimString& classType,
                                                       void* parent=NULL)const=0;

   /*!
    * Returns a native GUI Editor with popup
    */
   virtual void* createPopupEditor(ossimObject* obj,
                                   void* parent=NULL)const=0;
   
   /*!
    * Returns a native GUI child window without a popup.
    */
   virtual void* createPanelEditor(ossimObject* obj,
                                   void* parent=NULL)const=0;
TYPE_DATA
};

#endif
