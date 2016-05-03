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

#ifndef ossimSentinel1SarSensorModel_HEADER
#define ossimSentinel1SarSensorModel_HEADER

#include <boost/config.hpp>
#include "ossimSarSensorModel.h"

class ossimXmlDocument;
class ossimString;

namespace ossimplugins
{

class ossimSentinel1SarSensorModel : public ossimSarSensorModel
{
public:

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
    /** Constructor */
    ossimSentinel1SarSensorModel()=default;

    /** Copy constructor */
    ossimSentinel1SarSensorModel(ossimSentinel1SarSensorModel const& m)=default;
    /** Move constructor */
    ossimSentinel1SarSensorModel(ossimSentinel1SarSensorModel && m)=default;

    /** Destructor */
    virtual ~ossimSentinel1SarSensorModel()=default;
#endif

    void readAnnotationFile(const std::string & annotationXml);

private:
    void readCoordinates(
            ossimXmlDocument const& xmlDoc, ossimString const& xpath,
            ossimString const& rg0_xpath, ossimString const& coeffs_xpath,
            std::vector<CoordinateConversionRecordType> & outputRecords
            );


protected:
        /*
           std::string theProductType;
           std::string theMode;
           std::string theSwath;
           std::string thePolarisation;
         */
};

} // end namespace

#endif
