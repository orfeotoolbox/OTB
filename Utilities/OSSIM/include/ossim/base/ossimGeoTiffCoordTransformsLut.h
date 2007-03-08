//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for GeoTiffCoordTransformsLut. Used to map a
// geotiff coordinate transformation code to an ossim projection.
//*******************************************************************
//  $Id: ossimGeoTiffCoordTransformsLut.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimGeoTiffCoordTransformsLut_HEADER
#define ossimGeoTiffCoordTransformsLut_HEADER

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/base/ossimKeywordNames.h>

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
