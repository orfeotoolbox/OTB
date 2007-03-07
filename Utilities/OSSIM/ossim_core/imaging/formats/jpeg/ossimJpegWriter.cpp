//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
//*******************************************************************
//  $Id: ossimJpegWriter.cpp,v 1.51 2006/01/06 16:46:46 dburken Exp $

#include <cstdio>

//---
// Using windows .NET compiler there is a conflict in the libjpeg with INT32
// in the file jmorecfg.h.  Defining XMD_H fixes this.
extern "C"
{
#if defined(_MSC_VER) || defined(__MINGW32__)
#  ifndef XMD_H
#    define XMD_H
#  endif
#endif
#include <jpeglib.h>
}

#include <imaging/formats/jpeg/ossimJpegWriter.h>
#include <base/common/ossimKeywordNames.h>
#include <base/common/ossimTrace.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/misc/lookup_tables/ossimScalarTypeLut.h>
#include <base/property/ossimNumericProperty.h>
#include <projections/map_projections/ossimMapProjection.h>
#include <imaging/ossimImageData.h>
#include <imaging/ossimImageSource.h>
#include <imaging/tile_sources/ossimScalarRemapper.h>

RTTI_DEF1_INST(ossimJpegWriter, "ossimJpegWriter", ossimImageFileWriter)

static const char DEFAULT_FILE_NAME[] = "output.jpg";
static const ossim_int32 DEFAULT_JPEG_QUALITY = 75;
static ossimTrace traceDebug("ossimJpegWriter:debug");


//*******************************************************************
// Constructor:
//*******************************************************************
ossimJpegWriter::ossimJpegWriter()
   : ossimImageFileWriter(),
     theQuality(DEFAULT_JPEG_QUALITY),
     theRlevel(0),
     theOutputFilePtr(NULL)
{
   // Since there is no internal geometry set the flag to write out one.
   setWriteExternalGeometryFlag(true);
}

ossimJpegWriter::ossimJpegWriter(ossimImageSource* inputSource,
                                 const ossimFilename& filename)
   :ossimImageFileWriter(filename, inputSource),
    theQuality(DEFAULT_JPEG_QUALITY),
    theRlevel(0),
    theOutputFilePtr(NULL)
{
   // Since there is no internal geometry set the flag to write out one.
   setWriteExternalGeometryFlag(true);
}
//*******************************************************************
// Destructor
//*******************************************************************
ossimJpegWriter::~ossimJpegWriter()
{
   close();
}

bool ossimJpegWriter::writeFile()
{
   static const char MODULE[] = "ossimJpegWriter::writeFile";

   if( getErrorStatus() != ossimErrorCodes::OSSIM_OK )
   {
      return false;
   }

   bool needToDeleteInput = false;

   if(theInputConnection->getOutputScalarType() != OSSIM_UINT8)
   {
      ossimImageSource* inputSource=new ossimScalarRemapper;

      inputSource->connectMyInputTo(0, theInputConnection->getInput(0));
      theInputConnection->connectMyInputTo(0, inputSource);
      theInputConnection->initialize();
      needToDeleteInput = true;
   }

   if(theInputConnection->isMaster())
   {
      if (!isOpen())
      {
         open();
      }
      
      if (!theOutputFilePtr)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " ERROR:"
            << "\nCannot open:  " << theFilename.c_str()
            << "\nReturning from method." << endl;
         
         return false;
      }
      
      if (traceDebug())
      {
         CLOG << " DEBUG:"
              << "\nOutput Rect:  " << theAreaOfInterest << endl;
      }
      
      // Get the number of bands.  Must be one or three for this writer.
      ossim_int32 components   = theInputConnection->getNumberOfOutputBands();
      
      if (components != 1 && components != 3)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " Range Error:"
            << "\nInvalid number of input bands!  Must be one or three."
            << "\nInput bands = " << components
            << "\nReturning from method." << endl;
         
         return false;
      }
      
      //
      // There should be a check to see if "theRlevel" is out of range.
      //
      // if (theRlevel < theImageSource
      
      ossim_int32 image_height
         = theAreaOfInterest.lr().y - theAreaOfInterest.ul().y + 1;
      ossim_int32 image_width
         = theAreaOfInterest.lr().x - theAreaOfInterest.ul().x + 1;
      ossim_int32 tileWidth    = theInputConnection->getTileWidth();
      ossim_int32 tileHeight   = theInputConnection->getTileHeight();
      
      // Allocate a buffer to hold a row of tiles.
      ossim_int32 buf_size = tileHeight*image_width*components;
      
      if (traceDebug())
      {
         CLOG << "DEBUG:"
              << "\nbuf_size:  " << buf_size
              << endl;
      }
      
      ossim_uint8* buf = new ossim_uint8[buf_size];
      ossimImageData* blankTile = new ossimImageData(NULL,
                                                     OSSIM_UINT8,
                                                     components,
                                                     tileWidth,
                                                     tileHeight);
      blankTile->initialize();
      //
      // Stuff needed for the jpeg library.
      // Taken from "example.c" file of the "jpeg-6b" package.
      //
      struct jpeg_compress_struct cinfo;
      struct jpeg_error_mgr jerr;
      JSAMPROW row_pointer[1];  // pointer to JSAMPLE row[s]
      
      // physical row width in image buffer
      int row_stride = image_width * components;
      
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_compress(&cinfo);
      jpeg_stdio_dest(&cinfo, theOutputFilePtr);
      cinfo.image_width = image_width;   // image width and height, in pixels
      cinfo.image_height = image_height;
      cinfo.input_components = components;	// # of color components per pixel
      
      // colorspace of input image 
      if (components == 3)
      {
         cinfo.in_color_space = JCS_RGB;
      }
      else
      {
         cinfo.in_color_space = JCS_GRAYSCALE;
      }
      
      jpeg_set_defaults(&cinfo);
      jpeg_set_quality(&cinfo, theQuality, TRUE); //limit to baseline-JPEG values 
      jpeg_start_compress(&cinfo, TRUE);
      
      theInputConnection->setAreaOfInterest(theAreaOfInterest);
      theInputConnection->setToStartOfSequence();
      long maxY = (long)theInputConnection->getNumberOfTilesVertical();
      long maxX = (long)theInputConnection->getNumberOfTilesHorizontal();
      
      ossimProcessInterface::ossimProcessStatus processStatus = getProcessStatus();
      
      double tileCount = 0.0;
      double totalTiles = theInputConnection->getNumberOfTiles();
      //
      // Ok the jpeg stuff should be set.  Loop through and grab a row of tiles
      // and copy to the buffer.  Then write the buffer to the jpeg file.
      // Get the next row... until finished.
      //
      for (ossim_int32 i=0;
           ((i<maxY)&&
            (!needsAborting()));
           ++i)
      {
         ossimIrect buf_rect = theAreaOfInterest;
         buf_rect.set_uly(theAreaOfInterest.ul().y+i*tileHeight);
         buf_rect.set_lry(buf_rect.ul().y + tileHeight - 1);
         
         for (ossim_int32 j=0;
              ((j<maxX)&&
               (!needsAborting()));
              ++j)
         {
            // Grab the tile.
            const ossimRefPtr<ossimImageData> t =
               theInputConnection->getNextTile();

            if ( t.valid() )
            {
               if (t->getDataObjectStatus() != OSSIM_NULL)
               {
                  // Copy the tile to the buffer.
                  t->unloadTile(buf, buf_rect, OSSIM_BIP);
               }
               else
               {
                  blankTile->setOrigin(t->getOrigin());
                  blankTile->unloadTile(buf, buf_rect, OSSIM_BIP);
               }
            }
            ++tileCount;
            setPercentComplete((100.0*(tileCount/totalTiles)));
            
         }  // End of loop through tiles in the x direction.
         
         // Copy the buffer to the jpeg file.
         ossim_int32 lines_to_copy =
            min( (buf_rect.lr().y-buf_rect.ul().y+1),
                 (theAreaOfInterest.lr().y-buf_rect.ul().y+1) );
         
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE
               << "buf_rect:       " << buf_rect
               << "lines_to_copy:  " << lines_to_copy << endl;
         }
         
         processStatus = getProcessStatus();
         if(!needsAborting())
         {
            // Now copy the buffer that should be full to the jpeg file.
            ossim_int32 buf_offset = 0;
            for (ossim_int32 line=0; line<lines_to_copy; ++line)
            {
               row_pointer[0] = &buf[buf_offset];
               jpeg_write_scanlines(&cinfo, row_pointer, 1);
               buf_offset += row_stride;
            }
         }
         
      } // End of loop through tiles in the y direction.
      
      // Free memory.
      if(!needsAborting())
      {
	jpeg_finish_compress(&cinfo);
      }
      
      fclose(theOutputFilePtr);
      theOutputFilePtr = NULL;

      jpeg_destroy_compress(&cinfo);
      
      delete [] buf;
      delete blankTile;
      blankTile = NULL;
   }
   else
   {
      theInputConnection->slaveProcessTiles();
   }
   if(needToDeleteInput)
   {
      ossimConnectableObject* obj = theInputConnection->getInput(0);
      if(obj)
      {
         theInputConnection->connectMyInputTo(0, obj->getInput(0));
         delete obj;
         obj = NULL;
      }
   }
   
   return true;
}

bool ossimJpegWriter::saveState(ossimKeywordlist& kwl,
                                const char* prefix)const
{
   return ossimImageFileWriter::saveState(kwl, prefix);
}

bool ossimJpegWriter::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   const char* value;
   
   value = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(value)
   {
      setFilename(value);
   }
   
   value = kwl.find(prefix, ossimKeywordNames::COMPRESSION_QUALITY_KW);
   if(value)
   {
      setQuality(atoi(value));
   }
   
   value = kwl.find(prefix, ossimKeywordNames::OVERVIEW_FILE_KW);
   if(value)
   {
      theOverviewFlag = (atoi(value)) ? true : false;
   }
   
   value = kwl.find(prefix, ossimKeywordNames::REDUCED_RES_LEVEL_KW);
   if(value)
   {
      theRlevel = atoi(value);
   }
   theOutputImageType = "jpeg";
   
   return ossimImageFileWriter::loadState(kwl, prefix);
}

void ossimJpegWriter::setQuality(ossim_int32 quality)
{
   static const char MODULE[] = "ossimJpegWriter::setQuality";
   
   // Range 1 to 100 with 100 being best.
   if (quality > 0 && quality < 101)
   {
      theQuality = quality;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nquality out of range:  " << quality
            << "\nquality has been set to default:  " 
            << DEFAULT_JPEG_QUALITY
            << "\nvalid range:  1 to 100 with 100 being best."
            << endl;
      }
      
      theQuality = DEFAULT_JPEG_QUALITY;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG:"
         << "\nQuality set to:  "
         << theQuality << endl;
   }
}

void ossimJpegWriter::set_rr_level(ossim_uint32 rr)
{
   theRlevel = rr;
}

bool ossimJpegWriter::isOpen() const
{
   return (theOutputFilePtr!=NULL);
}

bool ossimJpegWriter::open()
{
   close();
   theOutputFilePtr = fopen(theFilename.c_str(), "wb");
   if(theOutputFilePtr)
   {
      return true;
   }
   return false;
}

void ossimJpegWriter::close()
{
   if(theOutputFilePtr)
   {
      fclose(theOutputFilePtr);
      theOutputFilePtr = NULL;
   }
}

void ossimJpegWriter::getImageTypeList(std::vector<ossimString>& imageTypeList)const
{
   imageTypeList.push_back(ossimString("jpeg"));
}

bool ossimJpegWriter::hasImageType(const ossimString& imageType) const
{
   if((imageType == "image/jpeg")||
      (imageType == "image/jpg"))
   {
      return true;
   }

   return ossimImageFileWriter::hasImageType(imageType);
}

void ossimJpegWriter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if (!property)
   {
      return;
   }
   
   if(property->getName() == ossimKeywordNames::COMPRESSION_QUALITY_KW)
   {
      ossimNumericProperty* numericProperty = PTR_CAST(ossimNumericProperty,
                                                       property.get());
      if (numericProperty)
      {
         setQuality( numericProperty->asInt32() );
      }
   }
   else
   {
      ossimImageFileWriter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimJpegWriter::getProperty(const ossimString& name)const
{
   if (name == ossimKeywordNames::COMPRESSION_QUALITY_KW)
   {
      ossimNumericProperty* prop =
         new ossimNumericProperty(name,
                                  ossimString::toString(theQuality),
                                  1.0,
                                  100.0);
      prop->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT);
      return prop;
   }

   return ossimImageFileWriter::getProperty(name);
}

void ossimJpegWriter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimString name = ossimKeywordNames::COMPRESSION_QUALITY_KW;
   propertyNames.push_back(name);
   
   ossimImageFileWriter::getPropertyNames(propertyNames);
}
