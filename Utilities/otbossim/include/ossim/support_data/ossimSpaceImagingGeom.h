//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:  Converts SpaceImaging geometry file to
//               OSSIM Geometry file.
//
//*******************************************************************
//  $Id: ossimSpaceImagingGeom.h 10251 2007-01-14 17:30:14Z dburken $
#ifndef ossimSpaceImagingGeom_HEADER
#define ossimSpaceImagingGeom_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>

class OSSIM_DLL ossimSpaceImagingGeom
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
