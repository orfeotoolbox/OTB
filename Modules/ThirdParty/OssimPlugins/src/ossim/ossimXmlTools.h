//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
#ifndef OSSIM_XMLTOOLS_H__
#define OSSIM_XMLTOOLS_H__

class ossimString;
// class ossimXmlNode;

#if defined(USE_BOOST_TIME)
#   include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include "ossimStringUtilities.h"
#include "ossimTimeUtilities.h"
#include <ossim/base/ossimXmlNode.h>

namespace ossimplugins {
   namespace time {
      class ModifiedJulianDate;
   }// time namespace
#if defined(USE_BOOST_TIME)
   typedef boost::posix_time::ptime         TimeType;
#else
   typedef time::ModifiedJulianDate         TimeType;
#endif

   ossimXmlNode const& getExpectedFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   ossimString const& getTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   ossimString const& getOptionalTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   TimeType getTimeFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   time::ModifiedJulianDate getModifiedJulianDateFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   double getDoubleFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);

   template <typename DataType> inline
      DataType getFromFirstNode(ossimXmlNode const& node, ossimString const& xpath) {
         ossimXmlNode const& childNode = getExpectedFirstNode(node, xpath);
         ossimString const& sValue = childNode.getText();
         DataType const& value = to<DataType>(sValue, "decoding " + xpath + " node");
         return value;
      }

} // ossimplugins namespace


#endif // OSSIM_XMLTOOLS_H__
