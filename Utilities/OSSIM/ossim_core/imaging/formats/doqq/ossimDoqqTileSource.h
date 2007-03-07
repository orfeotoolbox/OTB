//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimDoqqTileSource.h,v 1.10 2005/11/14 13:47:01 gpotts Exp $
#ifndef ossimDoqqTileSource_HEADER
#define ossimDoqqTileSource_HEADER
#include <imaging/formats/general_raster/ossimGeneralRasterTileSource.h>
#include <support_data/doqq/ossimDoqq.h>

class OSSIM_DLL ossimDoqqTileSource: public ossimGeneralRasterTileSource
{
public:
   ossimDoqqTileSource();
   virtual ~ossimDoqqTileSource();
   
   ossimString getShortName()const;
   ossimString getLongName()const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);
   
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);
   bool open(const ossimFilename& filename);
   bool open(const ossimGeneralRasterInfo& info);

   ossimString acqdate() const;

protected:
  ossimRefPtr<ossimDoqq>   theHeaderInformation;
  ossimString  theAcqDate;
   
TYPE_DATA
};

#endif
