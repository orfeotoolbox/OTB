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
// Author: Garrett Potts (gpotts@imagelinks)
// Description:
//
//*************************************************************************
// $Id: ossimUsgsQuad.h,v 1.4 2003/01/27 15:24:34 gpotts Exp $
#ifndef  ossimUsgsQuad_HEADER
#define ossimUsgsQuad_HEADER
#include "base/data_types/ossimGpt.h"
#include "base/data_types/ossimGrect.h"
#include "base/data_types/ossimString.h"
#include "base/common/ossimCommon.h"
#include "base/common/ossimErrorStatusInterface.h"

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
