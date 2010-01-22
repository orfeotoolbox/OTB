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
// Contains class declaration for ossimImageMetaData.
// 
//*******************************************************************
//  $Id: ossimImageMetaData.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageMetaData_HEADER
#define ossimImageMetaData_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>

class OSSIM_DLL ossimImageMetaData
{
public:
   ossimImageMetaData();
   ossimImageMetaData(ossimScalarType aType,
                      ossim_uint32 numberOfBands);
   ossimImageMetaData(const ossimImageMetaData& rhs);
   
   virtual ~ossimImageMetaData();
   
   void clear();
   void clearBandInfo();
   void setDefaultsForArrays();
   
   void setNumberOfBands(ossim_uint32 numberOfBands);
   
   ossim_uint32 getNumberOfBands()const;
   
   void setScalarType(ossimScalarType aType);
   
   ossimScalarType getScalarType()const;
   
   double getMinPix(ossim_uint32 band)const;
   
   void setMinPix(ossim_uint32 band, double pix);
   
   void setMaxPix(ossim_uint32 band, double pix);
   
   void setNullPix(ossim_uint32 band, double pix);
   
   double getMaxPix(ossim_uint32 band)const;
   
   double getNullPix(ossim_uint32 band)const;
   
   const double* getMinPixelArray()const;
   
   const double* getMaxPixelArray()const;
   
   const double* getNullPixelArray()const;
   
   void setMinValuesValid(bool flag);
   
   void setMaxValuesValid(bool flag);
   
   void setNullValuesValid(bool flag);

   bool getMinValuesValidFlag()const;

   bool getMaxValuesValidFlag()const;

   bool getNullValuesValidFlag()const;
   
   const ossimImageMetaData& operator=(const ossimImageMetaData& rhs);

   bool isValid()const;

   void assign(const ossimImageMetaData& rhs);
   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;

protected:
  double*         theNullPixelArray;
  double*         theMinPixelArray;
  double*         theMaxPixelArray;
  
  bool            theMinValuesValidFlag;
  bool            theMaxValuesValidFlag;
  bool            theNullValuesValidFlag;
  
  ossimScalarType theScalarType;
  ossim_uint32    theNumberOfBands;
  
  
  void loadBandInfo(const ossimKeywordlist& kwl, const char* prefix = 0);
};

#endif
