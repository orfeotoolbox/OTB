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
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimProperty.h,v 1.15 2005/06/09 11:25:35 gpotts Exp $
#ifndef ossimProperty_HEADER
#define ossimProperty_HEADER
#include "base/common/ossimObject.h"
#include "base/data_types/ossimString.h"

class OSSIMDLLEXPORT ossimProperty : public ossimObject
{
public:
   /*!
    * This are bitwise and can be ored together
    * The change type is very important and
    * it determines how things are refreshed.
    */
   enum ossimPropertyChangeType
   {
      ossimPropertyChangeType_NOTSPECIFIED   = 0,
      ossimPropertyChangeType_CACHE_REFRESH  = 1,
      ossimPropertyChangeType_FULL_REFRESH   = 2
   };
   ossimProperty(const ossimString& name=ossimString(""));
   ossimProperty(const ossimProperty& rhs);
   virtual ~ossimProperty();
   
   const  ossimProperty& operator = (const ossimProperty& rhs);
   virtual const ossimProperty& assign(const ossimProperty& rhs);

   const ossimString& getName()const;
   void setName(const ossimString& name);

   virtual bool setValue(const ossimString& value)=0;
   virtual void valueToString(ossimString& valueResult)const=0;
   virtual ossimString valueToString()const;
   
   void clearChangeType();
   void setChangeType(ossimPropertyChangeType type);
   void setFullRefreshBit();
   void setCacheRefreshBit();
   
   ossimPropertyChangeType getChangeType()const;
   bool isFullRefresh()const;
   bool isCacheRefresh()const;
   bool isChangeTypeSpecified()const;

   void setReadOnlyFlag(bool flag);
   bool getReadOnlyFlag()const;
   bool isReadOnly()const;

   void setModifiedFlag(bool flag);
   bool getModifiedFlag()const;
   bool isModified()const;
   void clearModifiedFlag();

   void setDescription(const ossimString& description);
   
   virtual ossimString getDescription()const;
   
protected:
   ossimString             theName;
   ossimString             theDescription;
   bool                    theReadOnlyFlag;
   ossimPropertyChangeType theChangeType;
   bool                    theModifiedFlag;
   
TYPE_DATA
};

#endif
