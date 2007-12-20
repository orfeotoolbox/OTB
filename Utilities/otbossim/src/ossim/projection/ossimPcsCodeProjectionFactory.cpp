//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Class definition of projection factory to return projection from
// a Projection Coordinate System (PCS) code.
//
// For current documentation on pcs codes see:
// 
// http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.3.1
// 
//----------------------------------------------------------------------------
//  $Id: ossimPcsCodeProjectionFactory.cpp 12082 2007-11-26 21:46:44Z dburken $

#include <ossim/projection/ossimPcsCodeProjectionFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/projection/ossimUtmProjection.h>


static const ossimTrace
traceDebug(ossimString("ossimPcsCodeProjectionFactory:debug"));

#if OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimPcsCodeProjectionFactory.cpp 12082 2007-11-26 21:46:44Z dburken $";
#endif

ossimPcsCodeProjectionFactory* ossimPcsCodeProjectionFactory::theInstance = 0;

ossimPcsCodeProjectionFactory::ossimPcsCodeProjectionFactory()
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPcsCodeProjectionFactory::ossimPcsCodeProjectionFactory"
         << " entered..." << std::endl;
#if OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif
   }
}

ossimPcsCodeProjectionFactory* ossimPcsCodeProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimPcsCodeProjectionFactory;
   }

   return (ossimPcsCodeProjectionFactory*) theInstance;
}

ossimProjection*
ossimPcsCodeProjectionFactory::createProjection(const ossimFilename& filename,
                                                ossim_uint32 entryIdx)const
{
   if(!filename.exists())
   {
      return NULL;
   }

   // Check for external geometry file.
   ossimProjection* proj = createProjectionFromGeometryFile(filename,
                                                            entryIdx);
   if (proj)
   {
      return proj;
   }

   ossimFilename geomFile = filename;
   geomFile = geomFile.setExtension("geom");
   if(!geomFile.exists())
   {
      return NULL;
   }
   
   ossimKeywordlist kwl;
   if(kwl.addFile(geomFile))
   {
      return createProjection(kwl);
   }

   return NULL;
}

ossimProjection* ossimPcsCodeProjectionFactory::createProjection(
   const ossimKeywordlist &keywordList, const char *prefix) const
{
   ossimProjection *result=NULL;
	
   const char *lookup = keywordList.find(prefix,
                                         ossimKeywordNames::PCS_CODE_KW);
   if(lookup)
   {
      result = createProjection(ossimString(lookup).trim());
      if(result)
      {
         result->loadState(keywordList, prefix);
      }
   }
   return result;
}

ossimProjection* ossimPcsCodeProjectionFactory::createProjection(
   const ossimString &name) const
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimPcsCodeProjectionFactory::createProjection entered..."
         << "\nname:  " << name << endl;
   }
   
   ossimProjection* result = NULL;
   
   ossim_int32 code = name.toInt32();

   //---
   // Divide the code by 100. Then check for a known type.  If it is a
   // utm projection the last two digits represent the zone.
   //---
   int type = code/100;
   int zone = code%100;

   switch (type)
   {
      case 322:
      {
         //---
         // utm, WGS72 (WGD), northern hemisphere
         // All 60 zones handled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs72()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs72());
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
         
      case 323:
      {
         //---
         // utm, WGS72 (WGD), southern hemisphere
         // All 60 zones handled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs72()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs72());
            proj->setZone(zone);
            proj->setHemisphere('S');
            result = proj;
            return result;
         }
         break;
      }
      
      case 326:
      {
         //---
         // utm, WGS84 (WGE), northern hemisphere
         // All 60 zones hadled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs84()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs84());
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
      
      case 327:
      {
         //---
         // utm, WGS84 (WGE), southern hemisphere
         // All 60 zones handled.
         //---
         if ( (zone > 0) && (zone < 61) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        wgs84()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           wgs84());
            proj->setZone(zone);
            proj->setHemisphere('S');
            result = proj;
            return result;
         }
         break;
      }
      
      case 267:
      {
         //---
         // utm, "NAS-C", northern hemisphere
         // Only UTM NAD27 North zones 3 to 22 are in the 267xx range...
         // 26729 through 26803 handled by state plane factory.
         //---
         if ( (code > 26702) && (code < 26723) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        create(ossimString("NAS-C"))->
                                        ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           create(ossimString("NAS-C")));
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
         
      case 269:
      {
         //---
         // utm, "NAR-C", northern hemisphere
         // Only UTM NAD83 North zones 3 to 23 are in the 269xx
         // range...
         // 26929 through 26998 handled by state plane factory.
         //---
         if ( (code > 26902) && (code < 26924) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        create(ossimString("NAR-C"))->
                                        ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->
                           create(ossimString("NAR-C")));
            proj->setZone(zone);
            proj->setHemisphere('N');
            result = proj;
            return result;
         }
         break;
      }
      
      case 248:
      {
         //---
         // Provisional S. American 1956
         // 24818 through 24880
         //---
         if ( (code > 24817) && (code < 24881) )
         {
            ossimUtmProjection* proj =
               new ossimUtmProjection(*(ossimDatumFactory::instance()->
                                        create(ossimString("PRB-M"))->
                                        ellipsoid()));
            
            proj->setDatum(ossimDatumFactory::instance()->
                           create(ossimString("PRB-M")));
            if (zone > 60)
            {
               proj->setZone(zone - 60);
               proj->setHemisphere('S');
            }
            else
            {
               proj->setZone(zone);
               proj->setHemisphere('N');
            }
            result = proj;
            return result;
         }
         break;
      }
      
   } // End of switch on code.

   return result;
}

ossimObject* ossimPcsCodeProjectionFactory::createObject(
   const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimPcsCodeProjectionFactory::createObject(
   const ossimKeywordlist& kwl,
   const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimPcsCodeProjectionFactory::getTypeNameList(
   std::vector<ossimString>& /* typeList */)const
{
}

ossim_int16 ossimPcsCodeProjectionFactory::getPcsCodeFromProjection(
   const ossimMapProjection* proj) const
{
   ossim_int16 pcsCode = 0;
   
   if (!proj)
   {
      return pcsCode;
   }
   
   ossimUtmProjection* utm = PTR_CAST(ossimUtmProjection, proj);
   if (utm)
   {
      ossim_int32 mapZone    = utm->getZone();
      ossimString hemisphere = utm->getHemisphere();
      ossimString datumCode  = utm->getDatum()->code();

      if (datumCode == "WGE")
      {
         if (hemisphere == "N") // Northern hemisphere.
         {
            pcsCode = 32600 + mapZone;
         }
         else // Southern hemisphere.
         {
            pcsCode = 32700 + mapZone;
         }
      }
      else if (datumCode == "WGD")
      {
         if (hemisphere == "N") // Northern hemisphere.
         {
            pcsCode = 32200 + mapZone;
         }
         else // Southern hemisphere.
         {
            pcsCode = 32300 + mapZone;
         }
      }
      else if (datumCode.contains("NAS-"))
      {
         if (hemisphere == "N") // Northern hemisphere.
         {
            pcsCode = 26700 + mapZone;
         }
         else // Southern hemisphere.
         {
            pcsCode = 32000 + mapZone;
         }
      }
      else if (datumCode.contains("NAR-"))
      {
         if (hemisphere == "N") // Northern hemisphere.
         {
            pcsCode = 26900 + mapZone;
         }
         else // Southern hemisphere.
         {
            pcsCode = 32100 + mapZone;
         }
      }
   }

   return pcsCode;
}
