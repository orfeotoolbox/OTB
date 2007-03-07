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
//  $Id: ossimERSTileSource.cpp,v 1.16 2005/05/09 12:39:38 dburken Exp $

#include <imaging/formats/ers/ossimERSTileSource.h>
#include <support_data/ers/ossimERS.h>

#include <base/data_types/ossimDirectory.h>
#include <base/common/ossimTrace.h>
#include <base/common/ossimKeywordNames.h>


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
            genRasterInfo.setNullPixelValue(i, theHdr->theNullCell);
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
               genRasterInfo.setMinPixelValue(i, theMetaData.getMinPix(i));
            }
            if(theMetaData.getMaxValuesValidFlag())
            {
               genRasterInfo.setMaxPixelValue(i, theMetaData.getMaxPix(i));
            }
            if(theMetaData.getNullValuesValidFlag())
            {
               genRasterInfo.setNullPixelValue(i, theMetaData.getNullPix(i));
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
   

bool ossimERSTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                          const char* prefix)
{
   if (theGeometryKwl.getSize())
   {
      kwl = theGeometryKwl;
      return true;
   }
   
   if(theHdr)
   {
      
      bool result = theHdr->toOssimProjectionGeom(kwl, prefix);
      if (result == true)
      {
         // Capture for next time.
         setImageGeometry(kwl);
      }
      return result;
   }

   return false;
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

