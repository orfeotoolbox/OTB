//*****************************************************************************
// FILE: ossimGeoid.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// DESCRIPTION:
//   Contains implementation of class ossimGeoid
//
// SOFTWARE HISTORY:
//>
//   17Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#include "ossimGeoid.h"

RTTI_DEF2(ossimGeoid, "ossimGeoid", ossimObject, ossimErrorStatusInterface)

ossimGeoid::ossimGeoid()
   :
      ossimObject(),
      ossimErrorStatusInterface()
{}

ossimGeoid::~ossimGeoid()
{}
