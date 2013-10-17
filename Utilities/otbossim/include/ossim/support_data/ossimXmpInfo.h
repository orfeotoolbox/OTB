//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Extensible Metadata Platform (XMP) Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimXmpInfo_HEADER
#define ossimXmpInfo_HEADER 1

#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/projection/ossimProjection.h>
#include <string>

// Forward class declarations:
class ossimDrect;
class ossimString;
class ossimXmlDocument;

/** @class Extensible Metadata Platform (XMP) Info object. */
class OSSIM_DLL ossimXmpInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimXmpInfo();

   /** virtual destructor */
   virtual ~ossimXmpInfo();

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   virtual bool open(const ossimFilename& file);
   
   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

   /** @return XMP APP1 XML block. */ 
   const std::string& getXmpApp1XmlBlock() const;

   /**
    * @brief Method to get a projection if possible.
    * @param imageRect Require image rectangle of associated meta data
    * parsed by this class.
    * @return Projection pointer wrapped in ref pointer.  Can be null if
    * projection cannot be established.
    */
   ossimRefPtr<ossimProjection> getProjection(
      const ossimDrect& imageRect ) const;

   /**
    *  @brief Gets the date if available in ISO8601 format:
    *  YYYY-MM-DDThh:mm:ss.sssZ
    *
    *  @param date Initialized by this.  Will be cleared if date not found.
    */
   void getDate( std::string& date ) const;
   
   /**
    *  @brief Gets the mission ID if available.
    *
    *  @param mission Initialized by this.  Will be cleared if date not found.
    */
   void getMissionId( std::string& mission ) const;

   /**
    *  @brief Gets the sensor ID if available.
    *
    *  @param sensor Initialized by this.  Will be cleared if date not found.
    */
   void getSensorId( std::string& sensor ) const;
   
private:

   /**
    * @brief Gets the xml block as a string from stream.
    * @param str Input stream.
    * @param xmpApp1XmlBlock String initialized by this.
    * @return true on success, false on error.
    */
   bool getXmpApp1XmlBlock( std::ifstream& str,
                            std::string& xmpApp1XmlBlock ) const;

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

   ossimFilename  m_file;
   std::string    m_xmpApp1XmlBlock;
};

#endif /* End of "#ifndef ossimXmpInfo_HEADER" */
