//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Jordi Inglada
//
// Description:
//
// Contains class implementaiton for the class "ossim TileMapTileSource".
//
//*******************************************************************
//  $Id: ossimTileMapTileSource.cpp 10752 2007-04-23 16:50:08Z dburken $

#include <ossim/imaging/ossimTileMapTileSource.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/support_data/ossimFfL7.h>
#include <ossim/support_data/ossimFfL5.h>
#include <ossim/projection/ossimTileMapModel.h>

RTTI_DEF1_INST(ossimTileMapTileSource,
               "ossimTileMapTileSource",
               ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimTileMapTileSource:debug");


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTileMapTileSource::ossimTileMapTileSource()
   :
      ossimGeneralRasterTileSource(),
      theFfHdr(NULL)
{
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimTileMapTileSource::ossimTileMapTileSource(const ossimKeywordlist& kwl,
                                               const char* prefix)
   :
      ossimGeneralRasterTileSource(),
      theFfHdr(NULL)
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimTileMapTileSource::~ossimTileMapTileSource()
{
   theFfHdr = NULL;
}

bool ossimTileMapTileSource::open()
{
   static const char MODULE[] = "ossimTileMapTileSource::open";

   if (traceDebug())
   {
      CLOG << " Entered..." << std::endl
           << " trying to open file " << theImageFile << std::endl;
   }
   if(theImageFile.ext() == "otb")
     {
     return true;
     }
   return false;
}

void ossimTileMapTileSource::openHeader(const ossimFilename& file)
{
   //***
   // TileMap file name example:  l71024031_03119990929_hpn.fst
   // Three header header file type substrings:
   // HPN = Pan
   // HRF = VNIR/SWIR (visible near infrared/shortwave infrared)
   // HTM = Thermal
   //***

   ossimFilename hdr = file.file();
   hdr.downcase();
   if ( hdr.contains("hpn") || hdr.contains("hrf") || hdr.contains("htm") )
   {
      theFfHdr = new ossimFfL7(file.c_str());
   } else if (hdr.contains("header.dat"))
   {
      theFfHdr = new ossimFfL5(file.c_str());
   } else {
      theFfHdr = NULL;
      return;
   }
   if (theFfHdr->getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      theFfHdr = 0;
   }
   return;

}

ossimImageGeometry* ossimTileMapTileSource::getImageGeometry()
{

   ossimImageGeometry* result = ossimImageHandler::getImageGeometry();
   if (result->getProjection())
      return theGeometry.get();

   if (!theFfHdr) return result;

   // Make a model
   ossimTileMapModel* model = new ossimTileMapModel (*theFfHdr);

   if (model->getErrorStatus() != ossimErrorCodes::OSSIM_OK)
      return false;

   //initialize the image geometry object with the model
   result->setProjection(model);
   return result;
}

bool ossimTileMapTileSource::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   const char* lookup = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   return true;

}

ossimRefPtr<ossimProperty> ossimTileMapTileSource::getProperty(
   const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;

   if (theFfHdr.valid())
   {
      result = theFfHdr->getProperty(name);
   }

   if ( result.valid() == false )
   {
      result = ossimGeneralRasterTileSource::getProperty(name);
   }

   return result;
}

void ossimTileMapTileSource::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   if (theFfHdr.valid())
   {
      theFfHdr->getPropertyNames(propertyNames);
   }
   ossimGeneralRasterTileSource::getPropertyNames(propertyNames);
}

ossimString ossimTileMapTileSource::getShortName() const
{
   return ossimString("TileMap");
}

ossimString ossimTileMapTileSource::getLongName() const
{
   return ossimString("TileMap reader");
}

ossimString  ossimTileMapTileSource::className() const
{
   return ossimString("ossimTileMapTileSource");
}

double ossimTileMapTileSource::getNullPixelValue(ossim_uint32)const
{
   return 0.0;
}

double ossimTileMapTileSource::getMinPixelValue(ossim_uint32)const
{
   return 1.0;
}

double ossimTileMapTileSource::getMaxPixelValue(ossim_uint32)const
{
   return 255.0;
}

ossimScalarType ossimTileMapTileSource::getOutputScalarType() const
{
   return OSSIM_UINT8;
}

bool ossimTileMapTileSource::getAcquisitionDate(ossimDate& date)const
{
   if(!theFfHdr) return false;

   theFfHdr->getAcquisitionDate(date);

   return true;
}

ossimString ossimTileMapTileSource::getSatelliteName()const
{
   if(!theFfHdr) return "";

   return theFfHdr->getSatelliteName();
}

ossimFilename ossimTileMapTileSource::getBandFilename(ossim_uint32 idx)const
{
   ossim_uint32 maxIdx = getNumberOfInputBands();

   if(!theFfHdr||(idx > maxIdx))
   {
      return "";
   }

   ossimFilename path = getFilename().path();
   ossimString filename = theFfHdr->getBandFilename(idx);
   filename = filename.trim();
   ossimFilename file = path.dirCat(filename);

   if (file.exists())
   {
      return file;
   }

   // Try downcased name.
   file = path.dirCat(filename.downcase());
   if (file.exists())
   {
      return file;
   }

   // Try upcase name.
   file = path.dirCat(filename.upcase());
   if (file.exists())
   {
      return file;
   }

   return ossimFilename();
}


