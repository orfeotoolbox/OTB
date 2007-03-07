//*****************************************************************************
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimMapProjectionFactory
//
//*****************************************************************************
//  $Id: ossimSrsProjectionFactory.cpp,v 1.2 2005/09/08 17:36:22 gpotts Exp $
#include <sstream>
#include "ossimSrsProjectionFactory.h"
#include <base/common/ossimKeywordNames.h>
#include <projections/map_projections/ossimUtmProjection.h>
#include <projections/map_projections/ossimEquDistCylProjection.h>
#include <projections/map_projections/ossimEquDistCylProjection.h>
#include <projections/map_projections/ossimTransMercatorProjection.h>
#include <projections/map_projections/ossimOrthoGraphicProjection.h>

ossimSrsProjectionFactory* ossimSrsProjectionFactory::theInstance = 0;

ossimSrsProjectionFactory* ossimSrsProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimSrsProjectionFactory;
   }

   return (ossimSrsProjectionFactory*) theInstance;
}

ossimProjection* ossimSrsProjectionFactory::createProjection(const ossimFilename& filename,
                                                             ossim_uint32 entryIdx)const
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
      tempName = tempName.substitute("AUTO:",
                                     "");
      tempName = tempName.substitute(",",
                                     " ",
                                     true);
      std::istringstream in(tempName);
      in >> projId >> projUnits >> projOriginLon >> projOriginLat;

      ossimGpt origin(projOriginLat.toDouble(),
                      projOriginLon.toDouble());

      switch(projId.toUInt32())
      {
         case 42001:
         {
            ossimUtmProjection* utm = new ossimUtmProjection;
            utm->setZone(origin);
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
      switch(tempName.toUInt32())
      {
         case 4326:
         {
            return new ossimEquDistCylProjection;
         }
         default:
         {
            return 0;
         }
      }
   }

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

void ossimSrsProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   
}

list<ossimString> ossimSrsProjectionFactory::getList()const
{
   list<ossimString> result;

   
   return result;
}
