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
// $Id: ossimCustomEditorWindow.h,v 1.7 2002/11/25 18:45:47 gpotts Exp $
#ifndef ossimCustomEditorWindow_HEADER
#define ossimCustomEditorWindow_HEADER
#include "base/common/ossimObject.h"

class OSSIMDLLEXPORT ossimCustomEditorWindow : public ossimObject
{
public:
   /*!
    * Returns the system dependent window.  If you are in wx it
    * will probalby be some type of wxWindow*.
    */
   virtual void* getWindow()=0;

   /*!
    * This is the parent window.  If the parent can be
    * set by the derived classes it will return true.
    */
   virtual void  setParent(void* parent)=0;
   
   /*!
    * Will return the parent of the managed wxWindow
    */
   virtual void* getParent()=0;
   
   /*!
    * WIll set the object to edit.  If this editor does not know
    * anything about the passed in obj it will return false.
    */
   virtual bool setObjectToEdit(ossimObject* obj)=0;

   /*!
    * Just asks if I can edit the passed in object.
    */
   virtual bool canEdit(ossimObject* obj)const=0;

   /*!
    * Returns the object we are editing.
    */
   virtual ossimObject* getObject()=0;

protected:
   ossimCustomEditorWindow(){}
   ossimCustomEditorWindow(const ossimCustomEditorWindow&){}
   void operator =(const ossimCustomEditorWindow& rhs){}

TYPE_DATA
};
#endif
