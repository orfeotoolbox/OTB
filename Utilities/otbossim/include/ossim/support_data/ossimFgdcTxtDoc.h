//----------------------------------------------------------------------------
//
// File ossimFgdcTxtDoc.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ossimFgdcTxtDoc class declaration.
//
// FGDC = "Federal Geographic Data Committee"
//
// See: http://www.fgdc.gov/
// 
//----------------------------------------------------------------------------
// $Id: ossimFgdcTxtDoc.h 2673 2011-06-06 14:57:24Z david.burken $
#ifndef ossimFgdcTxtDoc_HEADER
#define ossimFgdcTxtDoc_HEADER 1

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>

/**
 * @class ossimFgdcTxtDoc
 * @brief Support data container for FGDC in text format.
 *
 * Has minimum parse support to get Spatial_Reference_Information section to
 * extract projection information.
 *
 * TODO: Make an ossimFgdcBase and consolidate code from
 * ossimFgdcXmlDoc and this class. (drb - 15 Aug. 2011)
 */
class OSSIM_DLL ossimFgdcTxtDoc : public ossimReferenced
{
public:

   /** default constructor */
   ossimFgdcTxtDoc();

   /** virtual destructor */
   virtual ~ossimFgdcTxtDoc();

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   bool open(const ossimFilename& file);

   /**
    * @brief Close method.
    *
    * This doesn't really do anything.  Here only because I think every open()
    * should have a matching close().
    */
   void close();

   /**
    * @brief Gets projection from Spatial_Reference_Information block.
    * @param proj Intialized by this method.  Set to null on projection cannot
    * be created.
    */   
   void getProjection(ossimRefPtr<ossimProjection>& proj);

   /**
    * @brief Gets units from Altitude_Distance_Units.
    * @return Units if found, OSSIM_UNIT_UNKNOWN if not.
    */
   void getAltitudeDistanceUnits(std::string& units) const;

private:

   /**
    * @brief Gets projection from Spatial_Reference_Information block for
    * version FGDC-STD-001-1998.
    * @param str Stream to read from.
    * @param proj Intialized by this method.  Set to null on projection cannot
    * be created.
    */ 
   void getProjectionV1(std::ifstream& str,
                        ossimRefPtr<ossimProjection>& proj);

   /**
    * @brief Finds key and returns true if present.
    *
    * Leaves stream at position of last getline.
    * 
    * @param str Stream to read from.
    * @param key Key to find.
    * @return true if present false if not.
    */
   bool findKey( std::ifstream& str, const std::string& key);

   /**
    * @brief Finds key and returns and intializes value if present.
    *
    * Stream position at end of call is dependent on seekBack flag.
    * 
    * @param str Stream to read from.
    * @param seekBack If true the stream will be repositioned to original
    * position at beginning of the call.
    * @param key Key to find.
    * @param value Intialized with value if key is found.
    * @return true if present false if not.
    */
   bool findKey( std::ifstream& str,
                 bool seekBack,
                 const std::string& key,
                 std::string& value);

   /**
    * Gets ossim datum string from fgdc datum string.
    */
   void getOssimDatum( const std::string& fgdcDatumString,
                       std::string& ossimDatumCode ) const;

   // Container for relevant data from FGDC file.
   ossimRefPtr<ossimKeywordlist> m_kwl;
   
}; // End: class ossimFgdcTxtDoc

#endif /* #ifndef ossimFgdcTxtDoc_HEADER */
