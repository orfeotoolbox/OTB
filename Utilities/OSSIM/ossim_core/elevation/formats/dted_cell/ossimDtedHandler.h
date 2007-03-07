//*****************************************************************************
// FILE: ossimDtedHandler.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains declaration of class ossimDtedHandler. This class derives from
//   ossimElevHandler. It is responsible for loading an individual DTED cell
//   from disk. This elevation files are memory mapped.
//
// SOFTWARE HISTORY:
//>
//   05Feb2001  Ken Melero
//              Initial coding of ossimDted.h
//   19Apr2001  Oscar Kramer
//              Derived from ossimElevCellHandler.
//<
//*****************************************************************************
// $Id: ossimDtedHandler.h,v 1.19 2005/03/03 21:57:55 dburken Exp $

#ifndef ossimDtedHandler_HEADER
#define ossimDtedHandler_HEADER

#include <fstream>

#include <base/data_types/ossimString.h>
#include <elevation/formats/ossimElevCellHandler.h>

class ossimDtedHandler : public ossimElevCellHandler
{
public:
   ossimDtedHandler(const ossimFilename& dted_file);
   
   virtual ~ossimDtedHandler();

   enum
   {
      DATA_RECORD_OFFSET_TO_POST = 8,     // bytes
      DATA_RECORD_CHECKSUM_SIZE  = 4,     // bytes
      POST_SIZE                  = 2,     // bytes
      NULL_POST                  = -32767 // Fixed by DTED specification.
   };

   /*!
    * METHOD: getHeightAboveMSL
    * Height access methods.
    */
   virtual double getHeightAboveMSL(const ossimGpt&);

   /*!
    *  METHOD:  getSizeOfElevCell
    *  Returns the number of post in the cell.  Satisfies pure virtual.
    *  Note:  x = longitude, y = latitude
    */
   virtual ossimIpt getSizeOfElevCell() const;
      
   /*!
    *  METHOD:  getPostValue
    *  Returns the value at a given grid point as a double.
    *  Satisfies pure virtual.
    */
   virtual double getPostValue(const ossimIpt& gridPt) const;

   ossimString  edition()         const;
   ossimString  productLevel()    const;
   ossimString  compilationDate() const;

   /**
    * Opens a stream to the dted cell.
    *
    * @return Returns true on success, false on error.
    */
   virtual bool open() const;

   /**
    * Closes the stream to the file.
    */
   virtual void close() const;
   
private:
   // Disallow operator= and copy constrution...
   const ossimDtedHandler& operator=(const ossimDtedHandler& rhs)
      { return rhs; }
   ossimDtedHandler(const ossimDtedHandler&){}

   /*!
    *  If statistics file exist, stats will be initialized from that; else,
    *  this scans the dted cell and gets stats, then, writes new stats file.
    *  The statistics file will be named accordingly:
    *  If dted cell = n27.dt1 then the stats file = n27.statistics.
    *  Currently method only grabs the min and max posts value.
    */
   void gatherStatistics();

   ossim_sint16 convertSignedMagnitude(ossim_uint16& s) const;
   
   mutable ifstream theFileStr;
   ossim_int32      theNumLonLines;  // east-west dir
   ossim_int32      theNumLatPoints; // north-south
   ossim_int32      theDtedRecordSizeInBytes;
   ossimString      theEdition;
   ossimString      theProductLevel;
   ossimString      theCompilationDate;
   ossim_int32      theOffsetToFirstDataRecord;
   double           theLatSpacing;   // degrees
   double           theLonSpacing;   // degrees
   ossimDpt         theSwCornerPost; // cell origin;

   // Indicates whether byte swapping is needed.
   bool theSwapBytesFlag;

   TYPE_DATA
};

inline ossim_sint16 ossimDtedHandler::convertSignedMagnitude(ossim_uint16& s) const
{
   // DATA_VALUE_MASK 0x7fff = 0111 1111 1111 1111
   // DATA_SIGN_MASK  0x8000 = 1000 0000 0000 0000
   
   // First check to see if the bytes need swapped.
   s = (theSwapBytesFlag ? ( (s << 8) | (s >> 8) ) : s);
   
   // If the sign bit is set, mask it out then multiply by negative one.
   if (s & 0x8000)
   {
      return (static_cast<ossim_sint16>(s & 0x7fff) * -1);
   }
   
   return static_cast<ossim_sint16>(s);
}

inline bool ossimDtedHandler::open() const
{
   if (theFileStr.is_open())
   {
      return true;
   }

   theFileStr.open(theFilename.c_str(), ios::in | ios::binary);

   return theFileStr.is_open();
}

inline void ossimDtedHandler::close() const
{
   if (theFileStr.is_open())
   {
      theFileStr.close();
   }
}

#endif
