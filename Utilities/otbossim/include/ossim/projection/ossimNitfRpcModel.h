//*****************************************************************************
// FILE: ossimNitfRpcModel.h
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains declaration of class ossimNitfRpcModel. This 
//    derived class implements the capability of reading Nitf RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimNitfRpcModel.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimNitfRpcModel_HEADER
#define ossimNitfRpcModel_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/projection/ossimRpcModel.h>

class ossimFilename;
class ossimNitfImageHeader;

/*!****************************************************************************
 *
 * CLASS:  ossimNitfRpcModel
 *
 *****************************************************************************/
class OSSIM_DLL ossimNitfRpcModel : public ossimRpcModel
{
public:
   ossimNitfRpcModel();
   ossimNitfRpcModel(const ossimNitfRpcModel& rhs);
   ossimNitfRpcModel(const ossimFilename& nitfFile);
   virtual ossimObject* dup() const;

   /**
    * @brief worldToLineSample()
    * Calls ossimRpcModel::worldToLineSample(), then applies (if needed)
    * decimation.
    * This is a temp work around for decimation RPC NITFs.
    */
   virtual void  worldToLineSample(const ossimGpt& world_point,
                                   ossimDpt&       image_point) const;

   /**
    * @brief lineSampleHeightToWorld()
    * Backs out decimation of image_point (if needed) then calls:
    * ossimRpcModel::lineSampleHeightToWorld
    * This is a temp work around for decimation RPC NITFs.
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;

   /**
    * @brief Saves "decimation".  Then calls ossimRpcModel::saveState.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /**
    * @brief Looks for decimation. Then calls ossimRpcModel::loadState.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * @brief Method to parse an nitf file and initialize model.
    * @param entryIndex The entry to get model for.  Note the nitf's can have
    * multiple entries; each with a different projection.
    * @return true on success, false on error.
    */
   virtual bool parseFile(const ossimFilename& nitfFile,
                          ossim_uint32 entryIndex=0);
   
   virtual bool parseImageHeader(const ossimNitfImageHeader* ih);
private:

   /**
    * Attempts to get gsd from nitf tags and initialize
    * ossimSensorModel::theGsd.
    * 
    * If unsuccessful  theGsd will be initialized to OSSIM_DBL_NAN.
    */
   void getGsd(const ossimNitfImageHeader* ih);

   /**
    * Attempts to get sensor ID from nitf tags and initialize
    * ossimSensorModel::theSensorID.
    * 
    * If unsuccessful  theGsd will be initialized to UNKNOWN.
    */
   void getSensorID(const ossimNitfImageHeader* ih);

   /**
    * Get model information from either the RPC00A or RPC00B tag.
    * @return true on success, false on error.
    */
   bool getRpcData(const ossimNitfImageHeader* ih);

   /**
    * Stored from header field "IMAG".  This is a temp work around to handle
    * decimated rpc nitf's.
    */
   ossim_float64 theDecimation;

   TYPE_DATA
};

#endif
