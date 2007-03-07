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
// $Id: ossimNitfImageLutV2_0.h,v 1.2 2004/11/11 14:23:34 gpotts Exp $
#ifndef ossimNitfImageLutV2_0_HEADER
#define ossimNitfImageLutV2_0_HEADER
#include "ossimNitfImageLut.h"

#include <vector>
#include <iostream>
using namespace std;


class ossimNitfImageLutV2_0 : public ossimNitfImageLut
{
public:
   ossimNitfImageLutV2_0();
   virtual ~ossimNitfImageLutV2_0();
   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream& out)const;
   virtual ossim_uint32 getNumberOfEntries()const;
   virtual void setNumberOfEntries(ossim_uint32 numberOfEntries);
   virtual ossim_uint8 getValue(ossim_uint32 idx)const;
   
protected:
   vector<ossim_uint8> theData;
   ossim_uint32 theNumberOfEntries;
};
#endif
