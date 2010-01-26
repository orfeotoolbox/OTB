//*******************************************************************
// Copyright (C) 2004 Garrett Potts.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimNitfNameConversionTables.cpp 13219 2008-07-23 19:27:49Z dburken $

#include <cstddef>
#include <ossim/support_data/ossimNitfNameConversionTables.h>
#include <ossim/base/ossimString.h>

struct ossimNitfMapProjectionCodeToOssimName
{
   const char* nitfProjCode; // nitf code name
   const char* projectionName; // ossim projection name
};

struct ossimNitfMapProjectionCodeToNitfName
{
   const char* nitfProjCode; // nitf code name
   const char* projectionName; // ossim projection name
};

static const ossimNitfMapProjectionCodeToOssimName mapProjectiontable[]=
{
   {"AC", "ossimAlbersProjection"},
   {"AL", "ossimAzimEquDistProjection"},
   {"BF", "ossimBonneProjection"},
   {"CP", "ossimEquDistCylProjection"},
   {"CP", "ossimLlxyProjection"},
   {"CS", "ossimCassiniProjection"},
   {"ED", "ossimEckert6Projection"},
   {"EF", "ossimEckert4Projection"},
   {"GN", "ossimGnomonicProjection"},
   {"LE", "ossimLambertConformalConicProjection"},
   {"LI", "ossimCylEquAreaProjection"},
   {"MC", "ossimMercatorProjection"},
   {"MH", "ossimMillerProjection"},
   {"MP", "ossimMollweidProjection"},
   {"NT", "ossimNewZealandMapGridProjection"},
   {"OD", "ossimOrthoGraphicProjection"},
   {"PG", "ossimPolarStereoProjection"},
   {"PH", "ossimPolyconicProjection"},
   {"SA", "ossimSinusoidalProjection"},
   {"SX", "ossimSpaceObliqueMercatorProjection"},
   {"TC", "ossimTransMercatorProjection"},
   {"TX", "ossimTransCylEquAreaProjection"},
   {"VA", "ossimVanDerGrintenProjection"},
   { 0, 0 }
};

static const ossimNitfMapProjectionCodeToNitfName nitfMapProjectiontable[]=
{
   {"AC", "Albers Equal-Area Conic"},
   {"AK", "(Lambert) Azimuthal Equal-Area"},
   {"AL", "Azimuthal Equidistant"},
   {"BF", "Bonne"},
   {"CC", "Equidistant Conic with 1 Standard Parallel"},
   {"CP", "Equirectangular"},
   {"CS", "Cassini-Soldner"},
   {"ED", "Eckert VI"},
   {"EF", "Eckert IV"},
   {"GN", "Gnomonic"},
   {"HX", "Hotine Oblique Mercator based on 2 Points"},
   {"KA", " Equidistant Conic with 2 Standard Parallels"},
   {"LA", "Laborde"},
   {"LE", "Lambert Conformal Conic"},
   {"LI", "Cylindrical Equal Area"},
   {"LJ", " Lambert Equal-Area Meridional"},
   {"MC", "Mercator"},
   {"MH", "Miller Cylindrical"},
   {"MJ", "French Lambert"},
   {"MP", "Mollweide"},
   {"NT", "New Zealand Map Grid"},
   {"OC", "Oblique Mercator"},
   {"OD", "Orthographic"},
   {"PG", "Polar Stereographic"},
   {"PH", "Polyconic"},
   {"RS", "Hotine Oblique Mercator (Rectified Skew Orthomorphic)"},
   {"RB", "Hotine Oblique Mercator (Rectified Skew Orthomorphic)"},
   {"RX", "Robinson"},
   {"SA", "Sinusoidal"},
   {"SD", "Oblique Stereographic"},
   {"SX", "Space Oblique Mercator"},
   {"TC", " Transverse Mercator"},
   {"TX", "Transverse Cylindrical Equal Area"},
   {"VA", "Van der Grinten"},
   {"VX", "General Vertical NearSide Perspective"},
   { 0, 0 }
};

ossimNitfNameConversionTables::ossimNitfNameConversionTables()
{
}

ossimString ossimNitfNameConversionTables::convertMapProjectionNameToNitfCode(const ossimString& mapProjectionName)const
{
   ossim_uint32 idx = 0;

   while(mapProjectiontable[idx].nitfProjCode)
   {
      if(mapProjectionName == mapProjectiontable[idx].projectionName)
      {
         return mapProjectiontable[idx].nitfProjCode;
      }
      ++idx;
   }

   return "";
}

ossimString ossimNitfNameConversionTables::convertNitfCodeToOssimProjectionName(const ossimString& nitfProjectionCode)const
{
   ossim_uint32 idx = 0;

   while(mapProjectiontable[idx].nitfProjCode)
   {
      if(nitfProjectionCode == mapProjectiontable[idx].nitfProjCode)
      {
         return mapProjectiontable[idx].projectionName;
      }
      ++idx;
   }

   return "";
}

ossimString ossimNitfNameConversionTables::convertNitfCodeToNitfProjectionName(const ossimString& nitfProjectionCode)const
{
   ossim_uint32 idx = 0;

   while(nitfMapProjectiontable[idx].nitfProjCode)
   {
      if(nitfProjectionCode == nitfMapProjectiontable[idx].nitfProjCode)
      {
         return nitfMapProjectiontable[idx].projectionName;
      }
      ++idx;
   }

   return "";
}

ossimString ossimNitfNameConversionTables::convertNitfProjectionNameToNitfCode(const ossimString& nitfProjectionName)const
{
   ossim_uint32 idx = 0;

   while(nitfMapProjectiontable[idx].nitfProjCode)
   {
      if(nitfProjectionName ==  nitfMapProjectiontable[idx].projectionName)
      {
         return nitfMapProjectiontable[idx].nitfProjCode;
      }
      ++idx;
   }

   return "";
}
