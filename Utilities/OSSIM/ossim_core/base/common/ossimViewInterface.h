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
//
//*************************************************************************
// $Id: ossimViewInterface.h,v 1.7 2004/05/13 17:41:52 dburken Exp $
#ifndef ossimViewInterface_HEADER
#define ossimViewInterface_HEADER
#include "base/common/ossimRtti.h"

class OSSIMDLLEXPORT ossimObject;

class OSSIMDLLEXPORT ossimViewInterface
{
public:
   ossimViewInterface();
   ossimViewInterface(ossimObject* base);
   virtual ~ossimViewInterface();

   /*!
    * The derived classes should overrid this method and
    * return true or fals whether it was able to cast the
    * past in view to a view that it can interface to.
    * So if you are a projective view you might
    * cast to an ossimProjection.  The ownsTheView specifies
    * whether or not it owns the passed in view pointer.
    * This will specifiy whether or not the derived classes
    * need to re-allocate the pointer or just delete its old
    * pointer and set it to the passed in pointer.
    */
   virtual bool setView(ossimObject* baseObject, bool ownsTheView = false)=0;
   virtual ossimObject* getView()=0;
   virtual const ossimObject* getView()const=0;
   virtual void refreshView();
   ossimObject* getBaseObject();

protected:
   ossimObject* theObject;
TYPE_DATA
   
};

#endif
