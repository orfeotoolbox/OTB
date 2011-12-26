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

#ifndef ossimGeometricSarSensorModel_H
#define ossimGeometricSarSensorModel_H

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/base/ossimRtti.h>
#include <iostream>
#include <list>
#include <cstdlib>
#include <vector>
#include <ossim/projection/ossimCoarseGridModel.h>
#include <otb/CivilDateTime.h>
#include <ossim/elevation/ossimElevManager.h>

namespace ossimplugins
{

class PlatformPosition;
class SensorParams;
class RefPoint;

/**
 * @brief This class allows for direct localisation and indirect localisation
 * using the geometric model of SAR sensors.
 * This class must be specified for each sensor.
 */
class ossimGeometricSarSensorModel : public ossimSensorModel
{
public:
   static const char* CREATE_OCG_PREF_KW;

   /** @brief default constructor */
   ossimGeometricSarSensorModel();

   /** @brief copy constructor */
   ossimGeometricSarSensorModel(const ossimGeometricSarSensorModel& rhs);

   /** @brief Destructor */
   virtual ~ossimGeometricSarSensorModel();

   /**
    * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
    * @param col Column coordinate of the image point
    */
   virtual double getSlantRangeFromGeoreferenced(double col) const=0;

   /**
    * @brief This function associates an image column number to a slant range
    * @param col Column coordinate of the image point
    */
   virtual double getSlantRange(double col) const;

   /**
    * @brief This function associates an image line number to an azimuth time
    * @param line Line coordinate of the image point
    */
   virtual JSDDateTime getTime(double line) const;
 
   /**
    * @brief This function associates an image line number to a platform position and speed
    * @param line Line coordinate of the image point
    * @param position Position of the sensor at line line
    * @param speed Speed of the sensor at line line
    */
  virtual bool getPlatformPositionAtLine(double line, vector<double>& position, vector<double>& speed);

   /**
    * @brief This function is able to convert image coordinates into world
    * coordinates the geometric model of SAR sensors.
    * @param image_point Coordinates of the image point
    * @param heightEllipsoid Altitude of the world point
    * @param worldPoint Coordinates of the world point (OUT)
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;


   /**
    * @brief This function optimizes the model according to a list of Ground
    * Control Points.
    *
    * The input list of GCPs is appended to the one present in memory.
    * An inverse localization of each ground GCP is performed.
    * The error between localized GCPs and actual image coordinates is then
    * computed and modeled by a linear model on each dimension.
    * The time and range estimations are updated accordingly.
    *
    * @param groundCoordinates : input GCP ground coordinates
    * @param imageCoordinates : actual image coordinates corresponding to
    * input groundCoordinates
    *
    */
   virtual bool optimizeModel(const std::list<ossimGpt> & groundCoordinates,
                              const std::list<ossimDpt> & imageCoordinates) ;

   /**
    * @brief Clears _optimizationGCPsGroundCoordinates and
    * _optimizationGCPsImageCoordinates attributes
    * Updates the optimisation factors and bias
    */
   virtual void clearGCPlist() ;

   /**
    * @brief Returns _optimizationGCPsGroundCoordinates and
    * _optimizationGCPsImageCoordinates attributes
    *
    * @retval groundCoordinates : GCP ground coordinates
    * @retval imageCoordinates : actual image coordinates corresponding to
    * groundCoordinates
    */
   virtual void getGCPlist(std::list<ossimGpt> &groundCoordinates,
                           std::list<ossimDpt> & imageCoordinates) ;



   /**
    * @brief
    * @return true when it's better (more accurate / fast) to use forward
    * (from ground to image) than inverse(from image to ground)
    * @remark This function always return false
    */
   inline virtual bool useForward() const {return false;}

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
    * @brief Accessors to the optimization parameters.
    */
   double get_optimizationFactorX() const { return _optimizationFactorX; }
   double get_optimizationFactorY() const { return _optimizationFactorY; }
   double get_optimizationBiasX()   const { return _optimizationBiasX; }
   double get_optimizationBiasY()   const { return _optimizationBiasY; }

   ossimRefPtr<ossimCoarseGridModel> getReplacementOcgModel() { return _replacementOcgModel; }

   virtual void lineSampleToWorld(const ossimDpt& image_point,
                                  ossimGpt&       gpt) const;

protected:
   /**
    * @brief Creates replacement coarse grid model if user requested via ossim preferences 
    * keyword "geometric_sar_sensor_model.create_ocg: <bool>"
    * @return true if load OK, false on error
    */
   bool createReplacementOCG();

   /**
    * @brief Handle the position of the platform
    */
   PlatformPosition *_platformPosition;
   SensorParams * _sensor;
   RefPoint * _refPoint;

   /**
    * @brief True iff the product is ground range
    */
   bool _isProductGeoreferenced ;

   /**
    * @brief List Ground Control Points used by the optimization
    */
   std::list<ossimGpt> _optimizationGCPsGroundCoordinates ;
   std::list<ossimDpt> _optimizationGCPsImageCoordinates  ;

   /**
    * @brief Optimization result : linear error correction in both dimensions
    */
   double _optimizationFactorX ;
   double _optimizationFactorY ;
   double _optimizationBiasX ;
   double _optimizationBiasY ;

   ossimFilename _imageFilename; 
   ossimFilename _productXmlFile;

   ossimRefPtr<ossimCoarseGridModel> _replacementOcgModel;

private:
   /**
    * @brief Initializes the Platform Position from a projection keywordlist
    */
   virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)=0;
   /**
    * @brief Initializes the Sensor Params from a projection keywordlist
    */
   virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)=0;
   /**
    * @brief Initializes the Reference Point from a projection keywordlist
    */
   virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)=0;
   /**
    * @brief Initializes the Slant Range to/for each Ground Range data sets from a projection keywordlist
    */
   virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix)=0;


TYPE_DATA

};
}

#endif
