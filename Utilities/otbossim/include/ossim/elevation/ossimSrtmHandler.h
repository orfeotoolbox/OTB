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
// $Id: ossimSrtmHandler.h 17195 2010-04-23 17:32:18Z dburken $
#ifndef ossimSrtmHandler_HEADER
#define ossimSrtmHandler_HEADER

#include <ossim/base/ossimIoStream.h>
//#include <fstream>

#include <ossim/base/ossimString.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <ossim/support_data/ossimSrtmSupportData.h>

class ossimEndian;

/**
 * @class ossimSrtmHandler Elevation source for an srtm file.
 */
class OSSIMDLLEXPORT ossimSrtmHandler : public ossimElevCellHandler
{
public:

   /** Constructor that takes a file name. */
   ossimSrtmHandler();
   const ossimSrtmHandler& operator=(const ossimSrtmHandler& rhs);
   ossimSrtmHandler(const ossimSrtmHandler&);


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

   virtual bool isOpen()const;
   
   /**
    * Opens a stream to the srtm cell.
    *
    * @return Returns true on success, false on error.
    */
   virtual bool open(const ossimFilename& file, bool memoryMapFlag=false);

   /**
    * Closes the stream to the file.
    */
   virtual void close();
   
protected:
   /** destructor */
   virtual ~ossimSrtmHandler();
   ossimSrtmSupportData m_supportData;
   mutable OpenThreads::Mutex m_fileStrMutex;
   std::ifstream m_fileStr;

   /** @brief true if stream is open. */
   bool          m_streamOpen;
   
   ossim_int32      m_numberOfLines;
   ossim_int32      m_numberOfSamples;
   ossim_int32      m_srtmRecordSizeInBytes;
   double           m_latSpacing;   // degrees
   double           m_lonSpacing;   // degrees
   ossimDpt         m_nwCornerPost; // cell origin;
   ossimEndian*     m_swapper;
   ossimScalarType  m_scalarType;
   
   mutable std::vector<ossim_int8> m_memoryMap;
   
   template <class T>
   double getHeightAboveMSLFileTemplate(T dummy, const ossimGpt& gpt);
   template <class T>
   double getHeightAboveMSLMemoryTemplate(T dummy, const ossimGpt& gpt);
   TYPE_DATA
};

#endif /* End of "#ifndef ossimSrtmHandler_HEADER" */
