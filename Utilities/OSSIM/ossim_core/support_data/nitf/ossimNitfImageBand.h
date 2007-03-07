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
// $Id: ossimNitfImageBand.h,v 1.3 2004/11/11 14:23:33 gpotts Exp $
#ifndef ossimNitfImageBand_HEADER
#define ossimNitfImageBand_HEADER
#include <iostream>
#include "ossimNitfImageLut.h"
#include <base/common/ossimObject.h>
#include <base/common/ossimRefPtr.h>

class ossimNitfImageBand : public ossimObject
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimNitfImageBand &data);
   ossimNitfImageBand();
   virtual ~ossimNitfImageBand();
   virtual void parseStream(std::istream& in)=0;
   virtual void writeStream(std::ostream& out)=0;
   
   virtual std::ostream& print(std::ostream& out)const=0;

   virtual ossim_uint32 getNumberOfLuts()const=0;
   virtual const ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx)const=0;
   virtual ossimRefPtr<ossimNitfImageLut> getLut(ossim_uint32 idx)=0;

TYPE_DATA;
};

#endif
