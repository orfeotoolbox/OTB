//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Jordi Inglada
//
// Description:
//
// Contains class definition for the class "ossimNetworkedQuadTreeTileSource".
// ossimNetworkedQuadTreeTileSource is derived from ImageHandler which is derived from
// TileSource.
//
//*******************************************************************
//  $Id: ossimNetworkedQuadTreeTileSource.h 10751 2007-04-23 16:49:08Z dburken $

#ifndef ossimNetworkedQuadTreeTileSource_HEADER
#define ossimNetworkedQuadTreeTileSource_HEADER

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/base/ossimDate.h>

class ossimFfL7;

class OSSIM_DLL ossimNetworkedQuadTreeTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimNetworkedQuadTreeTileSource();

   ossimNetworkedQuadTreeTileSource(const ossimKeywordlist& kwl,
                           const char* prefix=0);
   
   virtual ~ossimNetworkedQuadTreeTileSource();

   virtual ossimString getShortName() const;
   
   virtual ossimString getLongName() const;

   virtual ossimString  className() const;

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
   
  /*!
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *  Returns true if geometry info is present, false if not.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);


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

   void openHeader(const ossimFilename& file);
   
   ossimFfL7* theFfHdr;

   TYPE_DATA
};

#endif
