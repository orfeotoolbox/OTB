//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Chong-Ket Chuah
//
// Description:
//
// Contains class definition for the class "ossimERSTileSource".
//
//*******************************************************************
//  $Id: ossimERSTileSource.h 17932 2010-08-19 20:34:35Z dburken $

#ifndef ossimERSTileSource_HEADER
#define ossimERSTileSource_HEADER

#include <ossim/imaging/ossimGeneralRasterTileSource.h>
#include <ossim/support_data/ossimERS.h>


class OSSIM_DLL ossimERSTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimERSTileSource();

   ossimERSTileSource(const ossimKeywordlist& kwl, const char* prefix=0);
   

   virtual ossimString getShortName() const;
   virtual ossimString getLongName() const;
   virtual ossimString className() const;

   virtual bool open(const ossimFilename& filename);
  
   //! Returns the image geometry object associated with this tile source or NULL if not defined.
   //! The geometry contains full-to-local image transform as well as projection (image-to-world)
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);
protected:
   virtual ~ossimERSTileSource();

private:

   void openHeader(const ossimFilename& file);
   
   ossimERS* theHdr;

   TYPE_DATA
};

#endif
