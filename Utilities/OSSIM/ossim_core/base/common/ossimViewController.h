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
// $Id: ossimViewController.h,v 1.16 2004/05/20 14:41:10 dburken Exp $
#ifndef ossimViewController_HEADER
#define ossimViewController_HEADER
#include "base/common/ossimSource.h"
//#include "base/common/ossimViewInterface.h"

/*!
 * All view sources will derive from this class.  For example
 * we can have a mapped view or Perspective view of the scene
 * being rendered.  All sources within the containers are expected
 * to derive from the view interface.
 */
class OSSIMDLLEXPORT ossimViewController : public ossimSource
{
public:

   ossimViewController();
   
   ossimViewController(ossimObject* owner,
                       ossim_uint32 inputListSize,
                       ossim_uint32 outputListSize,
                       bool   inputListFixedFlag=true,
                       bool   outputListFixedFlag=false);

   virtual ~ossimViewController();
 
   virtual ossimString getShortName()const;
   
   virtual ossimString getLongName()const;

   virtual bool propagateView();
   virtual bool setView(ossimObject* object);
   virtual ossimObject* getView();
   virtual const ossimObject* getView()const;
   virtual bool canConnectMyInputTo(ossim_int32 index,
                                    const ossimConnectableObject* obj)const;
   virtual const ossimObject* findFirstViewOfType(RTTItypeid typeId)const;
protected:
   ossimObject* theView;
   
TYPE_DATA
};

#endif
