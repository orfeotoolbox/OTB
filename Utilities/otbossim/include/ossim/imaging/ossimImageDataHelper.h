//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimImageDataHelper.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimImageDataHelper_HEADER
#define ossimImageDataHelper_HEADER

#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/base/ossimRgbVector.h>

class ossimImageDataHelper
{
public:
   /*!
    * This must be a valid image data object
    */
   ossimImageDataHelper(ossimImageData* imageData=0);

   void setImageData(ossimImageData* imageData);

   void fill(const double* values,
             const ossimIrect& rect,
             bool clipPoly=true);

   void fill(const double* values,
             std::vector<ossimPolygon>& regionList,
             bool clipPoly=true);
   
   void fill(const ossimRgbVector& color,
             std::vector<ossimPolygon>& regionList,
             bool clipPoly=true);
   
   void fill(const ossimRgbVector& color,
             const ossimIrect& rect,
             bool clipPoly=true);
   /*!
    * Values must be of the same type as the image data that
    * we are writing to.
    */
   void fill(const double*  values,
             const ossimPolygon& region,
             bool clipPoly=true);

   void fill(const ossimRgbVector& color,
             const ossimPolygon& region,
             bool clipPoly=true);
   /*!
    * The input is assumed to have the same origin, size and scalar
    * as this object.  This will implement a selection
    */
   void copyInputToThis(const void* input,
                        const ossimPolygon& region,
                        bool clipPoly=true);

   ossimImageData* theImageData;
   

protected:
   ossimIpt        theOrigin;
   ossimIrect      theImageRectangle;
   ossimPolyArea2d thePolyImageRectangle;
   
   template <class T>
   void fill(T dummyVariable,
             const double* values,
             const ossimPolygon& region,
             bool clipPoly);

   template <class T>
   void fill(T dummyVariable,
             const double* values,
             const ossimIrect& region,
             bool clipPoly);
   
   template <class T>
   void copyInputToThis(const T* inputBuf,
                        const ossimPolygon& region,
                        bool clipPoly);

   template <class T>
   void fill(T dummyVariable,
             const double* values,
             const ossimPolygon& region);
   template <class T>
   void fill(T dummyVariable,
             const double* values,
             const ossimIrect& region);
   
   template <class T>
   void copyInputToThis(const T* inputBuf,
                        const ossimPolygon& region);

   
};

#endif
