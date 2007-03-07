//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Ken Melero (kmelero@imagelinks.com)
// Description: This class encapsulates the image parameters and
//              support data for a ADRG image file(s).
//
//********************************************************************
// $Id: ossimAdrgHeader.h,v 1.3 2004/06/23 20:10:55 dburken Exp $

#ifndef ossimAdrgHeader_H
#define ossimAdrgHeader_H

#include "base/common/ossimConstants.h"
#include "base/data_types/ossimFilename.h"
#include "base/data_types/ossimIrect.h"

class OSSIMDLLEXPORT ossimAdrgHeader
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
   
   friend ostream& operator<<(ostream& os, const ossimAdrgHeader& adrg);

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
