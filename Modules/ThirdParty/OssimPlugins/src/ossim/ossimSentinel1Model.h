/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ossimSentinel1Model_HEADER
#define ossimSentinel1Model_HEADER

#include <iosfwd>
#include <string>
#include <boost/config.hpp>

#include "ossimPluginConstants.h" // OSSIM_PLUGINS_DLL

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlDocument.h>

//#include <ossim/projection/ossimSensorModel.h>
//#include <ossim/projection/ossimCoarseGridModel.h>
#include "ossim/ossimSarSensorModel.h"

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
      virtual ossimObject* dup() const override;

      /*!
       * Extends base-class implementation. Dumps contents of object to ostream.
       */
      virtual std::ostream& print(std::ostream& out) const override;

      /*!
       * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
       * KWL files. Returns true if successful.
       */
      virtual bool saveState(ossimKeywordlist& kwl,
            const char* prefix=NULL) const override;

      virtual bool loadState(ossimKeywordlist const& kwl,
            const char* prefix=NULL) override;

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

      virtual void imagingRay(ossimDpt const& image_point, ossimEcefRay& image_ray) const override;

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
#if defined(__clang__)
#pragma clang diagnostic push

#if defined(__apple_build_version__)
/* Apple's 3.6.0 based clang doesn't support -Winconsistent-missing-override */
#if __apple_build_version__ >= 7000053
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif
#elif __clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 7)
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif
      TYPE_DATA
#pragma clang diagnostic pop
#else
      TYPE_DATA
#endif

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
      void addAzimuthFmRateCoefficients(ossimXmlNode const& aziFmRateList);
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
