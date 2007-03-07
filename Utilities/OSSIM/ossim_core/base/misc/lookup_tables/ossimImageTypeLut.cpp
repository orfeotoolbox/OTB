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
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class definition for ImageTypeLUT.  Currently has mapping of
// ouput image writes from enumeration to string and string to
// enumeration.
//
//*******************************************************************
//  $Id: ossimImageTypeLut.cpp,v 1.3 2002/03/04 12:32:29 dburken Exp $

#include "ossimImageTypeLut.h"

//***
// Pixel type keyword to use for getState/saveState methods.
//***
const ossimKeyword ossimImageTypeLut::IMAGE_TYPE_KW("image_type",
                                                    "Image type.\n\
Valid types are:  ccf, tiff, tiled_tiff, jpeg, and general_raster.");

static const int TABLE_SIZE = 8;

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimImageTypeLut::ossimImageTypeLut()
   :
      ossimLookUpTable(TABLE_SIZE)
{
   //***
   // Complete initialization of data member "theImageTypeTable".
   // Note:  Output type enumerations defined in constants.h file.
   //***
   theTable[0].theKey   = OSSIM_TIFF_STRIP;
   theTable[0].theValue = "tiff_strip";
   theTable[1].theKey   = OSSIM_TIFF_STRIP_BAND_SEPARATE;
   theTable[1].theValue = "tiff_strip_band_separate";
   theTable[2].theKey   = OSSIM_TIFF_TILED;
   theTable[2].theValue = "tiff_tiled";
   theTable[3].theKey   = OSSIM_TIFF_TILED_BAND_SEPARATE;
   theTable[3].theValue = "tiff_tiled_band_separate";
   theTable[4].theKey   = OSSIM_GENERAL_RASTER_BIP;
   theTable[4].theValue = "general_raster_bip";
   theTable[5].theKey   = OSSIM_GENERAL_RASTER_BIL;
   theTable[5].theValue = "general_raster_bil";
   theTable[6].theKey   = OSSIM_GENERAL_RASTER_BSQ;
   theTable[6].theValue = "general_raster_bsq";
   theTable[7].theKey   = OSSIM_JPEG;
   theTable[7].theValue = "jpeg";
}
