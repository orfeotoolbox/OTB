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
// $Id: ossimDtedHandler.h 21210 2012-07-03 13:11:20Z gpotts $

#ifndef ossimDtedHandler_HEADER
#define ossimDtedHandler_HEADER

#include <fstream>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <OpenThreads/Mutex>
#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/support_data/ossimDtedDsi.h>
#include <ossim/support_data/ossimDtedAcc.h>
#include <ossim/support_data/ossimDtedRecord.h>

class OSSIM_DLL ossimDtedHandler : public ossimElevCellHandler
{
public:

   /// number of Dted posts per point.
   static const int TOTAL_POSTS = 4;
   /// number of Dted posts per block
   static const int NUM_POSTS_PER_BLOCK= 2;

   /// ossimDtedHandler
   ossimDtedHandler()
   {
      
   }
   ossimDtedHandler(const ossimFilename& dted_file, bool memoryMapFlag=false);
   

   enum
   {
      DATA_RECORD_OFFSET_TO_POST = 8,     // bytes
      DATA_RECORD_CHECKSUM_SIZE  = 4,     // bytes
      POST_SIZE                  = 2,     // bytes
      NULL_POST                  = -32767 // Fixed by DTED specification.
   };

   virtual bool open(const ossimFilename& file, bool memoryMapFlag=false);
   virtual void close();
   
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
   
   virtual bool getAccuracyInfo(ossimElevationAccuracyInfo& info, const ossimGpt& gpt) const;
   
   const ossimDtedVol& vol()const
   {
      return m_vol;
   }
   const ossimDtedHdr& hdr()const
   {
      return m_hdr;
   }
   const ossimDtedUhl& uhl()const
   {
      return m_uhl;
   }
   const ossimDtedDsi& dsi()const
   {
      return m_dsi;
   }
   const ossimDtedAcc& acc()const
   {
      return m_acc;
   }
protected:

   /// DtedPost, this class contains the height, weighting factor and status
   class DtedPost
   {
   public:
     // constructor - initialise variables
     DtedPost():
       m_height(0),
       m_weight(0),
       m_status(false)
     {
     }
     // destructor
     virtual ~DtedPost();
     // member variables
     double m_height;
     double m_weight;
     bool m_status;
   };

   /// DtedHeight is a class for storing DTED information
   /// - 4 posts are used to generate an interpolated height value.
   class DtedHeight
   {
   public:
     // constructor
     DtedHeight();
     // destructor
     virtual ~DtedHeight();
     // calculate the interpolated Height for the posts
     double calcHeight();
     // debug
     void debug();
     // post data
     DtedPost m_posts[TOTAL_POSTS];
   };


   virtual ~ossimDtedHandler();
  // Disallow operator= and copy construction...
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
   virtual double getHeightAboveMSL(const ossimGpt&, bool readFromFile);

  /**
   * read the height posts from the File
   * @param postData - post heights, status & weight
   * @param offset - file contents offset to start reading from
   */
   void readPostsFromFile(DtedHeight &postData, int offset);

   mutable OpenThreads::Mutex m_fileStrMutex;
   mutable std::ifstream m_fileStr;
   
   ossim_int32      m_numLonLines;  // east-west dir
   ossim_int32      m_numLatPoints; // north-south
   ossim_int32      m_dtedRecordSizeInBytes;
   ossimString      m_edition;
   ossimString      m_productLevel;
   ossimString      m_compilationDate;
   ossim_int32      m_offsetToFirstDataRecord;
   double           m_latSpacing;   // degrees
   double           m_lonSpacing;   // degrees
   ossimDpt         m_swCornerPost; // cell origin;

   // Indicates whether byte swapping is needed.
   bool m_swapBytesFlag;

   mutable OpenThreads::Mutex m_memoryMapMutex;
   mutable std::vector<ossim_uint8> m_memoryMap;
   
   ossimDtedVol m_vol;
   ossimDtedHdr m_hdr;
   ossimDtedUhl m_uhl;
   ossimDtedDsi m_dsi;
   ossimDtedAcc m_acc;
   TYPE_DATA
};

inline ossim_sint16 ossimDtedHandler::convertSignedMagnitude(ossim_uint16& s) const
{
   // DATA_VALUE_MASK 0x7fff = 0111 1111 1111 1111
   // DATA_SIGN_MASK  0x8000 = 1000 0000 0000 0000
   
   // First check to see if the bytes need swapped.
   s = (m_swapBytesFlag ? ( ((s & 0x00ff) << 8) | ((s & 0xff00) >> 8) ) : s);
   
   // If the sign bit is set, mask it out then multiply by negative one.
   if (s & 0x8000)
   {
      return (static_cast<ossim_sint16>(s & 0x7fff) * -1);
   }
   
   return static_cast<ossim_sint16>(s);
}

inline bool ossimDtedHandler::isOpen()const
{
   if(!m_memoryMap.empty()) return true;
   
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_fileStrMutex);
   return (m_fileStr.is_open());
}

inline void ossimDtedHandler::close()
{
   m_fileStr.close();
   m_memoryMap.clear();
}

#endif
