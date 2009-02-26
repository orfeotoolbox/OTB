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
// $Id: ossimGeneralRasterElevHandler.h 13524 2008-09-02 16:22:15Z gpotts $
#ifndef ossimGeneralRasterElevHandler_HEADER
#define ossimGeneralRasterElevHandler_HEADER
#include <list>
#include <ossim/base/ossimIoStream.h>
//#include <fstream>

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <ossim/imaging/ossimGeneralRasterInfo.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
class ossimProjection;
/**
 * @class ossimGeneralRasterElevHandler Elevation source for an srtm file.
 */
class  OSSIM_DLL ossimGeneralRasterElevHandler : public ossimElevCellHandler
{
public:
   class GeneralRasterInfo
      {
      public:
         GeneralRasterInfo()
         :theWidth(0),
         theHeight(0),
         theNullHeightValue(ossim::nan()),
         theScalarType(OSSIM_SCALAR_UNKNOWN),
         theBytesPerRawLine(0),
         theDatum(0),
         theProjection(0)
         {
         }
         GeneralRasterInfo(const  ossimGeneralRasterElevHandler::GeneralRasterInfo& src)
         :theFilename(src.theFilename),
         theImageRect(src.theImageRect),
         theUl(src.theUl),
         theLr(src.theLr),
         theWidth(src.theWidth),
         theHeight(src.theHeight),
         theWgs84GroundRect(src.theWgs84GroundRect),
         theNullHeightValue(src.theNullHeightValue),
         theByteOrder(src.theByteOrder),
         theScalarType(src.theScalarType),
         theBytesPerRawLine(src.theBytesPerRawLine),
         theDatum(src.theDatum),
         theProjection(src.theProjection)
         {
         }
         ossimFilename     theFilename;
         ossimIrect        theImageRect;
         ossimIpt          theUl;
         ossimIpt          theLr;
         ossim_uint32      theWidth;
         ossim_uint32      theHeight;
         ossimDrect        theWgs84GroundRect;
         ossim_float64     theNullHeightValue;
         ossimByteOrder    theByteOrder;
         ossimScalarType   theScalarType;
         ossim_uint32      theBytesPerRawLine;
         const ossimDatum* theDatum;
         ossimRefPtr<ossimProjection> theProjection;  //add by simbla
      };
   ossimGeneralRasterElevHandler(const ossimFilename& file="");
   ossimGeneralRasterElevHandler(const ossimGeneralRasterElevHandler::GeneralRasterInfo& generalRasterInfo);
   ossimGeneralRasterElevHandler(const ossimGeneralRasterElevHandler& rhs);
   const ossimGeneralRasterElevHandler& operator=(const ossimGeneralRasterElevHandler& rhs);

   /** destructor */
   virtual ~ossimGeneralRasterElevHandler();
   virtual ossimObject* dup()const;

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

   const ossimGeneralRasterElevHandler::GeneralRasterInfo& generalRasterInfo()const;
   
private:
   template <class T>
   double getHeightAboveMSLTemplate(T dummy,
                                    const ossimGeneralRasterElevHandler::GeneralRasterInfo& info,
                                    const ossimGpt& gpt);

   
   ossimGeneralRasterElevHandler::GeneralRasterInfo theGeneralRasterInfo;
   mutable ossimRefPtr<ossimIFStream> theInputStream;

TYPE_DATA
};

#endif /* End of "#ifndef ossimGeneralRasterElevHandler_HEADER" */
