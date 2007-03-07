//*****************************************************************************
// FILE: ossimGeoid.h
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// DESCRIPTION:
//   Contains declaration of class ossimGeoid. Maintains a grid of elevation
//   offsets for the geoid (mean sea level) relative to WGS-84 ellipsoid.
//
// SOFTWARE HISTORY:
//>
//   17Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#ifndef ossimGeoid_HEADER
#define ossimGeoid_HEADER

#include "base/common/ossimObject.h"
#include "base/common/ossimErrorStatusInterface.h"

class OSSIMDLLEXPORT ossimGpt;
class OSSIMDLLEXPORT ossimFilename;

class OSSIMDLLEXPORT ossimGeoid : public ossimObject,
				  public ossimErrorStatusInterface
{
public:
   ossimGeoid();
   ossimGeoid(const ossimFilename& dir);
   virtual ~ossimGeoid();

   virtual bool open(const ossimFilename& dir, ossimByteOrder byteOrder=OSSIM_LITTLE_ENDIAN) = 0;

   /*!
    *  Returns the offset from the ellipsoid to the geoid.
    *  Returns OSSIM_DBL_NAN if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const = 0;

protected:
   
   TYPE_DATA
};
#endif
