//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimSentinel1Model_HEADER
#define ossimSentinel1Model_HEADER

#include <iosfwd>
#include <boost/config.hpp>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlDocument.h>

//#include <ossim/projection/ossimSensorModel.h>
//#include <ossim/projection/ossimCoarseGridModel.h>
#include "ossimSarSensorModel.h"
#include "ossimPluginConstants.h" // OSSIM_PLUGINS_DLL

#include <ossim/support_data/ossimSupportFilesList.h>

//#include "ossimGeometricSarSensorModel.h"

class OSSIMDLLEXPORT ossimXmlNode;
class OSSIMDLLEXPORT ossimString;

namespace ossimplugins
{

   class OSSIM_PLUGINS_DLL ossimSentinel1Model : public ossimSarSensorModel
   {
   public:
      typedef ossimRefPtr<ossimXmlNode>     ossimXmlNodePtr;
      typedef ossimRefPtr<ossimXmlDocument> ossimXmlDocumentPtr;

      /**
       * CONSTRUCTORS.
       * @{
       */
      /** @brief Default constructor */
      ossimSentinel1Model();

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
      /** @brief Copy constructor */
      ossimSentinel1Model(ossimSentinel1Model const& rhs) = default;

      /** @brief Move constructor */
      ossimSentinel1Model(ossimSentinel1Model && rhs) = default;

      /** @brief Destructor */
      virtual ~ossimSentinel1Model() = default;
#endif
      //@}

      bool open(const ossimFilename& file);

      /*!
       * Returns pointer to a new instance, copy of this.
       */
      virtual ossimObject* dup() const;

      /*!
       * Extends base-class implementation. Dumps contents of object to ostream.
       */
      virtual std::ostream& print(std::ostream& out) const;

      /*!
       * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
       * KWL files. Returns true if successful.
       */
      virtual bool saveState(ossimKeywordlist& kwl,
            const char* prefix=NULL) const;

      virtual bool loadState(ossimKeywordlist const& kwl,
            const char* prefix=NULL);

      bool checkDirectory(const ossimFilename& file, const char* d, const char *ext) const;

      bool isLevel0(ossimFilename const& file) const;

      bool isLevel2(ossimFilename const& file) const;

      bool isLevel1(ossimFilename const& file) const;

      bool isSentinel1(ossimFilename const &manifestFile);

      bool readProduct(ossimFilename const &productXmlFile);

      ossimString const& getImageId(ossimXmlDocument const& manifestDoc) const;

      ossimString initSensorID(ossimXmlDocument const& manifestDoc);

      bool standAloneProductInformation(ossimXmlDocument const& manifestDoc);

      // bool getAnnotationFileLocation(const ossimFilename &manifestFile, const char* pattern);

      bool initImageSize(ossimIpt& imageSize) const;

      virtual void imagingRay(ossimDpt const& image_point, ossimEcefRay& image_ray) const;

      bool isSLC() const { return  theSLC; }
      bool isOCN() const { return  theOCN; }

#if 0
      bool initSRGR();
#endif

      ossimFilename searchManifestFile(const ossimFilename& file) const;

      void clearFields();

      double getBandTerrainHeight(ossimXmlDocument const& theProductXmlDocument);

      void setMetadataDirectory(ossimFilename const& d)
      {
         theManifestDirectory = d;
      }
   protected:

      TYPE_DATA;

   private:
      bool read(ossimFilename const& annotationXml);
      void readCalibrationMetadata();
      void readNoiseMetadata();
      void readBurstRecords(ossimXmlNode const& productRoot, ossimXmlNode const& imageInformation);
      void addSRGRCoefficients(ossimXmlNode const& coordinateConversion);
      void readCoordinates(
            ossimXmlNode const& node,
            ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
            std::string const& sr_gr_prefix);
      void readGeoLocationGrid(ossimXmlNode const& productRoot);
      void addOrbitStateVectors(ossimXmlNode const& orbitList);
      void addDopplerCentroidCoefficients(ossimXmlNode const& dcEstimateList);
      bool openMetadataFile(ossimXmlDocument& doc, ossimString const& file) const;

      ossimFilename      theManifestDirectory;
      ossimFilename      theProductXmlFile;
      // ossimFilename      theManifestFile;
      ossimKeywordlist   theManifestKwl;
      ossimKeywordlist   theProductKwl;
      bool               theOCN;
      bool               theSLC;
      bool               theTOPSAR;
   }; //end class ossimSentinel1Model

}  //end namespace ossimplugins


#endif /* #ifndef ossimSentinel1Model_HEADER */
