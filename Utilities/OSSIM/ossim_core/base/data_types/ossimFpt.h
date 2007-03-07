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
// Contains class declaration for fpt.
// Used to represent a two dimensional point containing data members x and y.
//*******************************************************************
//  $Id: ossimFpt.h,v 1.7 2003/05/13 11:31:02 dburken Exp $

#ifndef ossimFpt_HEADER
#define ossimFpt_HEADER
#include <iostream>
#include "base/common/ossimConstants.h"
#include "base/common/ossimCommon.h"

// Forward class declarations.
class OSSIMDLLEXPORT ossimIpt;
class OSSIMDLLEXPORT ossimDpt;


class OSSIMDLLEXPORT ossimFpt
{
public:

   ossimFpt() : x(0), y(0) {}

   ossimFpt(ossim_float32 x, ossim_float32 y)
      : x(x), y(y)
      {}

   ossimFpt(ossim_float64 x, ossim_float64 y)
      : x((ossim_float64)x), y((ossim_float64)y)
      {}
         
   ossimFpt(const ossimFpt& pt) : x(pt.x), y(pt.y) {}

   ossimFpt(const ossimDpt& pt);

   ossimFpt(const ossimIpt& pt);

   const ossimFpt& operator=(const ossimFpt& pt);

   const ossimFpt& operator=(const ossimDpt&);

   const ossimFpt& operator=(const ossimIpt&);

   bool operator==(const ossimFpt& pt) const
      { return ( (x == pt.x) && (y == pt.y) ); } 

   bool operator!=(const ossimFpt& pt) const
      { return ( (x != pt.x) || (y != pt.y) ); }

   void makeNan(){x = OSSIM_FLT_NAN; y=OSSIM_FLT_NAN;}
   
   bool hasNans()const
      {
         return (ossimIsNan(x) || ossimIsNan(y));
      }
   
   void print(std::ostream& os) const;
   
   friend std::ostream& operator<<(std::ostream& os, const ossimFpt& pt);

   //***
   // Public data members:
   //***
   ossim_float32 x;
   ossim_float32 y;
};

inline const ossimFpt& ossimFpt::operator=(const ossimFpt& pt)
{
   if (this != &pt)
   {
      x = pt.x;
      y = pt.y;
   }
   
   return *this;
}

#endif
