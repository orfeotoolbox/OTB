//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
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
//  $Id: ossimLandsatTileSource.h,v 1.13 2005/07/25 22:49:54 dburken Exp $

#ifndef ossimLandsatTileSource_HEADER
#define ossimLandsatTileSource_HEADER

#include <imaging/formats/general_raster/ossimGeneralRasterTileSource.h>
#include <base/common/ossimDate.h>

class ossimFfL7;

class OSSIM_DLL ossimLandsatTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimLandsatTileSource();

   ossimLandsatTileSource(const ossimKeywordlist& kwl,
                           const char* prefix=0);
   
   virtual ~ossimLandsatTileSource();

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
   
private:

   void openHeader(const ossimFilename& file);
   
   ossimFfL7* theFfHdr;

   TYPE_DATA
};

#endif
