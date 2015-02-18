//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
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
//  $Id: ossimOgcWktTranslator.cpp 20724 2012-04-13 21:00:52Z dburken $

#include <cstdio>
#include <gdal.h>
#include <geovalues.h>

#include "ossimOgcWktTranslator.h"
#include <ogr_spatialref.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimAlbersProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
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

#define EPSG_CODE_MAX 32767
#define USER_DEFINED  32767
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
   ossimString pcsCode = kwl.find(prefix, ossimKeywordNames::PCS_CODE_KW);

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

   int pcsCodeVal = (pcsCode.empty() == false) ? pcsCode.toInt() : EPSG_CODE_MAX;
   
   // since approximately ossim version r20036
   // kwl.find(prefix, ossimKeywordNames::PCS_CODE_KW)
   // return 0 instead of "" previously.
   // the to following lines ensure backward compatibility
   // Since EPSG:0 is not a valid epsg code, the fix is safe
   // (ref http://spatialreference.org/ref/epsg/)
   if(pcsCodeVal == 0)
        pcsCodeVal = EPSG_CODE_MAX;
   
   if(pcsCodeVal < EPSG_CODE_MAX)
   {
      // ESH 06/2008 -- HACK!!!
      // If the PCS code is for a HARN state plane and the implied pcs code's units 
      // is feet (us or intl), we find the equivalent code for units of meters.  
      // We're doing this because ArcMap (9.2 and less) doesn't understand the 
      // non-meters HARN codes.  However, the units are left unchanged in this 
      // process, so the units can be different than the user-specified pcs code. 
      // ArcMap 9.2 seems to understand the mixed definition just fine.
      ossimEpsgProjectionDatabase* proj_db = ossimEpsgProjectionDatabase::instance();
      ossimString pcsCodeName = proj_db->findProjectionName(pcsCodeVal);
      if ( pcsCodeName.contains("HARN") && pcsCodeName.contains("_Feet") )
      {
         ossimString feetStr("_Feet");
         ossimString newPcsCodeName( pcsCodeName.before(feetStr).c_str() );
         ossimString epsg_spec = proj_db->findProjectionCode(newPcsCodeName);
         ossim_uint32 new_code = epsg_spec.after(":").toUInt32();
         if (new_code)
          pcsCodeVal = new_code;
      }
      oSRS.importFromEPSG( pcsCodeVal );
   }
   if(projType == "ossimUtmProjection")
   {
#if 0
      hemisphere = hemisphere.trim().upcase();
      
      if(hemisphere != "")
      {
         oSRS.SetUTM(zone.toLong(), hemisphere != "S");
      }
      else
      {
         oSRS.SetUTM(zone.toLong(), true);
      }
#else
      // ESH 09/2008: Adapting code from ossimTiffWriter.cpp for 
      // converting UTM zone,hemisphere to a PCS code.

      short gcs = USER_DEFINED;

      if (datumType == "WGE") gcs = GCS_WGS_84;
      else if (datumType == "WGD") gcs = GCS_WGS_72;
      else if (datumType == "NAR-C") gcs = GCS_NAD83;
      else if (datumType == "NAR") gcs = GCS_NAD83;
      else if (datumType == "NAS-C") gcs = GCS_NAD27;
      else if (datumType == "NAS") gcs = GCS_NAD27;
      else if (datumType == "ADI-M") gcs = GCS_Adindan;
      else if (datumType == "ARF-M") gcs = GCS_Arc_1950;
      else if (datumType == "ARS-M") gcs = GCS_Arc_1960;
      else if (datumType == "EUR-7" || datumType == "EUR-M") gcs = GCS_ED50;
      else if ((datumType == "OGB-7") ||
               (datumType == "OGB-M") ||
               (datumType == "OGB-A") ||
               (datumType == "OGB-B") ||
               (datumType == "OGB-C") ||
               (datumType == "OGB-D")) gcs = GCS_OSGB_1936;
      else if  (datumType == "TOY-M")  gcs = GCS_Tokyo;
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DATUM = " << datumType << " tag not written " << std::endl
               << "Please let us know so we can add it"          << std::endl;
         }
      }

      int mapZone = zone.toInt();
      hemisphere = hemisphere.trim().upcase();
      bool bDoImportFromEPSG = false;

      switch ( gcs )
      {
         case GCS_WGS_84:
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCodeVal = 32600 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCodeVal = 32700 + mapZone;
            }
            bDoImportFromEPSG = true;
            break;
         }

         case GCS_WGS_72:
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCodeVal = 32200 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCodeVal = 32300 + mapZone;
            }
            bDoImportFromEPSG = true;
            break;
         }

         case GCS_NAD27:
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCodeVal = 26700 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCodeVal = 32000 + mapZone;
            }
            bDoImportFromEPSG = true;
            break;
         }

         case GCS_NAD83:
         {
            if (hemisphere == "N") // Northern hemisphere.
            {
               pcsCodeVal = 26900 + mapZone;
            }
            else // Southern hemisphere.
            {
               pcsCodeVal = 32100 + mapZone;
            }
            bDoImportFromEPSG = true;
            break;
         }

         default:
         {
            //***
            // Use a projection code that does not imply a datum.
            // See section "6.3.3.2 Projection Codes" for definition.
            //***
            if (mapZone > 0) // Northern hemisphere.
            {
               pcsCodeVal = 16000 + mapZone;
            }
            else if (mapZone < 0) // Southern hemisphere.
            {
               hemisphere = "S";
               pcsCodeVal = 16100 + abs(mapZone);
            }
            break;
         }
      } // End of "switch ( gcs )"

      if ( bDoImportFromEPSG == true )
         oSRS.importFromEPSG( pcsCodeVal );
      else
      {
         if(hemisphere != "")
         {
            oSRS.SetUTM(zone.toLong(), hemisphere != "S");
         }
         else
         {
            oSRS.SetUTM(zone.toLong(), true);
         }
      }
#endif
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
   
   if(pcsCodeVal >= EPSG_CODE_MAX)
   {
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
      else if(datumType == "NTF")
      {
        oSRS.SetWellKnownGeogCS("EPSG:4275");
      }
      else
      {
         cerr << "ossimOgcWktTranslator::fromOssimKwl: Datum translation for "
              << datumType
              <<" not supported"
              << endl;
      }
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

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }
   
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
   const char* epsg_code = OSRGetAttrValue( hSRS, "AUTHORITY", 1 );

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "epsg_code: " << (epsg_code?epsg_code:"null") << "\n";
   }
   
   //---
   // Extract Units 
   // ESH 11/2008: Check for geographic system when setting default units. 
   // If geographic, use degrees.
   //---
//   const char* units = OSRGetAttrValue( hSRS, "UNIT", 0 );
   const char* units = NULL;
   OGR_SRSNode* node = ((OGRSpatialReference *)hSRS)->GetRoot();
   int nbChild  = node->GetChildCount();
   for (int i = 0; i < nbChild; i++)
   {
      OGR_SRSNode* curChild = node->GetChild(i);
      if (strcmp(curChild->GetValue(), "UNIT") == 0)
      {
         units = curChild->GetChild(0)->GetValue();
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "units: " << (units?units:"null") << "\n";
   }
   
   ossimString ossim_units;
   bool bGeog = OSRIsGeographic(hSRS);
   if ( bGeog == false )
   {
      ossim_units = "meters";
      if ( units != NULL )
      {
         // Down case to avoid case mismatch.
         ossimString s = units;
         s.downcase();
         
         if( ( s == ossimString("us survey foot") ) ||
             ( s == ossimString("u.s. foot") ) ||
             ( s == ossimString("foot_us") ) )
         {
            ossim_units = "us_survey_feet";
         }
         else if( s == ossimString("degree") )
         {
            ossim_units = "degrees";
         }
         else if( ( s == ossimString("meter") ) ||
                  ( s == ossimString("metre") ) )
         {
            ossim_units = "meters";
         }
      }
   }
   else
   {
      ossim_units = "degrees";
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossim_units: " << ossim_units << "\n";
   }

   // OLK 05/2010 -- ossimProj string was never being set to projection type (except state plane
   // which is not even a projection type)! This was a bug. Using EPSG factory to attempt to get 
   // proper projection:
   if (epsg_code)
   {
      ossimString epsg_spec ("EPSG:"); 
      epsg_spec += ossimString::toString(ossimString(epsg_code));
      ossimProjection* proj = ossimEpsgProjectionFactory::instance()->createProjection(epsg_spec);
      if (proj)
         ossimProj = proj->getClassName();
      delete proj;
   }

   if(ossimProj == "") 
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

   // ESH 11/2008: Ticket #479, if we've got a geographic coordsys 
   // make sure the units are set to degrees.
   if(ossimProj == "ossimEquDistCylProjection" )
      ossim_units = "degrees";

   // ESH 11/2008: Ticket #479, don't set units until we've checked
   // whether or not the projection was updated.
   kwl.add(prefix, ossimKeywordNames::UNITS_KW, ossim_units, true);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << "DEBUG:"
         << "\nossimProj = " << ossimProj << endl;
   }
   
   kwl.add(prefix, ossimKeywordNames::TYPE_KW, ossimProj.c_str(), true);

   falseEastingNorthing.x = OSRGetProjParm(hSRS, SRS_PP_FALSE_EASTING, 0.0, NULL);
   falseEastingNorthing.y = OSRGetProjParm(hSRS, SRS_PP_FALSE_NORTHING, 0.0, NULL);
   if (epsg_code)
   {
      kwl.add(prefix, ossimKeywordNames::PCS_CODE_KW, epsg_code, true);
   }
   if(ossimProj == "ossimBngProjection")
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

      ossimUnitType units =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                    getEntryNumber(ossim_units.c_str()));
      if ( units == OSSIM_METERS || 
           units == OSSIM_FEET   || 
           units == OSSIM_US_SURVEY_FEET )
      {
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
   else if(ossimProj == "ossimEquDistCylProjection")
   {
      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimEquDistCylProjection",
              true);
      
      ossimUnitType units =
         static_cast<ossimUnitType>(ossimUnitTypeLut::instance()->
                                    getEntryNumber(ossim_units.c_str()));
      if ( units == OSSIM_METERS || 
           units == OSSIM_FEET   || 
           units == OSSIM_US_SURVEY_FEET )
      {
         kwl.add(prefix,
                 ossimKeywordNames::FALSE_EASTING_NORTHING_KW,
                 falseEastingNorthing.toString(),
                 true);
         kwl.add(prefix,
                 ossimKeywordNames::FALSE_EASTING_NORTHING_UNITS_KW,
                 ossim_units,
                 true);
      }

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
                 ossimKeywordNames::SCALE_FACTOR_KW,
                 OSRGetProjParm(hSRS, SRS_PP_SCALE_FACTOR, 1.0, NULL),
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
      return false;
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
   
   // Trouble with ESPG 3857 
   // The ellipsoid used by ossim is the WGS84 one with minor and major axis differs. 
   // We need to build our own projection in this case. 
   if( oDatum == "WGE" && ossimString(epsg_code) == "3857" ) 
   { 
      oDatum = "6055";
   }     
   kwl.add(prefix, ossimKeywordNames::DATUM_KW, oDatum, true);
     
   OSRDestroySpatialReference( hSRS );

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = true"
         << std::endl;
   }    
      
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
  
   theOssimToWktDatumTranslation.insert(make_pair(std::string("NAS-C"),
                                                  std::string("North_American_Datum_1927")));
   theOssimToWktDatumTranslation.insert(make_pair(std::string("NAR-C"),
                                                  std::string("North_American_Datum_1983")));
   theOssimToWktDatumTranslation.insert(make_pair(std::string("NAS"),
                                                  std::string("North_American_Datum_1927")));
   theOssimToWktDatumTranslation.insert(make_pair(std::string("NAR"),
                                                  std::string("North_American_Datum_1983")));
   theOssimToWktDatumTranslation.insert(make_pair(std::string("WGE"),
                                                  std::string("WGS_1984")));
   theWktToOssimDatumTranslation.insert(make_pair(std::string("OSGB_1936"),
                                                  std::string("OGB-B"))); 
   theWktToOssimDatumTranslation.insert(make_pair(std::string("Nouvelle_Triangulation_Francaise"),
                                                  std::string("NTF")));
}

void ossimOgcWktTranslator::initializeProjectionTable()
{
   theWktToOssimProjectionTranslation.insert(make_pair(std::string(SRS_PT_TRANSVERSE_MERCATOR),
                                                       std::string("ossimTransMercatorProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(std::string(SRS_PT_EQUIRECTANGULAR),
                                                       std::string("ossimEquDistCylProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(std::string(SRS_PT_ALBERS_CONIC_EQUAL_AREA),
                                                       std::string("ossimAlbersProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(std::string(SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP),
                                                       std::string("ossimLambertConformalConicProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(std::string(SRS_PT_SINUSOIDAL),
                                                       std::string("ossimSinusoidalProjection")));
   theWktToOssimProjectionTranslation.insert(make_pair(std::string(SRS_PT_MERCATOR_1SP),
                                                       std::string("ossimMercatorProjection")));
   theOssimToWktProjectionTranslation.insert(make_pair(std::string("ossimTransMercatorProjection"),
                                                       std::string(SRS_PT_TRANSVERSE_MERCATOR)));
   theOssimToWktProjectionTranslation.insert(make_pair(std::string("ossimEquDistCylProjection"),
                                                       std::string(SRS_PT_EQUIRECTANGULAR)));
   theOssimToWktProjectionTranslation.insert(make_pair(std::string("ossimAlbersProjection"),
                                                       std::string(SRS_PT_ALBERS_CONIC_EQUAL_AREA)));
   theOssimToWktProjectionTranslation.insert(make_pair(std::string("ossimLambertConformalConicProjection"),
                                                       std::string(SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP)));
   theOssimToWktProjectionTranslation.insert(make_pair(std::string("ossimSinusoidalProjection"),
                                                       std::string(SRS_PT_SINUSOIDAL)));
   theOssimToWktProjectionTranslation.insert(make_pair(std::string("ossimMercatorProjection"),
                                                       std::string(SRS_PT_MERCATOR_1SP)));
   
}

ossimString ossimOgcWktTranslator::wktToOssimDatum(const ossimString& datum)const
{
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
   if(datum.contains("OSGA"))
   {
      return "OGB-A";
   }
   if(datum.contains("OSGB"))
   {
      return "OGB-B";
   }
   if(datum.contains("OSGC"))
   {
      return "OGB-C";
   }
   if(datum.contains("OSGD"))
   {
      return "OGB-D";
   }
   if(datum.contains("Nouvelle_Triangulation_Francaise"))
   {
      return "NTF";
   }
   
   return "";
}

ossimString ossimOgcWktTranslator::ossimToWktDatum(const ossimString& datum)const
{
   ossimString result;
   map<std::string, std::string>::const_iterator i = theOssimToWktDatumTranslation.find(datum);
   if(i != theOssimToWktDatumTranslation.end())
   {
      result = (*i).second;
   }
   return result;
}

ossimString ossimOgcWktTranslator::wktToOssimProjection(const ossimString& datum)const
{
   std::string result;
   map<std::string, std::string>::const_iterator i =
      theWktToOssimProjectionTranslation.find(datum);
   if(i != theWktToOssimProjectionTranslation.end())
   {
      result = (*i).second;
   }
   return result;
}

ossimString ossimOgcWktTranslator::ossimToWktProjection(const ossimString& datum)const
{
   ossimString result;
   map<std::string, std::string>::const_iterator i =
      theOssimToWktProjectionTranslation.find(datum);
   if(i != theOssimToWktProjectionTranslation.end())
   {
      result = (*i).second;
   }
   return result;
}

