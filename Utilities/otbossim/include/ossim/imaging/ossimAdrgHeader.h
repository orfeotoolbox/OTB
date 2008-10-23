//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Ken Melero
// Description: This class encapsulates the image parameters and
//              support data for a ADRG image file(s).
//
//********************************************************************
// $Id: ossimAdrgHeader.h 10266 2007-01-14 19:27:14Z dburken $

#ifndef ossimAdrgHeader_H
#define ossimAdrgHeader_H

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>

class OSSIM_DLL ossimAdrgHeader
{
public:
   // Instantiated with the image file (.img)
   ossimAdrgHeader(const ossimFilename& img_file);
   ~ossimAdrgHeader();
   
   enum ErrorStatus
   {
      OSSIM_OK    = 0,
      OSSIM_ERROR = 1
   };

   enum
   {
      ADRG_TILE_SIZE = 128
   };
   
   int errorStatus() const { return theErrorStatus; }

   ossimFilename file()
      const { return theHeaderFile; }
   ossimFilename imageFile()
      const { return theImageFile; }
   ossimIrect validImageRect()
      const { return theValidImageRect; }

   ossim_uint32    pixelType()      const { return thePixelType; }
   ossim_uint32    interleaveType() const { return theInterleaveType; }
   ossim_uint32    numberOfBands()  const { return theNumberOfBands; }
   ossim_uint32    lines()          const { return theLines; }
   ossim_uint32    tlines()         const { return theLinesInTiles; }
   ossim_uint32    samples()        const { return theSamples; }
   ossim_uint32    tsamples()       const { return theSamplesInTiles; }
   ossim_uint32    headerSize()     const { return theHeaderSize; }
   ossim_uint32    startRow()       const { return theStartRow; }
   ossim_uint32    startCol()       const { return theStartCol; }
   ossim_uint32    stopRow()        const { return theStopRow; }
   ossim_uint32    stopCol()        const { return theStopCol; }

   ossim_uint32    tim(ossim_uint32 row, ossim_uint32 col) const;
   
   ossimString minLon() const { return theMinLon; }
   ossimString minLat() const { return theMinLat; }
   ossimString maxLon() const { return theMaxLon; }
   ossimString maxLat() const { return theMaxLat; }

   double minLongitude()   const;
   double minLatitude()    const;
   double maxLongitude()   const;
   double maxLatitude()    const;
   
   friend OSSIM_DLL std::ostream& operator<<(std::ostream& os,
                                             const ossimAdrgHeader& adrg);

private:

   // Returns the longitude in decimal degrees given the formatted string.
   double parseLongitudeString(const ossimString& lon) const;

   // Returns the latitude in decimal degrees given the formatted string.
   double parseLatitudeString(const ossimString& lat) const;
   
   // prevent use.
   ossimAdrgHeader();
   ossimAdrgHeader(const ossimAdrgHeader& source);

   ErrorStatus theErrorStatus;

   ossimFilename theHeaderFile;
   ossimFilename theImageFile;
   ossimIrect    theValidImageRect;
   
   ossim_uint32  thePixelType;
   ossim_uint32  theInterleaveType;
   ossim_uint32  theNumberOfBands;
   ossim_uint32  theLines;
   ossim_uint32  theLinesInTiles;
   ossim_uint32  theSamples;
   ossim_uint32  theSamplesInTiles;
   ossim_uint32  theHeaderSize;
   ossim_uint32  theStartRow;
   ossim_uint32  theStartCol;
   ossim_uint32  theStopRow;
   ossim_uint32  theStopCol;
   ossim_uint32* theTim;   // Tile index map value -- only valid if TIF is 'Y'.
   
   ossimString theMinLon;
   ossimString theMinLat;
   ossimString theMaxLon;
   ossimString theMaxLat;
   ossimString theTif;  // Tile index map flag -- Y indicates there are tiles
                        // with no data; N indicates that all tiles contain
                        // RGB graphic data.

   void parse();
};

#endif
