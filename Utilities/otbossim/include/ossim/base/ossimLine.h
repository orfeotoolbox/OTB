//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimLine.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimLine_HEADER
#define ossimLine_HEADER
#include <ossim/base/ossimDpt.h>
#include <iosfwd>

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
