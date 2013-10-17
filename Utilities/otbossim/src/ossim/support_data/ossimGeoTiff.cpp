//***************************************************************************
// FILE: ossimGeoTiff.cpp
//
// License:  See top level LICENSE.txt file.
//
// Description:
// 
// Class definition for ossimGeoTiff which is designed to read and hold tag
// information.
//
//***************************************************************************
// $Id: ossimGeoTiff.cpp 21024 2012-05-30 08:45:13Z dburken $

#include <ossim/support_data/ossimGeoTiff.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimGeoTiffCoordTransformsLut.h>
#include <ossim/base/ossimGeoTiffDatumLut.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimEpsgProjectionDatabase.h>
#include <ossim/projection/ossimStatePlaneProjectionInfo.h>
#include <ossim/projection/ossimPolynomProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <tiff.h>
#include <tiffio.h>
#include <xtiffio.h>
#include <geotiff.h>
#include <geo_normalize.h>
#include <geovalues.h>
#include <string.h>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <cstdlib>

static const ossimGeoTiffCoordTransformsLut COORD_TRANS_LUT;
static const ossimGeoTiffDatumLut DATUM_LUT;
OpenThreads::Mutex ossimGeoTiff::theMutex;

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimGeoTiff.cpp 21024 2012-05-30 08:45:13Z dburken $";
#endif

//---
// Static trace for debugging
//---
static ossimTrace traceDebug("ossimGeoTiff:debug");

// Prototype, defined at bottom of this file. ArcMAP 9.2 bug workaround.
ossim_uint16 getMetersEquivalentHarnCode(ossim_uint16 feet_harn_code);

//---
// This was created to remove/hide "libgetiff/geo_normalize.h" in ossimGeoTiff.h.
//---
class ossimPrivateGtifDef
{
public:
   ossimPrivateGtifDef()
      : m_defs(0)
   {}
   ~ossimPrivateGtifDef()
   {
      if ( m_defs )
      {
         delete m_defs;
         m_defs = 0;
      }
   }
   GTIFDefn* m_defs;
};

//*************************************************************************************************
// CONSTRUCTOR
//*************************************************************************************************
ossimGeoTiff::ossimGeoTiff()
   :
      theTiffPtr(0),
      theGeoKeyOffset(0),
      theGeoKeyLength(0),
      theGeoKeysPresentFlag(false),
      theZone(0),
      theHemisphere("N"),
      theProjectionName("unknown"),
      theDatumName("unknown"),
      theScale(),
      theTiePoint(),
      theModelTransformation(),
      theWidth(0),
      theLength(0),
      theBitsPerSample(),
      theModelType(0),
      theRasterType(UNDEFINED),
      theGcsCode(0),
      theDatumCode(0),
      theAngularUnits(0),
      thePcsCode(0),
      thePcsCitation(),
      theCoorTransGeoCode(0),
      theLinearUnitsCode(UNDEFINED),
      theStdPar1(0.0),
      theStdPar2(0.0),
      theOriginLon(0.0),
      theOriginLat(0.0),
      theFalseEasting(0.0),
      theFalseNorthing(0.0),
      theScaleFactor(0.0),
      thePrivateDefinitions(new ossimPrivateGtifDef())
{
}

//*************************************************************************************************
// CONSTRUCTOR
//*************************************************************************************************
ossimGeoTiff::ossimGeoTiff(const ossimFilename& file, ossim_uint32 entryIdx)
   :
      theTiffPtr(0),
      theGeoKeyOffset(0),
      theGeoKeyLength(0),
      theGeoKeysPresentFlag(false),
      theZone(0),
      theHemisphere("N"),
      theProjectionName("unknown"),
      theDatumName("unknown"),
      theScale(),
      theTiePoint(),
      theModelTransformation(),
      theWidth(0),
      theLength(0),
      theBitsPerSample(),
      theModelType(0),
      theRasterType(UNDEFINED),
      theGcsCode(0),
      theDatumCode(0),
      theAngularUnits(0),
      thePcsCode(0),
      thePcsCitation(),
      theCoorTransGeoCode(0),
      theLinearUnitsCode(UNDEFINED),
      theStdPar1(0.0),
      theStdPar2(0.0),
      theOriginLon(0.0),
      theOriginLat(0.0),
      theFalseEasting(0.0),
      theFalseNorthing(0.0),
      theScaleFactor(0.0),
      thePrivateDefinitions(new ossimPrivateGtifDef())
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::ossimGeoTiff: Entered..." << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::ossimGeoTiff: OSSIM_ID =  "
         << OSSIM_ID << endl;
#endif      
   }
   
   if(readTags(file, entryIdx) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::ossimGeoTiff: "
            << "Unable to reade tags."
            << std::endl;
      }
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimGeoTiff::ossimGeoTiff: "
         << "Unable to reade tags."
         << std::endl;
   }
   if (traceDebug())
   {
      print(ossimNotify(ossimNotifyLevel_DEBUG));
   }

}

ossimGeoTiff::~ossimGeoTiff()
{
   if(thePrivateDefinitions)
   {
      delete thePrivateDefinitions;
      thePrivateDefinitions = 0;
   }
   if(theTiffPtr)
   {
      XTIFFClose(theTiffPtr);
      theTiffPtr = 0;
   }
}

int ossimGeoTiff::getPcsUnitType(ossim_int32 pcsCode) 
{
   int pcsUnits = ossimGeoTiff::UNDEFINED;
   ossimUnitType units = OSSIM_UNIT_UNKNOWN; // default

   // Need to instantiate a projection given the pcs code:
   ossimMapProjection* proj = PTR_CAST(ossimMapProjection, 
      ossimEpsgProjectionDatabase::instance()->findProjection(pcsCode));
   if (proj)
      units = proj->getProjectionUnits();
   else   
      return ossimGeoTiff::UNDEFINED;
   
   switch (units)
   {
      case OSSIM_METERS:
         pcsUnits = ossimGeoTiff::LINEAR_METER;
         break;
      case OSSIM_FEET:
         pcsUnits = ossimGeoTiff::LINEAR_FOOT;
         break;
      case OSSIM_US_SURVEY_FEET:
         pcsUnits = ossimGeoTiff::LINEAR_FOOT_US_SURVEY;
         break;
      default:
         break;
   }

   return pcsUnits;
}

#define EPSG_CODE_MAX 32767
bool ossimGeoTiff::writeTags(TIFF* tifPtr,
                             const ossimRefPtr<ossimMapProjectionInfo> projectionInfo,
                             bool imagineNad27Flag)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);

   const ossimMapProjection* mapProj = projectionInfo->getProjection();

   if(!mapProj) return false;

   GTIF* gtif = GTIFNew(tifPtr);

    // Get some things we need throughout.
   ossimGpt origin      = mapProj->origin();
   double falseEasting  =  mapProj->getFalseEasting();
   double falseNorthing =  mapProj->getFalseNorthing();
   
   ossimKeywordlist kwl;
   mapProj->saveState(kwl);
   const char* stdParallel1 = kwl.find(ossimKeywordNames::STD_PARALLEL_1_KW);
   const char* stdParallel2 = kwl.find(ossimKeywordNames::STD_PARALLEL_2_KW);
   const char* scaleFactor  = kwl.find(ossimKeywordNames::SCALE_FACTOR_KW);

   bool gcsTypeSet = false;

   //---
   // Since using a pcs code is the easiest way to go, look for that first.
   //---
   ossim_int16 pcsCode = mapProj->getPcsCode();

   //---
   // Get the units now.  If user has overriden pcs units then go user defined
   // projection by setting pcs code to 0.
   //---
   ossimString projName = mapProj->getClassName();
   
   int units = ossimGeoTiff::UNDEFINED;
   if(mapProj->isGeographic())
      units = ossimGeoTiff::ANGULAR_DEGREE;
   else
      units = getPcsUnitType(pcsCode);
   if (units == UNDEFINED)
      units = LINEAR_METER;

   if (pcsCode)
   {
      if ((units==LINEAR_FOOT_US_SURVEY) || (units==LINEAR_FOOT))
      {
         // ArcMap 9.2 bug workaround (originally implemented by ESH 2008, reworked by OLK 04/2010
         ossim_uint16 meter_pcs = getMetersEquivalentHarnCode(pcsCode);
         if (meter_pcs)
            pcsCode = meter_pcs;
      }

      //int gcs_code = mapProj->getGcsCode();
      int datum_code = USER_DEFINED;
      int ellipsoid_code = USER_DEFINED;
      const ossimDatum* datum = mapProj->getDatum();
      if (datum)
      {
         datum_code = (int) datum->epsgCode();
         const ossimEllipsoid* ellipsoid = datum->ellipsoid();
         if (ellipsoid)
            ellipsoid_code = ellipsoid->getEpsgCode();
      }
      
      if(mapProj->isGeographic())
         GTIFKeySet(gtif, GeographicTypeGeoKey,    TYPE_SHORT, 1, pcsCode);

      GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1, datum_code);
      GTIFKeySet(gtif, ProjectionGeoKey ,       TYPE_SHORT, 1, pcsCode);
      GTIFKeySet(gtif, GeogEllipsoidGeoKey,     TYPE_SHORT, 1, ellipsoid_code);
      GTIFKeySet(gtif, ProjectedCSTypeGeoKey,   TYPE_SHORT, 1, pcsCode);
   }
   else
   {
      ossimString datumCode = "WGE";
      ossimString datumName = "WGE";
      // Attemp to get the datum code
      const ossimDatum* datum = mapProj->getDatum();
      if(datum)
      {
         datumCode = datum->code();
         datumName = datum->name();
      }

      short gcs = USER_DEFINED;

      if (datumCode == "WGE") gcs = GCS_WGS_84;
      else if (datumCode == "WGD") gcs = GCS_WGS_72;
      else if (datumCode == "NAR-C") gcs = GCS_NAD83;
      else if (datumCode == "NAR") gcs = GCS_NAD83;
      else if (datumCode == "NAS-C") gcs = GCS_NAD27;
      else if (datumCode == "NAS") gcs = GCS_NAD27;
      else if (datumCode == "ADI-M") gcs = GCS_Adindan;
      else if (datumCode == "ARF-M") gcs = GCS_Arc_1950;
      else if (datumCode == "ARS-M") gcs = GCS_Arc_1960;
      else if (datumCode == "EUR-7" || datumCode == "EUR-M") gcs = GCS_ED50;
      else if ((datumCode == "OGB-7") ||
               (datumCode == "OGB-M") ||
               (datumCode == "OGB-A") ||
               (datumCode == "OGB-B") ||
               (datumCode == "OGB-C") ||
               (datumCode == "OGB-D")) gcs = GCS_OSGB_1936;
      else if (datumCode == "TOY-M") gcs = GCS_Tokyo;
      else if(datumCode == "6055") gcs = 3785; // this is a special google datum so we will give it a gcs code of 3785
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "DATUM = " << datumCode << " tag not written " << std::endl
               << "Please let us know so we can add it"          << std::endl;
         }
      }

      // ***
      // ERDAS Imagine < v8.7 has a NAD27 Conus Bug.  They are not using the
      // proper GCS code.  They use user-defined fields and Geog citation tag to
      // define.  Sucks!  It is an open issue at Leica.  This is a work around
      // flag for this issue.
      // ***
      if((datumCode == "NAS-C") && imagineNad27Flag)
      {
         gcs = USER_DEFINED;

         std::ostringstream os;
         os << "IMAGINE GeoTIFF Support\nCopyright 1991 -  2001 by ERDAS, Inc. All Rights Reserved\n@(#)$RCSfile$ $Revision: 21024 $ $Date: 2012-05-30 10:45:13 +0200 (mer., 30 mai 2012) $\nUnable to match Ellipsoid (Datum) to a GeographicTypeGeoKey value\nEllipsoid = Clarke 1866\nDatum = NAD27 (CONUS)";

         GTIFKeySet(gtif,
                    GeogCitationGeoKey,
                    TYPE_ASCII,
                    1,
                    os.str().c_str());

         // User-Defined
         GTIFKeySet(gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1,
                    KvUserDefined );
         // User-Defined
         GTIFKeySet(gtif, GeogEllipsoidGeoKey, TYPE_SHORT, 1,
                    KvUserDefined );
      }
      else
      {
      GTIFKeySet( gtif, GeographicTypeGeoKey, TYPE_SHORT, 1, gcs );
      gcsTypeSet = true;
      }

      // Write the projection parameters.

      bool setFalseEastingNorthingFlag = false;
      
      if ( projName == "ossimUtmProjection" )
      {
         //---
         // UTM tags needed example from the geo tiff spec page:
         // ModelTiepointTag       = (0, 0, 0,  350807.4, 5316081.3, 0.0)
         // ModelPixelScaleTag     = (100.0, 100.0, 0.0)
         // GeoKeyDirectoryTag:
         //       GTModelTypeGeoKey        =  1      (ModelTypeProjected)
         //       GTRasterTypeGeoKey       =  1      (RasterPixelIsArea)
         //       ProjectedCSTypeGeoKey    =  32660  (PCS_WGS84_UTM_zone_60N)
         //       PCSCitationGeoKey        =  "UTM Zone 60 N with WGS84"
         //
         // NOTE:
         // The "ProjectedCSTypeGeoKey" can be constructed using the map zone
         // and the datum.
         //---
         const ossimUtmProjection* utmProjection = dynamic_cast<const ossimUtmProjection*>(mapProj);

         // Attempt to get the pcs key.
         int mapZone = utmProjection->getZone();
         ossimString hemisphere = utmProjection->getHemisphere();
         short projSysCode=0;

         //---
         // Use a projection code that does not imply a datum.
         // See section "6.3.3.2 Projection Codes" for definition.
         //---
         if (mapZone > 0) // Northern hemisphere.
         {
            projSysCode = 16000 + mapZone;
         }
         else if (mapZone < 0) // Southern hemisphere.
         {
            hemisphere = "S";
            projSysCode = 16100 + abs(mapZone);
         }
         
         // Set the Projected Coordinate System Type to be user defined.
         GTIFKeySet(gtif,
                    ProjectedCSTypeGeoKey,
                    TYPE_SHORT,
                    1,
                    USER_DEFINED);

         if ( !gcsTypeSet )
         {
            // Set the geographic type to be user defined.
            GTIFKeySet(gtif,
                       GeographicTypeGeoKey,
                       TYPE_SHORT,
                       1,
                       USER_DEFINED);
         }
         
         // Set the ProjectionGeoKey in place of the ProjectedCSTypeGeoKey.
         GTIFKeySet(gtif,
                    ProjectionGeoKey,
                    TYPE_SHORT,
                    1,
                    projSysCode);

         std::ostringstream os;
         os << "UTM Zone " << dec << mapZone << hemisphere.c_str()
            << " with " << datumName << " datum";
         
         GTIFKeySet(gtif,
                    PCSCitationGeoKey,
                    TYPE_ASCII,
                    1,
                    os.str().c_str());
         
      } // End of "if ( projName == "ossimUtmProjection" )
      
      else if(projName == "ossimBngProjection")
      {
         // User-Defined
         GTIFKeySet(gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
		    PCS_BRITISH_NATIONAL_GRID);//KvUserDefined );
         
         // User-Defined
         GTIFKeySet(gtif, ProjectionGeoKey, TYPE_SHORT, 1,
		    KvUserDefined );
         
         GTIFKeySet(gtif,
                    PCSCitationGeoKey,
                    TYPE_ASCII,
                    26,
                    "PCS_British_National_Grid");
         
         GTIFKeySet(gtif,
                    ProjCoordTransGeoKey,
                    TYPE_SHORT,
                    1,
                    (uint16)CT_TransverseMercator);
         
         GTIFKeySet(gtif,
		    ProjNatOriginLongGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.lond());
         
         GTIFKeySet(gtif,
		    ProjNatOriginLatGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.latd());

         setFalseEastingNorthingFlag = true;

         double scale = ossimString(scaleFactor).toDouble();

         GTIFKeySet(gtif,
                    ProjScaleAtNatOriginGeoKey,
                    TYPE_DOUBLE,
                    1,
                    scale);
      }
      else if( projName == "ossimSinusoidalProjection")
      {
         GTIFKeySet(gtif,
                    ProjCoordTransGeoKey,
                    TYPE_SHORT,
                    1,
                    (uint16)CT_Sinusoidal);
         
         GTIFKeySet(gtif,
		    ProjNatOriginLongGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.lond());

         GTIFKeySet(gtif,
		    ProjNatOriginLatGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.latd());

         setFalseEastingNorthingFlag = true;
      }
      else if( (projName == "ossimEquDistCylProjection")||
               (projName == "ossimLlxyProjection"))
      {
         GTIFKeySet(gtif,
		    ProjNatOriginLongGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.lond());
         
         GTIFKeySet(gtif,
		    ProjNatOriginLatGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.latd());
      }
      else if ( (projName == "ossimLambertConformalConicProjection") ||
                (projName == "ossimAlbersProjection") )
      {
         //---
         // Lambert Conformal Conic:
         // tags needed example from the geo tiff spec page:
         // ModelTiepointTag     = (  80,  100, 0,  200000,  1500000, 0)
         // ModelPixelScaleTag         = (1000, 1000, 0)
         // GeoKeyDirectoryTag:
         //       GTModelTypeGeoKey           =  1     (ModelTypeProjected)
         //       GTRasterTypeGeoKey          =  1     (RasterPixelIsArea)
         //       GeographicTypeGeoKey        =  4267  (GCS_NAD27)
         //       ProjectedCSTypeGeoKey       =  32767 (user-defined)
         //       ProjectionGeoKey            =  32767 (user-defined)
         //       ProjLinearUnitsGeoKey       =  9001     (Linear_Meter)
         //       ProjCoordTransGeoKey        =  8 (CT_LambertConfConic_2SP)
         //            ProjStdParallel1GeoKey     =  41.333
         //            ProjStdParallel2GeoKey     =  48.666
         //            ProjCenterLongGeoKey       =-120.0
         //            ProjNatOriginLatGeoKey     =  45.0
         //            ProjFalseEastingGeoKey,    = 200000.0
         //            ProjFalseNorthingGeoKey,   = 1500000.0
         //
         // NOTE: Albers Same as Lambert with the exception of the
         //       ProjCoordTransGeoKey which is:  CT_AlbersEqualArea.
         //---
         
         if (projName == "ossimLambertConformalConicProjection")
         {
            GTIFKeySet(gtif,
                       ProjCoordTransGeoKey,
                       TYPE_SHORT,
                       1,
                       (uint16)CT_LambertConfConic_2SP );
         }
         else // Albers
         {
            GTIFKeySet(gtif,
                       ProjCoordTransGeoKey,
                       TYPE_SHORT,
                       1,
                       (uint16)CT_AlbersEqualArea);
         }
         
         // User-Defined
         GTIFKeySet(gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                    KvUserDefined );

         // User-Defined
         GTIFKeySet(gtif, ProjectionGeoKey, TYPE_SHORT, 1,
		    KvUserDefined );

         double phi1 = ossimString(stdParallel1).toDouble();

         GTIFKeySet(gtif,
		    ProjStdParallel1GeoKey,
		    TYPE_DOUBLE,
		    1,
		    phi1);  // 1st parallel

         double phi2 = ossimString(stdParallel2).toDouble();

         GTIFKeySet(gtif,
		    ProjStdParallel2GeoKey,
		    TYPE_DOUBLE,
		    1,
		    phi2);  // 2nd parallel

         GTIFKeySet(gtif,
		    ProjCenterLongGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.lond());  // Longitude at the origin.

         GTIFKeySet(gtif,
		    ProjNatOriginLatGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.latd());  // Origin

         setFalseEastingNorthingFlag = true;

      }  // End of Lambert.


      else if ( projName == "ossimMercatorProjection" )
      {
         GTIFKeySet(gtif,
                    ProjCoordTransGeoKey,
                    TYPE_SHORT,
                    1,
                    (uint16)CT_Mercator);
         
         GTIFKeySet(gtif,
		    ProjNatOriginLongGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.lond());

         GTIFKeySet(gtif,
		    ProjNatOriginLatGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.latd());

         setFalseEastingNorthingFlag = true;

         double scale = ossimString(scaleFactor).toDouble();

         GTIFKeySet(gtif,
                    ProjScaleAtNatOriginGeoKey,
                    TYPE_DOUBLE,
                    1,
                    scale);
      }
      else if ( projName == "ossimTransMercatorProjection" )
      {
         //---
         // Transverse Mercator ( no example in the geo tiff spec.
         // Requires:
         //    - latitude/longitude of the origin
         //    - easting/northing of some tie point(line/sample 0,0)
         //    - false easting/northing
         //    - The scale factor.
         //---

         //---
         // The easting/northing is the distance from the origin plus the
         // false easting/northing.  In other words if line 0 is 5,000
         // meters from the origin and the false northing is 5,000 meters,
         // then the northing would be 10,000.  The same goes for the easting.
         //---
         GTIFKeySet(gtif,
                    ProjCoordTransGeoKey,
                    TYPE_SHORT,
                    1,
                    (uint16)CT_TransverseMercator);
         
         // User-Defined
         GTIFKeySet(gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1, KvUserDefined );
         
         // User-Defined
         GTIFKeySet(gtif, ProjectionGeoKey, TYPE_SHORT, 1, KvUserDefined );
         
         GTIFKeySet(gtif,
		    ProjNatOriginLongGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.lond());

         GTIFKeySet(gtif,
		    ProjNatOriginLatGeoKey,
		    TYPE_DOUBLE,
		    1,
		    origin.latd());

         setFalseEastingNorthingFlag = true;

         double scale = ossimString(scaleFactor).toDouble();

         GTIFKeySet(gtif,
                    ProjScaleAtNatOriginGeoKey,
                    TYPE_DOUBLE,
                    1,
                    scale);
      } // End of TM

      if (setFalseEastingNorthingFlag == true)
      {

         GTIFKeySet(gtif,
		    ProjFalseEastingGeoKey,
		    TYPE_DOUBLE,
		    1,
		    falseEasting);

         GTIFKeySet(gtif,
		    ProjFalseNorthingGeoKey,
		    TYPE_DOUBLE,
		    1,
		    falseNorthing);
      }
   }

   //---
   // Set the model type and units.
   //---
   if (units == ossimGeoTiff::ANGULAR_DEGREE)
   {
      GTIFKeySet(gtif,
         GTModelTypeGeoKey,
         TYPE_SHORT,
         1,
         ModelTypeGeographic);

      // Set the units key.
      GTIFKeySet(gtif,
         GeogAngularUnitsGeoKey,
         TYPE_SHORT,
         1,
         units);
   }
   else
   {
      GTIFKeySet(gtif,
         GTModelTypeGeoKey,
         TYPE_SHORT,
         1,
         ModelTypeProjected);

      // Set the units key.
      GTIFKeySet(gtif,
         ProjLinearUnitsGeoKey,
         TYPE_SHORT,
         1,
         units);
   }

   // Set the ellipsoid major/minor axis.
   GTIFKeySet(gtif,
              GeogSemiMajorAxisGeoKey,
              TYPE_DOUBLE,
              1,
              mapProj->getA());

   GTIFKeySet(gtif,
              GeogSemiMinorAxisGeoKey,
              TYPE_DOUBLE,
              1,
              mapProj->getB());

   // Set the pixel type.
   if (projectionInfo->getPixelType() == OSSIM_PIXEL_IS_POINT)
   {
      // Tie point relative to center of pixel.
      GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsPoint);
   }
   else
   {
      // Tie point relative to upper left corner of pixel
      GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
   }

   //---
   // Set the tie point and scale.
   //---
   double  tiePoints[6]  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
   double  pixScale[3]   = { 0.0, 0.0, 0.0 };
   switch (units)
   {
      case LINEAR_FOOT:
      {
         tiePoints[3]  = ossim::mtrs2ft(projectionInfo->ulEastingNorthingPt().x);
         tiePoints[4]  = ossim::mtrs2ft(projectionInfo->ulEastingNorthingPt().y);
         pixScale[0]   = ossim::mtrs2ft(projectionInfo->getMetersPerPixel().x);
         pixScale[1]   = ossim::mtrs2ft(projectionInfo->getMetersPerPixel().y);
         falseEasting  = ossim::mtrs2ft(falseEasting);
         falseNorthing = ossim::mtrs2ft(falseNorthing);

         break;
      }
      case LINEAR_FOOT_US_SURVEY:
      {
         tiePoints[3]  = ossim::mtrs2usft(projectionInfo->ulEastingNorthingPt().x);
         tiePoints[4]  = ossim::mtrs2usft(projectionInfo->ulEastingNorthingPt().y);
         pixScale[0]   = ossim::mtrs2usft(projectionInfo->getMetersPerPixel().x);
         pixScale[1]   = ossim::mtrs2usft(projectionInfo->getMetersPerPixel().y);
         falseEasting  = ossim::mtrs2usft(falseEasting);
         falseNorthing = ossim::mtrs2usft(falseNorthing);
         break;
      }
      case ANGULAR_DEGREE:
      {
         tiePoints[3] = projectionInfo->ulGroundPt().lond();
         tiePoints[4] = projectionInfo->ulGroundPt().latd();
         pixScale[0]  = projectionInfo->getDecimalDegreesPerPixel().x;
         pixScale[1]  = projectionInfo->getDecimalDegreesPerPixel().y;
         break;
      }
      case LINEAR_METER:
      default:
      {
         tiePoints[3] = projectionInfo->ulEastingNorthingPt().x;
         tiePoints[4] = projectionInfo->ulEastingNorthingPt().y;
         pixScale[0]  = projectionInfo->getMetersPerPixel().x;
         pixScale[1]  = projectionInfo->getMetersPerPixel().y;
         break;
      }

   } // End of "switch (units)"

   TIFFSetField( tifPtr, TIFFTAG_GEOTIEPOINTS,  6, tiePoints );
   TIFFSetField( tifPtr, TIFFTAG_GEOPIXELSCALE, 3, pixScale );


   GTIFWriteKeys(gtif); // Write out geotiff tags.
   GTIFFree(gtif);

   return true;
}

bool ossimGeoTiff::writeJp2GeotiffBox(const ossimFilename& tmpFile,
                                      const ossimIrect& rect,
                                      const ossimProjection* proj,
                                       std::vector<ossim_uint8>& buf,
                                       ossimPixelType pixelType )
{
   //---
   // Snip from The "GeoTIFF Box" Specification for JPEG 2000 Metadata:
   // This box contains a valid GeoTIFF image.  The image is "degenerate", in
   // that it represents a very simple image with specific constraints:
   // . the image height and width are both 1
   // . the datatype is 8-bit
   // . the colorspace is grayscale
   // . the (single) pixel must have a value of 0 for its (single) sample
   //
   // NOTE: It also states little endian but I think libtiff writes whatever
   // endianesss the host is.
   //
   // Also assuming class tiff for now.  Not big tiff.
   //---
   bool result = true;
   
   TIFF* tiff = XTIFFOpen(tmpFile.c_str(), "w");
   if (tiff)
   {
      // Write the projection info out.
      ossimMapProjection* mapProj = PTR_CAST(ossimMapProjection, proj);
      if(mapProj)
      {
         ossimRefPtr<ossimMapProjectionInfo> projectionInfo
	   = new ossimMapProjectionInfo(mapProj, ossimDrect(rect));

         // Set the pixel type to point of area.
         projectionInfo->setPixelType(pixelType);

         // Write the geotiff keys.
         ossimGeoTiff::writeTags(tiff, projectionInfo, false);
      }

      // Basic tiff tags.
      TIFFSetField( tiff, TIFFTAG_IMAGEWIDTH, 1 );
      TIFFSetField( tiff, TIFFTAG_IMAGELENGTH, 1 );
      TIFFSetField( tiff, TIFFTAG_BITSPERSAMPLE, 8 );
      TIFFSetField( tiff, TIFFTAG_SAMPLESPERPIXEL, 1 );
      TIFFSetField( tiff, TIFFTAG_ROWSPERSTRIP, 1 );
      TIFFSetField( tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG );
      TIFFSetField( tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK );

      // One pixel image:
      ossim_uint8 pixel = 0;
      TIFFWriteEncodedStrip( tiff, 0, (char *) &pixel, 1 );

      TIFFWriteDirectory( tiff );
      XTIFFClose( tiff );

      // Get the size.  Note 16 bytes added for the JP2 UUID:
      const std::vector<ossim_uint8>::size_type UUID_SIZE = 16;
      const std::vector<ossim_uint8>::size_type BOX_SIZE = UUID_SIZE +
         static_cast<std::vector<ossim_uint8>::size_type>(tmpFile.fileSize());

      // Create the buffer.
      buf.resize( BOX_SIZE );

      if ( BOX_SIZE == buf.size() )
      {
         const ossim_uint8 GEOTIFF_UUID[UUID_SIZE] = 
         {
            0xb1, 0x4b, 0xf8, 0xbd,
            0x08, 0x3d, 0x4b, 0x43,
            0xa5, 0xae, 0x8c, 0xd7,
            0xd5, 0xa6, 0xce, 0x03
         };

         // Copy the UUID.
         std::vector<ossim_uint8>::size_type i;
         for (i = 0; i < UUID_SIZE; ++i)
         {
            buf[i] = GEOTIFF_UUID[i];
         }

         // Copy the tiff.
         std::ifstream str;
         str.open(tmpFile.c_str(), ios::in | ios::binary);
         if (str.is_open())
         {
            char ch;
            for (; i < BOX_SIZE; ++i)
            {
               str.get(ch);
               buf[i] = static_cast<ossim_uint8>(ch);
            }
         }
      }
      else
      {
         result = false;
      }

      // Remove the temp file.
      tmpFile.remove();
      
   }
   else
   {
      result = false;
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimGeoTiff::writeJp2GeotiffBox ERROR:\n"
            << "Could not open " << tmpFile << std::endl;
      }
   }
   return result;
}

bool ossimGeoTiff::readTags(const ossimFilename& file, ossim_uint32 entryIdx)
{
   bool result = false;
   
   TIFF* tiff = XTIFFOpen(file.c_str(), "r");
   if(tiff)
   {
      result = readTags(tiff, entryIdx, true);
   }

   return result;
}

bool ossimGeoTiff::readTags(
   TIFF* tiff, ossim_uint32 entryIdx, bool ownTiffPtrFlag)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);

   if ( !tiff )
   {
      return false;
   }

   if(!TIFFSetDirectory(tiff, (ossim_uint16)entryIdx))
   {
      return false;
   }
   
   GTIF* gtif = GTIFNew(tiff);
   if(!gtif)
   {
      return false;
   }

   if(theTiffPtr)
   {
      XTIFFClose(theTiffPtr);
   }
   theTiffPtr = tiff;
   
   if(thePrivateDefinitions->m_defs)
   {
      delete thePrivateDefinitions->m_defs;;
   }
   thePrivateDefinitions->m_defs = new GTIFDefn;
   GTIFGetDefn(gtif, thePrivateDefinitions->m_defs);
   ossim_uint32 idx = 0;
   theGeoKeysPresentFlag = true;
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeoTiff::readTags: Raw Geotiff Tags are\n";
      GTIFPrint(gtif,0,0);
   }
   if(TIFFGetField( theTiffPtr,
                    TIFFTAG_IMAGELENGTH,
                    &theLength ))
   {
   }
   if(TIFFGetField( theTiffPtr,
                    TIFFTAG_IMAGEWIDTH,
                    &theWidth ))
   {
   }
   theScaleFactor     = 0.0;
   theModelType       = thePrivateDefinitions->m_defs->Model;
   theGcsCode         = thePrivateDefinitions->m_defs->GCS;
   thePcsCode         = thePrivateDefinitions->m_defs->PCS;
   theDatumCode       = thePrivateDefinitions->m_defs->Datum;
   theAngularUnits    = thePrivateDefinitions->m_defs->UOMAngle;
   theLinearUnitsCode = thePrivateDefinitions->m_defs->UOMLength;

   if (theAngularUnits == ANGULAR_DMS_HEMISPHERE)
   {
      //---
      // Hack for bug, where the libgeotiff funtion GTIFGetDefn sets the angular units 
      // incorrectly to ANGULAR_DMS_HEMISPHERE:
      if ( traceDebug() )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << " WARNING ossimGeoTiff::addImageGeometry:"           
            << "The angular units (key 2054) is set to ANGULAR_DMS_HEMISPHERE!"  
            << "\nAssuming \"Angular_Degree\"..." << std::endl;
      }
      theAngularUnits = ANGULAR_DEGREE;  
   }
   
#if 0
   ossim_uint16 modelType;
   if(GTIFKeyGet(gtif, GTModelTypeGeoKey, &modelType, 0, 1))
   {
      theModelType = (ModelType)modelType;
   }
#endif
   if(GTIFKeyGet(gtif, GTRasterTypeGeoKey, &theRasterType, 0, 1))
   {
   }
   
#if 0
   if(GTIFKeyGet(gtif, GeographicTypeGeoKey, &theGcsCode, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, GeogGeodeticDatumGeoKey, &theDatumCode, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, GeogAngularUnitsGeoKey, &theAngularUnits, 0, 1))
   {
   }
#endif
   
   if (GTIFKeyGet(gtif, ProjectedCSTypeGeoKey, &thePcsCode, 0, 1))
      parsePcsCode();
   
   //---
   // ESH 2/2008 -- Handle geotiff's with state plane coordinate systems produced by ERDAS.
   // They use the citation filed to specify the geometry (complete HACK by Erdas)
   //---
   else
   {
      const int CITATION_STRING_SIZE = 512;
      char citationStr[CITATION_STRING_SIZE];
      if ( GTIFKeyGet(gtif, GTCitationGeoKey,  &citationStr,
                      0, CITATION_STRING_SIZE))
      {
         ossimString gTCitation = citationStr; // key 1026

         // Extract state plane string from the citation key
         ossimString projStrTemp =
            gTCitation.afterRegExp( "Projection Name = " );
         
         ossimString projStr  = projStrTemp.beforeRegExp( "\n" );
         if ( projStr.empty() == false )
         {
            ossimEpsgProjectionFactory* f = ossimEpsgProjectionFactory::instance();
            ossimProjection* proj = f->createProjection(projStr);
            ossimMapProjection* map_proj = PTR_CAST(ossimMapProjection, proj);
            parseProjection(map_proj);
         }
      }  // End of "if(GTIFKeyGet(gtif, GTCitationGeoKey..."
   }
   
   char* buf = 0;
   theOriginLon = ossim::nan();
   theOriginLat = ossim::nan();
   if(GTIFKeyGet(gtif, PCSCitationGeoKey , &buf, 0, 1))
   {
      thePcsCitation = ossimString(buf);
   }
   GTIFKeyGet(gtif, ProjCoordTransGeoKey , &theCoorTransGeoCode, 0, 1);
   for(idx = 0; idx < (ossim_uint32)thePrivateDefinitions->m_defs->nParms; ++idx)
   {
      switch(thePrivateDefinitions->m_defs->ProjParmId[idx])
      {
         case ProjStdParallel1GeoKey:
         {
            theStdPar1 = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjStdParallel2GeoKey:
         {
            theStdPar2 = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjOriginLongGeoKey:
         {
            theOriginLon = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjOriginLatGeoKey:
         {
            theOriginLat = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjFalseEastingGeoKey:
         {
            theFalseEasting = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjFalseNorthingGeoKey:
         {
            theFalseNorthing = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjCenterLongGeoKey:
         {
            theOriginLon = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjCenterLatGeoKey:
         {
            theOriginLat = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjFalseOriginLatGeoKey:
         {
            theOriginLat = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjFalseOriginLongGeoKey:
         {
            theOriginLon = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
         case ProjScaleAtNatOriginGeoKey:
         {
            theScaleFactor = thePrivateDefinitions->m_defs->ProjParm[idx];
            break;
         }
      }
   }
   
#if 0
   if(GTIFKeyGet(gtif, ProjStdParallel1GeoKey, &theStdPar1, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, ProjStdParallel2GeoKey, &theStdPar2, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, ProjNatOriginLongGeoKey, &tempDouble, 0, 1))
   {
      theOriginLon = tempDouble;
   }
   else if(GTIFKeyGet(gtif, ProjOriginLongGeoKey, &tempDouble, 0, 1))
   {
      theOriginLon = tempDouble;
   }
   if(GTIFKeyGet(gtif, ProjNatOriginLatGeoKey, &tempDouble, 0, 1))
   {
      theOriginLat = tempDouble;
   }
   else if(GTIFKeyGet(gtif, ProjOriginLatGeoKey, &tempDouble, 0, 1))
   {
      theOriginLat = tempDouble;
   }
   if(GTIFKeyGet(gtif, ProjFalseEastingGeoKey, &theFalseEasting, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, ProjFalseNorthingGeoKey, &theFalseNorthing, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, ProjCenterLongGeoKey, &theCenterLon, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, ProjCenterLatGeoKey, &theCenterLat, 0, 1))
   {
   }
   if(GTIFKeyGet(gtif, ProjScaleAtNatOriginGeoKey, &theScaleFactor, 0, 1))
   {
   }
#endif
   theScale.clear();
   ossim_uint16 pixScaleSize = 0;
   double* pixScale=0;
   if(TIFFGetField(theTiffPtr, TIFFTAG_GEOPIXELSCALE, &pixScaleSize, &pixScale))
   {
      theScale.insert(theScale.begin(), pixScale, pixScale+pixScaleSize);
      if ( theModelType == ModelTypeGeographic )
      {
         // The origin latitude must be computed so as to achieve the proper horizontal scaling:
         theOriginLat = ossim::acosd(theScale[1]/theScale[0]);
      }
   }
   theTiePoint.clear();
   ossim_uint16 tiePointSize = 0;
   double* tiepoints=0;
   if(TIFFGetField(theTiffPtr, TIFFTAG_GEOTIEPOINTS,  &tiePointSize, &tiepoints))
   {
      theTiePoint.insert(theTiePoint.begin(), tiepoints, tiepoints+tiePointSize);
 
      // ESH 05/2009 -- If the image is in a projected coordinate system, the
      // tiepoints will be projected coordinates not lat/lon. Let's avoid setting
      // the origin lon/lat to projected x/y. Fix for ticket #711.
      //if ( theModelType == ModelTypeGeographic )
      //{
      //   if(ossim::isnan(theOriginLon) && 
      //      (pixScaleSize > 1) &&
      //      (tiePointSize > 3))
      //   {
      //      theOriginLon = tiepoints[3] - tiepoints[0] * pixScale[0]; 
      //   }
      //   
      //   if(ossim::isnan(theOriginLat) && (pixScaleSize > 1) && (tiePointSize > 3))
      //   {
      //      theOriginLat = tiepoints[4] + tiepoints[1] * fabs(pixScale[1]);
      //   }
      //}
   }
   theModelTransformation.clear();
   ossim_uint16 transSize = 0;
   double* trans = 0;
   
   if(TIFFGetField(theTiffPtr, TIFFTAG_GEOTRANSMATRIX, &transSize, &trans))
   {
      theModelTransformation.insert(theModelTransformation.begin(),
                                    trans, trans+transSize);
   }
//    if(!theTiePoint.size()&&(theModelTransform.size()==16))
//    {
//       // for now we will stuff the tie point with the model transform tie points.
//       theTiePoint.push_back(0.0);
//       theTiePoint.push_back(0.0);
//       theTiePoint.push_back(0.0);
//       theTiePoint.push_back(theModelTransformation[3]); 
//       theTiePoint.push_back(theModelTransformation[7]);
//       theTiePoint.push_back(0.0);
//    }
   ossim_uint16 doubleParamSize = 0;
   double* tempDoubleParam = 0;
   theDoubleParam.clear();
   if(TIFFGetField(theTiffPtr, TIFFTAG_GEODOUBLEPARAMS, &doubleParamSize, &tempDoubleParam))
   {
      theDoubleParam.insert(theDoubleParam.begin(),
                            tempDoubleParam,
                            tempDoubleParam+doubleParamSize);
   }

   char* tempAsciiParam=0;
   theAsciiParam = "";
	
	// Note: this tag does not have the setting set to return the size
	// so this call is only a 3 argument call without a size parameter
	if(TIFFGetField(theTiffPtr, TIFFTAG_GEOASCIIPARAMS, &tempAsciiParam))
   {
      theAsciiParam = tempAsciiParam;
   }

   if(theGeoKeysPresentFlag)
   {
      setOssimProjectionName();  // Initialize the ossim projection name.
      setOssimDatumName();       // Initialize the ossim datum name.
   }

   // commenting this out.  Frank mentioned the GTIFFGetDefn which in geo_normalize
   // this should be all we need.
   //
#if 0  
   /* 
      ESH 05/2009: Replacing badly broken code for making 
      use of TIFFTAG_GEODOUBLEPARAMS.

      Read the geokey directory tag in order to see how 
      the TIFFTAG_GEODOUBLEPARAMS are defined.

      For structure of geokey directory, see:
      http://www.remotesensing.org/geotiff/spec/geotiff2.4.html
   */
   ossim_uint16  gkdParamSize = 0;
   ossim_uint16* gkdParams = 0;
   if(TIFFGetField(theTiffPtr, TIFFTAG_GEOKEYDIRECTORY, &gkdParamSize, &gkdParams))
   {
      ossim_uint16 numKeys = gkdParams ? gkdParams[3] : 0;
      ossim_uint16 key = 0;
      for( key=0; key<numKeys; ++key )
      {
         ossim_uint16 loc = (key+1) * 4;
         ossim_uint16 ind = gkdParams[loc+3];

         if ( gkdParams[loc+1] == TIFFTAG_GEODOUBLEPARAMS && 
              gkdParams[loc+2] == 1 && 
              ind >= 0 && ind < doubleParamSize )
         {
            double dval = theDoubleParam[ind];
            switch( gkdParams[loc] )
            {
               case ProjStdParallel1GeoKey:     theStdPar1       = dval; break;
               case ProjStdParallel2GeoKey:     theStdPar2       = dval; break;
               case ProjNatOriginLongGeoKey:    theOriginLon     = dval; break;
               /* case ProjOriginLongGeoKey:       theOriginLon     = dval; break; (alias) */
               case ProjNatOriginLatGeoKey:     theOriginLat     = dval; break;
               /* case ProjOriginLatGeoKey:        theOriginLat     = dval; break; (alias) */
               case ProjFalseEastingGeoKey:     theFalseEasting  = dval; break;
               case ProjFalseNorthingGeoKey:    theFalseNorthing = dval; break;
               case ProjCenterLongGeoKey:       theCenterLon     = dval; break;
               case ProjCenterLatGeoKey:        theCenterLat     = dval; break;
               case ProjScaleAtNatOriginGeoKey: theScaleFactor   = dval; break;
               default: 
                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "ossimGeoTiff::readTags: Unrecognized geokey directory entry."
                        << "\ngeokey directory index = " << loc
                        << "\ngeokey = " << gkdParams[loc]
                        << "\ndouble array index = " << ind
                        << "\ndval = " << dval
                        << std::endl;
                  }
                  break;
            }
         }
      }
   }
#endif

   GTIFFree(gtif);

   if (ownTiffPtrFlag == false)
   {
      //---
      // Zero out the pointer so the destructor doesn't close it on some
      // external code.
      //---
      theTiffPtr = 0;
   }
   
   return true;
}

bool ossimGeoTiff::addImageGeometry(ossimKeywordlist& kwl, const char* prefix) const
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeoTiff::addImageGeometry: Entered............."
         << std::endl;
   }

   // NOT SURE THIS IS A GOOD IDEA HERE. KEPT FOR LEGACY SAKE. (OLK 5/10)
   if(theGcsCode == 3785)
   {
      ossimRefPtr<ossimProjection> proj = 
         ossimProjectionFactoryRegistry::instance()->createProjection(ossimString("EPSG:3785"));
      if(proj.valid())
      {
         proj->saveState(kwl, prefix);
      }
   }

   //---
   // Sanity check...
   // NOTE: It takes six doubles to make one tie point ie:
   // x,y,z,longitude,latitude,height or x,y,z,easting,northing,height
   //---
   if (theErrorStatus ||
       (!usingModelTransform() &&
        ( (theScale.size() < 2) && // no scale
          ( theTiePoint.size() < 24) ) ) )//need at least 3 ties if no scale.
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimGeoTiff::addImageGeometry: Failed sanity check "
            << std::endl;
         if(theErrorStatus)
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "for error status" << std::endl;
         }
         else if( theTiePoint.size()<5)
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "for tie points, size = " <<  theTiePoint.size()
               << std::endl;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "for scale" << std::endl;
         }
      }
      return false;
   }

   double x_tie_point = 0.0;
   double y_tie_point = 0.0;
   ossim_uint32 tieCount = (ossim_uint32)theTiePoint.size()/6;

   if( (theScale.size() == 3) && (tieCount == 1))
   {
      //---
      // Shift the tie point to the (0, 0) position if it's not already.
      //
      // Note:
      // Some geotiff writers like ERDAS IMAGINE set the "GTRasterTypeGeoKey"
      // key to RasterPixelIsArea, then set the tie point to (0.5, 0.5).
      // This really means "RasterPixelIsPoint" with a tie point of (0.0, 0.0).
      // Anyway we will check for this blunder and attempt to do the right
      // thing...
      //---
      x_tie_point = theTiePoint[3] - theTiePoint[0]*theScale[0];
      y_tie_point = theTiePoint[4] + theTiePoint[1]*theScale[1];
   }
   else if(tieCount > 1)
   {
      //---
      // Should we check the model type??? (drb)
      // if (theModelType == ModelTypeGeographic)
      //---
      if(tieCount >= 4)
      {
         ossimTieGptSet tieSet;
         getTieSet(tieSet);

         if(tieCount > 4)
         {
            // create a cubic polynomial model
            //ossimRefPtr<ossimPolynomProjection> proj = new ossimPolynomProjection;
            //proj->setupOptimizer("1 x y x2 xy y2 x3 y3 xy2 x2y z xz yz");
            ossimRefPtr<ossimBilinearProjection> proj = new ossimBilinearProjection;
            proj->optimizeFit(tieSet);
            proj->saveState(kwl, prefix);
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimGeoTiff::addImageGeometry: "
                  << "Creating a Cubic polynomial projection" << std::endl;
            }
            
            return true;
            
         }
         else if(tieCount == 4)
         {
            // create a bilinear model

            // Should we check the model type (drb)
            // if (theModelType == ModelTypeGeographic)
            
            ossimRefPtr<ossimBilinearProjection> proj = new ossimBilinearProjection;
            proj->optimizeFit(tieSet);
            proj->saveState(kwl, prefix);
            
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimGeoTiff::addImageGeometry: "
                  << "Creating a bilinear projection" << std::endl;
            }
           return true;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimGeoTiff::addImageGeometry: "
               << "Not enough tie points to create a interpolation model"
               <<std::endl;
         }
         return false;
      }
   }
   else if (usingModelTransform())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimGeoTiff::addImageGeometry: Do not support rotated "
            << "map models yet.  You should provide the image as a sample "
            << "and we will fix it" << std::endl;
      }
   }
  
   if ((theRasterType == PIXEL_IS_AREA))
   {
      // Since the internal pixel representation is "point", shift the
      // tie point to be relative to the center of the pixel.
      if (theScale.size() > 1)
      {
         x_tie_point += (theScale[0])/2.0;
         y_tie_point -= (theScale[1])/2.0;
      }
   }

   if( thePcsCode && (thePcsCode != USER_DEFINED) )
   {
      ossimString epsg_spec ("EPSG:");
      epsg_spec += ossimString::toString(thePcsCode);
      ossimRefPtr<ossimProjection> proj = 
         ossimEpsgProjectionFactory::instance()->createProjection(epsg_spec);
      if (proj.valid())
      {
         proj->saveState(kwl, prefix);
      }
      // Should be some else "WARNING" here maybe. (drb)
   }
   else if (getOssimProjectionName() == "unknown")
   {
      //---
      // Get the projection type.  If unknown no point going on, so get out.
      //---
      return false;
   }
   else
   {
      // No PCS code but we do have a projection name
      // Add these for all projections.
      kwl.add(prefix, ossimKeywordNames::TYPE_KW, getOssimProjectionName());
      kwl.add(prefix, ossimKeywordNames::DATUM_KW, getOssimDatumName());
   }

   // Now set the image-specific projection info (scale and image tiepoint):
   if (theModelType == MODEL_TYPE_GEOGRAPHIC)
   {
      if (theAngularUnits != ANGULAR_DEGREE)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING ossimGeoTiff::addImageGeometry:"
            << "\nNot coded yet for unit type:  "
            << theAngularUnits << endl;
         return false;
      }

      //---
      // Tiepoint
      // Have data with tie points -180.001389 so use ossimGpt::wrap() to handle:
      //---
      ossimGpt tieGpt(y_tie_point, x_tie_point, 0.0);
      tieGpt.wrap();
      ossimDpt tiepoint(tieGpt);
      kwl.add(prefix, ossimKeywordNames::TIE_POINT_XY_KW, tiepoint.toString(), true);
      kwl.add(prefix, ossimKeywordNames::TIE_POINT_UNITS_KW, "degrees", true);

      // scale or gsd
      if (theScale.size() > 1)
      {
         ossimDpt scale (theScale[0], theScale[1]);
         kwl.add(prefix, ossimKeywordNames::PIXEL_SCALE_XY_KW, scale.toString(), true);
         kwl.add(prefix, ossimKeywordNames::PIXEL_SCALE_UNITS_KW, "degrees", true);

         // origin
         if ( ossim::isnan(theOriginLat) )
         {
            //---
            // Put the origin lat at the center of the image so the meters per
            // pixel is somewhat real.
            //---
            double centerY = theLength/2.0;
            theOriginLat = tieGpt.lat - theScale[1]*centerY;
         }

         if (  ossim::isnan(theOriginLon) )
         {
            theOriginLon = 0.0;
         }
      }
      
      if (!(ossim::isnan(theOriginLat) || ossim::isnan(theOriginLon)))
      {
         kwl.add(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW,  theOriginLat, true);
         kwl.add(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW, theOriginLon, true);
      }

   }
   else // Projected
   {
      // Tiepoint
      ossimDpt tiepoint (convert2meters(x_tie_point),convert2meters(y_tie_point));
      kwl.add(prefix, ossimKeywordNames::TIE_POINT_XY_KW, tiepoint.toString(), true);
      kwl.add(prefix, ossimKeywordNames::TIE_POINT_UNITS_KW, "meters", true);

      // scale or gsd
      if (theScale.size() > 1)
      {
         ossimDpt scale (convert2meters(theScale[0]), convert2meters(theScale[1]));
         kwl.add(prefix, ossimKeywordNames::PIXEL_SCALE_XY_KW, scale.toString(), true);
         kwl.add(prefix, ossimKeywordNames::PIXEL_SCALE_UNITS_KW, "meters", true);
      }
      
      // origin
      if(!ossim::isnan(theOriginLat))
         kwl.add(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW, theOriginLat);
      if(!ossim::isnan(theOriginLon))
         kwl.add(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW, theOriginLon);

      // std paralles for conical projections
      kwl.add(prefix, ossimKeywordNames::STD_PARALLEL_1_KW, theStdPar1);
      kwl.add(prefix, ossimKeywordNames::STD_PARALLEL_2_KW, theStdPar2);
      
      // false easting and northing.
      kwl.add(prefix, ossimKeywordNames::FALSE_EASTING_KW, convert2meters(theFalseEasting));
      kwl.add(prefix, ossimKeywordNames::FALSE_NORTHING_KW, convert2meters(theFalseNorthing));

      // Based on projection type, override/add the appropriate info.
      if (getOssimProjectionName() == "ossimUtmProjection")
      {
         // Check the zone before adding...
         if (theZone > 0 && theZone < 61)
         {
            kwl.add(prefix, ossimKeywordNames::ZONE_KW, theZone);
         }
         else
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimGeoTiff::addImageGeometry: "
               << "UTM zone " << theZone << " out of range!\n"
               << "Valid range:  1 to 60" << endl;
            return false;
         }

         // Check the hemisphere before adding.
         if (theHemisphere == "N" || theHemisphere == "S")
         {
            kwl.add(prefix, ossimKeywordNames::HEMISPHERE_KW, theHemisphere);
         }
         else
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimGeoTiff::addImageGeometry: "
               << "UTM hemisphere " << theHemisphere << " is invalid!\n"
               << "Valid hemisphere:  N or S" << std::endl;
            return false;
         }

         //---
         // Must set the central meridian even though the zone should do it.
         // (in decimal degrees)
         //---
         double central_meridian = ( 6.0 * abs(theZone) ) - 183.0;
         kwl.add(prefix, ossimKeywordNames::CENTRAL_MERIDIAN_KW, central_meridian, true);
         kwl.add(prefix, ossimKeywordNames::ORIGIN_LATITUDE_KW, 0.0, true);

      } // End of "if (UTM)"

      else if (getOssimProjectionName() == "ossimTransMercatorProjection")
         kwl.add(prefix, ossimKeywordNames::SCALE_FACTOR_KW, theScaleFactor, true); 

   } // end of projected CS

   //---
   // Get the model transformation info if it's present.
   //---
   if (usingModelTransform())
   {
      std::vector<double> v = getModelTransformation();
      std::ostringstream out;
      out << std::setprecision(15); // To avoid truncating.
      ossim_uint32 idx = 0;
      for(idx =0; idx < 16; ++idx)
      {
         out << v[idx] << " ";
      }
      kwl.add(prefix, ossimKeywordNames::IMAGE_MODEL_TRANSFORM_MATRIX_KW, out.str().c_str(), true);
      ossimUnitType modelTransformUnitType = OSSIM_UNIT_UNKNOWN;
      if(theModelType == ModelTypeGeographic)
      {
         switch(theAngularUnits)
         {
            case ANGULAR_DEGREE:
               modelTransformUnitType = OSSIM_DEGREES;
               break;
            case ANGULAR_ARC_MINUTE:
               modelTransformUnitType = OSSIM_MINUTES;
               break;
            case ANGULAR_ARC_SECOND:
               modelTransformUnitType = OSSIM_SECONDS;
               break;
            default:
               return false;
         }
      }
      else if(theModelType == ModelTypeProjected)
      {
         switch(theLinearUnitsCode)
         {
            case LINEAR_METER:
               modelTransformUnitType = OSSIM_METERS;
               break;
            default:
               return false;
         }
      }
      kwl.add(prefix, ossimKeywordNames::IMAGE_MODEL_TRANSFORM_UNIT_KW,
              ossimUnitTypeLut::instance()->getEntryString(modelTransformUnitType), true);
   } 

   if(theScaleFactor > 0.0)
   {
      kwl.add(prefix, ossimKeywordNames::SCALE_FACTOR_KW, theScaleFactor, true);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::addImageGeometry: Keyword list dump:\n"
         << kwl << std::endl;
   }
   
   return true;
}

double ossimGeoTiff::convert2meters(double d) const
{
   switch(theLinearUnitsCode)
   {
   case LINEAR_METER:
      return d;
   case LINEAR_FOOT:
      return ossim::ft2mtrs(d);
   case LINEAR_FOOT_US_SURVEY:
      return ossim::usft2mtrs(d);
   default:
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimGeoTiff::convert2meters: " 
            << "Linear units code was undefined!\n"
            << "No conversion was performed." << std::endl;
      }
      break;
   }
   
   return d;
}

ossimString ossimGeoTiff::getOssimProjectionName() const
{
   return theProjectionName;
}

void ossimGeoTiff::setOssimProjectionName()
{
   //---
   // The "parsePcsCode" method can also set the projection name.  So check
   // it prior to looking in the lookup table.
   //---
   if (theProjectionName == "unknown")
   {
      ossimString name =  COORD_TRANS_LUT.getEntryString(theCoorTransGeoCode);
      
      if (name.size())
      {
         theProjectionName = name;
      }
   }

   // If still unknown check for the model type.
   if (theProjectionName == "unknown")
   {
      if (theModelType == ModelTypeGeographic)
      {
         theProjectionName = "ossimEquDistCylProjection";
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::setOssimProjectionName: "
         << "theProjectionName:  "
         << theProjectionName
         << std::endl;
   }
   
}

ossimString ossimGeoTiff::getOssimDatumName() const
{
   return theDatumName;
}

void ossimGeoTiff::setOssimDatumName()
{
   //---
   // The "parsePcsCode" method can also set the datum name.  So check
   // it prior to trying to assign.
   //---
   if (theDatumName == "unknown")
   {
      ossimString name = DATUM_LUT.getEntryString(theDatumCode);
      
      if (!name.empty())
      {
         theDatumName = name;
      }
      else
      {
         // Try the GCS code.
         name = DATUM_LUT.getEntryString(theGcsCode);
         if (name.size())
         {
            theDatumName = name;
         }
      }
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimGeoTiff::setOssimDatumName: "
         << "theDatumName:  "
         << theDatumName
         << std::endl;
   } 
}

//*************************************************************************************************
//! Initializes data members given a projection code. Returns TRUE if valid PCS code specified.
//*************************************************************************************************
bool ossimGeoTiff::parsePcsCode()
{
   // key 3072 Section 6.3.3.1 codes
   ossimString epsg_spec (ossimString("EPSG:") + ossimString::toString(thePcsCode));
   ossimRefPtr<ossimProjection> proj = 
      ossimEpsgProjectionFactory::instance()->createProjection(epsg_spec);
   ossimMapProjection* map_proj = PTR_CAST(ossimMapProjection, proj.get());
   if (!parseProjection(map_proj))
      thePcsCode = 0;

   return (thePcsCode != 0);
}

//*************************************************************************************************
//! Initializes data members given a projection.
//*************************************************************************************************
bool ossimGeoTiff::parseProjection(ossimMapProjection* map_proj)
{
   if (map_proj == NULL)
      return false;

   // Initialize parameters from base-class ossimMapProjection:
   if (map_proj->isGeographic())
   {
      theModelType = ModelTypeGeographic;
      theAngularUnits = ANGULAR_DEGREE;
      theLinearUnitsCode = ANGULAR_DEGREE;
   }
   else
      theModelType = ModelTypeProjected;

   theProjectionName = map_proj->getProjectionName();
   theFalseEasting   = map_proj->getFalseEasting();
   theFalseNorthing  = map_proj->getFalseNorthing();
   theStdPar1        = map_proj->getStandardParallel1();
   theStdPar2        = map_proj->getStandardParallel2();
   thePcsCode        = map_proj->getPcsCode();

   // GCS code only defined for geographic projections and is basically the same as PCS in OSSIM:
   theGcsCode = 0;
   
   ossimGpt origin (map_proj->origin());
   theOriginLat      = origin.lat;
   theOriginLon      = origin.lon;
   
   const ossimDatum* datum = map_proj->getDatum();
   if (datum)
   {
      theDatumName  = datum->name();
      theDatumCode  = datum->epsgCode();
   }
   
   // Now intercept a select few that have additional parameters specific to their derived type:
   if (map_proj->getProjectionName() == "ossimEquDistCylProjection")
   {
      theGcsCode = map_proj->getPcsCode();
   }
   else if (theProjectionName == "ossimUtmProjection")
   {
      ossimUtmProjection* utm_proj = PTR_CAST(ossimUtmProjection, map_proj);
      theHemisphere = utm_proj->getHemisphere();
      theScaleFactor = 0.9996; // UTM fixed
      theZone = utm_proj->getZone();
   }
   else if (theProjectionName == "ossimTransMercatorProjection")
   {
      ossimTransMercatorProjection* tm_proj = PTR_CAST(ossimTransMercatorProjection, map_proj);
      theScaleFactor = tm_proj->getScaleFactor();
   }
   else if (theProjectionName == "ossimBngProjection")
   {
      // ### LEGACY HACK ###
      theDatumName = "OGB-M";
      theFalseEasting = 400000.0;
      theFalseNorthing = -100000.0;
      theScaleFactor   = .9996012717;
      theOriginLat     = 49.0;
      theOriginLon     = -2.0;
      theHemisphere    = "N";
   }

   return true;
}

int ossimGeoTiff::mapZone() const
{
   return theZone;
}

void ossimGeoTiff::getScale(std::vector<double>& scale) const
{
   scale = theScale;
}

void ossimGeoTiff::getTiePoint(std::vector<double>& tie_point) const
{
   tie_point = theTiePoint;
}

void ossimGeoTiff::getModelTransformation(std::vector<double>& transform) const
{
   transform = theModelTransformation;
}

const std::vector<double>& ossimGeoTiff::getScale() const
{
   return theScale;
}

ossimPixelType ossimGeoTiff::getRasterType() const
{
   if (theRasterType == PIXEL_IS_AREA)
      return OSSIM_PIXEL_IS_AREA;

   return OSSIM_PIXEL_IS_POINT;
}

const std::vector<double>& ossimGeoTiff::getTiePoint() const
{
   return theTiePoint;
}

const std::vector<double>& ossimGeoTiff::getModelTransformation() const
{
   return theModelTransformation;
}

int ossimGeoTiff::getWidth() const
{
   return theWidth;
}

int ossimGeoTiff::getLength() const
{
   return theLength;
}

std::ostream& ossimGeoTiff::print(std::ostream& out) const
{
   // Capture stream flags.
   std::ios_base::fmtflags f = out.flags();
   
   out << setiosflags(ios::fixed) << setprecision(15)
       << "ossimGeoTiff::print" << std::endl;

   if (theScale.size())
   {
      std::vector<double>::const_iterator i = theScale.begin();
      ossim_uint32 index = 0;
      while ( i < theScale.end() )
      {
         out << "theScale[" << index << "]: " << (*i) << std::endl;
         ++index;
         ++i;
      }
   }
   else
   {
      out << "theScale is empty..." << endl;
   }
   
   if (theTiePoint.size())
   {
      std::vector<double>::const_iterator i = theTiePoint.begin();
      ossim_uint32 index = 0;
      while ( i < theTiePoint.end() )
      {
         out << "theTiePoint[" << index << "]: " << (*i) << std::endl;
         ++index;
         ++i;
      }
   }
   else
   {
      out << "theTiePoint is empty..." << endl;
   }

   if (theModelTransformation.size())
   {
      std::vector<double>::const_iterator i = theModelTransformation.begin();
      ossim_uint32 index = 0;
      while ( i < theModelTransformation.end() )
      {
         out << "theModelTransformation[" << index << "]: "
             << (*i) << std::endl;
         ++index;
         ++i;
      }
   }  
   else
   {
      out << "theModelTransformation is empty..." << endl;
   }

   // Set the flags back.
   out.flags(f);
   
   return out;
}


bool ossimGeoTiff::usingModelTransform() const
{
   //---
   // If we have 16 model points do we always use them? (drb)
   //
   // In other word should the check just be if size == 16?
   //--- 
   if (getModelTransformation().size() == 16)
   {
      if (theScale.size() == 0)
      {
         // Need at least 24 (which is four ties) to use bilinear.
         if (theTiePoint.size() < 24)
         {
            return true;
         }
      }
   }
   return false;
}

void ossimGeoTiff::getTieSet(ossimTieGptSet& tieSet) const
{
   ossim_uint32 idx = 0;
   ossim_uint32 tieCount = (ossim_uint32)theTiePoint.size()/6;
   const double* tiePointsPtr = &theTiePoint.front();
   double offset = 0;
   if (hasOneBasedTiePoints())
   {
      offset = -1.0;
   }
   
   for(idx = 0; idx < tieCount; ++idx)
   {
      ossimDpt xyPixel(tiePointsPtr[0]+offset, tiePointsPtr[1]+offset);
      // tie[3] = x, tie[4]
      ossimGpt gpt(tiePointsPtr[4], tiePointsPtr[3], tiePointsPtr[5]);
      
      tieSet.addTiePoint(new ossimTieGpt(gpt, xyPixel, .5));
      tiePointsPtr+=6;
   }
}

bool ossimGeoTiff::hasOneBasedTiePoints() const
{
   bool result = false;
   
   // Assuming ties of (x,y,z,lat,lon,hgt) so size should be divide by 3.
   if (theTiePoint.size()%6)
   {
      return result;
   }
   
   ossim_float64 minX = 999999.0;
   ossim_float64 minY = 999999.0;
   ossim_float64 maxX = 0.0;
   ossim_float64 maxY = 0.0;

   const ossim_uint32 SIZE = (ossim_uint32)theTiePoint.size();
   ossim_uint32 tieIndex = 0;

   while (tieIndex < SIZE)
   {
      if ( theTiePoint[tieIndex]   < minX ) minX = theTiePoint[tieIndex];
      if ( theTiePoint[tieIndex]   > maxX ) maxX = theTiePoint[tieIndex];
      if ( theTiePoint[tieIndex+1] < minY ) minY = theTiePoint[tieIndex+1];
      if ( theTiePoint[tieIndex+1] > maxY ) maxY = theTiePoint[tieIndex+1];
      tieIndex += 6;
   }

   if ( (minX == 1) && (maxX == theWidth) &&
        (minY == 1) && (maxY == theLength) )
   {
      result = true;
   }

#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimGeoTiff::hasOneBasedTiePoints DEBUG:"
         << "\nminX:       " << minX
         << "\nmaxX:       " << maxX
         << "\nminY:       " << minY
         << "\nmaxY:       " << maxY
         << "\ntheWidth:   " << theWidth
         << "\ntheLength:  " << theLength
         << "\none based:  " << (result?"true":"false")
         << std::endl;
   }
#endif
   
   return result;
}

//*************************************************************************************************
// ArcMAP 9.2 bug workaround.
// ESH 05/2008 -- ArcMap 9.2 compatibility hack
// If the PCS code is for a HARN state plane and the implied pcs 
// code's units is feet (us or intl), we find the equivalent code 
// for units of meters.  We're doing this because ArcMap (9.2 and 
// less) doesn't understand the non-meters HARN codes.  However, 
// the units are left unchanged in this process, so the units can 
// be different than the user-specified pcs code. ArcMap 9.2 
// seems to understand the mixed definition just fine.
// OLK 04/2010 -- Converted to vector<pair> scheme after refactoring EPSG factory. 
//*************************************************************************************************
ossim_uint16 getMetersEquivalentHarnCode(ossim_uint16 feet_harn_code)
{
   static const ossim_uint16 harn_feet[] =
   {
      2867, 2868, 2869, 2870, 2871, 2872, 2873, 2874, 2875, 2876, 2877, 2878, 2879, 2880, 2881, 2882, 
      2883, 2884, 2885, 2886, 2887, 2888, 2891, 2892, 2893, 2894, 2895, 2896, 2897, 2898, 2899, 2900, 
      2901, 2902, 2903, 2904, 2905, 2906, 2907, 2908, 2909, 2910, 2911, 2912, 2913, 2914, 2915, 2916, 
      2917, 2918, 2919, 2920, 2921, 2922, 2923, 2924, 2925, 2926, 2927, 2928, 2929, 2930, 2967, 2968
   };
   static const ossim_uint16 harn_meters[] =
   {
      2761, 2762, 2763, 2766, 2767, 2768, 2769, 2770, 2771, 2772, 2773, 2774, 2775, 2776, 2777, 2778, 
      2779, 2780, 2781, 2787, 2788, 2789, 2798, 2799, 2804, 2805, 2806, 2807, 2808, 2809, 2813, 2814, 
      2818, 2825, 2826, 2827, 2828, 2829, 2830, 2831, 2832, 2833, 2836, 2837, 2838, 2839, 2843, 2844, 
      2845, 2846, 2847, 2848, 2849, 2850, 2851, 2853, 2854, 2855, 2856, 2859, 2860, 2861, 2792, 2793
   };

   ossim_uint16 result = 0;
   int index = 0;
   while ((result == 0) && (index < 64))
   {
      if (harn_feet[index] == feet_harn_code)
         result = harn_meters[index];
      ++index;
   }
   return result;
};

