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
//  $Id: ossimDiscreteConvolutionKernel.cpp,v 1.11 2003/05/13 11:31:04 dburken Exp $
#include "ossimDiscreteConvolutionKernel.h"

 
ossimDiscreteConvolutionKernel::ossimDiscreteConvolutionKernel(long width,
                                                               long height,
                                                               bool doWeightedAverage)
   : theKernel(NULL),
     theWidth(width),
     theHeight(height),
     theComputeWeightedAverageFlag(doWeightedAverage)
{
   theWidth  = theWidth <0?1:theWidth;
   theHeight = theHeight <0?1:theHeight;
   
   theKernel = new NEWMAT::Matrix(theHeight, theWidth);
   *theKernel = (1.0/(theHeight*theWidth));
}

ossimDiscreteConvolutionKernel::ossimDiscreteConvolutionKernel(const NEWMAT::Matrix& kernel,
                                                               bool doWeightedAverage)
   :theKernel(new NEWMAT::Matrix(kernel)),
   theComputeWeightedAverageFlag(doWeightedAverage)
{
   theWidth  = theKernel->Ncols();
   theHeight = theKernel->Nrows();
}

ossimDiscreteConvolutionKernel::~ossimDiscreteConvolutionKernel()
{
   if(theKernel)
   {
      delete theKernel;
      theKernel = NULL;
   }
}

void ossimDiscreteConvolutionKernel::buildSymmetric(const std::vector<float>& coefficients,
                                                    NEWMAT::Matrix& result)
{
   long size = coefficients.size();
   if(size > 0)
   {

      NEWMAT::RowVector row(size);
      NEWMAT::ColumnVector col(size);
      for(long index = 0; index < size; ++index)
      {
         row[index] = coefficients[index];
         col[index] = coefficients[index];
      }
      result = col*row;
   }
}

void ossimDiscreteConvolutionKernel::buildSymmetric(float *coefficients,
                                                    long size,
                                                    NEWMAT::Matrix& result)
{
   if(size > 0)
   {
      NEWMAT::RowVector    row(size);
      NEWMAT::ColumnVector col(size);
      for(long index = 0; index < size; ++index)
      {
         row[index] = coefficients[index];
         col[index] = coefficients[index];
      }
      result = col*row;
   }
}

void ossimDiscreteConvolutionKernel::setKernel(const NEWMAT::Matrix& kernel)
{
   if(theKernel)
   {
      theKernel = new NEWMAT::Matrix;
   }
   *theKernel = kernel;
   theWidth  = theKernel->Ncols();
   theHeight = theKernel->Nrows();   
}

void ossimDiscreteConvolutionKernel::convolve(const float* data,
                                              double& result,
                                              float nullPixel)const
{
   result = 0;

   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
   }
}

void ossimDiscreteConvolutionKernel::convolveSubImage(const float* data,
                                                      long dataWidth,
                                                      double& result,
                                                      float nullPixel)const
{
   const float* dataStart = NULL;
   result = 0;
   
   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
   }
}

void ossimDiscreteConvolutionKernel::convolve(const unsigned char* data,
                                              double& result,
                                              ossim_uint8 nullPixel)const
{
   result = 0;

   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
   }
}

void ossimDiscreteConvolutionKernel::convolveSubImage(const unsigned char* data,
                                                      long dataWidth,
                                                      double& result,
                                                      ossim_uint8 nullPixel)const
{
   const ossim_uint8* dataStart = NULL;
   result = 0;
   
   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
   }
}


void ossimDiscreteConvolutionKernel::convolve(const unsigned short* data,
                                              double& result,
                                              ossim_uint16 nullPixel)const
{
   result = 0;

   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
   }
}

void ossimDiscreteConvolutionKernel::convolveSubImage(const unsigned short* data,
                                                      long dataWidth,
                                                      double& result,
                                                      ossim_uint16 nullPixel)const
{
   const ossim_uint16* dataStart = NULL;
   result = 0;
   
   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
   }
}

void ossimDiscreteConvolutionKernel::convolve(const short* data,
                                              double& result,
                                              ossim_sint16 nullPixel)const
{
   result = 0;

   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
   }
}

void ossimDiscreteConvolutionKernel::convolveSubImage(const short* data,
                                                      long dataWidth,
                                                      double& result,
                                                      ossim_sint16 nullPixel)const
{
   const ossim_sint16* dataStart = NULL;
   result = 0;
   
   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
   }
}


void ossimDiscreteConvolutionKernel::convolve(const double* data,
                                              double& result,
                                              double nullPixel)const
{
   result = 0;

   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         for(long col=0; col < theWidth; ++col)
         {
            if(*data != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*data));
            }
            ++data;
         }
      }
   }
}

void ossimDiscreteConvolutionKernel::convolveSubImage(const double* data,
                                                      long dataWidth,
                                                      double& result,
                                                      double nullPixel)const
{
   const double* dataStart = NULL;
   result = 0;
   
   if(theComputeWeightedAverageFlag)
   {
      double divisor = 0.0;
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               divisor += (*theKernel)[row][col];
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
      if(divisor > 0)
      {
         result /= divisor;
      }
   }
   else
   {
      for(long row=0; row < theHeight; ++row)
      {
         dataStart=data;
         for(long col=0; col < theWidth; ++col)
         {
            if(*dataStart != nullPixel)
            {
               result  += ((*theKernel)[row][col] * (*dataStart));
            }
            ++dataStart;
         }
         data+=dataWidth;
      }
   }
}
