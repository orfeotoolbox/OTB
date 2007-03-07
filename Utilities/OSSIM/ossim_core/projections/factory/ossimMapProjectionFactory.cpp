//*****************************************************************************
// FILE: ossimMapProjectionFactory.cc
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimMapProjectionFactory
//
// SOFTWARE HISTORY:
//>
//   15Jun2001  Garrett Potts
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimMapProjectionFactory.cpp,v 1.42 2004/09/24 17:19:40 dburken Exp $

#include <projections/factory/ossimMapProjectionFactory.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimKeywordNames.h>
#include <base/data_types/ossimFilename.h>

//***
// Note to programmer: To add a new projection, search this file for
// "ADD_PROJECTION" to locate places requiring editing. Functional example
// below...
//
// ADD_PROJECTION: Include all map projection headers here:
//***

#include "projections/map_projections/ossimAlbersProjection.h"
#include "projections/map_projections/ossimAzimEquDistProjection.h"
#include "projections/map_projections/ossimBonneProjection.h"
#include "projections/map_projections/ossimBngProjection.h"
#include "projections/map_projections/ossimCassiniProjection.h"
#include "projections/map_projections/ossimCylEquAreaProjection.h"
#include "projections/map_projections/ossimEquDistCylProjection.h"
#include "projections/map_projections/ossimEckert4Projection.h"
#include "projections/map_projections/ossimEckert6Projection.h"
#include "projections/map_projections/ossimGnomonicProjection.h"
#include "projections/map_projections/ossimLambertConformalConicProjection.h"
#include "projections/map_projections/ossimLlxyProjection.h"
#include "projections/map_projections/ossimMercatorProjection.h"
#include "projections/map_projections/ossimMillerProjection.h"
#include "projections/map_projections/ossimMollweidProjection.h"
#include "projections/map_projections/ossimNewZealandMapGridProjection.h"
#include "projections/map_projections/ossimObliqueMercatorProjection.h"
#include "projections/map_projections/ossimOrthoGraphicProjection.h"
#include "projections/map_projections/ossimPolarStereoProjection.h"
#include "projections/map_projections/ossimPolyconicProjection.h"
#include "projections/map_projections/ossimSinusoidalProjection.h"
#include "projections/map_projections/ossimStereographicProjection.h"
#include "projections/map_projections/ossimTransCylEquAreaProjection.h"
#include "projections/map_projections/ossimTransMercatorProjection.h"
#include "projections/map_projections/ossimUpsProjection.h"
#include "projections/map_projections/ossimUtmProjection.h"
#include "projections/map_projections/ossimVanDerGrintenProjection.h"
#include "projections/map_projections/ossimSpaceObliqueMercatorProjection.h"
#include <support_data/ikonos/ossimSpaceImagingGeom.h>
//***
// ADD_PROJECTION: List names of all map projections produced by this factory:
//***

ossimMapProjectionFactory* ossimMapProjectionFactory::theInstance = 0;

//*****************************************************************************
//  STATIC METHOD: ossimMapProjectionFactory::instance()
//  
//*****************************************************************************
ossimMapProjectionFactory* ossimMapProjectionFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimMapProjectionFactory;
   }

   return (ossimMapProjectionFactory*) theInstance;
}

ossimProjection*
ossimMapProjectionFactory::createProjection(const ossimFilename& filename,
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

//*****************************************************************************
//  METHOD: ossimSensorModelFactory::create(kwl, prefix)
//  
//*****************************************************************************
ossimProjection* ossimMapProjectionFactory::createProjection(const ossimKeywordlist &keywordList,
															 const char *prefix) const
{
   ossimProjection *result=NULL;
	
   const char *lookup = keywordList.find(prefix, ossimKeywordNames::TYPE_KW);
   const char *lookupSpaceImaging = keywordList.find(prefix, ossimSpaceImagingGeom::SIG_PRODUCER_KW);
   if(lookup)
   {
      result = createProjection(ossimString(lookup).trim());
      if(result)
      {
         result->loadState(keywordList, prefix);
      }
   }
   else
   {
      if(lookupSpaceImaging)
      {
		  ossimKeywordlist kwl;
		  ossimKeywordlist kwl2;
		  kwl.add(keywordList,
				  prefix,
				  true);
         
         ossimSpaceImagingGeom spaceImaging;
         
         spaceImaging.setGeometry(kwl);

         spaceImaging.exportToOssim(kwl2);

         result = ossimProjectionFactoryRegistry::instance()->createProjection(kwl2);
         if(result)
         {
            return result;
         }
      }
      // look for the geom_file keyword.
      //
      lookup = keywordList.find(prefix, ossimKeywordNames::GEOM_FILE_KW);
      if(lookup)
      {
         ossimKeywordlist kwl;

         kwl.addFile(lookup);

         result = createProjection(kwl);
         
         if(!result)
         {
         // maybe they prepend projection. in the geom
         // file.
            result = createProjection(kwl, "projection.");
         }

      }      
   }
   return result;
}

//*****************************************************************************
//  METHOD: ossimSensorModelFactory::create(projection_name)
//  
//*****************************************************************************
ossimProjection* ossimMapProjectionFactory::createProjection(const ossimString &name) const
{
   //***
   // ADD_PROJECTION: Test for each map projection here:
   //***
   if(name ==  STATIC_TYPE_NAME(ossimAlbersProjection))
      return new ossimAlbersProjection;
   if(name ==  STATIC_TYPE_NAME(ossimAzimEquDistProjection))
      return new ossimAzimEquDistProjection;
   if( name == STATIC_TYPE_NAME(ossimBonneProjection))
      return new ossimBonneProjection;
   if( name == STATIC_TYPE_NAME(ossimBngProjection))
      return new ossimBngProjection;
   if(name ==  STATIC_TYPE_NAME(ossimCassiniProjection))
      return new ossimCassiniProjection;
   if(name ==  STATIC_TYPE_NAME(ossimCylEquAreaProjection))
      return new ossimCylEquAreaProjection;
   if(name ==  STATIC_TYPE_NAME(ossimEquDistCylProjection))
      return new ossimEquDistCylProjection;
   if(name ==  STATIC_TYPE_NAME(ossimEckert4Projection))
      return new ossimEckert4Projection;
   if(name ==  STATIC_TYPE_NAME(ossimEckert6Projection))
      return new ossimEckert6Projection;
   if(name == STATIC_TYPE_NAME(ossimGnomonicProjection))
      return new ossimGnomonicProjection;
   if(name ==  STATIC_TYPE_NAME(ossimLambertConformalConicProjection))
      return new ossimLambertConformalConicProjection;
   if(name ==  STATIC_TYPE_NAME(ossimLlxyProjection))
      return new ossimLlxyProjection;
   if(name == STATIC_TYPE_NAME(ossimMercatorProjection))
      return new ossimMercatorProjection;
   if(name == STATIC_TYPE_NAME(ossimMillerProjection))
      return new ossimMillerProjection;
   if(name == STATIC_TYPE_NAME(ossimMollweidProjection))
      return new ossimMollweidProjection;
   if(name == STATIC_TYPE_NAME(ossimNewZealandMapGridProjection))
      return new ossimNewZealandMapGridProjection;
   if(name == STATIC_TYPE_NAME(ossimObliqueMercatorProjection))
      return new ossimObliqueMercatorProjection;
   if(name == STATIC_TYPE_NAME(ossimOrthoGraphicProjection))
      return new ossimOrthoGraphicProjection;
   if(name == STATIC_TYPE_NAME(ossimPolarStereoProjection))
      return new ossimPolarStereoProjection;
   if(name == STATIC_TYPE_NAME(ossimPolyconicProjection))
      return new ossimPolyconicProjection;
   if(name == STATIC_TYPE_NAME(ossimSinusoidalProjection))
      return new ossimSinusoidalProjection;
   if(name == STATIC_TYPE_NAME(ossimStereographicProjection))
      return new ossimStereographicProjection;
   if(name == STATIC_TYPE_NAME(ossimTransCylEquAreaProjection))
      return new ossimTransCylEquAreaProjection;
   if(name == STATIC_TYPE_NAME(ossimTransMercatorProjection))
      return new ossimTransMercatorProjection;
   if(name == STATIC_TYPE_NAME(ossimUpsProjection))
      return new ossimUpsProjection;
   if(name == STATIC_TYPE_NAME(ossimUtmProjection))
      return new ossimUtmProjection;
   if(name == STATIC_TYPE_NAME(ossimVanDerGrintenProjection))
      return new ossimVanDerGrintenProjection;
   if(name == STATIC_TYPE_NAME(ossimSpaceObliqueMercatorProjection))
   {
      return new ossimSpaceObliqueMercatorProjection;
   }
   return NULL;
}

ossimObject* ossimMapProjectionFactory::createObject(const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimMapProjectionFactory::createObject(const ossimKeywordlist& kwl,
                                                     const char* prefix)const
{
   return createProjection(kwl, prefix);
}

void ossimMapProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimAlbersProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimAzimEquDistProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimBonneProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimBngProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimCassiniProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimCylEquAreaProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimEquDistCylProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimEckert4Projection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimEckert6Projection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimGnomonicProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimLambertConformalConicProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimLlxyProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimMercatorProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimMillerProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimMollweidProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimNewZealandMapGridProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimObliqueMercatorProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimOrthoGraphicProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimPolarStereoProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimPolyconicProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimSinusoidalProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimStereographicProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimTransCylEquAreaProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimTransMercatorProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimUpsProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimUtmProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimVanDerGrintenProjection)));
   typeList.push_back(ossimString(STATIC_TYPE_NAME(ossimSpaceObliqueMercatorProjection)));
}

list<ossimString> ossimMapProjectionFactory::getList()const
{
   list<ossimString> result;

   result.push_back(ossimString(STATIC_TYPE_NAME(ossimAlbersProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimAzimEquDistProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimBonneProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimBngProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimCassiniProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimCylEquAreaProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimEquDistCylProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimEckert4Projection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimEckert6Projection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimGnomonicProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimLambertConformalConicProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimLlxyProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimMercatorProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimMillerProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimMollweidProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimNewZealandMapGridProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimObliqueMercatorProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimOrthoGraphicProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimPolarStereoProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimPolyconicProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimSinusoidalProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimStereographicProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimTransCylEquAreaProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimTransMercatorProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimUpsProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimUtmProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimVanDerGrintenProjection)));
   result.push_back(ossimString(STATIC_TYPE_NAME(ossimSpaceObliqueMercatorProjection)));
   
   return result;
}
