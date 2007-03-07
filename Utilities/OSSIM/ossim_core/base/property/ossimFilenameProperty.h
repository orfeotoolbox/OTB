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
// $Id: ossimFilenameProperty.h,v 1.5 2004/05/19 18:29:38 dburken Exp $
#ifndef ossimFilenameProperty_HEADER
#define ossimFilenameProperty_HEADER
#include <vector>
#include "ossimProperty.h"
#include "base/data_types/ossimFilename.h"
#include "base/data_types/ossimString.h"

class OSSIMDLLEXPORT ossimFilenameProperty : public ossimProperty
{
public:

   enum ossimFilenamePropertyIoType
   {
      ossimFilenamePropertyIoType_NOT_SPECIFIED = 0,
      ossimFilenamePropertyIoType_INPUT         = 1,
      ossimFilenamePropertyIoType_OUTPUT        = 2
   };
   
   ossimFilenameProperty(const ossimString& name = ossimString(""),
                         const ossimFilename& value = ossimFilename(""),
                         const std::vector<ossimString>& filterList = std::vector<ossimString>());

   ossimFilenameProperty(const ossimFilenameProperty& rhs);
   virtual ~ossimFilenameProperty();
   ossimObject* dup()const;
   virtual void valueToString(ossimString& valueResult)const;
  virtual bool setValue(const ossimString& value);
   const ossimFilenameProperty& operator = (ossimFilenameProperty& rhs);
   virtual const ossimProperty& assign(const ossimProperty& rhs);

   void clearFilterList();
   ossim_uint32 getNumberOfFilters()const;
   void setFilter(ossim_uint32 idx,
                  const ossimString& filterValue);
   ossimString getFilter(ossim_uint32 idx)const;
   ossimString getFilterListAsString(const ossimString& separator=ossimString("\n"))const;
   const std::vector<ossimString>& getFilterList()const;
   void addFilter(const ossimString& filter);

   void setIoType(ossimFilenamePropertyIoType ioType);
   ossimFilenamePropertyIoType getIoType()const;
   bool isIoTypeInput()const;
   bool isIoTypeOutput()const;
   
protected:
   ossimFilename            theValue;
   std::vector<ossimString> theFilterList;
   ossimFilenamePropertyIoType theIoType;
   
TYPE_DATA
};

#endif
