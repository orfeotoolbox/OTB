//----------------------------------------------------------------------------
//
// License:  see top level LICENSE.txt
// 
// Author:  Garrett Potts
//
// Description:
// 
//
//----------------------------------------------------------------------------
// $Id: ossimGeneralRasterElevHandler.h 10043 2006-12-07 16:09:34Z dburken $
#ifndef ossimGeneralRasterElevHandler_HEADER
#define ossimGeneralRasterElevHandler_HEADER
#include <list>
#include <ossim/base/ossimIoStream.h>
//#include <fstream>

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <ossim/imaging/ossimGeneralRasterInfo.h>

/**
 * @class ossimGeneralRasterElevHandler Elevation source for an srtm file.
 */
class OSSIM_DLL ossimGeneralRasterElevHandler : public ossimElevCellHandler
{
public:
   /** Constructor that takes a file name. */
   ossimGeneralRasterElevHandler(const ossimFilename& file="");
   ossimGeneralRasterElevHandler(const ossimGeneralRasterElevHandler& rhs);
   const ossimGeneralRasterElevHandler& operator=(const ossimGeneralRasterElevHandler& rhs);

   /** destructor */
   virtual ~ossimGeneralRasterElevHandler();
   virtual ossimObject* dup()const;

   double getNullHeightValue()const;
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
   virtual bool open();

   /**
    * Closes the stream to the file.
    */
   virtual void close();

   virtual bool setFilename(const ossimFilename& file);

   /**
    * This method does not really fit the handler since this handle a
    * directory not a cell that could have holes in it.  So users looking for
    * valid coverage should call "pointHasCoverage".
    */
   ossimDrect getBoundingRect()const;

   /**
    * Loops through ossimGeneralRasterElevHandler::BoundingRectListType and
    * checks for coverage.
    *
    * @param gpt Point to look for.
    *
    * @return true if coverage is found false if not.
    */
   virtual bool pointHasCoverage(const ossimGpt& gpt) const;
   
private:
   class BoundingRectInfo
   {
   public:
      BoundingRectInfo()
         :theScalarType(OSSIM_SCALAR_UNKNOWN),
         theBytesPerRawLine(0)
      {
      }
      ossimFilename     theFilename;
      mutable ossimRefPtr<ossimIStream> theInputStream;
      ossimDrect        theBounds;
      ossimDpt          thePostSpacing;
      ossimGpt          theUlGpt;
      ossimGpt          theLrGpt;
      ossim_int32       theNumberOfSamples;
      ossim_int32       theNumberOfLines;
      ossim_float64     theNullHeightValue;
      ossimByteOrder    theByteOrder;
      ossimScalarType   theScalarType;
      ossim_uint32      theBytesPerRawLine;
      const ossimDatum* theDatum; 
   };
   typedef std::vector<ossimGeneralRasterElevHandler::BoundingRectInfo> BoundingRectListType;
   // Disallow operator= and copy constrution...
   template <class T>
   double getHeightAboveMSLTemplate(T dummy,
                                    const ossimGeneralRasterElevHandler::BoundingRectInfo& info,
                                    const ossimGpt& gpt);

   void initializeList(const ossimFilename& file);
   bool initializeInfo(ossimGeneralRasterElevHandler::BoundingRectInfo& info,
                       const ossimFilename& file);
   void addInfo(const BoundingRectInfo& info);
   bool open(ossim_uint32 idx);
   

   BoundingRectListType  theBoundingRectInfoList;
   ossimDrect            theBoundingRect;
/*    mutable std::ifstream theFileStr;  */
   mutable ossim_int32   theCurrentIdx;
TYPE_DATA
};

#endif /* End of "#ifndef ossimGeneralRasterElevHandler_HEADER" */
