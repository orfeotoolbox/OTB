//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
//*******************************************************************
//  $Id: ossimGeneralRasterWriter.cpp 10059 2006-12-11 13:30:13Z dburken $

#include <fstream>

#include <ossim/imaging/ossimGeneralRasterWriter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimEnviHeaderFileWriter.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimEndian.h>


static ossimTrace traceDebug("ossimGeneralRasterWriter:debug");

RTTI_DEF1(ossimGeneralRasterWriter,
          "ossimGeneralRasterWriter",
          ossimImageFileWriter)

static const char DEFAULT_FILE_NAME[] = "output.ras";

ossimGeneralRasterWriter::ossimGeneralRasterWriter()
   :
      ossimImageFileWriter(),
      theRlevel(0)
{
  setOutputImageType(OSSIM_GENERAL_RASTER_BSQ);

  // Since there is no internal geometry set the flag to write out one.
  setWriteExternalGeometryFlag(true);
  theOutputByteOrder = ossimEndian().getSystemEndianType();
}

ossimGeneralRasterWriter::ossimGeneralRasterWriter(
   ossimImageSource* inputSource, const ossimFilename& file)
   :
      ossimImageFileWriter(file,
                         inputSource,
                         NULL),
      theRlevel(0)
{
  setOutputImageType(OSSIM_GENERAL_RASTER_BSQ);

  // Since there is no internal geometry set the flag to write out one.
  setWriteExternalGeometryFlag(true);
  theOutputByteOrder = ossimEndian().getSystemEndianType();
}

ossimGeneralRasterWriter::~ossimGeneralRasterWriter()
{
   if(isOpen())
   {
     close();
   }
}

bool ossimGeneralRasterWriter::isOpen()const
{
   // fstream::is_open not const; hence, the messy code...
   return const_cast<std::ofstream*>(&theFileStream)->is_open();
}

bool ossimGeneralRasterWriter::open()
{
   if(isOpen())
   {
      close();
   }

   theFileStream.open(theFilename.c_str(), ios::out | ios::binary);
   
   if(theFileStream.is_open())
   {
      return true;
   }

   return false;
}

void ossimGeneralRasterWriter::close()
{
   if (theFileStream.is_open())
   {
      theFileStream.close();
   }
   theFileStream.clear();
}

bool ossimGeneralRasterWriter::writeFile()
{
   static const char MODULE[] = "ossimGeneralRasterWriter::writeFile";
   
   if( getErrorStatus() != ossimErrorCodes::OSSIM_OK )
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " ERROR:"
         << "\nError status previously set!" << std::endl;
      return false;
   }
   
   if(!theInputConnection->isMaster())
   {
      theInputConnection->slaveProcessTiles();
      return true;
   }
   
   open();
   
   if(theFileStream.fail())
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " ERROR:"
         << "\nOutput file not open:  " << theFilename.c_str()
         << "\nReturning from method." << std::endl;
      setErrorStatus();
      return false;
   }

   bool result = true;
   // Write the file with the image data.
   if ( (theOutputImageType == "general_raster_bip") ||
        (theOutputImageType == "general_raster_bip_envi") )
   {
      result = writeToBip();
   }
   else if ( (theOutputImageType == "general_raster_bil") ||
             (theOutputImageType == "general_raster_bil_envi") )
   {
      result = writeToBil();
   }
   else if ( (theOutputImageType == "general_raster_bsq") ||
             (theOutputImageType == "general_raster_bsq_envi") )
   {
      result = writeToBsq();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << MODULE << " ERROR:"
         << "\nUnsupported output type:  " << theOutputImageType << std::endl;
      result = false;
   }
   
   if (result)
   {
      // Flush the stream to disk...
      theFileStream.flush();

      // Do this only on the master process. Note left to right precedence!
      if (getSequencer() && getSequencer()->isMaster())
      {
         //---
         // Write the header out.  We do this last since we must
         // compute min max pixel while we are writting the image.
         // since the header is an external text file this is Ok
         // to do.
         //---
         writeHeader();

         if (theOutputImageType.contains("envi"))
         {
            writeEnviHeader();
         }
      }
   }
   
   if (traceDebug()) CLOG << " Exited..." << std::endl;

   close();
   
   return result;
}


bool ossimGeneralRasterWriter::writeToBip()
{
   ossimEndian endian;
   static const char* const MODULE = "ossimGeneralRasterWriter::writeToBip";
   
   if (traceDebug()) CLOG << " Entered." << std::endl;
   
   
   //---
   // Get an arbitrary tile just to get the size in bytes!
   // This should be changed later... An ossimImageSourceInterface should know
   // this.
   //---
   ossimRefPtr<ossimImageData> id;
   
   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();
   
   ossim_uint64 bands     = theInputConnection->getNumberOfOutputBands();
   ossim_uint64 tilesWide =  theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint64 tilesHigh     = theInputConnection->getNumberOfTilesVertical();
   ossim_uint64 tileHeight    = theInputConnection->getTileHeight();
   ossim_uint64 numberOfTiles = theInputConnection->getNumberOfTiles();
   ossim_uint64 width         = theAreaOfInterest.width();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "\nossimGeneralRasterWriter::writeToBip DEBUG:"
         << "\nbands:          " << bands
         << "\ntilesWide:      " << tilesWide
         << "\ntilesHigh:      " << tilesHigh
         << "\ntileHeight:     " << tileHeight
         << "\nnumberOfTiles:  " << numberOfTiles
         << "\nwidth:          " << width
         << std::endl;
   }
   
   //---
   // Buffer to hold one line x tileHeight
   //---
   ossim_uint64 bufferSizeInBytes = 0;
   ossim_uint64 bytesInLine       = 0;
   unsigned char* buffer = NULL;
   
   theMinPerBand.clear();
   theMaxPerBand.clear();
   ossim_uint64 tileNumber = 0;
   bool wroteSomethingOut = false;
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   for(ossim_uint64 i = 0; ((i < tilesHigh)&&(!needsAborting())); ++i)
   {
      // Clear the buffer.
      if(buffer)
      {
	 memset(buffer, 0, bufferSizeInBytes);
      }
      
      ossimIrect bufferRect(theAreaOfInterest.ul().x,
			    theAreaOfInterest.ul().y + i*tileHeight,
			    theAreaOfInterest.ul().x + (width - 1),
			    theAreaOfInterest.ul().y + i*tileHeight + (tileHeight - 1));
      // Tile loop in the sample (width) direction.
      for(ossim_uint64 j = 0; ((j < tilesWide)&&(!needsAborting())); ++j)
      {
         // Get the tile and copy it to the buffer.
         id = theInputConnection->getNextTile();
	 if(id.valid())
         {
            id->computeMinMaxPix(theMinPerBand, theMaxPerBand);
            if(!buffer)
            {
               bytesInLine     = id->getScalarSizeInBytes() * width * bands;
               
               //---
               // Buffer to hold one line x tileHeight
               //---
               bufferSizeInBytes = bytesInLine * tileHeight;
               buffer = new unsigned char[bufferSizeInBytes];
               memset(buffer, 0, bufferSizeInBytes);
            }
            id->unloadTile(buffer,
                           bufferRect,
                           OSSIM_BIP);
         }
         ++tileNumber;
      }
      
      // Get the number of lines to write from the buffer.
      ossim_uint64 linesToWrite =
         min(tileHeight,
             static_cast<ossim_uint64>(theAreaOfInterest.lr().y -
                                       bufferRect.ul().y + 1));
      // Write the buffer out to disk.  
      ossim_uint8* buf = buffer;
      if(buf)
      {
         for (ossim_uint64 ii=0; ((ii<linesToWrite)&&(!needsAborting())); ++ii)
         {
            std::streamsize lineBytes = bytesInLine;
            wroteSomethingOut = true;

            if(endian.getSystemEndianType() != theOutputByteOrder)
            {
               endian.swap(scalarType,
                           buf,
                           lineBytes/ossimGetScalarSizeInBytes(scalarType));
            }
            theFileStream.write((char*)buf, lineBytes);
            if (theFileStream.fail())
            {
               ossimNotify(ossimNotifyLevel_FATAL)
                  << MODULE << " ERROR:"
                  << "Error returned writing line!" << std::endl;
               setErrorStatus();
               if(buffer)
               {
                  // Free the memory.
                  delete [] buffer;
               }
               return false;
            }
	    
            buf += bytesInLine;
	    
         } // End of loop to write lines from buffer to tiff file.
      }
      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);
      if(needsAborting())
      {
         setPercentComplete(100.0);
      }
      
   } // End of loop in the line (height) direction.
   if(buffer)
   {
      // Free the memory.
      delete [] buffer;
   }
   
   if (traceDebug()) CLOG << " Exited." << std::endl;
   
   return wroteSomethingOut;
}

bool ossimGeneralRasterWriter::writeToBil()
{
   ossimEndian endian;
   static const char* const MODULE = "ossimGeneralRasterWriter::writeToBil";

   if (traceDebug()) CLOG << " Entered." << std::endl;

   //***
   // Get an arbitrary tile just to get the size in bytes!
   // This should be changed later... An ossimImageSourceInterface should know
   // this.
   //***
   ossimRefPtr<ossimImageData> id;
   theInputConnection->setToStartOfSequence();

   ossim_uint64 bands     = theInputConnection->getNumberOfOutputBands();
   ossim_uint64 tilesWide = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint64 tilesHigh = theInputConnection->getNumberOfTilesVertical();
   ossim_uint64 tileHeight        = theInputConnection->getTileHeight();
   ossim_uint64 numberOfTiles     = theInputConnection->getNumberOfTiles();
   ossim_uint64 width             = theAreaOfInterest.width();
   ossim_uint64 bufferSizeInBytes = 0;
   ossim_uint64 bytesInLine       = 0;
   unsigned char* buffer    = NULL;

   // Start with a clean min/max.
   theMinPerBand.clear();
   theMaxPerBand.clear();

   ossim_uint64 tileNumber = 0;
   bool wroteSomethingOut = false;
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   for(ossim_uint64 i = 0; ((i < tilesHigh)&&(!needsAborting())); ++i)
   {
      // Clear the buffer.
      // Clear the buffer.
      if(buffer)
      {
         memset(buffer, 0, bufferSizeInBytes);
      }
      
      ossimIrect bufferRect(theAreaOfInterest.ul().x,
                            theAreaOfInterest.ul().y + i*tileHeight,
                            theAreaOfInterest.ul().x + (width - 1),
                            theAreaOfInterest.ul().y + i *
                            tileHeight + (tileHeight - 1));
      
      // Tile loop in the sample (width) direction.
      for(ossim_uint64 j = 0; ((j < tilesWide)&&(!needsAborting())); ++j)
      {
         // Get the tile and copy it to the buffer.
         id = theInputConnection->getNextTile();
	 if(id.valid())
         {
            id->computeMinMaxPix(theMinPerBand, theMaxPerBand);
            
            if(!buffer)
            {
               bytesInLine     = id->getScalarSizeInBytes() * width;
               
               // Buffer to hold one line x tileHeight
               bufferSizeInBytes = bytesInLine * tileHeight * bands;
               buffer = new unsigned char[bufferSizeInBytes];
               memset(buffer, 0, bufferSizeInBytes);
            }
            id->unloadTile(buffer,
                           bufferRect,
                           OSSIM_BIL);
         }
         ++tileNumber;
      }
      
      // Get the number of lines to write from the buffer.
      ossim_uint64 linesToWrite =
         min(tileHeight,
             static_cast<ossim_uint64>(theAreaOfInterest.lr().y -
                                       bufferRect.ul().y + 1));
      
      // Write the buffer out to disk.  
      ossim_uint8* buf = buffer;
      for (ossim_uint64 ii=0; ((ii<linesToWrite)&(!needsAborting())); ++ii)
      {
         for (ossim_uint64 band = 0;
              ((band < bands)&&(!needsAborting()));
              ++band)
         {
            wroteSomethingOut = true;
            if(endian.getSystemEndianType() != theOutputByteOrder)
            {
               endian.swap(scalarType,
                           buf,
                           bytesInLine/ossimGetScalarSizeInBytes(scalarType));
            }
            theFileStream.write((char*)buf, bytesInLine);
            if (theFileStream.fail())
            {
               ossimNotify(ossimNotifyLevel_FATAL)
                  << MODULE << " ERROR:"
                  << "Error returned writing line!" << std::endl;
               setErrorStatus();
	       if(buffer)
               {
                  // Free the memory.
                  delete [] buffer;
               }
               return false;
            }

            buf += bytesInLine;
         }
         
      } // End of loop to write lines from buffer to tiff file.

      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);

      if(needsAborting())
      {
         setPercentComplete(100.0);
      }

   } // End of loop in the line (height) direction.

   if(buffer)
   {
      // Free the memory.
      delete [] buffer;
   }
   
   if (traceDebug()) CLOG << " Exited." << std::endl;

   return wroteSomethingOut;
}

bool ossimGeneralRasterWriter::writeToBsq()
{
   ossimEndian endian;
   static const char* const MODULE = "ossimGeneralRasterWriter::writeToBsq";

   if (traceDebug()) CLOG << " Entered." << std::endl;

   //***
   // Get an arbitrary tile just to get the size in bytes!
   // This should be changed later... An ossimImageSourceInterface should know
   // this.
   //***
   ossimRefPtr<ossimImageData> id;

   // Start the sequence at the first tile.
   theInputConnection->setToStartOfSequence();

   ossim_uint64 bands     = theInputConnection->getNumberOfOutputBands();
   ossim_uint64 tilesWide = theInputConnection->getNumberOfTilesHorizontal();
   ossim_uint64 tilesHigh = theInputConnection->getNumberOfTilesVertical();
   ossim_uint64 tileHeight      = theInputConnection->getTileHeight();
   ossim_uint64 numberOfTiles   = theInputConnection->getNumberOfTiles();
   ossim_uint64 width           = theAreaOfInterest.width();
   ossim_uint64 height          = theAreaOfInterest.height();

   ossim_uint64 bytesInLine     = 0;
   ossim_uint64 buf_band_offset = 0;
   
   // Use the system "streampos" typedef for future 64 bit seeks (long long).
   streampos file_band_offset = 0;
   
   //***
   // Buffer to hold one line x tileHeight
   //***
   ossim_uint64 bufferSizeInBytes = 0;
   unsigned char* buffer = NULL;
      
   theMinPerBand.clear();
   theMaxPerBand.clear();

   ossim_uint64 tileNumber = 0;
   bool wroteSomethingOut = false;
   ossimScalarType scalarType = theInputConnection->getOutputScalarType();
   for(ossim_uint64 i = 0; ((i < tilesHigh)&&(!needsAborting())); ++i)
   {
      if(buffer)
      {
	 // Clear the buffer.
	 memset(buffer, 0, bufferSizeInBytes);
      }
      
      ossimIrect bufferRect(theAreaOfInterest.ul().x,
			    theAreaOfInterest.ul().y + i*tileHeight,
			    theAreaOfInterest.ul().x + (width - 1),
			    theAreaOfInterest.ul().y + i *
                            tileHeight + (tileHeight - 1));
      
      // Tile loop in the sample (width) direction.
      for(ossim_uint64 j = 0; ((j < tilesWide)&&(!needsAborting())); ++j)
      {
         // Get the tile and copy it to the buffer.
         id = theInputConnection->getNextTile();
	 if(id.valid())
         {
            id->computeMinMaxPix(theMinPerBand, theMaxPerBand);
            if(!buffer)
            {
               bytesInLine     = id->getScalarSizeInBytes() * width;
               buf_band_offset = bytesInLine * tileHeight;
               file_band_offset = height * bytesInLine;
               bufferSizeInBytes = bytesInLine * tileHeight * bands;
               buffer = new unsigned char[bufferSizeInBytes];
               memset(buffer, 0, bufferSizeInBytes);
            }
            id->unloadTile(buffer,
                           bufferRect,
                           OSSIM_BSQ);
         }
         ++tileNumber;
      }
      
      // Get the number of lines to write from the buffer.
      ossim_uint64 linesToWrite =
         min(tileHeight,
             static_cast<ossim_uint64>(theAreaOfInterest.lr().y -
                                       bufferRect.ul().y + 1));
      
      // Write the buffer out to disk.  
      ossim_uint64 start_line =
         static_cast<ossim_uint64>(bufferRect.ul().y -
                                   theAreaOfInterest.ul().y);
      for (ossim_uint64 band = 0; ((band < bands)&&(!needsAborting())); ++band)
      {
         ossim_uint8* buf = buffer;
         buf += buf_band_offset * band;
         
         // Put the file pointer in the right spot.
         streampos pos = file_band_offset * band + start_line * bytesInLine;
         theFileStream.seekp(pos, ios::beg);
         if (theFileStream.fail())
         {
            ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:"
                 << "Error returned seeking to image data position!" << std::endl;
            setErrorStatus();
            return false;
         }
         
         for (ossim_uint64 ii=0; ((ii<linesToWrite)&&(!needsAborting())); ++ii)
         {
            wroteSomethingOut = true;
            if(endian.getSystemEndianType() != theOutputByteOrder)
            {
               endian.swap(scalarType,
                           buf,
                           bytesInLine/ossimGetScalarSizeInBytes(scalarType));
            }

            theFileStream.write((char*)buf, bytesInLine);
            
            if (theFileStream.fail())
            {
               ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:"
                    << "Error returned writing line!" << std::endl;
               setErrorStatus();
               return false;
            }
            
            buf += bytesInLine;
         }
         
      } // End of loop to write lines from buffer to tiff file.
      
      double tile = tileNumber;
      double numTiles = numberOfTiles;
      setPercentComplete(tile / numTiles * 100);

      if(needsAborting())
      {
         setPercentComplete(100.0);
      }
      
   } // End of loop in the line (height) direction.
   
   // Free the memory.
   delete [] buffer;
   
   if (traceDebug()) CLOG << " Exited." << std::endl;
   
   return wroteSomethingOut;
}

bool ossimGeneralRasterWriter::saveState(ossimKeywordlist& kwl,
                                         const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::BYTE_ORDER_KW,
           ((theOutputByteOrder==OSSIM_LITTLE_ENDIAN)?"little_endian":"big_endian"),
           true);
   return ossimImageFileWriter::saveState(kwl,
                                      prefix);
}

bool ossimGeneralRasterWriter::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   const char* value;
   
   value = kwl.find(prefix, ossimKeywordNames::FILENAME_KW);
   if(value)
   {
      setFilename(ossimFilename(value));
   }
   
   value = kwl.find(prefix, ossimKeywordNames::INPUT_RR_LEVEL_KW);
   if(value)
   {
      theRlevel = atoi(value);
   }

   if(ossimImageFileWriter::loadState(kwl, prefix))
   {
      if( (theOutputImageType!="general_raster_bip")      &&
          (theOutputImageType!="general_raster_bil")      &&
          (theOutputImageType!="general_raster_bsq")      &&
          (theOutputImageType!="general_raster_bip_envi") &&
          (theOutputImageType!="general_raster_bil_envi") &&
          (theOutputImageType!="general_raster_bsq_envi")
         )
      {
         theOutputImageType = "general_raster_bsq";
      }
   }
   else
   {
      return false;
   }
   const char* outputByteOrder = kwl.find(prefix, ossimKeywordNames::BYTE_ORDER_KW);
   theOutputByteOrder = ossimEndian().getSystemEndianType();
   if(outputByteOrder)
   {
      ossimString byteOrder = outputByteOrder;
      byteOrder = byteOrder.downcase();
      if(byteOrder.contains("little"))
      {
         theOutputByteOrder = OSSIM_LITTLE_ENDIAN;
      }
      else if(byteOrder.contains("big"))
      {
         theOutputByteOrder = OSSIM_BIG_ENDIAN;
      }
   }
   
   return true;
}

void ossimGeneralRasterWriter::writeHeader() const
{
   static const char MODULE[] = "ossimGeneralRasterWriter::writeHeader";

   if (traceDebug()) CLOG << " Entered..." << std::endl;

   // Make a header file name from the image file.
   ossimFilename headerFile = theFilename;
   headerFile.setExtension(".omd"); // ossim meta data

   std::ofstream os;
   os.open(headerFile.c_str(), ios::out);
   if (!os)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " Error:\n"
           << "Could not open:  " << headerFile << std::endl;
      return;
   }

   ossimString interleaveType = getInterleaveString();

   ossimString scalar =
      ossimScalarTypeLut::instance()->getEntryString(theInputConnection->
                                                     getOutputScalarType());
   
   os << "// *** ossim meta data general raster header file ***\n"
      << ossimKeywordNames::IMAGE_FILE_KW << ":  "
      << theFilename.file().c_str() << "\n"
      << ossimKeywordNames::IMAGE_TYPE_KW << ":  "
      << getOutputImageTypeString() << "\n"
      << ossimKeywordNames::INTERLEAVE_TYPE_KW << ":  "
      << interleaveType.c_str() << "\n"
      << ossimKeywordNames::NUMBER_BANDS_KW << ":  "
      << theInputConnection->getNumberOfOutputBands() << "\n"
      << ossimKeywordNames::NUMBER_LINES_KW << ":  "
      << (theAreaOfInterest.lr().y - theAreaOfInterest.ul().y + 1) << "\n"
      << ossimKeywordNames::NUMBER_SAMPLES_KW << ":  "
      << (theAreaOfInterest.lr().x - theAreaOfInterest.ul().x + 1) << "\n"
      << ossimKeywordNames::SCALAR_TYPE_KW << ":  "
      << scalar.c_str() << "\n"
      << ossimKeywordNames::BYTE_ORDER_KW <<": "
      << ((theOutputByteOrder==OSSIM_BIG_ENDIAN)?"big_endian":"little_endian")
      << "\n"
      << std::endl;

   // Output the null/min/max for each band.
   os << "\n// NOTE:  Bands are one based, band1 is the first band."
      << std::endl;

   for (ossim_uint32 i=0; i<theInputConnection->getNumberOfOutputBands(); ++i)
   {
      ossimString prefix = ossimKeywordNames::BAND_KW +
                           ossimString::toString(i+1) + ".";
      
      ossimString null_pix = ossimString::toString(theInputConnection->
                                                   getNullPixelValue(i));
      ossimString min_pix;
      ossimString max_pix;

      if(!theMinPerBand.size()||!theMaxPerBand.size())
      {
         min_pix  = ossimString::toString(theInputConnection->
					  getMinPixelValue(i));
         max_pix  =  ossimString::toString(theInputConnection->
                                           getMaxPixelValue(i));
      }
      else
      {
         min_pix   = ossimString::toString(theMinPerBand[i]);
         max_pix  = ossimString::toString(theMaxPerBand[i]);
      }
      
      os << prefix.c_str() << ossimKeywordNames::NULL_VALUE_KW << ":  "
         << null_pix.c_str() << "\n"
         << prefix << ossimKeywordNames::MIN_VALUE_KW << ":  "
         << min_pix.c_str() << "\n"
         << prefix << ossimKeywordNames::MAX_VALUE_KW << ":  "
         << max_pix.c_str() << std::endl;
   }
   
   os.close();
   
   if (traceDebug()) CLOG << " Exited..." << endl;
}

void ossimGeneralRasterWriter::writeEnviHeader() const
{
   static const char MODULE[] = "ossimGeneralRasterWriter::writeEnviHeader";

   if (traceDebug()) CLOG << " Entered..." << endl;

   if (!theInputConnection)
   {
      return;
   }
   
   // Make a header file name from the image file.
   ossimFilename headerFile = theFilename;
   headerFile.setExtension(".hdr"); // ossim meta data

   ossimString interleaveType = getInterleaveString();
   ossimKeywordlist kwl;
   kwl.add(ossimKeywordNames::INTERLEAVE_TYPE_KW, interleaveType.c_str());

   ossimEnviHeaderFileWriter hdr;
   hdr.connectMyInputTo(0, theInputConnection);
   hdr.initialize();
   hdr.setFilename(headerFile);
   hdr.loadState(kwl);
   hdr.setAreaOfInterest(theAreaOfInterest);
   hdr.execute();
   
   if (traceDebug()) CLOG << " Exited..." << endl;
}

void ossimGeneralRasterWriter::getImageTypeList(std::vector<ossimString>& imageTypeList)const
{
   imageTypeList.push_back(ossimString("general_raster_bip"));
   imageTypeList.push_back(ossimString("general_raster_bil"));
   imageTypeList.push_back(ossimString("general_raster_bsq"));
   imageTypeList.push_back(ossimString("general_raster_bip_envi"));
   imageTypeList.push_back(ossimString("general_raster_bil_envi"));
   imageTypeList.push_back(ossimString("general_raster_bsq_envi"));
}

ossimString ossimGeneralRasterWriter::getInterleaveString() const
{
   ossimString interleaveType = "unknown";
   if ( (theOutputImageType == "general_raster_bip") ||
        (theOutputImageType == "general_raster_bip_envi") )
   {
      interleaveType = "bip";
   }
   else if ( (theOutputImageType == "general_raster_bil") ||
             (theOutputImageType == "general_raster_bil_envi") )
   {
      interleaveType = "bil";
   }
   else if ( (theOutputImageType == "general_raster_bsq") ||
             (theOutputImageType == "general_raster_bsq_envi") )
   {
      interleaveType = "bsq";
   }
   return interleaveType;
}
