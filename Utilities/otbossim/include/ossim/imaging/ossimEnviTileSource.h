//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Image handler class for a raster files with an ENVI header file.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimEnviTileSource_HEADER
#define ossimEnviTileSource_HEADER 1

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/support_data/ossimEnviHeader.h>


class OSSIMDLLEXPORT ossimEnviTileSource : public ossimGeneralRasterTileSource
{
public:

   /** @brief default constructor */
   ossimEnviTileSource();

   /** @return "ENVI" */
   virtual ossimString getShortName() const;

   /** @return "ENVI reader" */
   virtual ossimString getLongName()  const;

   /** @brief open method. */
   virtual bool open();

   /**
    * @return The image geometry object associated with this tile source or
    * NULL if not defined.  The geometry contains full-to-local image transform
    * as well as projection (image-to-world).
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   /**
    * Method to save the state of an object to a keyword list.
    * @return true on success, false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.
    *
    * @return true on success, false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);

   /**
    * @brief Gets a property.
    * @param The name of the property to get.
    * @return The property if found.
    */
   virtual ossimRefPtr<ossimProperty> getProperty(
      const ossimString& name)const;

   /**
    * @brief Adds property names to array.
    * @param propertyNames Array to populate with property names.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:
   
   /** @brief Hidden from use destructor.  This is a ref ptr. */
   virtual ~ossimEnviTileSource();

private:

   /**
    * @brief Sets default output band list if we are a band selector and
    * "default bands" is found in the header file.
    */
   void setDefaultBandList();

   /**
    * @brief Creates geometry from known support data, i.e. envi header,
    * and so on if found.
    */
   ossimRefPtr<ossimImageGeometry> getInternalImageGeometry() const;
   
   ossimEnviHeader m_enviHdr;
   
TYPE_DATA
   
}; // End: class ossimEnviTileSource

#endif /* #ifndef ossimEnviTileSource_HEADER */
