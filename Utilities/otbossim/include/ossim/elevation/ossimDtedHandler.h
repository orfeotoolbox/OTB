//*****************************************************************************
// FILE: ossimDtedHandler.h
//
// License:  See top level LICENSE.txt file.
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
// $Id: ossimDtedHandler.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimDtedHandler_HEADER
#define ossimDtedHandler_HEADER

#include <fstream>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <OpenThreads/ReadWriteMutex>
#include <OpenThreads/ReadWriteMutex>

class OSSIM_DLL ossimDtedHandler : public ossimElevCellHandler
{
public:
   ossimDtedHandler(const ossimFilename& dted_file, bool memoryMapFlag=false);
   

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

   virtual bool isOpen()const;
   /**
    * Opens a stream to the dted cell.
    *
    * @return Returns true on success, false on error.
    */
   virtual bool open();

   /**
    * Closes the stream to the file.
    */
   virtual void close();

   virtual void setMemoryMapFlag(bool flag);
   
protected:
   virtual ~ossimDtedHandler();
  // Disallow operator= and copy constrution...
   const ossimDtedHandler& operator=(const ossimDtedHandler& rhs);
   ossimDtedHandler(const ossimDtedHandler&);

   /*!
    *  If statistics file exist, stats will be initialized from that; else,
    *  this scans the dted cell and gets stats, then, writes new stats file.
    *  The statistics file will be named accordingly:
    *  If dted cell = n27.dt1 then the stats file = n27.statistics.
    *  Currently method only grabs the min and max posts value.
    */
   void gatherStatistics();

   ossim_sint16 convertSignedMagnitude(ossim_uint16& s) const;
   virtual double getHeightAboveMSLFile(const ossimGpt&);
   virtual double getHeightAboveMSLMemory(const ossimGpt&);
   void mapToMemory();
   
   mutable OpenThreads::Mutex theFileStrMutex;
   mutable std::ifstream theFileStr;
   
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

   mutable OpenThreads::ReadWriteMutex theMemoryMapMutex;
   mutable bool theMemoryMapFlag;
   mutable std::vector<ossim_uint8> theMemoryMap;
   
   TYPE_DATA
};

inline ossim_sint16 ossimDtedHandler::convertSignedMagnitude(ossim_uint16& s) const
{
   // DATA_VALUE_MASK 0x7fff = 0111 1111 1111 1111
   // DATA_SIGN_MASK  0x8000 = 1000 0000 0000 0000
   
   // First check to see if the bytes need swapped.
   s = (theSwapBytesFlag ? ( ((s & 0x00ff) << 8) | ((s & 0xff00) >> 8) ) : s);
   
   // If the sign bit is set, mask it out then multiply by negative one.
   if (s & 0x8000)
   {
      return (static_cast<ossim_sint16>(s & 0x7fff) * -1);
   }
   
   return static_cast<ossim_sint16>(s);
}

inline bool ossimDtedHandler::isOpen()const
{
   if(theMemoryMapFlag)
   {
      OpenThreads::ScopedReadLock lock(theMemoryMapMutex);
      return theMemoryMap.size()>0;
   }
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theFileStrMutex);
      return theFileStr.is_open();
   }
}

inline bool ossimDtedHandler::open()
{
   if(theMemoryMapFlag)
   {
      theMemoryMapMutex.readLock();
      if(theMemoryMap.size())
      {
         theMemoryMapMutex.readUnlock();
         return true;
      }
      else
      {
         theMemoryMapMutex.readUnlock();
         mapToMemory();
         return (theMemoryMap.size()>0);
      }
   }
   else
   {
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theFileStrMutex);
      if (theFileStr.is_open())
      {
         return true;
      }
      
      theFileStr.open(theFilename.c_str(), std::ios::in | std::ios::binary);
      
      return theFileStr.is_open();
   }
   
   return false;
}

inline void ossimDtedHandler::close()
{
   {  
      OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theFileStrMutex);
      if (theFileStr.is_open())
      {
         theFileStr.close();
      }
   }
   {
      OpenThreads::ScopedWriteLock lock(theMemoryMapMutex);
      theMemoryMap.clear();
   }
}

#endif
