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
#include "ossimSarSensorModel.h"

#include <cstdio>
#include <vector>
#include <sstream>
#include <iostream>
#include <iterator>

namespace ossimplugins
{

   // TODO: move everything into the SensorModel class
   // and deprecate this one!
class OSSIM_PLUGINS_DLL ossimSentinel1ProductDoc
// : public ossimObject, public ossimErrorStatusInterface
: public ossimSarSensorModel, public ossimErrorStatusInterface
{
public:

   typedef ossimRefPtr<ossimXmlNode>     ossimXmlNodePtr;
   typedef ossimRefPtr<ossimXmlDocument> ossimXmlDocumentPtr;
   ossimSentinel1ProductDoc();

   virtual ~ossimSentinel1ProductDoc();

   void cleaFields();

   bool openMetadataFile(ossimXmlDocument& doc, ossimString const& file) const;

   bool initImageSize(ossimIpt& imageSize) const;

   bool initGsd( ossimDpt & gsd) const;

   void addDopplerCentroidCoefficients(ossimXmlNode const& dcEstimateList);

   void addOrbitStateVectors(ossimXmlNode const& orbitList);

   void readCalibrationMetadata();

   void readNoiseMetadata();

   bool read(ossimFilename const& annotationXml);

   double getBandTerrainHeight(ossimXmlDocument const& theProductXmlDocument);

   void clearFields();
   ossimKeywordlist const& getProductKwl() const
   {
      return theProductKwl;
   }

   void setMetadataDirectory(ossimFilename const& d)
   {
      theManifestDirectory = d;
   }

   void setSLC()
   {
      theSLC = true;
   }

   void setTOPSAR()
   {
      theTOPSAR = true;
   }

private:
   void readBurstRecords(ossimXmlNode const& productRoot, ossimXmlNode const& imageInformation);
   void addSRGRCoefficients(ossimXmlNode const& coordinateConversion);
   void readCoordinates(
         ossimXmlNode const& node,
         ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
         char const * sr_gr_prefix);
   void readGeoLocationGrid(ossimXmlNode const& productRoot);


   ossimFilename    theManifestDirectory;
   // double           theRangeSpacingTotal;
   // double           theAzimuthSpacingTotal;
   bool             theTOPSAR;
   bool             theSLC;
   ossimFilename    theProductXmlFile;
   ossimString      theSampleType;
   ossimKeywordlist theProductKwl;
};

}

#endif /* #ifndef ossimSentinel1ProductDoc_HEADER */
