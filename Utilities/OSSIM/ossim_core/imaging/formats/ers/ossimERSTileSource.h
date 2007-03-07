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
//  $Id: ossimERSTileSource.h,v 1.6 2005/07/25 22:47:29 dburken Exp $

#ifndef ossimERSTileSource_HEADER
#define ossimERSTileSource_HEADER

#include <imaging/formats/general_raster/ossimGeneralRasterTileSource.h>
#include <support_data/ers/ossimERS.h>


class OSSIM_DLL ossimERSTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimERSTileSource();

   ossimERSTileSource(const ossimKeywordlist& kwl, const char* prefix=0);
   
   virtual ~ossimERSTileSource();

   virtual ossimString getShortName() const;
   virtual ossimString getLongName() const;
   virtual ossimString className() const;

   virtual bool open(const ossimFilename& filename);
  
  /*!
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *  Returns true if geometry info is present, false if not.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);


   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);
private:

   void openHeader(const ossimFilename& file);
   
   ossimERS* theHdr;

   TYPE_DATA
};

#endif
