//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------

#include "ossimXmlTools.h"
#include "ossimTimeUtilities.h"
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimNotify.h>
// #include <ossim/base/ossimXmlNode.h>

std::vector<ossimString> ossimplugins::getTextNodes(
      ossimXmlNode const& node, ossimString const& xpath)
{
   std::vector<ossimString> res;
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
   node.findChildNodes(xpath, xnodes);
   for (std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator b = xnodes.begin(), e = xnodes.end()
         ; b != e
         ; ++b
       )
   {
      res.push_back((*b)->getText());
   }
   return res;
}

ossimXmlNode const& ossimplugins::getExpectedFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
   ossimRefPtr<ossimXmlNode> const& wh = node.findFirstNode(xpath);
   if (!wh) {
      throw std::runtime_error("No '" + xpath.string() + "' subnode found");
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
#if defined(USE_BOOST_TIME)
   ossimString s = getTextFromFirstNode(node, xpath);
   std::replace(s.begin(), s.end(), 'T', ' ');
   return boost::posix_time::time_from_string(s);
#else
   return getModifiedJulianDateFromFirstNode(node, xpath);
#endif
}

ossimplugins::time::ModifiedJulianDate
ossimplugins::getModifiedJulianDateFromFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
   ossimString const& s = getTextFromFirstNode(node, xpath);
   return time::toModifiedJulianDate(s);
}

double ossimplugins::getDoubleFromFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
   const ossimString & s = getTextFromFirstNode(node, xpath);
   return s.toDouble();
}

ossimString const& ossimplugins::getOptionalTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath)
{
   static const ossimString empty;
   ossimRefPtr<ossimXmlNode> const& wh = node.findFirstNode(xpath);
   return wh.valid() ? wh->getText() : empty;
}

ossimString const& ossimplugins::getOnlyText(ossimXmlDocument const& doc, ossimString const& xpath)
{
   static const ossimString empty;
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
   doc.findNodes(xpath, xnodes);
   switch (xnodes.size()) {
      case 1: // perfect
         if (xnodes[0].valid()) {
            return xnodes[0]->getText();
         }

      case 0:
         ossimNotify(ossimNotifyLevel_WARN) << "No node found with the name: " << xpath << "\n";
         break;
      default:
         ossimNotify(ossimNotifyLevel_WARN) << xnodes.size()
            << " (too many) nodes found with the name: " << xpath << "\n";
         break;
   }
   return empty;
}

