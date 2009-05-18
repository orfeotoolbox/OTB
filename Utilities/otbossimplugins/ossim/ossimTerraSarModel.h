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
#include <iostream>
#include <list>

#include <otb/CivilDateTime.h>

class PlatformPosition;
class SensorParams;
class RefPoint;
class ossimXmlDocument;
class ossimTerraSarProductDoc;


/**
 * @brief This class allows for direct localisation and indirect localisation
 * using the TerraSar sensor model
 */
class OSSIM_PLUGINS_DLL ossimTerraSarModel : public ossimGeometricSarSensorModel
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

protected:


private:
   virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
   virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
   virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
   /**
    * @brief Initializes the Slant Range to Ground Range data sets : 
    *				_SrToGr_R0,_SrToGr_coeffs_number,_SrToGr_exponent,_SrToGr_coeffs,_nbCol
    */
   virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

   bool UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate);

   
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

   ossimFilename theProductXmlFile;

   TYPE_DATA
};

#endif /* #ifndef ossimTerraSarModel_H */
