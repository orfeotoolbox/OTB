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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//*******************************************************************
//  $Id: ossimLine.h,v 1.4 2002/09/01 19:21:56 gpotts Exp $
#ifndef ossimLine_HEADER
#define ossimLine_HEADER
#include "base/data_types/ossimDpt.h"
#include <iostream>

class OSSIMDLLEXPORT ossimLine
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                               const ossimLine& rhs);
   ossimLine(const ossimDpt& p1=ossimDpt(0,0),
             const ossimDpt& p2=ossimDpt(0,0))
      :theP1(p1),
       theP2(p2)
      {
      }

   
   ossimDpt getVector()const
      {
         return (theP2-theP1);
      }

   /*!
    * Computes the following equation:
    *
    * Note: this object will be line a and the passed
    *       in object will be line b;
    *       and
    *       P1 and P2 coorespond to this object and
    *       P3 and P4 will coorespond to the passed in object.
    *
    * Now find point formed at the intersection of line a and b:
    *
    * Pa = P1 + ua ( P2 - P1 ) 
    * Pb = P3 + ub ( P4 - P3 )
    *const ossimDpt&   ul_corner,
              const ossimDpt&   lr_corner,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);
    * x1 + ua (x2 - x1) = x3 + ub (x4 - x3) 
    * and 
    * y1 + ua (y2 - y1) = y3 + ub (y4 - y3)
    *
    * Solve:
    *  ua = ((x4-x3)(y1-y3) - (y4-y3)(x1-x3))/
    *       ((y4-y3)(x2-x1) - (x4-x3)(y2-y1))
    *  ub = ((x2-x1)(y1-y3) - (y2-y1)(x1-x3))/
    *       ((y4-y3)(x2-x1) - (x4-x3)(y2-y1))
    * substitute:
    *
    * x = x1 + ua (x2 - x1) 
    * y = y1 + ua (y2 - y1)
    *
    */
   ossimDpt intersectInfinite(const ossimLine& line)const;

   ossimDpt intersectSegment(const ossimLine& line)const;

   ossimDpt midPoint()const;
   double length()const;
   ossimDpt normal()const;
   /*!
    * Will return true if the point is on the line.
    */
   bool isPointWithin(const ossimDpt& point, double delta=FLT_EPSILON)const;

   bool isPointOnInfiniteLine(const ossimDpt& point, double delta=FLT_EPSILON)const;

   ossimDpt   theP1;
   ossimDpt   theP2;
};

#endif
