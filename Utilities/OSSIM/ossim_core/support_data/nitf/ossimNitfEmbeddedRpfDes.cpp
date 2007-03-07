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
// $Id: ossimNitfEmbeddedRpfDes.cpp,v 1.3 2004/09/24 14:38:46 gpotts Exp $
#include "ossimNitfEmbeddedRpfDes.h"
#include "ossimNitfEmbeddedRpfHeader.h"


RTTI_DEF1(ossimNitfEmbeddedRpfDes, "ossimNitfEmbeddedRpfDes", ossimNitfRegisteredTag)


ossimNitfEmbeddedRpfDes::ossimNitfEmbeddedRpfDes()
{
}

ossimNitfEmbeddedRpfDes::~ossimNitfEmbeddedRpfDes()
{
}

void ossimNitfEmbeddedRpfDes::parseStream(istream &in)
{
   if(in)
   {
   }
}

std::ostream& ossimNitfEmbeddedRpfDes::print(std::ostream& out)const
{
   return out;
}
