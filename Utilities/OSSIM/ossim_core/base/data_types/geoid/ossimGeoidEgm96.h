//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc.
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
// Author:  David Burken  <dburken@imagelinks.com>
//
// Description:
//
// Contains class definition for ossimGeoidEgm96 which is
// "Earth Gravity Model 1996".
//
//*******************************************************************
//  $Id: ossimGeoidEgm96.h,v 1.5 2005/11/10 14:15:33 gpotts Exp $

#ifndef ossimGeoidEgm96_HEADER
#define ossimGeoidEgm96_HEADER

#include "ossimGeoid.h"


#define GEOID_NO_ERROR              0x0000
#define GEOID_FILE_OPEN_ERROR       0x0001
#define GEOID_INITIALIZE_ERROR      0x0002
#define GEOID_NOT_INITIALIZED_ERROR 0x0004
#define GEOID_LAT_ERROR             0x0008
#define GEOID_LON_ERROR             0x0010

class OSSIMDLLEXPORT ossimGpt;

class OSSIMDLLEXPORT ossimGeoidEgm96 : public ossimGeoid
{

public:
   ossimGeoidEgm96();
   ossimGeoidEgm96(const ossimFilename& grid_file, ossimByteOrder byteOrder=OSSIM_BIG_ENDIAN);
   
   virtual ~ossimGeoidEgm96();

   virtual bool open(const ossimFilename& grid_file, ossimByteOrder byteOrder=OSSIM_BIG_ENDIAN);

   virtual ossimString getShortName()const;
   
   /*!
    *  Returns the offset from the ellipsoid to the geoid.
    *  Returns OSSIM_DBL_NAN if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const;

   double geoidToEllipsoidHeight(double lat,
                           double lon,
                           double geoidHeight);
   
   double ellipsoidToGeoidHeight(double lat,
                           double lon,
                           double ellipsoidHeight);

protected:

   float* theGeoidHeightBuffer;

   TYPE_DATA
};

#endif
