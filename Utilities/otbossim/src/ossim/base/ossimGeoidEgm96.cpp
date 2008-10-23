//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
//*******************************************************************
//  $Id: ossimGeoidEgm96.cpp 11513 2007-08-06 11:40:18Z gpotts $

#include <ossim/base/ossimGeoidEgm96.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h> /* for ossim::nan() */
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDatumFactory.h>
#include <fstream>

static ossimTrace traceDebug ("ossimGeoidEgm96:debug");


#define NumbGeoidCols 1441   /* 360 degrees of longitude at 15 minute spacing */
#define NumbGeoidRows  721   /* 180 degrees of latitude  at 15 minute spacing */
#define NumbHeaderItems 6    /* min, max lat, min, max long, lat, long spacing*/
#define ScaleFactor     4    /* 4 grid cells per degree at 15 minute spacing  */
#define NumbGeoidElevs NumbGeoidCols * NumbGeoidRows
// #define PI              3.14159265358979323e0


RTTI_DEF1(ossimGeoidEgm96, "ossimGeoidEgm96", ossimGeoid)

ossimGeoidEgm96::ossimGeoidEgm96()
   :theGeoidHeightBufferPtr(0)
{
}

ossimGeoidEgm96::ossimGeoidEgm96(const ossimFilename& grid_file,
                                 ossimByteOrder byteOrder)
   :theGeoidHeightBufferPtr(0)
{
   open(grid_file, byteOrder);
   if (getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      theGeoidHeightBuffer.clear();
   }
}

ossimGeoidEgm96::~ossimGeoidEgm96()
{
}

ossimString ossimGeoidEgm96::getShortName()const
{
   return "geoid1996";
}

bool ossimGeoidEgm96::open(const ossimFilename& grid_file,
                           ossimByteOrder byteOrder)
{
   static const char MODULE[] = "ossimGeoidEgm96::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " Entered...\n";
   }

   if(theGeoidHeightBuffer.size() != NumbGeoidElevs)
   {
      theGeoidHeightBuffer.resize(NumbGeoidElevs);
      theGeoidHeightBufferPtr = &theGeoidHeightBuffer.front();
   }
   
   // int   ItemsRead = 0;
   long  ElevationsRead = 0;
   // long  ItemsDiscarded = 0;
   long  num = 0;

   ossimFilename grid = grid_file;
   if (grid_file.isDir())
   {
      grid = grid_file.dirCat("egm96.grd");
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " Grid file:" << grid << "\n";
   }
   
   // Open the File READONLY, or Return Error Condition:
   std::ifstream gridHeightFile(grid.c_str(), std::ios::in|std::ios::binary);
//   FILE* GeoidHeightFile;

   if ( gridHeightFile.fail())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << MODULE << " could not open file "
                                            << grid.c_str() << "\n";
      }
      setErrorStatus();
      return false;
   }
  
   // Skip the Header Line:
   ossimEndian oe;
   bool swap_bytes = (ossim::byteOrder() != byteOrder) ? true : false;
   while ( (num < NumbHeaderItems)&&(!gridHeightFile.fail()))
   {      
      float f;
      gridHeightFile.read( (char*)(&f), 4);
      if (swap_bytes) oe.swap(f);
      theGeoidHeightBuffer[num] = f;
      ++num;
   }
   // Determine if header read properly, or NOT:
   if ((!ossim::almostEqual(theGeoidHeightBuffer[0], (float)-90.0)) ||
       (!ossim::almostEqual(theGeoidHeightBuffer[1], (float)90.0)) ||
       (!ossim::almostEqual(theGeoidHeightBuffer[2], (float)0.0)) ||
       (!ossim::almostEqual(theGeoidHeightBuffer[3],(float)360.0))||
       (!ossim::almostEqual(theGeoidHeightBuffer[4],(float)(1.0 / ScaleFactor ))) ||
       (!ossim::almostEqual(theGeoidHeightBuffer[5],(float)( 1.0 / ScaleFactor ))) ||
       gridHeightFile.fail())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << MODULE << " bad header file "
                                            << grid.c_str() << "\n";
      }
      setErrorStatus();
      return false;
  }

   // Extract elements from the file:
   num = 0;
   while ( (num < NumbGeoidElevs)&&(!gridHeightFile.fail()) )
   {
//       if (feof( GeoidHeightFile )) break;
//       if (ferror( GeoidHeightFile )) break;
      
      float f;
      gridHeightFile.read( (char*)(&f), 4);
      if (swap_bytes) oe.swap(f);
      theGeoidHeightBuffer[num] = f;
      ++ElevationsRead;
      ++num;
   }

//    fclose(GeoidHeightFile);
   
   // Determine if all elevations of file read properly, or NOT:
   if (ElevationsRead != NumbGeoidElevs)
   {
      setErrorStatus();
      ossimSetError("ossimGeoidEgm96::open",
                    ossimErrorCodes::OSSIM_ERROR,
                    "Bad grid file...%s", grid.c_str());
     return false;
  }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Opened geoid grid:  " << grid.c_str() << std::endl;
   }
   
   return true;
}

double ossimGeoidEgm96::offsetFromEllipsoid(const ossimGpt& gpt) const
{
   double offset = ossim::nan();
   ossimGpt savedGpt = gpt;
   if(ossimDatumFactory::instance()->wgs84())
   {
      savedGpt.changeDatum(ossimDatumFactory::instance()->wgs84());
   }
   
   if (!theGeoidHeightBufferPtr)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "ossimGeoidEgm96::offsetFromEllipsoid, "
            << "Object not initialized!\n";
      }

      return offset;
   }
   
   long    Index;
   double DeltaX, DeltaY;
   double ElevationSE, ElevationSW, ElevationNE, ElevationNW;
   double LatitudeDD, LongitudeDD;
   double OffsetX, OffsetY;
   double PostX, PostY;
   double UpperY, LowerY;

   LatitudeDD  = savedGpt.latd();
   
   // Check for wrap.
   if (LatitudeDD < -90.0)
   {
      LatitudeDD = -180.0 - LatitudeDD;
   }
   else if (LatitudeDD > 90.0)
   {
      LatitudeDD = 180.0 - LatitudeDD;
   }

   if ( (LatitudeDD < -90.0) || LatitudeDD > 90.0)
   {
      if(traceDebug())
      {
         // Latitude out of range
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL: " << "ossimGeoidEgm96::offsetFromEllipsoid, "
            << "Point out of range:  " << savedGpt << "\n";
      }
      return offset;
   }
   
   LongitudeDD = savedGpt.lond();

   // Check for wrap.
   if (LongitudeDD < -180)
   {
      LongitudeDD = LongitudeDD + 360.0;
   }
   else if (LongitudeDD > 180.0)
   {
      LongitudeDD = LongitudeDD - 360.0;
   }

   if ( (LongitudeDD < -180.0) || (LongitudeDD > 180.0) )
   {
      // Longitude out of range
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL: " << "ossimGeoidEgm96::offsetFromEllipsoid, "
            << "Point out of range:  " << savedGpt << "\n";
      }
      return offset;
   }
   
   // Compute X and Y Offsets into Geoid Height Array:
   
   if (LongitudeDD < 0.0)
   {
      OffsetX = ( LongitudeDD + 360.0 ) * ScaleFactor;
   }
   else
   {
      OffsetX = LongitudeDD * ScaleFactor;
   }
   OffsetY = ( 90.0 - LatitudeDD ) * ScaleFactor;
   
   //***
        // Find Four Nearest Geoid Height Cells for specified Latitude,
        // Longitude;  Assumes that (0,0) of Geoid Height Array is at
        // Northwest corner:
        //***
             PostX = floor( OffsetX );
   if ((PostX + 1) == NumbGeoidCols)
      PostX--;
   PostY = floor( OffsetY );
   if ((PostY + 1) == NumbGeoidRows)
      PostY--;
   
   Index = (long)(PostY * NumbGeoidCols + PostX);
   ElevationNW = theGeoidHeightBufferPtr[ Index ];
   ElevationNE = theGeoidHeightBufferPtr[ Index+ 1 ];
   
   Index = (long)((PostY + 1) * NumbGeoidCols + PostX);
   ElevationSW = theGeoidHeightBufferPtr[ Index ];
   ElevationSE = theGeoidHeightBufferPtr[ Index + 1 ];
   
   //Perform Bi-Linear Interpolation to compute Height above Ellipsoid:
   DeltaX = OffsetX - PostX;
   DeltaY = OffsetY - PostY;
   
   UpperY = ElevationNW + DeltaX * ( ElevationNE - ElevationNW );
   LowerY = ElevationSW + DeltaX * ( ElevationSE - ElevationSW );
   
   offset = UpperY + DeltaY * ( LowerY - UpperY );
   
   return offset;
}

double ossimGeoidEgm96::geoidToEllipsoidHeight(double lat,
                                               double lon,
                                               double geoidHeight) const
{
   ossimGpt gpt(lat, lon);
   double height = offsetFromEllipsoid(gpt);
   if (!ossim::isnan(height))
   {
      height += geoidHeight;
   }
   return height;
}

double ossimGeoidEgm96::ellipsoidToGeoidHeight(double lat,
                                               double lon,
                                               double ellipsoidHeight) const
{
   ossimGpt gpt(lat, lon);
   double height = offsetFromEllipsoid(gpt);
   if (!ossim::isnan(height))
   {
      return (ellipsoidHeight - height);
   }
   return height; // nan
}
