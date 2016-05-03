//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
#include <boost/date_time/posix_time/posix_time.hpp>
#ifndef OSSIM_XMLTOOLS_H__
#define OSSIM_XMLTOOLS_H__

class ossimString;
class ossimXmlNode;

namespace ossimplugins {
    typedef boost::posix_time::ptime         TimeType;

    ossimXmlNode const& getExpectedFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    ossimString const& getTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    TimeType getTimeFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
    double getDoubleFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
} // ossimplugins namespace


#endif // OSSIM_XMLTOOLS_H__
