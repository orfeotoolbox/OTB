//*****************************************************************************
// FILE: ossimNitfRpcModel.h
//
// License:  See top level LICENSE.txt file.
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
//  $Id: ossimNitfRpcModel.h 10251 2007-01-14 17:30:14Z dburken $

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
   virtual ossimObject* dup() const { return new ossimNitfRpcModel(*this); }

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

   TYPE_DATA
};

#endif
