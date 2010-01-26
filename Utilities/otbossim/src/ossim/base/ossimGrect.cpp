//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// Description:
//
//*************************************************************************
// $Id: ossimGrect.cpp 14390 2009-04-22 22:36:02Z gpotts $

#include <ossim/base/ossimGrect.h>
using namespace std;

ostream& operator<<(ostream& os, const ossimGrect& rect)
{
   return os << rect.theUlCorner << ", " << rect.theLrCorner << endl;
}

ossimGrect::ossimGrect(vector<ossimGpt>& points)
{
   unsigned long index;
   
   // initialize everyone to the first point
   if(points.size() > 0)
   {
      theUlCorner = points[0];
      theLrCorner = theUlCorner;
   }
   
   // find the bounds
   for(index = 1; index < points.size(); index++)
   {
      // find left most
      if(theUlCorner.lond() > points[index].lond())
      {
         theUlCorner.lond(points[index].lond());
      } // find right most
      else if(theLrCorner.lond() < points[index].lond())
      {
         theLrCorner.lond(points[index].lond());
      }
      //find top most
      if(points[index].latd() > theUlCorner.latd())
      {
         theUlCorner.latd(points[index].latd());
      }// find bottom most
      else if(points[index].latd() < theLrCorner.latd())
      {
         theLrCorner.latd(points[index].latd());
      }
   }
   
   theUrCorner = ossimGpt(theUlCorner.latd(),
                          theLrCorner.lond(),
                          0,
                          theUlCorner.datum());
   theLlCorner = ossimGpt(theLrCorner.latd(),
                          theUlCorner.lond(),
                          0,
                          theLrCorner.datum());
}

ossimGrect::ossimGrect(const ossimGpt& p1,
                       const ossimGpt& p2,
                       const ossimGpt& p3,
                       const ossimGpt& p4)
{
   unsigned long index;
   std::vector<ossimGpt> points(4);
   points[0] = p1;
   points[1] = p2;
   points[2] = p3;
   points[3] = p4;
   // initialize everyone to the first point
   if(points.size() > 0)
   {
      theUlCorner = points[0];
      theLrCorner = theUlCorner;
   }
   
   // find the bounds
   for(index = 1; index < points.size(); index++)
   {
      // find left most
      if(theUlCorner.lond() > points[index].lond())
      {
         theUlCorner.lond(points[index].lond());
      } // find right most
      else if(theLrCorner.lond() < points[index].lond())
      {
         theLrCorner.lond(points[index].lond());
      }
      //find top most
      if(points[index].latd() > theUlCorner.latd())
      {
         theUlCorner.latd(points[index].latd());
      }// find bottom most
      else if(points[index].latd() < theLrCorner.latd())
      {
         theLrCorner.latd(points[index].latd());
      }
   }
   theUrCorner = ossimGpt(theUlCorner.latd(),
                          theLrCorner.lond(),
                          0,
                          theUlCorner.datum());
   theLlCorner = ossimGpt(theLrCorner.latd(),
                          theUlCorner.lond(),
                          0,
                          theLrCorner.datum());
}

ossimGrect ossimGrect::stretchToEvenBoundary(double latSpacingInDegrees,
                                             double lonSpacingInDegrees)const
{
   double ulLat = ((long)ceil(theUlCorner.latd()/latSpacingInDegrees))*
                  latSpacingInDegrees;
   double ulLon = ((long)floor(theUlCorner.lond()/lonSpacingInDegrees))*
                  lonSpacingInDegrees;
   double lrLat = ((long)floor(theLrCorner.latd()/latSpacingInDegrees))*
                  latSpacingInDegrees;
   double lrLon = ((long)ceil(theLrCorner.lond()/lonSpacingInDegrees))*
                  lonSpacingInDegrees;
   
   return ossimGrect(ulLat, ulLon, lrLat, lrLon);
}

void ossimGrect::computeEvenTiles(std::vector<ossimGrect>& result,
                                  double latSpacingInDegrees,
                                  double lonSpacingInDegrees,
                                  bool clipToGeographicBounds)const
{
   ossimGrect clipRect = ossimGrect(90, -180, -90, 180);
   result.clear();
   ossimGrect temp = stretchToEvenBoundary(latSpacingInDegrees,
                                           lonSpacingInDegrees);

   ossimGpt point = temp.ul();
   
   while(temp.pointWithin(point))
   {
      while(temp.pointWithin(point))
      {
         ossimGrect rect(point.latd(),
                         point.lond(),
                         point.latd()-latSpacingInDegrees,
                         point.lond()+lonSpacingInDegrees);

         rect.theUlCorner.datum( theUlCorner.datum());
         rect.theLlCorner.datum( theUlCorner.datum());
         rect.theLrCorner.datum( theUlCorner.datum());
         rect.theUrCorner.datum( theUlCorner.datum());
         if(clipToGeographicBounds)
         {
            rect = rect.clipToRect(clipRect);
         }
         result.push_back(rect);

         point.lond(point.lond()+lonSpacingInDegrees);
      }
      point.lond(temp.ul().lond());
      point.latd(point.latd()-latSpacingInDegrees);
   }
}

//*******************************************************************
// Public Method: ossimGrect::completely_within
//*******************************************************************
bool ossimGrect::completely_within(const ossimGrect& rect) const
{
   if(rect.isLonLatNan() || isLonLatNan())
   {
      return false;
   }
   
   /*  --------------
       |     1      |
       | ---------- |
       | |        | |
       | |        | |
       | |   2    | |
       | |        | |
       | |        | |
       | ---------- |
       |            |
       --------------  */

   bool rtn = true;
   
   if (theUlCorner.lon < rect.ul().lon)
   {
      rtn = false;
   }
   else if (theLrCorner.lon > rect.lr().lon)
   {
      rtn = false;
   }
   else if (theUlCorner.lat < rect.ul().lat)
   {
      rtn = false;
   }
   else if (theLrCorner.lat > rect.lr().lat)
   {
      rtn = false;
   }

   return rtn;
}

//*******************************************************************
// Public Method: ossimGrect::intersects
//*******************************************************************
bool ossimGrect::intersects(const ossimGrect& rect) const
{
		
   if(rect.isLonLatNan() || isLonLatNan())
   {
      return false;
   }
   
   ossim_float64  ulx = ossim::max(rect.ul().lon, ul().lon);
   ossim_float64  lrx = ossim::min(rect.lr().lon, lr().lon);
   ossim_float64  uly, lry;
   bool rtn;
   
   uly  = ossim::min(rect.ul().lat, ul().lat);
   lry  = ossim::max(rect.lr().lat, lr().lat);
   rtn = ((ulx <= lrx) && (uly >= lry));
   
      return (rtn);
}
