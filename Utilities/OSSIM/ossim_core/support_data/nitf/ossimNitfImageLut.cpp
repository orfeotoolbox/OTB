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
// $Id: ossimNitfImageLut.cpp,v 1.2 2004/11/11 14:23:34 gpotts Exp $
#include "ossimNitfImageLut.h"
#include <string.h> // memcpy

#ifndef NULL
#include <stddef.h>
#endif

ostream& operator <<(ostream& out,
                     const ossimNitfImageLut &data)
{
   data.print(out);

   return out;
}

RTTI_DEF1(ossimNitfImageLut, "ossimNitfImageLut", ossimObject);
ossimNitfImageLut::ossimNitfImageLut()
{
}

ossimNitfImageLut::~ossimNitfImageLut()
{
}

