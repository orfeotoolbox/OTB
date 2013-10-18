//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimRadarSat2RPCModel_H
#define ossimRadarSat2RPCModel_H 1

#include <ossim/base/ossimConstants.h>
#include <ossim/projection/ossimRpcModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <iostream>
#include <list>
#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

#include <otb/CivilDateTime.h>

class ossimFilename;
class ossimString;
class ossimXmlDocument;

namespace ossimplugins
{

class ossimRadarSat2ProductDoc;
class SensorParams;

/**
 * @brief This class allows for direct localisation and indirect localisation
 * using the RadarSat2 sensor model
 */
class OSSIM_PLUGINS_DLL ossimRadarSat2RPCModel : public ossimRpcModel
{
public:
   /** @brief default constructor */
   ossimRadarSat2RPCModel();

   /** @brief copy constructor */
   ossimRadarSat2RPCModel(const ossimRadarSat2RPCModel& rhs);

    ossimRadarSat2RPCModel(const ossimFilename& RS2File);


   /** @brief Destructor */
   virtual ~ossimRadarSat2RPCModel();

   /**
    * @brief Method to return the class name.
    * @return The name of this class.
    */
   virtual ossimString getClassName()   const;

   /**
    * @brief Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const;


	   /**
    * @brief Method to parse an nitf file and initialize model.
    * @param entryIndex The entry to get model for.  Note the nitf's can have
    * multiple entries; each with a different projection.
    * @return true on success, false on error.
    */
    bool open(const ossimFilename& file);


   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   virtual bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);

   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;


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


private:


   bool InitSensorParams(const ossimKeywordlist &kwl,
                                 const char *prefix);


   bool initSensorParams(const ossimXmlDocument* xdoc,
                         const ossimRadarSat2ProductDoc& rsDoc);


   /**
    * Stored from header field "IMAG".  This is a temp work around to handle
    * decimated rpc nitf's.
    */
   ossim_float64 theDecimation;

   SensorParams * _sensor;



   ossimFilename theProductXmlFile;

TYPE_DATA

};
}

#endif
