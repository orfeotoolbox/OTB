//----------------------------------------------------------------------------
//
// License:  See tope level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Shuttle Radar Topography Mission (SRTM) elevation source.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmHandler.h 11180 2007-06-07 19:56:09Z dburken $
#ifndef ossimSrtmHandler_HEADER
#define ossimSrtmHandler_HEADER

#include <ossim/base/ossimIoStream.h>
//#include <fstream>

#include <ossim/base/ossimString.h>
#include <ossim/elevation/ossimElevCellHandler.h>

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

   mutable ossimRefPtr<ossimIFStream> theFileStr;
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
