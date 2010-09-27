//*****************************************************************************
// FILE: ossimMapProjectionFactory.cc
//
// License:  See top level LICENSE.txt file.
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
//  $Id: ossimMapProjectionFactory.cpp 17815 2010-08-03 13:23:14Z dburken $

#include <ossim/projection/ossimMapProjectionFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimFilename.h>

//***
// Note to programmer: To add a new projection, search this file for
// "ADD_PROJECTION" to locate places requiring editing. Functional example
// below...
//
// ADD_PROJECTION: Include all map projection headers here:
//***

#include <ossim/projection/ossimAlbersProjection.h>
#include <ossim/projection/ossimAzimEquDistProjection.h>
#include <ossim/projection/ossimBonneProjection.h>
#include <ossim/projection/ossimBngProjection.h>
#include <ossim/projection/ossimCassiniProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimEckert4Projection.h>
#include <ossim/projection/ossimEckert6Projection.h>
#include <ossim/projection/ossimGnomonicProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimLlxyProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimMillerProjection.h>
#include <ossim/projection/ossimMollweidProjection.h>
#include <ossim/projection/ossimNewZealandMapGridProjection.h>
#include <ossim/projection/ossimObliqueMercatorProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/projection/ossimPolarStereoProjection.h>
#include <ossim/projection/ossimPolyconicProjection.h>
#include <ossim/projection/ossimSinusoidalProjection.h>
#include <ossim/projection/ossimStereographicProjection.h>
#include <ossim/projection/ossimTransCylEquAreaProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimUpsProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimVanDerGrintenProjection.h>
#include <ossim/projection/ossimSpaceObliqueMercatorProjection.h>
#include <ossim/support_data/ossimSpaceImagingGeom.h>
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
//  METHOD: ossimMapProjectionFactory::create(kwl, prefix)
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
//  METHOD: ossimMapProjectionFactory::create(projection_name)
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
      return new ossimSpaceObliqueMercatorProjection;
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
