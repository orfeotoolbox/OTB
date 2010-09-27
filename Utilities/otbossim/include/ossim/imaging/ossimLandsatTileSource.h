//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for the class "ossimLandsatTileSource".
// ossimLandsatTileSource is derived from ImageHandler which is derived from
// TileSource.
//
//*******************************************************************
//  $Id: ossimLandsatTileSource.h 17932 2010-08-19 20:34:35Z dburken $

#ifndef ossimLandsatTileSource_HEADER
#define ossimLandsatTileSource_HEADER

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/base/ossimDate.h>
#include <ossim/support_data/ossimFfL7.h>

class OSSIM_DLL ossimLandsatTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimLandsatTileSource();

   ossimLandsatTileSource(const ossimKeywordlist& kwl,
                           const char* prefix=0);
   

   virtual ossimString getShortName() const;
   
   virtual ossimString getLongName() const;

   virtual bool open();
   
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   virtual double getMinPixelValue(ossim_uint32 band=0)const;

   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossimScalarType getOutputScalarType() const;

   bool getAcquisitionDate(ossimDate& date)const;
   ossimString getSatelliteName()const;

   ossimFilename getBandFilename(ossim_uint32 idx)const;
   
   bool isPan()const;
   bool isVir()const;
   bool isTm()const;
   
   //! Returns the image geometry object associated with this tile source or NULL if non defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);

   /**
    * @brief Gets a property for matching name.
    * @param name The name of the property to get.
    * @return Returns property matching "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   
   /**
    * @brief Gets a list of property names available.
    * @param propertyNames The list to push back names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
private:
   virtual ~ossimLandsatTileSource();

   void openHeader(const ossimFilename& file);
   
   ossimRefPtr<ossimFfL7> theFfHdr;

   TYPE_DATA
};

#endif
