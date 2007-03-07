//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:  Converts SpaceImaging geometry file to
//               OSSIM Geometry file.
//
//*******************************************************************
//  $Id: ossimSpaceImagingGeom.h,v 1.2 2004/05/24 19:44:31 gpotts Exp $
#ifndef ossimSpaceImagingGeom_HEADER
#define ossimSpaceImagingGeom_HEADER
#include "base/data_types/ossimFilename.h"
#include "base/data_types/ossimKeywordlist.h"

class ossimSpaceImagingGeom
{
public:
   ossimSpaceImagingGeom(const ossimFilename& file,
                         const char* prefix = 0);
   ossimSpaceImagingGeom();

   void setGeometry(const ossimFilename& file);
   void setGeometry(const ossimKeywordlist& kwl);

   void exportToOssim(ossimKeywordlist& kwl,
                      const char* prefix=0)const;

   ossimString spaceImagingToOssimKeyword(const ossimString& spaceImagingKeyword)const;
   ossimString spaceImagingToOssimValue(const ossimString& spaceImagingKeyword,
                                        const ossimString& spaceImagingValue)const;

   ossimString getBandName()const;
   ossimFilename getFilename()const;
   ossimString getProducer()const;
   
   static const char* SIG_PRODUCER_KW;
   static const char* SIG_PROJECT_NAME_KW;
   static const char* SIG_FILENAME_KW;
   static const char* SIG_BAND_KW;
   static const char* SIG_BITS_PER_PIXEL_KW;
   static const char* SIG_NUMBER_OF_BANDS_KW;
   static const char* SIG_DATUM_KW;
   static const char* SIG_PROJECTION_KW;
   static const char* SIG_SELECTED_PROJECTION_KW;
   static const char* SIG_ZONE_KW;
   static const char* SIG_UL_EASTING_KW;
   static const char* SIG_UL_NORTHING_KW;
   static const char* SIG_PIXEL_SIZE_X_KW;
   static const char* SIG_PIXEL_SIZE_Y_KW;
   static const char* SIG_COLUMNS_KW;
   static const char* SIG_ROWS_KW;
   
protected:
   ossimKeywordlist theSpaceImagingGeometry;
   ossimString      thePrefix;
};

#endif
