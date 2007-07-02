//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimLine.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/base/ossimLine.h>
#include <ossim/base/ossimDrect.h>
#include  <iostream>
#include <algorithm>
using namespace std;

std::ostream& operator <<(std::ostream& out, const ossimLine& rhs)
{
   return out << "line: " << rhs.theP1 << "," << rhs.theP2 << endl;
}

ossimDpt ossimLine::intersectInfinite(const ossimLine& line)const
{
   ossimDpt result;
   ossimDpt p3 = line.theP1;
   ossimDpt p4 = line.theP2;

   double numerator  = ((p4.x-p3.x)*(theP1.y-p3.y)    - (p4.y-p3.y)*(theP1.x-p3.x));
   double denominator = ((p4.y-p3.y)*(theP2.x-theP1.x) - (p4.x-p3.x)*(theP2.y-theP1.y));
   result.makeNan();
   // as long as the lines are not parallel
   if(fabs(denominator) > FLT_EPSILON)
   {
      double ua = numerator/ denominator;
      result = ossimDpt(theP1.x + ua*(theP2.x-theP1.x),
                        theP1.y + ua*(theP2.y-theP1.y));
   }

   return result;
}

ossimDpt ossimLine::intersectSegment(const ossimLine& line)const
{
   ossimDpt result;
   ossimDpt p3 = line.theP1;
   ossimDpt p4 = line.theP2;

   double numerator   = ((p4.x-p3.x)*(theP1.y-p3.y)    - (p4.y-p3.y)*(theP1.x-p3.x));
   double denominator = ((p4.y-p3.y)*(theP2.x-theP1.x) - (p4.x-p3.x)*(theP2.y-theP1.y));
   result.makeNan();
   // as long as the lines are not parallel
   if(fabs(denominator) > FLT_EPSILON)
   {
      double ua = numerator/ denominator;

      if((ua >= -FLT_EPSILON) && (ua <= (1.0 + FLT_EPSILON)))
      {
         ossimDpt test = ossimDpt(theP1.x + ua*(theP2.x-theP1.x),
                                  theP1.y + ua*(theP2.y-theP1.y));
         ossimDpt midPoint = line.theP1 + (line.theP2-line.theP1)*.5;
         if( ((test-midPoint).length()/line.length()) <= (.5+FLT_EPSILON))
         {
            result = test;
         }
      }
   }

   return result;
}

double ossimLine::length()const
{
   return (theP2-theP1).length();
}

ossimDpt ossimLine::midPoint()const
{
   return (theP1 + (theP2-theP1)*.5);
}

bool ossimLine::isPointWithin(const ossimDpt& point, double delta)const
{   
   if(isPointOnInfiniteLine(point, delta))
   {
      double minx = std::min(theP1.x, theP2.x);
      double miny = std::min(theP1.y, theP2.y);
      double maxx = std::max(theP1.x, theP2.x);
      double maxy = std::max(theP1.y, theP2.y);
      
      ossimDrect rect(minx - delta,
                      miny - delta,
                      maxx + delta,
                      maxy + delta);
      
      return rect.pointWithin(point);
   }
   
   return false;
}

bool ossimLine::isPointOnInfiniteLine(const ossimDpt& point, double delta)const
{
   if((point == theP1) || (point == theP2))
   {
      return true;
   }
   else if(fabs(theP1.x-theP2.x) <= FLT_EPSILON)
   {
      return (fabs(point.x - theP1.x) <= delta);
   }
   else if(fabs(theP1.y-theP2.y) <= FLT_EPSILON)
   {
      return (fabs(point.y - theP1.y) <= delta);      
   }
   else
   {
      ossimDpt v1  = getVector();
      v1 = v1 * (1.0/v1.length());
      ossimDpt v2  = (point - theP1);
      double s     = v1.x*v2.x + v1.y*v2.y;
      ossimDpt p   = theP1 + v1*s;
      double len   = (point-p).length();

      if(len < delta)
      {
         return true;
      }
      else
      {
         return false;
      }
   }

   return false;
}

ossimDpt ossimLine::normal()const
{
   ossimDpt delta = getVector();
   ossimDpt result;
   if(fabs(delta.x) <= FLT_EPSILON)
   {
      result.y = 0.0;
      result.x = -delta.y;
   }
   else if(fabs(delta.y) <= FLT_EPSILON)
   {
      result.x = 0.0;
      result.y = delta.x;
   }
   else
   {
      result.x = -delta.y;
      result.y = delta.x;
   }
   double len = result.length();
   
   if(len >FLT_EPSILON)
   {
      result = result*(1.0/len);
   }

   return result;
}
