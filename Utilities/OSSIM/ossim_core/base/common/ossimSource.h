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
// Base class for any object that can be a source.  Derives from
// ossimConnectableObject since all source's should be connectable
//
//*************************************************************************
// $Id: ossimSource.h,v 1.41 2004/11/11 20:17:25 gpotts Exp $

#ifndef ossimSource_HEADER
#define ossimSource_HEADER

#include "base/common/ossimConnectableObject.h"
#include "base/common/ossimErrorStatusInterface.h"
#include "base/common/ossimConstants.h"

class OSSIMDLLEXPORT ossimDataObject;

class OSSIMDLLEXPORT ossimSource : public ossimConnectableObject,
				   public ossimErrorStatusInterface
{
public:
   ossimSource(ossimObject* owner=0);
   ossimSource(ossimObject* owner,
               ossim_uint32 inputListSize,
               ossim_uint32 outputListSize,
               bool inputListIsFixedFlag=true,
               bool outputListIsFixedFlag=true);
   
   virtual ~ossimSource();

   virtual bool isSourceEnabled()const;
   virtual void enableSource();
   virtual void disableSource();
   virtual bool getEnableFlag() const;
   virtual void setEnableFlag(bool flag);

   virtual bool isInitialized() const;
   virtual void setInitializedFlag(bool flag);
   virtual void initialize();

   /*---------------------- PROPERTY INTERFACE ---------------------------*/
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /*--------------------- PROPERTY INTERFACE END ------------------------*/

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual std::ostream& print(std::ostream& out) const;

protected:
   ossimSource(const ossimSource &rhs);
   const ossimSource& operator=(const ossimSource &rhs);

   bool   theEnableFlag;
   bool   theInitializedFlag;

TYPE_DATA
};

#endif
