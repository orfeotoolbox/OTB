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
// $Id: ossimTextProperty.h,v 1.7 2004/05/19 18:29:38 dburken Exp $
#ifndef ossimTextProperty_HEADER
#define ossimTextProperty_HEADER
#include "ossimProperty.h"

class OSSIMDLLEXPORT ossimTextProperty : public ossimProperty
{
public:
   ossimTextProperty(const ossimString& name  = ossimString(""),
                     const ossimString& value = ossimString(""),
                     bool isMultiLine = false);
   ossimTextProperty(const ossimTextProperty& rhs);
   virtual ossimObject* dup()const;

   bool isMulitLine()const;
   void setMultiLineFlag(bool flag);

   virtual void valueToString(ossimString& valueResult)const;
  virtual bool setValue(const ossimString& value);
   virtual const ossimProperty& assign(const ossimProperty& rhs);
   
protected:
   ossimString theValue;
   bool theMultiLineFlag;

TYPE_DATA
};

#endif
