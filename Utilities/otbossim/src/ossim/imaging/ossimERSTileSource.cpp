//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Chong-Ket Chuah
//
// Description:
//
// Implementation for the class "ossimERSTileSource".  ossimERSTileSource
// is used for reading ER Mapper raster file format.  The format consists
// of a raster file with no extension and a header file with the same name 
// as the raster file but with an .ers extension.
//
//*******************************************************************
//  $Id: ossimERSTileSource.cpp 21512 2012-08-22 11:53:57Z dburken $

#include <ossim/imaging/ossimERSTileSource.h>
#include <ossim/support_data/ossimERS.h>

#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>

RTTI_DEF1(ossimERSTileSource, "ossimERSTileSource", ossimGeneralRasterTileSource);

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimERSTileSource::ossimERSTileSource()
   :  ossimGeneralRasterTileSource(),
      theHdr(NULL)
{
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimERSTileSource::ossimERSTileSource(const ossimKeywordlist& kwl,
                                               const char* prefix)
   :  ossimGeneralRasterTileSource(),
      theHdr(NULL)
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimERSTileSource::~ossimERSTileSource()
{
   if (theHdr)
   {
      delete theHdr;
      theHdr = NULL;
   }
}

bool ossimERSTileSource::open(const ossimFilename& fileName)
{
   // Open and parse the header file
   theHdr = new ossimERS(fileName);
   
//   theHdr->dump(cout);
   
   if(theHdr->errorStatus() == ossimErrorCodes::OSSIM_ERROR)
   {
      delete theHdr;
      theHdr = NULL;
      return false;
   }
   else
   {
      vector<ossimFilename> f;
	  ossimFilename fne;
	  ossimFilename fpath = fileName.path();
	  if (fpath.empty())
		fne = fileName.fileNoExtension();
	  else
		fne = fpath+"/"+fileName.fileNoExtension();
      f.push_back(fne);
      ossimGeneralRasterInfo genRasterInfo(f,
                                           theHdr->theCelltype,
                                           OSSIM_BIL,
                                           theHdr->theBands,
                                           theHdr->theLine,
                                           theHdr->theSample,
                                           0,
                                           ossimGeneralRasterInfo::NONE,
                                           0);
      if(theHdr->theHasNullCells)
      {
         ossim_uint32 i = 0;
         ossim_uint32 bands = static_cast<ossim_uint32>(theHdr->theBands);
         for(i = 0; i < bands; ++i)
         {
            genRasterInfo.getImageMetaData().setNullPix(i, theHdr->theNullCell);
         }
      }
      ossimFilename metadataFile = fne;
      metadataFile = metadataFile + ".omd";

      if(metadataFile.exists())
      {
         ossimKeywordlist kwl;
      
         kwl.addFile(metadataFile.c_str());
      
         theMetaData.loadState(kwl);
         ossim_uint32 i = 0;
         ossim_uint32 bands = static_cast<ossim_uint32>(theHdr->theBands);
         for(i = 0; i < bands; ++i)
         {
            if(theMetaData.getMinValuesValidFlag())
            {
               genRasterInfo.getImageMetaData().setMinPix(i, theMetaData.getMinPix(i));
            }
            if(theMetaData.getMaxValuesValidFlag())
            {
               genRasterInfo.getImageMetaData().setMaxPix(i, theMetaData.getMaxPix(i));
            }
            if(theMetaData.getNullValuesValidFlag())
            {
               genRasterInfo.getImageMetaData().setNullPix(i, theMetaData.getNullPix(i));
            }
         }
      }
      ossimNotify(ossimNotifyLevel_INFO)
         << "general raster info is\n";
      genRasterInfo.print(ossimNotify(ossimNotifyLevel_INFO));
      ossimGeneralRasterTileSource::open(genRasterInfo);
   }
	   
   return true;
}
   

ossimRefPtr<ossimImageGeometry> ossimERSTileSource::getImageGeometry()
{
   if ( !theGeometry )
   {
      // Check for external geom:
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         
         theGeometry = new ossimImageGeometry;

         if(theHdr)
         {
            ossimKeywordlist kwl;
            if ( theHdr->toOssimProjectionGeom(kwl) )
            {
               theGeometry->loadState(kwl);  
            }
         }

         // At this point it is assured theGeometry is set.
         
         //---
         // WARNING:
         // Must create/set the geometry at this point or the next call to
         // ossimImageGeometryRegistry::extendGeometry will put us in an infinite loop
         // as it does a recursive call back to ossimImageHandler::getImageGeometry().
         //---         
         
         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }
   return theGeometry;
}

bool ossimERSTileSource::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);

   if (lookup)
   {
      ossimFilename fileName = lookup;

      bool result =  open(fileName);
      ossimImageSource::loadState(kwl, prefix);
      return result;
   }
      
   return ossimGeneralRasterTileSource::loadState(kwl, prefix);
}

ossimString ossimERSTileSource::getShortName() const
{
   return ossimString("ERS");
}

ossimString ossimERSTileSource::getLongName() const
{
   return ossimString("ER Mapper Raster");
}

ossimString  ossimERSTileSource::className() const
{
   return ossimString("ossimERSTileSource");
}

