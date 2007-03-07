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
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimImageDataHelper.h,v 1.2 2002/05/03 16:41:25 gpotts Exp $
#ifndef ossimImageDataHelper_HEADER
#define ossimImageDataHelper_HEADER

#include "ossimImageData.h"
#include "base/data_types/ossimPolyArea2d.h"
#include "base/data_types/color_space/ossimRgbVector.h"

class ossimImageDataHelper
{
public:
   /*!
    * This must be a valid image data object
    */
   ossimImageDataHelper(ossimImageData* imageData);

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
