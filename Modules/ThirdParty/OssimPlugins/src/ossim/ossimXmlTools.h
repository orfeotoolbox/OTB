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

#if 0
#include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include "ossimStringUtilities.h"
#include "ossimTimeUtilities.h"
#include <ossim/base/ossimXmlNode.h>

namespace ossimplugins {
   namespace time {
      class ModifiedJulianDate;
   }// namespace
#if 0
   typedef boost::posix_time::ptime         TimeType;
#endif

   ossimXmlNode const& getExpectedFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   ossimString const& getTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   ossimString const& getOptionalTextFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
#if 0
   TimeType getTimeFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
#endif
   time::ModifiedJulianDate getModifiedJulianDateFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);
   double getDoubleFromFirstNode(ossimXmlNode const& node, ossimString const& xpath);

   template <typename DataType> inline
      DataType getFromFirstNode(ossimXmlNode const& node, ossimString const& xpath) {
         ossimXmlNode const& childNode = getExpectedFirstNode(node, xpath);
         ossimString const& sValue = childNode.getText();
         DataType const& value = to<DataType>(sValue);
         return value;
      }

} // ossimplugins namespace


#endif // OSSIM_XMLTOOLS_H__
