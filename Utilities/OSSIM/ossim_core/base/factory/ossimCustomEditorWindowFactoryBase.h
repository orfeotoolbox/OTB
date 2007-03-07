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
// $Id: ossimCustomEditorWindowFactoryBase.h,v 1.7 2003/02/07 13:54:55 gpotts Exp $
#ifndef ossimCustomEditorWindowFactoryBase_HEADER
#define ossimCustomEditorWindowFactoryBase_HEADER
#include "base/factory/ossimObjectFactory.h"

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
