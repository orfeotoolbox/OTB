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
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:  
//
// Contains class declaration for fpt3d.
//
// Used to represent a three dimensional point containing an x and y data
// member.
//
//*******************************************************************
// $Id: ossimFpt3d.h,v 1.3 2003/05/13 11:31:02 dburken Exp $

#ifndef ossimFpt3d_HEADER
#define ossimFpt3d_HEADER

#include "../common/ossimConstants.h"
#include "ossimFpt.h"


class OSSIMDLLEXPORT ossimFpt3d
{
public:

   ossimFpt3d()
      : x(0.0), y(0.0), z(0.0)
      {}
   ossimFpt3d(const ossim_float32& aX,
              const ossim_float32& aY,
              const ossim_float32& aZ)
      : x(aX), y(aY), z(aZ)
      {}

   ossimFpt pt2d() const { return ossimFpt(x, y); }
   
   ostream& print(ostream& os) const;

   friend ostream& operator<<(ostream& os, const ossimFpt3d& pt);
   friend istream& operator>>(istream& is, ossimFpt3d& pt);

   ossim_float32 x;
   ossim_float32 y;
   ossim_float32 z;
};

#endif
