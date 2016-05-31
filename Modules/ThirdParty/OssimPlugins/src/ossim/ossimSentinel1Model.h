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

#include <ossim/support_data/ossimSupportFilesList.h>

//#include "ossimGeometricSarSensorModel.h"

#include "ossimSentinel1ProductDoc.h"

//ossimGeometricSarSensorModel

class OSSIM_PLUGINS_DLL ossimString;

namespace ossimplugins
{

   class OSSIM_PLUGINS_DLL ossimSentinel1Model : public ossimSarSensorModel
   {
   public:
     /*!
      * CONSTRUCTORS:
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

     virtual bool loadState(const ossimKeywordlist& kwl,
         const char* prefix=NULL);

     bool checkDirectory(const ossimFilename& file, const char* d, const char *ext) const;

     bool isLevel0(const ossimFilename& file) const;

     bool isLevel2(const ossimFilename& file) const;

     bool isLevel1(const ossimFilename& file) const;

     bool isSentinel1(const ossimFilename &manifestFile);

     bool readProduct(const ossimFilename &productXmlFile);

     // bool getImageId(ossimString& s) const;

     // bool initSensorID(ossimString& s);

     // bool standAloneProductInformation();

     // bool getAnnotationFileLocation(const ossimFilename &manifestFile, const char* pattern);

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

     ossimKeywordlist const& getManifestKwl() const
     {
       return theManifestKwl;
     }

     bool isSLC() const { return  theSLC; }
     bool isOCN() const { return  theOCN; }

     bool initSRGR()
     {
       //   theProduct->initSRGR( );
       return true;
     }

     // bool findSafeManifest(const ossimFilename& file, ossimFilename& safeFile);

     void clearFields();

   protected:

     TYPE_DATA

   private:

       ossimFilename                          theProductXmlFile;
       // ossimFilename                          theManifestFile;
       ossimKeywordlist                       theManifestKwl;
       // ossimRefPtr<ossimXmlDocument>          theManifestDoc;
       ossimRefPtr<ossimSentinel1ProductDoc>  theProduct;
       bool                                   theOCN;
       bool                                   theSLC;
   }; //end class ossimSentinel1Model

}  //end namespace ossimplugins


#endif /* #ifndef ossimSentinel1Model_HEADER */
