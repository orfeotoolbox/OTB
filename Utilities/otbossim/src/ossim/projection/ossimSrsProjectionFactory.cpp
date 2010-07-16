//*****************************************************************************
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimMapProjectionFactory
//
//*****************************************************************************
//  $Id: ossimSrsProjectionFactory.cpp 17206 2010-04-25 23:20:40Z dburken $
#include <sstream>
#include <ossim/projection/ossimSrsProjectionFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>

ossimSrsProjectionFactory* ossimSrsProjectionFactory::theInstance = 0;

ossimSrsProjectionFactory* ossimSrsProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimSrsProjectionFactory;
   }

   return (ossimSrsProjectionFactory*) theInstance;
}

ossimProjection* ossimSrsProjectionFactory::createProjection(const ossimFilename& /* filename */,
                                                             ossim_uint32 /* entryIdx */)const
{
   return 0;
}

ossimProjection* ossimSrsProjectionFactory::createProjection(const ossimKeywordlist &keywordList,
                                                             const char *prefix) const
{
   const char *lookup = keywordList.find(prefix, ossimKeywordNames::TYPE_KW);

   if(lookup)
   {
      ossimProjection* proj = createProjection(ossimString(lookup));
      if(proj)
      {
         proj->loadState(keywordList, prefix);
         return proj;
      }
   }

   return 0;
}

ossimProjection* ossimSrsProjectionFactory::createProjection(const ossimString &name) const
{
   ossimString tempName = name;
   tempName = tempName.trim();
   tempName = tempName.upcase();
   if(tempName.contains("AUTO"))
   {
      ossimString projId;
      ossimString projUnits;
      ossimString projOriginLon;
      ossimString projOriginLat;
      tempName = tempName.substitute("AUTO:", "");
      tempName = tempName.substitute(",", " ", true);
      std::istringstream in(tempName);
      in >> projId >> projUnits >> projOriginLon >> projOriginLat;

      ossimGpt origin(projOriginLat.toDouble(), projOriginLon.toDouble());

      switch(projId.toUInt32())
      {
         case 42001:
         {
            ossimUtmProjection* utm = new ossimUtmProjection;
            utm->setZone(origin);
            utm->setHemisphere(origin);
            utm->setOrigin(origin);
            utm->update();
            return utm;
         }
         case 42002:
         {
            ossimTransMercatorProjection* transMerc = new ossimTransMercatorProjection;

            transMerc->setFalseNorthing(origin.latd()>=0.0?0.0:10000000.0);
            transMerc->setOrigin(ossimGpt(0.0, origin.lond()));
            transMerc->setScaleFactor(.9996);
            transMerc->update();
            return transMerc;
         }
         case 42003:
         {
            ossimOrthoGraphicProjection* ortho = new ossimOrthoGraphicProjection;
            ortho->setOrigin(origin);
            ortho->update();
            return ortho;
         }
         case 42004:
         {
            ossimEquDistCylProjection* geographic = new ossimEquDistCylProjection;
            geographic->setOrigin(origin);
            geographic->update();
            return geographic;
         }
         default:
         {
            return 0;
         }
      }
   }
   else if(tempName.contains("EPSG"))
   {
      tempName = tempName.substitute("EPSG:", "");
      ossim_uint32 code = tempName.toUInt32();
      switch(code)
      {
         case 4326:
         {
            ossimEquDistCylProjection* proj =
               new ossimEquDistCylProjection(*(ossimDatumFactory::instance()->wgs84()->ellipsoid()));
            proj->setDatum(ossimDatumFactory::instance()->wgs84());
            proj->setPcsCode(4326);
            return proj;
         }
         case 900913: // global mercator projection
         case 3785:
         {
            const ossimDatum* datum = ossimDatumFactoryRegistry::instance()->
               create( ossimString("6055") );
            if(datum)
            {
               ossimMercatorProjection* merc = new ossimMercatorProjection();
               ossimGpt origin(0.0,0.0,0.0, datum);
               merc->setFalseEasting(0.0);
               merc->setFalseNorthing(0.0);
               merc->setOrigin(origin);
               merc->update();
               merc->setPcsCode(3785);
               return merc;
            }
            break;
         }
         default:
         {
            return 0;
         }
      }
   }

   // Got here? No projection code recognized:
   return 0;
}

ossimObject* ossimSrsProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimSrsProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimSrsProjectionFactory::getTypeNameList(std::vector<ossimString>& /* typeList */)const
{
   
}
