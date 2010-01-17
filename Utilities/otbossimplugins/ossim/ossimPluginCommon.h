//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Common code for this plugin.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimPluginCommon_HEADER
#define ossimPluginCommon_HEADER 1

#include <string>
#include <vector>
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
