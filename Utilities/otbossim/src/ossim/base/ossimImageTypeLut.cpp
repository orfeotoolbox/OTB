//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for ImageTypeLUT.  Currently has mapping of
// ouput image writes from enumeration to string and string to
// enumeration.
//
//*******************************************************************
//  $Id: ossimImageTypeLut.cpp 22221 2013-04-11 15:30:08Z dburken $

#include <ossim/base/ossimImageTypeLut.h>

//***
// Pixel type keyword to use for getState/saveState methods.
//***
const ossimKeyword ossimImageTypeLut::IMAGE_TYPE_KW("image_type",
                                                    "Image type.\n\
Valid types are:  ccf, tiff, tiled_tiff, jpeg, and general_raster.");

static const int TABLE_SIZE = 9;

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
   theTable[8].theKey   = OSSIM_PDF;
   theTable[8].theValue = "ossim_pdf";
}
