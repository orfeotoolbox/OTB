//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Jordi Inglada
//
// Description:
//
// Contains class definition for the class "ossimTileMapTileSource".
// ossimTileMapTileSource is derived from ImageHandler which is derived from
// TileSource.
//
//*******************************************************************
//  $Id: ossimTileMapTileSource.h 10751 2007-04-23 16:49:08Z dburken $

#ifndef ossimTileMapTileSource_HEADER
#define ossimTileMapTileSource_HEADER

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimRefPtr.h>

class ossimFfL7;

namespace ossimplugins
{
class OSSIM_DLL ossimTileMapTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimTileMapTileSource();

   ossimTileMapTileSource(const ossimKeywordlist& kwl,
                           const char* prefix=0);



   virtual ossimString getShortName() const;

   virtual ossimString getLongName() const;

   virtual ossimString  className() const;

   virtual bool open();


   //! Returns the image geometry object associated with this tile source or NULL if non defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();



   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);


protected:
  virtual ~ossimTileMapTileSource();

private:

   TYPE_DATA
};
} // End: namespace ossimplugins
#endif
