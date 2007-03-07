//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimCustomEditorWindowRegistry.h,v 1.8 2003/04/09 15:48:46 gpotts Exp $
#ifndef ossimCustomEditorWindowRegistry_HEADER
#define ossimCustomEditorWindowRegistry_HEADER
#include "base/common/ossimObject.h"
#include "base/common/ossimCustomEditorWindow.h"
#include "base/factory/ossimCustomEditorWindowFactoryBase.h"

#include <vector>
class OSSIMDLLEXPORT ossimCustomEditorWindowFactoryBase;

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
   void operator =(const ossimCustomEditorWindowRegistry& rhs){}
   
   std::vector<ossimCustomEditorWindowFactoryBase*> theFactoryList;
   
   static ossimCustomEditorWindowRegistry*                 theInstance;

TYPE_DATA
};

extern "C"
{
  OSSIMDLLEXPORT void* ossimCustomEditorWindowRegistryGetInstance();
}
#endif
