/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef OSSIM_XMLTOOLS_H__
#define OSSIM_XMLTOOLS_H__

class ossimString;
class ossimXmlDocument;
// class ossimXmlNode;

#if defined(USE_BOOST_TIME)
#   include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include "ossim/ossimStringUtilities.h"
#include "ossim/ossimTimeUtilities.h"
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

   ossimString const& getOnlyText(ossimXmlDocument const& doc, ossimString const& xpath);

} // ossimplugins namespace


#endif // OSSIM_XMLTOOLS_H__
