/*
 * Copyright (C) 2010-2016 by David Burken
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


#ifndef ossimPluginCommon_HEADER
#define ossimPluginCommon_HEADER 1

#include <string>
#include <vector>
#include <ossimPluginConstants.h>
#include <ossim/base/ossimRefPtr.h>

class ossimString;
class ossimXmlDocument;
class ossimXmlNode;

namespace ossimplugins
{


class CivilDateTime;


namespace ossim
{
   /**
    * @note This code was moved out of ossimRadarSat2Model.
    *
    * @brief Converts date string from ISO 8601 format to CivilDateTime.
    *
    * @param dateString Input date string.
    *
    * @param outputDate Output date object to initialize.
    *
    * @return true on success false on error.
    */
   bool iso8601TimeStringToCivilDate(const std::string& dataString,
                                     CivilDateTime& outputDate);

   /**
    * @brief Gets path from doc and initializes string.
    * @param path Xml path to look for.
    * @param xdoc Xml doc to look in.
    * @param s String to initialize.
    * @return true on success and false if path is not found or if there
    * are more than one of path.
    * @note You should call getPath that takes a vector for things that are
    * repetitive.
    */
   bool getPath(const ossimString& path,
                const ossimXmlDocument* xdoc,
                ossimString& s);

   /**
    * @brief Gets path from doc and initializes string.
    * @param path Xml path to look for.
    * @param xdoc Xml doc to look in.
    * @param v Vector to initialize.
    * @return true on success and false if path is not found.
    */
   bool getPath(const ossimString& path,
                const ossimXmlDocument* xdoc,
                std::vector<ossimString>& v);

   /**
    * @brief Finds from node with path from node and initializes string.
    *
    * @param path Xml path to look for.
    *
    * @param node Xml node to look in.
    *
    * @param s String to initialize.
    *
    * @return true on success and false if path is not found or if text is
    * empty from found node..
    */
   bool findFirstNode(const ossimString& path,
                      ossimRefPtr<ossimXmlNode> node,
                      ossimString& s);

} // matches: namespace ossim
}

#endif /* matches: #ifndef ossimPluginCommon_HEADER */
