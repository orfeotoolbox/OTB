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

#ifndef ossimTerraSarModel_H
#define ossimTerraSarModel_H

#include <ossimPluginConstants.h>
#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimString.h>
#include <iostream>
#include <list>

#include <otb/CivilDateTime.h>
#include <otb/Noise.h>
#include <otb/SceneCoord.h>

// Forward class declarations outside of namespace ossimplugins.
class ossimXmlDocument;

namespace ossimplugins
{
   // Forward class declarations inside of namespace ossimplugins.
   class ossimTerraSarProductDoc;
   
   /**
    * @brief This class allows for direct localisation and indirect localisation
    * using the TerraSar sensor model
    */
   class OSSIM_PLUGINS_DLL ossimTerraSarModel :
   public ossimplugins::ossimGeometricSarSensorModel
   {
   public:
      /** @brief default constructor */
      ossimTerraSarModel();
      
      /** @brief copy constructor */
      ossimTerraSarModel(const ossimTerraSarModel& rhs);
      
      /** @brief Destructor */
      virtual ~ossimTerraSarModel();
      
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
       * @brief This function associates an image column number to a slant
       * range when the image is georeferenced (ground projected).
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

      //---
   	  // Convenient method to print important image info:
      //---
      void  printInfo (ostream& os) const;
   
      private:

      virtual bool InitPlatformPosition(const ossimKeywordlist &kwl,
                                        const char *prefix);
      virtual bool InitSensorParams(const ossimKeywordlist &kwl,
                                    const char *prefix);
      virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);

      /**
       * @brief Initializes the Slant Range to Ground Range data sets : 
       * _SrToGr_R0,_SrToGr_coeffs_number,_SrToGr_exponent,_SrToGr_coeffs,_nbCol
       */
      virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

      /**
       * @note This method must be called before initRefPoint.
       */
      bool initSRGR(const ossimXmlDocument* xdoc,
                    const ossimTerraSarProductDoc& rsDoc);
   
      bool initPlatformPosition(const ossimXmlDocument* xdoc,
                                const ossimTerraSarProductDoc& rsDoc);

      bool initSensorParams(const ossimXmlDocument* xdoc,
                            const ossimTerraSarProductDoc& rsDoc);

      /**
       * @brief Method to initialize RefPoint object from
       * TerraSAR product xml file.
       * @param xdoc Opened product xml file.
       * @return true on success, false on error.
       */
      bool initRefPoint(const ossimXmlDocument* xdoc,
                        const ossimTerraSarProductDoc& rsDoc);
      /**
       * @brief Method to initialize AcquisitionInfo parameters from
       * TerraSAR product xml file.
       * @param xdoc Opened product xml file.
       * @return true on success, false on error.
       */
      bool initAcquisitionInfo( const ossimXmlDocument* xdoc, 
                        const ossimTerraSarProductDoc& tsDoc);

      /**
       * @brief Method to initialize Calibration parameters from
       * TerraSAR product xml file.
       * @param xdoc Opened product xml file.
       * @return true on success, false on error.
       */
	  bool initCalibration( const ossimXmlDocument* xmlDocument, 
                        const ossimTerraSarProductDoc& tsDoc);

      /**
       * @brief Method to initialize InfoIncidenceAngle parameters from
       * TerraSAR product xml file.
       * @param xdoc Opened product xml file.
       * @return true on success, false on error.
       */
      bool initSceneCoord(
		     const ossimXmlDocument* xdoc, const ossimTerraSarProductDoc& tsDoc);

      /**
       * @brief Method to initialize ImageNoise parameters from
       * TerraSAR product xml file.
       * @param xdoc Opened product xml file.
       * @return true on success, false on error.
       */
      bool initNoise( const ossimXmlDocument* xmlDocument, const ossimTerraSarProductDoc& tsDoc);

      /**
       * @brief Method to get ImageNoise parameters from
       * TerraSAR product xml file at a given node.
       * @param xdoc Opened product xml file.
       * @return true on success, false on error.
       */
      bool getNoiseAtGivenNode( const ossimRefPtr<ossimXmlNode> xmlDocument, ossimplugins::Noise& noise);

      /**
       * @brief Method to find the metadata file
       * TerraSAR file (image or xml).
       * @param file image or metadata path.
       * @param metadataFile matadata path.
       * @return ture if mateadata found, false otherwise.
       */
      bool findTSXLeader(const ossimFilename& file, ossimFilename& metadataFile);


      /**
       * @brief Method to find the polLayer from the image file
       * @param xdoc Opened product xml file.
       * @param file image or metadata path.
       * @return true on success, false on error.
       */
       bool getPolLayerFromImageFile( const ossimXmlDocument* xmlDocument, const ossimFilename& imageFilename);

      /**
       * @brief Slant Range TO Ground Range Projection reference point
       */
      double _SrToGr_R0;
   
      /**
       * @brief Slant Range TO Ground Range Projection exponents
       */
      std::vector<int> _SrToGr_exponent;

      /**
       * @brief Slant Range TO Ground Range Projection coefficients
       */
      std::vector<double> _SrToGr_coeffs;
      /**
       * @brief Scene Center range time
       */
      double _sceneCenterRangeTime;
   
      /**
       * @brief Slant Range TO Ground Range scaling factor at scene center
       */
      double _SrToGr_scaling_factor;

      /**
       * @brief alternative SRGR computation coefficients. Three doubles.
       */
      std::vector<double> _alt_srgr_coefset;

      /**
       * @brief ProductType.
       */
      ossimString _productType;

      /**
       * @brief RadiometricCorrection.
       */
      ossimString _radiometricCorrection;

      /**
       * @brief ImagingMode (AcquisitionInfo node).
       */
      ossimString _imagingMode;

      /**
       * @brief Sensor (AcquisitionInfo node).
       */
      ossimString _acquisitionSensor;

      /**
       * @brief LookDirection (AcquisitionInfo node).
       */
      ossimString _lookDirection;

      /**
       * @brief PolarisationMode (AcquisitionInfo node).
       */
      ossimString _polarisationMode;

      /**
       * @brief PolLayer (AcquisitionInfo node).
       */
      ossimString _polLayer;      
      std::vector<ossimString> _polLayerList;

      /**
       * @brief Noise (Noise node).
       */
      std::vector<Noise>  _noise;
      
      /**
       * @brief SceneCoord (SceneInfo node)
       */
      SceneCoord *_sceneCoord;
      
      /**
       * @brief CalFactor (Calibration node).
       */
      std::vector<double> _calFactor;

      /**
       * @brief centerFrequency (instrument node).
       */
      double _radarFrequency;

      /**
       * @brief number of layer (ImageDataInfo node).
       */
      ossim_uint32 _numberOfLayers;

      /**
       * @brief Azimuthal Start Time (Start acquisition time).
       */
      ossimString _azStartTime;

      /**
       * @brief Azimuthal Stop Time (Start acquisition time).
       */
      ossimString _azStopTime;

      /**
       * @brief First pixel Range Time (Start acquisition time in range direction).
       */
      ossimString _rgFirstPixelTime;

      /**
       * @brief Last pixel Range Time (Start acquisition time in range direction).
       */
      ossimString _rgLastPixelTime;


      /**
       * @brief Generation time.
       */
       ossimString _generationTime;

      ossimFilename theProductXmlFile;

      TYPE_DATA

   };

} // End: namespace ossimplugins

#endif /* #ifndef ossimTerraSarModel_H */
