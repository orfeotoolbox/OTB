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
// Description:
//
//*************************************************************************
// $Id: ossimCmyVector.cpp,v 1.1 2002/11/01 19:08:48 gpotts Exp $
#include "ossimCmyVector.h"
#include "ossimRgbVector.h"

ossimCmyVector::ossimCmyVector(const ossimRgbVector& rgb)
{
   theC = 255 - rgb.getR();
   theM = 255 - rgb.getG();
   theY = 255 - rgb.getB();
}

const ossimCmyVector& ossimCmyVector::operator = (const ossimRgbVector& rgb)
{
   theC = 255 - rgb.getR();
   theM = 255 - rgb.getG();
   theY = 255 - rgb.getB();
   
   return *this;
}
