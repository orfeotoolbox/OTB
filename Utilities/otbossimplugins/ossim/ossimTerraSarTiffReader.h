//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Image handelr for TerraSar-X data that is TIFF based.
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimTerraSarTiffReader_HEADER
#define ossimTerraSarTiffReader_HEADER 1

#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/base/ossimFilename.h>

class ossimXmlDocument;

namespace ossimplugins
{


class ossimTerraSarTiffReader : public ossimTiffTileSource
{
public:

   /** default constructor */
   ossimTerraSarTiffReader();

   /** virtual destructor */
   virtual ~ossimTerraSarTiffReader();

   /** @return "terrasarx" */
   virtual ossimString getShortName() const;

   /** @return "ossim TerraSAR-X reader" */
   virtual ossimString getLongName()  const;

   /** @return "ossimTerraSarTiffReader" */
   virtual ossimString getClassName()    const;

   /**
    *  This open take the product xml file.
    *
    *  @param file File to open.
    *
    *  @return true on success, false on error.
    */
   virtual bool open(const ossimFilename& file);

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /** Close method. */
   virtual void close();

   /**
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *
    *  @param kwl Keyword list that will be initialized with geometry info.
    *  Returns true if geometry info is present, false if not.
    *
    *  @param prefix The prefix is added to the resulting keyword list
    *  keywords.  So is you if you pass in a prefix of "image01.", the
    *  keyword ul_lat will be like:  "image01.ul_lat:  -40.00000"
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);

private:

   /**
    * @brief Method to check file to see if it's a TerraSAR-X product xml file.
    * @param file The file to check.
    * @return true on success, false on error.
    */
   bool isTerraSarProductFile(const ossimFilename& file) const;

   /**
    * @brief Method to check file to see if it's a TerraSAR-X product xml file.
    * @param xdoc XML document opened up to product.xml file.
    * @return true on success, false on error.
    */
   bool isTerraSarProductFile(const ossimXmlDocument* xdoc) const;


   ossimFilename theProductXmlFile;

TYPE_DATA
};
}

#endif /* #ifndef ossimTerraSarTiffReader_HEADER */

