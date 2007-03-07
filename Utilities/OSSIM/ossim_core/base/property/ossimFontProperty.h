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
// $Id: ossimFontProperty.h,v 1.4 2004/01/29 13:29:31 gpotts Exp $
#ifndef ossimFontProperty_HEADER
#define ossimFontProperty_HEADER
#include "ossimProperty.h"
#include "base/common/ossimFontInformation.h"

class OSSIMDLLEXPORT ossimFontProperty : public ossimProperty
{
public:
   ossimFontProperty(const ossimString& name,
                     const ossimFontInformation& value);
   ossimFontProperty(const ossimFontProperty& rhs);
   virtual ~ossimFontProperty();
   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
  virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;

   void setFontInformation(ossimFontInformation& fontInfo);
   const ossimFontInformation& getFontInformation()const;
protected:
   ossimFontInformation theValue;

TYPE_DATA
};

#endif
