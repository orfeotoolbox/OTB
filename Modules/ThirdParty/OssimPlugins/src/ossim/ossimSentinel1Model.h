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

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>

//#include <ossim/projection/ossimSensorModel.h>
//#include <ossim/projection/ossimCoarseGridModel.h>
#include <ossim/projection/ossimSarModel.h>

#include <ossim/support_data/ossimSupportFilesList.h>

#include "ossimPluginConstants.h"
#include "ossimPluginCommon.h"
#include <iostream>
#include <cmath>
#include <cstdio>

//#include "ossimGeometricSarSensorModel.h"

#include "ossimSentinel1ProductDoc.h"

//ossimGeometricSarSensorModel

namespace ossimplugins
{

   class OSSIM_PLUGINS_DLL ossimSentinel1Model : public ossimSarModel
   {
   public:
      /*!
       * CONSTRUCTORS:
       */
      /** @brief default constructor */
      ossimSentinel1Model();

      /** @brief copy constructor */
      ossimSentinel1Model(const ossimSentinel1Model& rhs);

      /** @brief Destructor */
      virtual ~ossimSentinel1Model();


      bool open(const ossimFilename& file);

      /*!
       * Returns pointer to a new instance, copy of this.
       * Not implemented yet!  Returns NULL...
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

      virtual bool loadState(const ossimKeywordlist& kwl,
                             const char* prefix=NULL);

    bool checkDirectory(const ossimFilename& file, const char* d, const char *ext);

    bool isLevel0(const ossimFilename& file);

    bool isLevel2(const ossimFilename& file);

    bool isLevel1(const ossimFilename& file);

    bool isSentinel1(const ossimFilename &manifestFile);

    bool readProduct(const ossimFilename &manifestFile);

    bool getImageId(ossimString& s) const;

    bool initSensorID(ossimString& s);

    bool standAloneProductInformation();

    bool getAnnotationFileLocation(const ossimFilename &manifestFile, const char* pattern);

    virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                         const double&   heightEllipsoid,
                                         ossimGpt&       worldPoint) const;

    virtual void lineSampleToWorld(const ossimDpt& image_point,
                                   ossimGpt&       gpt) const;

    virtual void  worldToLineSample(const ossimGpt& world_point,
                                    ossimDpt&       image_point) const;

    virtual void imagingRay(const ossimDpt& image_point,
                            ossimEcefRay&   image_ray) const;

    bool initImageSize(ossimIpt& imageSize) const
    {
       // theImageClipRect = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);
       // theSubImageOffset.x = 0.0;
       // theSubImageOffset.y = 0.0;

       return theProduct->initImageSize(imageSize );
    }

    bool initGsd(ossimDpt& gsd) const
    {
       return theProduct->initGsd( gsd );
    }

    const ossimKeywordlist getManifestKwl() const
    {
       return theManifestKwl;
    }

    bool isSLC() { return  theSLC; }
    bool isOCN() { return  theOCN; }

    bool initSRGR()
    {
       //   theProduct->initSRGR( );
       return true;
    }

    bool findSafeManifest(const ossimFilename& file, ossimFilename& safeFile);

    void clearFields();


   protected:

    TYPE_DATA

  private:

    ossimFilename                          theProductXmlFile;
    ossimFilename                          theManifestFile;
    ossimKeywordlist                       theManifestKwl;
    ossimRefPtr<ossimXmlDocument>          theManifestDoc;
    ossimRefPtr<ossimSentinel1ProductDoc>  theProduct;
    bool                                   theOCN;
    bool                                   theSLC;


    }; //end class ossimSentinel1Model

}  //end namespace ossimplugins


#endif /* #ifndef ossimSentinel1Model_HEADER */
