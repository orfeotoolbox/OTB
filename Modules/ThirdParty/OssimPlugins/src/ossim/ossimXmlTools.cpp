//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------

#include <ossimXmlTools.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlNode.h>

ossimXmlNode const& ossimplugins::getExpectedFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
    ossimRefPtr<ossimXmlNode> wh = node.findFirstNode(xpath);
    if (!wh) {
        throw std::runtime_error("No subnode found for xpath '" + xpath + "'");
    }
    return *wh;
}

ossimString const& ossimplugins::getTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
    ossimString const& s = getExpectedFirstNode(node, xpath).getText();
    return s;
}

ossimplugins::TimeType ossimplugins::getTimeFromFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
    ossimString s = getTextFromFirstNode(node, xpath);
    std::replace(s.begin(), s.end(), 'T', ' ');
    return boost::posix_time::time_from_string(s);
}

double ossimplugins::getDoubleFromFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
    const ossimString & s = getTextFromFirstNode(node, xpath);
    return s.toDouble();
}

