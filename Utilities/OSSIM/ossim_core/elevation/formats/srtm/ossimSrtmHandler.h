//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Shuttle Radar Topography Mission (SRTM) elevation source.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmHandler.h,v 1.4 2005/11/10 14:15:33 gpotts Exp $
#ifndef ossimSrtmHandler_HEADER
#define ossimSrtmHandler_HEADER

#include <base/common/ossimIoStream.h>
//#include <fstream>

#include <base/data_types/ossimString.h>
#include <elevation/formats/ossimElevCellHandler.h>

class ossimEndian;

/**
 * @class ossimSrtmHandler Elevation source for an srtm file.
 */
class OSSIMDLLEXPORT ossimSrtmHandler : public ossimElevCellHandler
{
public:

   /** Constructor that takes a file name. */
   ossimSrtmHandler(const ossimFilename& srtmFile);

   /** destructor */
   virtual ~ossimSrtmHandler();

   enum
   {
      NULL_POST = -32768 // Fixed by SRTM specification.
   };

   /**
    * METHOD: getHeightAboveMSL
    * Height access methods.
    */
   virtual double getHeightAboveMSL(const ossimGpt&);

   /**
    *  METHOD:  getSizeOfElevCell
    *  Returns the number of post in the cell.  Satisfies pure virtual.
    *  Note:  x = longitude, y = latitude
    */
   virtual ossimIpt getSizeOfElevCell() const;
      
   /**
    *  METHOD:  getPostValue
    *  Returns the value at a given grid point as a double.
    *  Satisfies pure virtual.
    */
   virtual double getPostValue(const ossimIpt& gridPt) const;

   /**
    * Opens a stream to the srtm cell.
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
   const ossimSrtmHandler& operator=(const ossimSrtmHandler& rhs);
   ossimSrtmHandler(const ossimSrtmHandler&);

   mutable ossimRefPtr<ossimIStream> theFileStr;
//   mutable std::istream* theFileStr;
//   mutable ifstream theFileStr;
   ossim_int32      theNumberOfLines;
   ossim_int32      theNumberOfSamples;
   ossim_int32      theSrtmRecordSizeInBytes;
   double           theLatSpacing;   // degrees
   double           theLonSpacing;   // degrees
   ossimDpt         theNwCornerPost; // cell origin;
   ossimEndian*     theSwapper;
   ossimScalarType theScalarType;
   
   template <class T>
   double getHeightAboveMSLTemplate(T dummy,
                                    const ossimGpt& gpt);
TYPE_DATA
};

#endif /* End of "#ifndef ossimSrtmHandler_HEADER" */
