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
//
// Description:
//
//*******************************************************************
//  $Id: ossimGeoidNgs.h,v 1.3 2005/11/10 14:15:33 gpotts Exp $
#ifndef ossimGeoidNgs_HEADER
#define ossimGeoidNgs_HEADER

#include "ossimGeoid.h"
#include "ossimGeoidNgsHeader.h"

class OSSIMDLLEXPORT ossimGeoidNgs : public ossimGeoid
{

public:
   ossimGeoidNgs();
   ossimGeoidNgs(const ossimFilename& dir,
                 ossimByteOrder byteOrder = OSSIM_LITTLE_ENDIAN);
   virtual ~ossimGeoidNgs();

   virtual ossimString getShortName()const;
   
   virtual bool open(const ossimFilename& dir,
                     ossimByteOrder byteOrder = OSSIM_LITTLE_ENDIAN);

   virtual bool addFile(const ossimFilename& file,
                        ossimByteOrder byteOrder = OSSIM_LITTLE_ENDIAN);
   /*!
    *  Returns the offset from the ellipsoid to the geoid.
    *  Returns OSSIM_DBL_NAN if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt&) const;
   
   double geoidToEllipsoidHeight(double lat,
                           double lon,
                           double geoidHeight);
   
   double ellipsoidToGeoidHeight(double lat,
                                 double lon,
                                 double ellipsoidHeight);
protected:
   void fixLatLon(double &lat, double &lon) const;
   double deltaHeight(double lat, double lon)const;
   
   std::vector<ossimGeoidNgsHeader> theGridVector;
   ossimString theShortName;

   TYPE_DATA
};

#endif
