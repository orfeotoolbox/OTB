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
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfLabelHeader.h,v 1.2 2004/05/23 18:42:33 dburken Exp $
#ifndef ossimNitfLabelHeader_HEADER
#define ossimNitfLabelHeader_HEADER
#include <iostream>
using namespace std;
#include "base/common/ossimObject.h"
#include "base/data_types/ossimDrect.h"

class OSSIMDLLEXPORT ossimNitfLabelHeader : public ossimObject
{
public:
   ossimNitfLabelHeader(){}
   virtual ~ossimNitfLabelHeader(){}
   
   virtual void parseStream(istream &in)=0;
   virtual long getDisplayLevel()const=0;
   virtual ossimDrect getImageRect()const=0;
   
TYPE_DATA
};

#endif
