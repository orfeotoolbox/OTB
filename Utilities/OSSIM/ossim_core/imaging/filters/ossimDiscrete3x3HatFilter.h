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
// Description:
//
//*******************************************************************
//  $Id: ossimDiscrete3x3HatFilter.h,v 1.6 2003/05/13 11:31:04 dburken Exp $
#ifndef ossimDiscrete3x3HatFilter_HEADER
#define ossimDiscrete3x3HatFilter_HEADER
#include "ossimDiscreteConvolutionKernel.h"
class ossimDiscrete3x3HatFilter : public ossimDiscreteConvolutionKernel
{
public:
   ossimDiscrete3x3HatFilter();

   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const float* data,
                         double& result,
                         float nullPixel=OSSIM_FLT_NAN)const;
   
   virtual void convolveSubImage(const float* data,
                                 long dataWidth,
                                 double& result,
                                 float nullPixel=OSSIM_FLT_NAN)const;
   
   virtual void convolve(const unsigned char* data,
                         double& result,
                         ossim_uint8 nullPixel=0.0)const;
   /*!
    * this allows you to pass a subImage to
    * the convolution engine.  It needs to know
    * the width of the buffer so it can increment
    * to the next element.
    */
   virtual void convolveSubImage(const unsigned char* data,
                                 long dataWidth,
                                 double& result,
                                 ossim_uint8 nullPixel=0.0)const;
   
   virtual void buildConvolution(double widthPercent,
                                 double heightPercent);   
};

#endif
