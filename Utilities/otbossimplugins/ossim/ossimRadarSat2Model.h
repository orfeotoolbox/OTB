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

#ifndef ossimRadarSat2Model_H
#define ossimRadarSat2Model_H 1

#include <ossimPluginConstants.h>
#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <iostream>
#include <list>

#include <otb/CivilDateTime.h>
#include <otb/RadarSat2NoiseLevel.h>

class ossimFilename;
class ossimString;
class ossimXmlDocument;

namespace ossimplugins
{

  class ossimRadarSat2ProductDoc;

class PlatformPosition;
class SensorParams;
class RefPoint;

/**
 * @brief This class allows for direct localisation and indirect localisation
 * using the RadarSat2 sensor model
 */
class OSSIM_PLUGINS_DLL ossimRadarSat2Model : public ossimGeometricSarSensorModel
{
public:
   /** @brief default constructor */
   ossimRadarSat2Model();

   /** @brief copy constructor */
   ossimRadarSat2Model(const ossimRadarSat2Model& rhs);

   /** @brief Destructor */
   virtual ~ossimRadarSat2Model();

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
    * @brief This function associates an image column number to a slant range
    * when the image is georeferenced (ground projected)
    * @param col Column coordinate of the image point
    */
   virtual double getSlantRangeFromGeoreferenced(double col) const;

   /**
    * @brief Method to intantial model from a file.  Attempts to find the
    * required xml file.
    *
    * @param file
    *
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

private:

   virtual bool InitPlatformPosition(const ossimKeywordlist &kwl,
                                     const char *prefix);

   virtual bool InitSensorParams(const ossimKeywordlist &kwl,
                                 const char *prefix);

   virtual bool InitRefPoint(const ossimKeywordlist &kwl,
                             const char *prefix);

   virtual bool InitRefNoiseLevel(const ossimXmlDocument* xmlDocument);
   virtual bool InitLut( const ossimXmlDocument* xmlDocument,
   						 RadarSat2NoiseLevel& noise);

   /**
    * @brief Initializes the Slant Range to Ground Range data sets :
    * _srgr_update,_SrGr_R0,_SrGr_coeffs_number,_SrGr_coeffs,_nbCol,
    * _pixel_spacing
    */
   virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

   /**
    * @brief Finds the SRGR data set which update time is the closest
    * to the center scene time
    */
   int FindSRGRSetNumber(JSDDateTime date)  const;

   /**
    * @note This method must be called before initRefPoint.
    */
   bool initSRGR(const ossimXmlDocument* xdoc,
                 const ossimRadarSat2ProductDoc& rsDoc);

   bool initPlatformPosition(const ossimXmlDocument* xdoc,
                             const ossimRadarSat2ProductDoc& rsDoc);

   bool initSensorParams(const ossimXmlDocument* xdoc,
                         const ossimRadarSat2ProductDoc& rsDoc);

   /**
    * @brief Method to initialize RefPoint object from
    * RadarSat "product.xml" file.
    * @param xdoc Opened product.xml file.
    * @return true on success, false on error.
    */
   bool initRefPoint(const ossimXmlDocument* xdoc,
                     const ossimRadarSat2ProductDoc& rsDoc);

   /**
    * @brief Sets ossimSensorModel members theRefImgPt and theRefGndPt from
    * tie points.
    * @param groundGcpCoordinates Ground tie points.
    * @param imageGcpCoordinates Image tie points.
    * @return true on success, false on error.
    */
   bool setModelRefPoint(const std::list<ossimGpt>& groundGcpCoordinates,
                         const std::list<ossimDpt>& imageGcpCoordinates);

   /**
    * @brief Slant Range FOR EACH Ground Range (SRGR) number of coefficients
    * sets
    */
   int   _n_srgr;

   /**
    * @brief Slant Range FOR EACH Ground Range coefficient sets update times
    */

   std::vector<double> _srgr_update;
   /**
    * @brief Slant Range FOR EACH Ground Range Projection reference point
    */
   std::vector<double> _SrGr_R0 ;

   /**
    * @brief Slant Range FOR EACH Ground Range Projection coefficients
    */
   std::vector< std::vector<double> > _SrGr_coeffs ;

   /**
    * @brief noise level values
    */
   std::vector< RadarSat2NoiseLevel > _noiseLevel ;


TYPE_DATA

};
}

#endif
