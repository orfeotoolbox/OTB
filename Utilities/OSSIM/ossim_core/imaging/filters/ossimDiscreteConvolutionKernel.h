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
//  $Id: ossimDiscreteConvolutionKernel.h,v 1.12 2003/05/13 11:31:04 dburken Exp $
#ifndef ossimDiscreteConvolutionKernel_HEADER
#define ossimDiscreteConvolutionKernel_HEADER
#include <vector>
#include "base/common/ossimConstants.h"
#include <iostream>
using namespace std;

#include "matrix/newmat.h"
#include "matrix/newmatio.h"

class ossimDiscreteConvolutionKernel
{
public:
   ossimDiscreteConvolutionKernel(long width,
                                  long height,
                                  bool doWeightedAverage=true);
   ossimDiscreteConvolutionKernel(const NEWMAT::Matrix& kernel,
                                  bool doWeightedAverage=true);
   virtual ~ossimDiscreteConvolutionKernel();
   static void buildSymmetric(const std::vector<float>& coefficients,
                              NEWMAT::Matrix& result);
   static void buildSymmetric(float* coefficients, long size,
                              NEWMAT::Matrix& result);

   virtual void setKernel(const NEWMAT::Matrix& kernel);

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

   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const double* data,
                         double& result,
                         double nullPixel=OSSIM_DBL_NAN)const;

   virtual void convolveSubImage(const double* data,
                                 long dataWidth,
                                 double& result,
                                 double nullPixel=OSSIM_DBL_NAN)const;
   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const short* data,
                         double& result,
                         ossim_sint16 nullPixel=OSSIM_SSHORT_NAN)const;
   
   virtual void convolveSubImage(const short* data,
                                 long dataWidth,
                                 double& result,
                                 ossim_sint16 nullPixel=OSSIM_SSHORT_NAN)const;

   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const unsigned short* data,
                         double& result,
                         ossim_uint16 nullPixel=OSSIM_USHORT_NAN)const;
   
   virtual void convolveSubImage(const unsigned short* data,
                                 long dataWidth,
                                 double& result,
                                 ossim_uint16 nullPixel=OSSIM_USHORT_NAN)const;
   
   
   virtual void convolve(const unsigned char* data,
                              double& result,
                              ossim_uint8 nullPixel=0)const;
   /*!
    * this allows you to pass a subImage to
    * the convolution engine.  It needs to know
    * the width of the buffer so it can increment
    * to the next element.
    */
   virtual void convolveSubImage(const unsigned char* data,
                                      long dataWidth,
                                      double& result,
                                      ossim_uint8 nullPixel=0)const;
   /*!
    * This is used to allow me to continually adjust a convolution kernel
    * based on where it center lies on a pixel. The xLocation and yLocation
    * are all from the center of pixel.  If I am convolving at full resolution
    * then xLocation and yLocation should both be 0.  Let's pretend that we
    * contract an image by .5 scale factor or zooming out by a factor of 2.  Then
    * we actually of it .5 off from cneter of pixel in both the x and y direction.
    * This information can be used to allow a kernel to adjust it's waits accordingly.
    *
    * Typically if the xLocation and yLocation is directly on the center indicated
    * by 0 vor both then the kernel defaults to 1 in the middle and all other weights
    * are zero.
    */
   virtual void buildConvolution(double xLocation,
                                 double yLocation)
      {}
   virtual long getWidth()const
      {
         return theWidth;
      }
   virtual long getHeight()const
      {
         return theHeight;
      }
   const NEWMAT::Matrix& getKernel()const
      {
         return *theKernel;
      }
protected:
   NEWMAT::Matrix  *theKernel;
   long theWidth;
   long theHeight;
   bool theComputeWeightedAverageFlag;
};

#endif
