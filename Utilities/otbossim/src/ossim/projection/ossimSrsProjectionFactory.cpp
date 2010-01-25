//*****************************************************************************
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains implementation of class ossimMapProjectionFactory
//
//*****************************************************************************
//  $Id: ossimSrsProjectionFactory.cpp 13459 2008-08-20 12:48:50Z gpotts $
#include <sstream>
#include <ossim/projection/ossimSrsProjectionFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>

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
