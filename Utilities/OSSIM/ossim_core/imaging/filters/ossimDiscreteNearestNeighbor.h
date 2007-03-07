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
//  $Id: ossimDiscreteNearestNeighbor.h,v 1.2 2001/07/11 15:17:01 gpotts Exp $
#ifndef ossimDiscreteNearestNeighbor_HEADER
#define ossimDiscreteNearestNeighbor_HEADER
#include "ossimDiscreteConvolutionKernel.h"
class ossimDiscreteNearestNeighbor : public ossimDiscreteConvolutionKernel
{
public:
   ossimDiscreteNearestNeighbor()
      :ossimDiscreteConvolutionKernel(1, 1)
      {
         
      }

   /*!
    * We will override and optimize by unrolling the
    * loop
    */
   virtual void convolveFloat(const float* data,
                              double& result)const
      {
         result = *data;
      }

   /*!
    * We will override and optimize by unrolling the
    * loop
    */
   virtual void convolveSubImageFloat(const float* data,
                                      long dataWidth,
                                      double& result)const
      {
         result = *data;
      }
   
   virtual void convolveDouble(const double* data,
                               double& result)const
      {
         result = *data;
      }

   virtual void convolveSubImageDouble(const double* data,
                                      long dataWidth,
                                       double& result)const
      {
         result = *data;
      }
   /*!
    * We will override and optimize by unrolling the
    * loop
    */
   virtual void convolveUchar(const unsigned char* data,
                              double& result)const
      {
         result = *data;
      }
   /*!
    * We will override and optimize by unrolling the
    * loop
    */
   virtual void convolveSubImageUchar(const unsigned char* data,
                                      long dataWidth,
                                      double& result)const
      {
         result = *data;
      }
   
   virtual void convolveSshort(const short* data,
                              double& result)const
      {
         result = *data;        
      }

   virtual void convolveSubImageSshort(const short* data,
                                       long dataWidth,
                                       double& result)const

      {
         result = *data;
      }
   
   virtual void convolveUshort(const unsigned short* data,
                               double& result)const
      {
         result = *data;        
      }

   virtual void convolveSubImageUshort(const unsigned short* data,
                                      long dataWidth,
                                       double& result)const
      {
         result = *data;
      }
   
   virtual void buildConvolution(double widthPercent,
                                 double heightPercent)
      {
      }
};

#endif
