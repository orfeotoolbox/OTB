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
// $Id: ossimColorProperty.h,v 1.6 2004/09/22 15:32:24 gpotts Exp $
#ifndef ossimColorProperty_HEADER
#define ossimColorProperty_HEADER
#include <base/property/ossimProperty.h>
#include <base/common/ossimRtti.h>
#include <base/data_types/color_space/ossimRgbVector.h>

class OSSIMDLLEXPORT ossimColorProperty : public ossimProperty
{
public:
   ossimColorProperty(const ossimString& name=ossimString(""),
                      const ossimRgbVector& value = ossimRgbVector(0,0,0));
   ossimColorProperty(const ossimColorProperty& rhs);
   virtual ~ossimColorProperty();
   virtual ossimObject* dup()const;
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   virtual bool setValue(const ossimString& value);
   virtual void valueToString(ossimString& valueResult)const;
   const ossimRgbVector& getColor()const;
   void setColor(const ossimRgbVector& value);
   ossim_uint8 getRed()const;
   ossim_uint8 getGreen()const;
   ossim_uint8 getBlue()const;
   void setRed(ossim_uint8 r);
   void setGreen(ossim_uint8 r);
   void setBlue(ossim_uint8 r);
protected:
   ossimRgbVector theValue;

TYPE_DATA
};

#endif
