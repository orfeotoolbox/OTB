//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Image handler class for a Shuttle Radar Topography Mission (SRTM) file.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmTileSource.h 15800 2009-10-23 20:03:45Z gpotts $
#ifndef ossimSrtmTileSource_HEADER
#define ossimSrtmTileSource_HEADER

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/support_data/ossimSrtmSupportData.h>


class OSSIMDLLEXPORT ossimSrtmTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimSrtmTileSource();

   virtual ossimString getShortName() const;
   
   virtual ossimString getLongName() const;

   virtual ossimString  className() const;

   virtual bool open();
   
   //! Returns the image geometry object associated with this tile source or NULL if non defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimImageGeometry* getInternalImageGeometry();

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

protected:
   virtual ~ossimSrtmTileSource();
   ossimSrtmSupportData m_SrtmSupportData;
   
TYPE_DATA
};

#endif
