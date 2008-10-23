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
// $Id: ossimSrtmTileSource.h 9262 2006-07-14 20:49:28Z dburken $
#ifndef ossimSrtmTileSource_HEADER
#define ossimSrtmTileSource_HEADER

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/support_data/ossimSrtmSupportData.h>


class OSSIMDLLEXPORT ossimSrtmTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimSrtmTileSource();

   virtual ~ossimSrtmTileSource();

   virtual ossimString getShortName() const;
   
   virtual ossimString getLongName() const;

   virtual ossimString  className() const;

   virtual bool open();
   
   /**
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *
    *  @return true if geometry info is present, false if not.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);

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

private:
   ossimSrtmSupportData theSrtmSupportData;
   
TYPE_DATA
};

#endif
