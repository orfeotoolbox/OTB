//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  ossimRpfUtil.cpp
// 
// Utility class to stuff with rpf files.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/util/ossimRpfUtil.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimRpfToc.h>
#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ctime>
#include <iomanip>

static ossimTrace traceDebug = ossimTrace("ossimRpfUtil:debug");

ossimRpfUtil::ossimRpfUtil()
{
}

ossimRpfUtil::~ossimRpfUtil()
{
}


// Note: throws ossimException on error.
void ossimRpfUtil::writeDotRpfFiles( const ossimFilename& aDotTocFile,
                                     const ossimFilename& outputDir )
{
   static const char MODULE[] = "ossimRpfUtil::writeDotRpfFiles";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "\na.toc file:        " << aDotTocFile
         << "\noutput directory:  " << outputDir
         << "\n";
   }
   
   // Parse the a.toc file:
   ossimRefPtr<ossimRpfToc> toc = new ossimRpfToc();
   
   if ( toc->parseFile(aDotTocFile) != ossimErrorCodes::OSSIM_OK )
   {
      std::string e = MODULE;
      e += " ERROR:\nCould not open: ";
      e+= aDotTocFile.string();
      throw ossimException(e);
   }

   if ( outputDir.expand().exists() == false )
   {
      if ( !outputDir.createDirectory(true, 0775) )
      {
         std::string e = MODULE;
         e += " ERROR:\nCould not create directory: ";
         e+= outputDir.c_str();
         throw ossimException(e);
      }
   }

   //---
   // Go through the entries...
   //---
   ossim_uint32 entries = toc->getNumberOfEntries();
   for (ossim_uint32 entry = 0; entry < entries; ++entry)
   {
      const ossimRpfTocEntry* tocEntry = toc->getTocEntry(entry);
      if (tocEntry)
      {
         if ( tocEntry->isEmpty() == false )
         {
            writeDotRpfFile(toc.get(), tocEntry, outputDir, entry);
         }
      }
      else
      {
         std::string e = MODULE;
         e += " ERROR:  Null entry: ";
         e += ossimString::toString(entry).string();
         throw ossimException(e);
      }
   }
   
} // End: ossimRpfUtil::writeDotRpfFiles

//---
// Writer a dot rpf file for entry to output directory.
// 
// NOTES:
//
// 1) All coordinate written out in AREA or edge to edge format.
// 2) Throws ossimException on error.
//---
void ossimRpfUtil::writeDotRpfFile( const ossimRpfToc* toc,
                                    const ossimRpfTocEntry* tocEntry,
                                    const ossimFilename& outputDir,
                                    ossim_uint32 entry)
{
   static const char MODULE[] = "ossimRpfUtil::writeDotRpfFile";

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered..."
         << "\noutput directory:  " << outputDir
         << "\nentry: " << entry << "\n";
   }

   if ( !toc )
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR toc pointer null!";
      throw ossimException(errMsg);
   }
   if ( !tocEntry )
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR toc entry pointer null!";
      throw ossimException(errMsg);
   }

   // Get the file name.
   ossimFilename outFile;
   if ( outputDir.expand().isDir() )
   {
      getDotRfpFilenameForEntry(outputDir, entry, outFile);
   }
   else
   {
      outFile = outputDir;
   }
   
   // Open the file to write.
   std::ofstream os;
   os.open(outFile.c_str(), ios::out);
   if ( os.good() == false )
   {
      std::string errMsg = MODULE;
      errMsg += "ERROR could not open: ";
      errMsg += outFile.string();
      throw ossimException(errMsg);
   }
   
   // Set up the output stream fix with full precision for ground points.
   os << setiosflags(std::ios_base::fixed) << setprecision(15);
   
   //---
   // Overall TOC entry bounds:
   // 
   // Write the first line which is the bounding box of the entry in the form of:
   // "89.9850464205332, 23.9892538162654|90.5085823882692, 24.5002602501599|1"
   //      lr-lon            lr-lat           ul-lon            ul-lat
   //---
   ossimRefPtr<ossimImageGeometry> geom = tocEntry->getImageGeometry();
   if( geom.valid() == false)
   {
      std::string errMsg = "ERROR could not get geometry.";
      errMsg += outFile.string();
      throw ossimException(errMsg);  
   }

   // Rectangle in image space.
   ossimIrect outputRect;
   tocEntry->getBoundingRect(outputRect);

   // bands:
   ossim_uint32 bands = tocEntry->getNumberOfBands();

   // scale:
   ossimDpt scale;
   tocEntry->getDecimalDegreesPerPixel(scale);
   ossimDpt halfPix = scale / 2.0;

   ossimGpt llg;
   ossimGpt urg;
   geom->localToWorld(outputRect.ur(), urg);
   geom->localToWorld(outputRect.ll(), llg);

   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "outputRect: " << outputRect
         << "\nbands: " << bands
         << "\nscale: " << scale
         << "\nllg:   " << llg
         << "\nurg:   " << urg
         << std::endl;
   }

   // Expand coordinates to edge:
   llg.lon -= halfPix.x;
   llg.lat -= halfPix.y;
   urg.lon += halfPix.x;
   urg.lat += halfPix.y;
   
   // Test for 360 degrees apart.
   checkLongitude(llg, urg);
   
   os << llg.lon << "," // lower left longitude
      << llg.lat << "|" // lower left latitude
      << urg.lon << "," // upper right longitude
      << urg.lat << "|" // upper right latitude
      << bands << "\n";

   // Frame loop:
   const ossim_int32 FRAMESIZE = 1536;
   const ossim_int32 ROWS = static_cast<ossim_int32>(tocEntry->getNumberOfFramesVertical());
   if( ROWS == 0 )
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR no rows!";
      throw ossimException(errMsg);  
   }
   const ossim_int32 COLS = static_cast<ossim_int32>(tocEntry->getNumberOfFramesHorizontal());
   if( COLS == 0 )
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR no columns!";
      throw ossimException(errMsg);  
   }

   // Set the initial lower left and upper right image points for localToWorld call.
   //ossimDpt urd( ( (ROWS-1)*FRAMESIZE) -1, 0.0);
   //ossimDpt lld(0.0, (ROWS*FRAMESIZE)-1);
   ossimDpt urd( FRAMESIZE-1, 0.0);
   ossimDpt lld(0.0, FRAMESIZE-1);
   
   for (ossim_int32 row = ROWS-1; row > -1; --row)
   {
      for (ossim_int32 col = 0; col < COLS; ++col)
      {
         //---
         // Example format (only with 15 digit precision):
         // /data/spadac/rpf/world/cb01/ng467a1/0xslpk1a.i41|90.0448,24.3621|90.0598,24.3750
         //---
         
         // Get the path to the frame.
         ossimFilename path;
         toc->getRootDirectory(path);
         
         path = path.dirCat( toc->getRelativeFramePath(entry, row, col) );

         // Not sure if this is backwards:
         geom->localToWorld(urd, urg);
         geom->localToWorld(lld, llg);

         // Expand coordinates to edge:
         llg.lon -= halfPix.x;
         llg.lat -= halfPix.y;
         urg.lon += halfPix.x;
         urg.lat += halfPix.y;
         
         // Test for 360 degrees apart.
         checkLongitude(llg, urg);

         os << path.c_str() << "|"
            << llg.lon << "," // lower left longitude
            << llg.lat << "|" // lower left latitude
            << urg.lon << "," // upper right longitude
            << urg.lat        // upper right latitude
            << "\n";

         if ( traceDebug() )
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "row[" << row << "]col[" << col << "]path: " << path
               << "\nlld: " << lld
               << "\nllg: " << llg
               << "\nurd: " << urd
               << "\nurg: " << urg
               << std::endl;
         }

         // Go to next col.
         urd.x += FRAMESIZE;
         lld.x += FRAMESIZE;
            
      } // End column loop.

      // Go to nex row.
      urd.y += FRAMESIZE;
      urd.x = FRAMESIZE-1;
      lld.y += FRAMESIZE;
      lld.x = 0;
      
   } // End row loop.

   // Close the file.
   os.close();

   ossimNotify(ossimNotifyLevel_DEBUG) << "wrote file: " << outFile << std::endl;
   
} // End: ossimRpfUtil::writeDotRpfFile

void ossimRpfUtil::checkLongitude(ossimGpt& left, ossimGpt& right) const
{
   //---
   // Test for scene coordinates being 180 to 180 (360 degree spread) and
   // adjust leftLon to -180 if so.
   //
   // NOTE:
   // Setting tolerance to 1/7200 about 15 meters.
   // Not sure if this is too loose or not. (drb)
   //---
   const ossim_float64 TOLERANCE = 0.000138889; // 1/7200 about 15 meters.

   if ( ossim::almostEqual(left.lon, 180.0, TOLERANCE) )
   {
      if ( ossim::almostEqual(right.lon, 180.0, TOLERANCE) )
      {
         left.lon = -180.0;
         right.lon = 180.0;
      }
   }
}

void ossimRpfUtil::getDotRfpFilenameForEntry(const ossimFilename& outputDir,
                                             ossim_uint32 entry,
                                             ossimFilename& outFile) const
{
   // Get the build date in the format of (yyyymmddhhmmss).
   char s[15];
   s[14] = '\0';
   time_t t;
   time(&t);
   tm* lt = localtime(&t);
   strftime(s, 15, "%Y%m%d%H%M%S", lt);
   std::string date = s;
   
   outFile = outputDir.dirCat(s);
   outFile += "_e";
   outFile += ossimString::toString(entry);
   outFile += ".rpf";
}
