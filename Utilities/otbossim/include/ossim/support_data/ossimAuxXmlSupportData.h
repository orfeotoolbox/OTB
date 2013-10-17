//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: See class description below.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimAuxXmlSupportData_HEADER
#define ossimAuxXmlSupportData_HEADER 1

#include <ossim/base/ossimRefPtr.h>
#include <iosfwd>
#include <string>

// Forward class declarations:
class ossimDrect;
class ossimFilename;
class ossimProjection;
class ossimString;
class ossimXmlDocument;

/**
 * @class ossimAuxXmlSupportData
 *
 * Utility/support data class to extract the projection from a dot aux dot xml
 * file commonly found with mrsid data.
 */
class OSSIM_DLL ossimAuxXmlSupportData
{
public:
   
   /** default constructor */
   ossimAuxXmlSupportData();

   /** virtual destructor */
   virtual ~ossimAuxXmlSupportData();

   /**
    * @brief Method to get a projection if possible.
    * @param file File name to open.
    * @return Projection pointer wrapped in ref pointer.  Can be null if
    * projection cannot be established.
    */
   ossimRefPtr<ossimProjection> getProjection(const ossimFilename& file) const;

private:

   /**
    * @brief Gets path from doc and initializes string.
    * @param path Xml path to look for.
    * @param xdoc Xml doc to look in.
    * @param s String to initialize.
    * @return true on success and false if path is not found or if there
    * are more than one of path.
    */
   bool getPath( const ossimString& path,
                 const ossimXmlDocument& xdoc,
                 ossimString& s ) const;

   /**
    * @brief Sets tie and scale.
    * @param xdoc Xml document.
    * @param wkt Well known tet string.
    * @return True on success, false on error.
    */
   bool initializeProjection( const ossimXmlDocument xdoc,
                              const std::string& wkt,
                              ossimProjection* proj ) const;

   /**
    * @brief Get first parameter from PROJCS[ ... ] string.
    * @param wkt Well known tet string.
    * @param name Initialized by this with name if found.
    * @return true on success, false on error.
    */
   bool getProjcsName( const std::string& wkt, std::string& name ) const;

   /**
    * @brief Gets units of Projected Coordinate System.
    * @param wkt Well known tet string.
    * @return Units.
    */
   ossimUnitType getUnits( const std::string& wkt ) const;

};

#endif /* End of "#ifndef ossimAuxXmlSupportData_HEADER" */
