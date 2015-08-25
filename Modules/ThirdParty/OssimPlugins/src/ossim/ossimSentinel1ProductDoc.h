#ifndef ossimSentinel1ProductDoc_HEADER
#define ossimSentinel1ProductDoc_HEADER


#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimCommon.h>
#include "ossimPluginCommon.h"
#include "ossimPluginConstants.h"

#include <cstdio>
#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>

namespace ossimplugins
{

   class OSSIM_PLUGINS_DLL ossimSentinel1ProductDoc : public ossimObject,
                                                   public ossimErrorStatusInterface
   {
   public:

      ossimSentinel1ProductDoc();

      virtual ~ossimSentinel1ProductDoc();

      void clearFields();

      bool openMetadataFile(ossimRefPtr<ossimXmlDocument>& doc, const ossimString file) const;

      bool initImageSize(ossimIpt& imageSize) const;

      bool initGsd( ossimDpt& gsd) const;

      void addSRGRCoefficients(const char* prefix, const ossimRefPtr<ossimXmlNode>& coordinateConversion);

      void addDopplerCentroidCoefficients(const ossimRefPtr<ossimXmlNode>& dcEstimateList);

      void addOrbitStateVectors(const ossimRefPtr<ossimXmlNode>& dcEstimateList);

      void readCalibrationMetadata();

      void readNoiseMetadata();

      double getModifiedJulianDate(const ossimString utc_time);

      bool readProductMetadata( );

      double getBandTerrainHeight( const ossimRefPtr<ossimXmlDocument>& theProductXmlDocument );

      inline const ossimKeywordlist getProductKwl() const
      {
         return theProductKwl;
      }

      inline void setMetadataDirectory(const ossimFilename d)
      {
         theManifestDirectory = d;
      }


      inline bool setSLC(bool b)
      {
         theSLC = true;
      }

      inline bool setTOPSAR(bool b)
      {
         theTOPSAR = true;
      }

   private:
      ossimFilename theManifestDirectory;
      double theRangeSpacingTotal;
      double theAzimuthSpacingTotal;
      bool theTOPSAR;
      bool theSLC;
      ossimFilename theProductXmlFile;
      ossimString theSampleType;
      ossimKeywordlist theProductKwl;

      };

}

#endif /* #ifndef ossimSentinel1ProductDoc_HEADER */
