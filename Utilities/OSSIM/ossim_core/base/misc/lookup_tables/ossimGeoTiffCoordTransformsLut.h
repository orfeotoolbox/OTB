//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
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
// Contains class declaration for GeoTiffCoordTransformsLut. Used to map a
// geotiff coordinate transformation code to an ossim projection.
//*******************************************************************
//  $Id: ossimGeoTiffCoordTransformsLut.h,v 1.2 2002/11/25 19:16:28 gpotts Exp $

#ifndef ossimGeoTiffCoordTransformsLut_HEADER
#define ossimGeoTiffCoordTransformsLut_HEADER

#include "ossimLookUpTable.h"
#include "base/common/ossimKeywordNames.h"

class OSSIMDLLEXPORT ossimGeoTiffCoordTransformsLut : public ossimLookUpTable
{
public:

   enum
   {
      //***
      // Coordinate Transformation Codes cut from geotiff specification
      // section 6.3.3.3.
      //***
      CT_TransverseMercator             = 1,
      CT_TransvMercator_Modified_Alaska = 2,
      CT_ObliqueMercator                = 3,
      CT_ObliqueMercator_Laborde        = 4,
      CT_ObliqueMercator_Rosenmund      = 5,
      CT_ObliqueMercator_Spherical      = 6,
      CT_Mercator                       = 7,
      CT_LambertConfConic_2SP           = 8,
      CT_LambertConfConic_Helmert       = 9,
      CT_LambertAzimEqualArea           = 10,
      CT_AlbersEqualArea                = 11,
      CT_AzimuthalEquidistant           = 12,
      CT_EquidistantConic               = 13,
      CT_Stereographic                  = 14,
      CT_PolarStereographic             = 15,
      CT_ObliqueStereographic           = 16,
      CT_Equirectangular                = 17,
      CT_CassiniSoldner                 = 18,
      CT_Gnomonic                       = 19,
      CT_MillerCylindrical              = 20,
      CT_Orthographic                   = 21,
      CT_Polyconic                      = 22,
      CT_Robinson                       = 23,
      CT_Sinusoidal                     = 24,
      CT_VanDerGrinten                  = 25,
      CT_NewZealandMapGrid              = 26,
      CT_TransvMercator_SouthOriented   = 27
   };
   
   ossimGeoTiffCoordTransformsLut();

   virtual ~ossimGeoTiffCoordTransformsLut(){}
   
   virtual ossimKeyword getKeyword() const
      { return ossimKeyword("geotiff_coordinate_transform_code", ""); }
};

#endif
