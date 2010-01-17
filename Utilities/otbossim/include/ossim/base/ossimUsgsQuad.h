//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimUsgsQuad.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef  ossimUsgsQuad_HEADER
#define ossimUsgsQuad_HEADER
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorStatusInterface.h>

#include <vector>

class OSSIMDLLEXPORT  ossimUsgsQuad
{
public:
   ossimUsgsQuad(const ossimGpt& lrGpt);
   ossimUsgsQuad(const ossimString& name,
                 const ossimDatum* datum);
//                 double paddingInMeters = 100);

   void setQuadName(const ossimString& name);
   /*!  Returns the bounding rectangle of the quarter quad segment.
    *   A segment is 1/4 of a quarter quad.
    *   Segment assingments:  upper_left=A, upper_right=B, lower_left=C,
    *   lower_right=D   NOTE:  padding is added in.
    */
   ossimGrect quarterQuadSegRect(char seg);

   /*!  Returns the bounding rectangle of the quarter quad segment
    *   in the "area.lat_lon" keyword format:
    *   start_lat, start_lon, stop_lat, stop_lon
    *   NOTE:  padding is added in.
    */
   ossimString quarterQuadSegKwRect(char seg);

   /*!  Returns the bounding rectangle of the quarter quad (3.75 minutes).  
    *   NOTE:  padding is added in.
    */
   ossimGrect quarterQuadRect() const;

   /*!  Returns the bounding rectangle of the quarter quad (3.75 minutes)  
    *   in the "area.lat_lon" keyword format:
    *   start_lat, start_lon, stop_lat, stop_lon
    *   NOTE:  padding is added in.
    */
   ossimString quarterQuadKwRect() const;

   /*!  Returns the bounding rectangle of the quarter quad (7.5 minutes).  
    *   NOTE:  padding is added in.
    */
   ossimGrect quadRect() const;

   /*!  Returns the bounding rectangle of the quarter quad (7.5 minutes)
    *   in the "area.lat_lon" keyword format:
    *   start_lat, start_lon, stop_lat, stop_lon
    *   NOTE:  padding is added in.
    */
   ossimString quadKwRect() const;

   /*!
    * Returns the USGS quarter quad name.
    */
   ossimString quarterQuadName() const
      {
         return ossimString(theName.begin(),
                            theName.begin()+7);
      }

   ossimString quarterQuadSegName() const
      {
         return theName;
      }

   
   /*!
    *  Returns the lower right hand corner of quad with no padding.
    */
   ossimGpt lrQuadCorner() const { return theQuadLowerRightCorner ; }
   
   /*!
    * Returns the lower right hand corner of quarter quad with no padding.
    */
   ossimGpt lrQuarterQuadCorner() const
      { return theQuarterQuadLowerRightCorner; }

   static void getQuadList(std::vector<ossimUsgsQuad>& result,
                           const ossimGrect& rect);
   
protected:
   ossimString         theName;
   ossimGpt            theQuadLowerRightCorner; // Lower right corner of quad.
   ossimGpt            theQuarterQuadLowerRightCorner;// quarter quad lr corner.
//   ossimDpt            thePaddingInDegrees;
   char                theQuarterQuadSegment; // A, B, C, D
};

#endif
