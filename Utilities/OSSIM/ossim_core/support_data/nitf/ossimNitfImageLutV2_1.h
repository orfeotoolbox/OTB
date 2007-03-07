//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
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
// Author:  David Burken <dburken@imagelinks.com>
// 
// Description: Nitf support class.  Copied from ossimNitfImageLutV2_0.
// 
//********************************************************************
// $Id: ossimNitfImageLutV2_1.h,v 1.2 2004/11/11 14:23:34 gpotts Exp $

#ifndef ossimNitfImageLutV2_1_HEADER
#define ossimNitfImageLutV2_1_HEADER
#include "ossimNitfImageLutV2_0.h"

#include <vector>
#include <iostream>
using namespace std;

class ossimNitfImageLutV2_1 : public ossimNitfImageLutV2_0
{
public:
   ossimNitfImageLutV2_1();
   virtual ~ossimNitfImageLutV2_1();
   
};
#endif
