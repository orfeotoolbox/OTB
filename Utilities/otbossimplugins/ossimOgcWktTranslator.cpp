//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerda@home.com)
// Contributor: David A. Horner
//
// Description:
//
// Contains code implementation translation between OGC WKT format,
// and OSSIM projection keyword lists.  Note that services from 
// GDAL's OGRSpatialReference class are used via the GDAL bridge to
// accomplish the translation. 
//
//*******************************************************************
//  $Id: ossimOgcWktTranslator.cpp 12031 2007-11-13 18:03:57Z gpotts $

#include <cstdio>
#include <gdal.h>

#include "ossimOgcWktTranslator.h"
#include <ogr_spatialref.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/projection/ossimAlbersProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimStatePlaneProjectionFactory.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>

   
static const double SEMI_MAJOR_AXIS_WGS84 = 6378137.0;
static const double SEMI_MINOR_AXIS_WGS84 = 6356752.3142;

static ossimTrace traceDebug("ossimOgcWktTranslator:debug");

//*******************************************************************
// Public Function:
//*******************************************************************
ossimOgcWktTranslator::ossimOgcWktTranslator()
{
   initializeDatumTable();
   initializeProjectionTable();
}

ossimString ossimOgcWktTranslator::fromOssimKwl(const ossimKeywordlist &kwl,
                                                const char *prefix)const
{
   ossimString projType = kwl.find(ossimKeywordNames::TYPE_KW);
   ossimString datumType = kwl.find(ossimKeywordNames::DATUM_KW);
   
   ossimString wktString;
   OGRSpatialReference oSRS;
   
   if(projType == "")
   {
      return wktString;
   }
   
   ossimString zone = kwl.find(prefix, ossimKeywordNames::ZONE_KW);
   ossimString hemisphere = kwl.find(prefix, ossimKeywordNames::HEMISPHERE_KW);
   ossimString parallel1  = kwl.find(prefix, ossimKeywordNames::STD_PARALLEL_1_KW);
   ossimString parallel2  = kwl.find(prefix, ossimKeywordNames::STD_PARALLEL_2_KW);
   ossimString originLat  = kwl.find(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW);
   ossimString centralMeridian = kwl.find(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW);
   ossimString scale = kwl.find(prefix, ossimKeywordNames::SCALE_FACTOR_KW);

   ossimDpt falseEastingNorthing;
   // Get the false easting northing.
   falseEastingNorthing.x = 0.0;
   falseEastingNorthing.y = 0.0;
   const char *lookup =
      kwl.find(prefix, ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW);
   if (lookup)
   {
      ossimUnitType units =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                    getEntryNumber(lookup));
      
      lookup = kwl.find(prefix, ossimKeywordNames::FALSE_EASTING_NORTHING_KW);
      if (lookup)
      {
         ossimDpt eastingNorthing;
         eastingNorthing.toPoint(std::string(lookup));
         
         switch (units)
         {
            case OSSIM_METERS:
            {
               falseEastingNorthing = eastingNorthing;
               break;
            }
            case OSSIM_FEET:
            case OSSIM_US_SURVEY_FEET:
            {
               ossimUnitConversionTool ut;
               ut.setValue(eastingNorthing.x, units);
               falseEastingNorthing.x = ut.getValue(OSSIM_METERS);
               ut.setValue(eastingNorthing.y, units);
               falseEastingNorthing.y = ut.getValue(OSSIM_METERS);
               break;
            }
            default:
            {
               // Unhandled unit type!
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossimOgcWktTranslator::fromOssimKwl WARNING!"
                  << "Unhandled unit type for "
                  << ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW
                  << ":  " 
                  << ( ossimUnitTypeLut::instance()->
                       getEntryString(units).c_str() )
                  << endl;
               break;
            }
         } // End of switch (units)
         
      }  // End of if (FALSE_EASTING_NORTHING_KW)

   } // End of if (FALSE_EASTING_NORTHING_UNITS_KW)
   else
   {
      // BACKWARDS COMPATIBILITY LOOKUPS...
      lookup =  kwl.find(prefix, ossimKeywordNames::FALSE_EASTING_KW);
      if(lookup)
      {
         falseEastingNorthing.x = fabs(ossimString(lookup).toFloat64());
      }
      
      lookup =  kwl.find(prefix, ossimKeywordNames::FALSE_NORTHING_KW);
      if(lookup)
      {
         falseEastingNorthing.y = fabs(ossimString(lookup).toFloat64());
      }
   }

   oSRS.SetLinearUnits("Meter", 1.0);
   
   if(projType == "ossimUtmProjection")
   {
      hemisphere = hemisphere.trim().upcase();
      
      if(hemisphere != "")
      {
         oSRS.SetUTM(zone.toLong(), hemisphere != "S");
      }
      else
      {
         oSRS.SetUTM(zone.toLong(), true);
      }
   }
   else if(projType == "ossimLlxyProjection")
   {
      OGRSpatialReference oGeogCS;
      
      oGeogCS.SetEquirectangular(0.0,
                                 0.0,
                                 0.0,
                                 0.0);
      oGeogCS.SetAngularUnits(SRS_UA_DEGREE, atof(SRS_UA_DEGREE_CONV));
      
      oSRS.CopyGeogCSFrom( &oGeogCS );  
   }
   else if(projType == "ossimEquDistCylProjection")
   {
      OGRSpatialReference oGeogCS;
      
      oGeogCS.SetEquirectangular(originLat.toDouble(),
                                 centralMeridian.toDouble(),
                                 falseEastingNorthing.x,
                                 falseEastingNorthing.y);
      oGeogCS.SetAngularUnits(SRS_UA_DEGREE, atof(SRS_UA_DEGREE_CONV));
      
      oSRS.CopyGeogCSFrom( &oGeogCS );  
   }
   else if(projType == "ossimSinusoidalProjection")
   {
      oSRS.SetSinusoidal(centralMeridian.toDouble(),
                         falseEastingNorthing.x,
                         falseEastingNorthing.y);
   }
   else if(projType == "ossimCylEquAreaProjection")
   {
      oSRS.SetCEA(originLat.toDouble(),
                  centralMeridian.toDouble(),
                  falseEastingNorthing.x,
                  falseEastingNorthing.y);
   }
   else if(projType == "ossimCassiniProjection")
   {
      oSRS.SetCS(originLat.toDouble(),
                 centralMeridian.toDouble(),
                 falseEastingNorthing.x,
                 falseEastingNorthing.y);
   }
   else if(projType == "ossimAlbersProjection")
   {
      oSRS.SetACEA(parallel1.toDouble(),
                   parallel2.toDouble(),
                   originLat.toDouble(),
                   centralMeridian.toDouble(),
                   falseEastingNorthing.x,
                   falseEastingNorthing.y);
   }
   else if(projType == "ossimAzimEquDistProjection")
   {
      oSRS.SetAE(originLat.toDouble(),
                 centralMeridian.toDouble(),
                 falseEastingNorthing.x,
                 falseEastingNorthing.y);
   }
   else if(projType == "ossimEckert4Projection")
   {
      oSRS.SetEckertIV(centralMeridian.toDouble(),
                       falseEastingNorthing.x,
                       falseEastingNorthing.y);
   }
   else if(projType == "ossimEckert6Projection")
   {
      oSRS.SetEckertVI(centralMeridian.toDouble(),
                       falseEastingNorthing.x,
                       falseEastingNorthing.y);
   }
   else if(projType == "ossimGnomonicProjection")
   {
      oSRS.SetGnomonic(originLat.toDouble(),
                       centralMeridian.toDouble(),
                       falseEastingNorthing.x,
                       falseEastingNorthing.y);
   }
   else if(projType == "ossimLambertConformalConicProjection")
   {
      oSRS.SetLCC(parallel1.toDouble(),
                  parallel2.toDouble(),
                  originLat.toDouble(),
                  centralMeridian.toDouble(),
                  falseEastingNorthing.x,
                  falseEastingNorthing.y);
   }
   else if(projType == "ossimVanDerGrintenProjection")
   {
      oSRS.SetVDG(centralMeridian.toDouble(),
                  falseEastingNorthing.x,
                  falseEastingNorthing.y);
   }
   else if(projType == "ossimMillerProjection")
   {
      oSRS.SetMC(originLat.toDouble(),
                 centralMeridian.toDouble(),
                 falseEastingNorthing.x,
                 falseEastingNorthing.y);
   }
   else if(projType == "ossimMercatorProjection")
   {
      oSRS.SetMercator(originLat.toDouble(),
                       centralMeridian.toDouble(),
                       scale.toDouble(),
                       falseEastingNorthing.x,
                       falseEastingNorthing.y);
   }
   else if(projType == "ossimMollweidProjection")
   {
      oSRS.SetMollweide(centralMeridian.toDouble(),
                        falseEastingNorthing.x,
                        falseEastingNorthing.y);
   }
   else if(projType == "ossimNewZealandMapGridProjection")
   {
      oSRS.SetNZMG(originLat.toDouble(),
                   centralMeridian.toDouble(),
                   falseEastingNorthing.x,
                   falseEastingNorthing.y);
   }
   else if(projType == "ossimOrthoGraphicProjection")
   {
      oSRS.SetOrthographic(originLat.toDouble(),
                           centralMeridian.toDouble(),
                           falseEastingNorthing.x,
                           falseEastingNorthing.y);
   }
   else if(projType == "ossimPolarStereoProjection")
   {
      oSRS.SetPS(originLat.toDouble(),
                 centralMeridian.toDouble(),
                 scale.toDouble(),
                 falseEastingNorthing.x,
                 falseEastingNorthing.y);
   }
   else if(projType == "ossimPolyconicProjectio")
   {
      oSRS.SetPolyconic(originLat.toDouble(),
                        centralMeridian.toDouble(),
                        falseEastingNorthing.x,
                        falseEastingNorthing.y);
   }
   else if(projType == "ossimStereographicProjection")
   {
      oSRS.SetStereographic(originLat.toDouble(),
                            centralMeridian.toDouble(),
                            scale.toDouble(),
                            falseEastingNorthing.x,
                            falseEastingNorthing.y);
   }
   else if(projType == "ossimTransMercatorProjection")
   {
      oSRS.SetTM(originLat.toDouble(),
                 centralMeridian.toDouble(),
                 scale.toDouble(),
                 falseEastingNorthing.x,
                 falseEastingNorthing.y);
   }
   else 
   {
      cerr << "ossimOgcWktTranslator::fromOssimKwl:\n"
           << "Projection translation for "
           << projType
           << " not supported "
           << endl;
   }
   
   datumType =  datumType.upcase();
   
   if(datumType == "WGE")
   {
      oSRS.SetWellKnownGeogCS("WGS84");
   }
   else if(datumType == "WGD")
   {
      oSRS.SetWellKnownGeogCS("WGS72");      
   }
   else if(datumType == "NAS-C") //1927
   {
      oSRS.SetWellKnownGeogCS("NAD27");      
   }
   else if(datumType == "NAS") //1927
   {
      oSRS.SetWellKnownGeogCS("NAD27");      
   }
   else if(datumType == "NAR-C") // 1983
   {
      oSRS.SetWellKnownGeogCS("NAD83");
   }
   else if(datumType == "NAR") // 1983
   {
      oSRS.SetWellKnownGeogCS("NAD83");
   }
   else
   {
      cerr << "ossimOgcWktTranslator::fromOssimKwl: Datum translation for "
           << datumType
           <<" not supported"
           << endl;
   }
   
   char* exportString = NULL;
   oSRS.exportToWkt(&exportString);
   
   if(exportString)
   {
      wktString = exportString;
      OGRFree(exportString);
   }
   
   return wktString;
   
}

bool ossimOgcWktTranslator::toOssimKwl( const ossimString& wktString,
                                        ossimKeywordlist &kwl,
                                        const char *prefix)const

{
   static const char MODULE[] = "ossimOgcWktTranslator::toOssimKwl";
   
   const char* wkt = wktString.c_str();
   
   OGRSpatialReferenceH  hSRS = NULL;
   ossimDpt falseEastingNorthing;
   
   // Translate the WKT into an OGRSpatialReference. 
   hSRS = OSRNewSpatialReference(NULL);
   if( OSRImportFromWkt( hSRS, (char **) &wkt ) != OGRERR_NONE )
   {
      OSRDestroySpatialReference( hSRS );
      return false;
   }
   
   // Determine if State Plane Coordinate System
   ossimString ossimProj = "";
   const ossimStatePlaneProjectionInfo* spi = NULL;
   const char* epsg_code = OSRGetAttrValue( hSRS, "AUTHORITY", 1 );
   if(epsg_code)
   {
      ossimStatePlaneProjectionFactory* spf =
         ossimStatePlaneProjectionFactory::instance();
      spi = spf->getInfo(ossimString::toInt(epsg_code));
      
      if(spi)
      {
         ossimProj = "ossimStatePlaneProjection";
      }
    }


   // Extract Units 
   //
   const char* units = OSRGetAttrValue( hSRS, "UNIT", 0 );
   ossimString ossim_units = "meters";
   
   if(units)
   {
      if(ossimString(units) == ossimString("U.S. Foot"))
         ossim_units = "us_survey_feet";
      else if(ossimString(units) == ossimString("US survey foot"))
         ossim_units = "us_survey_feet";
      else if(ossimString(units) == ossimString("degree"))
         ossim_units = "degrees";
      else if(ossimString(units) == ossimString("Meter"))
         ossim_units = "meters";
      else if(ossimString(units) == ossimString("metre"))
         ossim_units = "meters";
   }
   kwl.add(prefix, ossimKeywordNames::UNITS_KW, ossim_units, true);
   if(ossimProj == "") // Not State Plane Projection
   {
      // Determine which other Projection System is represented.
      const char* pszProjection = OSRGetAttrValue( hSRS, "PROJECTION", 0 );
      if(pszProjection)
      {
         ossimProj = wktToOssimProjection(pszProjection);
      }
      else
      {
         ossimString localCs = OSRGetAttrValue( hSRS, "LOCAL_CS", 0 );
         localCs = localCs.upcase();
 //        std::cout << "localCs ========== " << localCs << std::endl;
         if(localCs == "GREATBRITAIN_GRID")
         {
	    ossimProj = "ossimBngProjection";
         }
         else if (ossim_units.contains("degree"))
         {
            // Assumption...
            ossimProj = "ossimEquDistCylProjection";
         }
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "DEBUG:"
         << "\nossimProj = " << ossimProj << endl;
   }
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           ossimProj.c_str(),
           true);

   falseEastingNorthing.x = OSRGetProjParm(hSRS, SRS_PP_FALSE_EASTING, 0.0, NULL);
   falseEastingNorthing.y = OSRGetProjParm(hSRS, SRS_PP_FALSE_NORTHING, 0.0, NULL);
   if (ossimProj == "ossimStatePlaneProjection")
   {
      kwl.add(prefix, ossimKeywordNames::PCS_CODE_KW, epsg_code, true);
   }
   else if(ossimProj == "ossimBngProjection")
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimBngProjection",
              true);
   }
   else if(ossimProj == "ossimCylEquAreaProjection")
   {
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_1_KW,
              OSRGetProjParm(hSRS, SRS_PP_STANDARD_PARALLEL_1, 0.0, NULL),
              true);
      
      kwl.add(prefix,
              ossimKeywordNames::ORIGIN_LATITUDE_KW,
              OSRGetProjParm(hSRS, SRS_PP_STANDARD_PARALLEL_1, 0.0, NULL),
              true);

      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
              falseEastingNorthing.toString(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
              ossim_units,
              true);
              
//       kwl.add(prefix,
//               ossimKeywordNames::FALSE_EASTING_KW,
//               OSRGetProjParm(hSRS, SRS_PP_FALSE_EASTING, 0.0, NULL),
//               true);
      
//       kwl.add(prefix,
//               ossimKeywordNames::FALSE_NORTHING_KW,
//               OSRGetProjParm(hSRS, SRS_PP_FALSE_NORTHING, 0.0, NULL),
//               true);
   }
   else if(ossimProj == "ossimEquDistCylProjection")
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimEquDistCylProjection",
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
              falseEastingNorthing.toString(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
              ossim_units,
              true);
      kwl.add(prefix,
              ossimKeywordNames::ORIGIN_LATITUDE_KW,
              OSRGetProjParm(hSRS, SRS_PP_LATITUDE_OF_ORIGIN, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::CENTRAL_MERIDIAN_KW,
              OSRGetProjParm(hSRS, SRS_PP_CENTRAL_MERIDIAN, 0.0, NULL),
              true);
   }
   else if( (ossimProj == "ossimLambertConformalConicProjection") ||
            (ossimProj == "ossimAlbersProjection") )
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              ossimProj.c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
              falseEastingNorthing.toString(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
              ossim_units,
              true);
      kwl.add(prefix,
              ossimKeywordNames::ORIGIN_LATITUDE_KW,
              OSRGetProjParm(hSRS, SRS_PP_LATITUDE_OF_ORIGIN, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::CENTRAL_MERIDIAN_KW,
              OSRGetProjParm(hSRS, SRS_PP_CENTRAL_MERIDIAN, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_1_KW,
              OSRGetProjParm(hSRS, SRS_PP_STANDARD_PARALLEL_1, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::STD_PARALLEL_2_KW,
              OSRGetProjParm(hSRS, SRS_PP_STANDARD_PARALLEL_2, 0.0, NULL),
              true);
   }
   else if(ossimProj == "ossimMercatorProjection")
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimMercatorProjection",
              true);
      kwl.add(prefix,
              ossimKeywordNames::ORIGIN_LATITUDE_KW,
              OSRGetProjParm(hSRS, SRS_PP_LATITUDE_OF_ORIGIN, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::CENTRAL_MERIDIAN_KW,
              OSRGetProjParm(hSRS, SRS_PP_CENTRAL_MERIDIAN, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
              falseEastingNorthing.toString(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
              ossim_units,
              true);
   }
   else if(ossimProj == "ossimSinusoidalProjection")
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimSinusoidalProjection",
              true);
      kwl.add(prefix,
              ossimKeywordNames::CENTRAL_MERIDIAN_KW,
              OSRGetProjParm(hSRS, SRS_PP_CENTRAL_MERIDIAN, 0.0, NULL),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
              falseEastingNorthing.toString(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
              ossim_units,
              true);
   }
   else if(ossimProj == "ossimTransMercatorProjection")
   {
      int bNorth;
      int nZone = OSRGetUTMZone( hSRS, &bNorth );
      
      if( nZone != 0 )
      {
         kwl.add(prefix,
                 ossimKeywordNames::TYPE_KW,
                 "ossimUtmProjection",
                   true);
         
         kwl.add(prefix,
                 ossimKeywordNames::ZONE_KW,
                 nZone,
                 true);
         if( bNorth )
         {
            kwl.add(prefix, ossimKeywordNames::HEMISPHERE_KW, "N", true);
         }
         else
         {
            kwl.add(prefix, ossimKeywordNames::HEMISPHERE_KW, "S", true);
         }
      }            
      else
      {
         kwl.add(prefix,
                 ossimKeywordNames::TYPE_KW,
                 "ossimTransMercatorProjection",
                   true);
         
         kwl.add(prefix,
                 ossimKeywordNames::ORIGIN_LATITUDE_KW,
                 OSRGetProjParm(hSRS, SRS_PP_LATITUDE_OF_ORIGIN, 0.0, NULL),
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::CENTRAL_MERIDIAN_KW,
                 OSRGetProjParm(hSRS, SRS_PP_CENTRAL_MERIDIAN, 0.0, NULL),
                 true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
              falseEastingNorthing.toString(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
              ossim_units,
              true);
      }
   }
   else
   {
      if (traceDebug())
       {
          ossimNotify(ossimNotifyLevel_DEBUG)
             << "ossimOgcWktTranslator::toOssimKwl DEBUG:\n"
             << "Projection conversion to OSSIM not supported !!!!!!!!!\n"
             << "Please send the following string to the development staff\n" 
             << "to be added to the transaltion to OSSIM\n"
             << wkt << endl;
       }
    }

    // extract out the datum
    //
    const char *datum = OSRGetAttrValue( hSRS, "DATUM", 0 );
    ossimString oDatum = "WGE";
    
    if( datum )
    {
       oDatum = wktToOssimDatum(datum);
       if(oDatum == "")
       {
          oDatum = "WGE";
       }
    }
       
    kwl.add(prefix, ossimKeywordNames::DATUM_KW, oDatum, true);

     
    OSRDestroySpatialReference( hSRS );
    
    return true;
}


void ossimOgcWktTranslator::initializeDatumTable()
{
//    theWktToOssimDatumTranslation.insert(make_pair(ossimString("North_American_Datum_1927"),
//                                                   ossimString("NAS-C")));
  
//    theWktToOssimDatumTranslation.insert(make_pair(ossimString("North_American_Datum_1983"),
//                                                   ossimString("NAR-C")));
//    theWktToOssimDatumTranslation.insert(make_pair(ossimString("WGS_1984"),
//                                                   ossimString("WGE")));
//    theWktToOssimDatumTranslation.insert(make_pair(ossimString("OSGB_1936"),
//                                                   ossimString("OSGB_1936")));
  
   theOssimToWktDatumTranslation.insert(make_pair(ossimString("NAS-C"),
                                                  ossimString("North_American_Datum_1927")));
   theOssimToWktDatumTranslation.insert(make_pair(ossimString("NAR-C"),
                                                  ossimString("North_American_Datum_1983")));
   theOssimToWktDatumTranslation.insert(make_pair(ossimString("NAS"),
                                                  ossimString("North_American_Datum_1927")));
   theOssimToWktDatumTranslation.insert(make_pair(ossimString("NAR"),
                                                  ossimString("North_American_Datum_1983")));
   theOssimToWktDatumTranslation.insert(make_pair(ossimString("WGE"),
                                                  ossimString("WGS_1984")));
   theWktToOssimDatumTranslation.insert(make_pair(ossimString("OSGB_1936"),
                                                  ossimString("OSGB_1936"))); 
}

void ossimOgcWktTranslator::initializeProjectionTable()
{
   theWktToOssimProjectionTranslation.insert(make_pair(ossimString(SRS_PT_TRANSVERSE_MERCATOR),
                                                       ossimString("ossimTransMercatorProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(ossimString(SRS_PT_EQUIRECTANGULAR),
                                                       ossimString("ossimEquDistCylProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(ossimString(SRS_PT_ALBERS_CONIC_EQUAL_AREA),
                                                       ossimString("ossimAlbersProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(ossimString(SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP),
                                                       ossimString("ossimLambertConformalConicProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(ossimString(SRS_PT_SINUSOIDAL),
                                                       ossimString("ossimSinusoidalProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(ossimString(SRS_PT_MERCATOR_1SP),
                                                       ossimString("ossimMercatorProjection")));
   theOssimToWktProjectionTranslation.insert(make_pair(ossimString("ossimTransMercatorProjection"),
                                                       ossimString(SRS_PT_TRANSVERSE_MERCATOR)));
   theOssimToWktProjectionTranslation.insert(make_pair(ossimString("ossimEquDistCylProjection"),
                                                       ossimString(SRS_PT_EQUIRECTANGULAR)));
   theOssimToWktProjectionTranslation.insert(make_pair(ossimString("ossimAlbersProjection"),
                                                       ossimString(SRS_PT_ALBERS_CONIC_EQUAL_AREA)));
   theOssimToWktProjectionTranslation.insert(make_pair(ossimString("ossimLambertConformalConicProjection"),
                                                       ossimString(SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP)));
   theOssimToWktProjectionTranslation.insert(make_pair(ossimString("ossimSinusoidalProjection"),
                                                       ossimString(SRS_PT_SINUSOIDAL)));
   theOssimToWktProjectionTranslation.insert(make_pair(ossimString("ossimMercatorProjection"),
                                                       ossimString(SRS_PT_MERCATOR_1SP)));
   
}

ossimString ossimOgcWktTranslator::wktToOssimDatum(const ossimString& datum)const
{
   map<ossimString, ossimString>::const_iterator i = theWktToOssimDatumTranslation.find(datum);

   if(datum.contains("North_American_Datum_1927"))
   {
      return "NAS-C";
   }
   if(datum.contains("North_American_Datum_1983"))
   {
      return "NAR-C";
   }
   if(datum.contains("WGS_1984"))
   {
      return "WGE";
   }
   if(datum.contains("OSGB_1936"))
   {
      return "OSGB_1936";
   }
//    if(i != theWktToOssimDatumTranslation.end())
//    {
//       return (*i).second;
//    }

   return "";
}

ossimString ossimOgcWktTranslator::ossimToWktDatum(const ossimString& datum)const
{
   map<ossimString, ossimString>::const_iterator i = theOssimToWktDatumTranslation.find(datum);

   if(i != theOssimToWktDatumTranslation.end())
   {
      return (*i).second;
   }

   return "";
}

ossimString ossimOgcWktTranslator::wktToOssimProjection(const ossimString& datum)const
{
   map<ossimString, ossimString>::const_iterator i = theWktToOssimProjectionTranslation.find(datum);

   if(i != theWktToOssimProjectionTranslation.end())
   {
      return (*i).second;
   }

   return "";
}

ossimString ossimOgcWktTranslator::ossimToWktProjection(const ossimString& datum)const
{
   map<ossimString, ossimString>::const_iterator i = theOssimToWktProjectionTranslation.find(datum);

   if(i != theOssimToWktProjectionTranslation.end())
   {
      return (*i).second;
   }

   return "";
}
