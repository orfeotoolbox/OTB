//*******************************************************************
//
// LICENSE: See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimDiscrete3x3HatFilter.cpp 11419 2007-07-27 16:24:57Z dburken $
#include <ossim/imaging/ossimDiscrete3x3HatFilter.h>
#include <ossim/base/ossimCommon.h>

ossimDiscrete3x3HatFilter::ossimDiscrete3x3HatFilter()
   :ossimDiscreteConvolutionKernel(3,3)
{
   // make the default be the nearest neighbor
   (*theKernel)[0][0] = 0;
   (*theKernel)[0][1] = 0;
   (*theKernel)[0][2] = 0;

   (*theKernel)[1][0] = 0;
   (*theKernel)[1][1] = 1;
   (*theKernel)[1][2] = 0;

   (*theKernel)[2][0] = 0;
   (*theKernel)[2][1] = 0;
   (*theKernel)[2][2] = 0;
}

void ossimDiscrete3x3HatFilter::convolve(const float* data,
                                         double& result,
                                         float nullPixel)const
{
   double divisor = 0;
   result         = 0;

   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[0][0];
      result  += (*theKernel)[0][0] * data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[0][1];
      result  += (*theKernel)[0][1] * data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[0][2];
      result  += (*theKernel)[0][2] * data[2];
   }
   if(data[3] != nullPixel)
   {
      divisor += (*theKernel)[1][0];
      result  += (*theKernel)[1][0] * data[3];
   }
   if(data[4] != nullPixel)
   {
      divisor += (*theKernel)[1][1];
      result  += (*theKernel)[1][1] * data[4];
   }
   if(data[5] != nullPixel)
   {
      divisor += (*theKernel)[1][2];
      result  += (*theKernel)[1][2] * data[5];
   }
   if(data[6] != nullPixel)
   {
      divisor += (*theKernel)[2][0];
      result  += (*theKernel)[2][0] * data[6];
   }
   if(data[7] != nullPixel)
   {
      divisor += (*theKernel)[2][1];
      result  += (*theKernel)[2][1] * data[7];
   }
   if(data[8] != nullPixel)
   {
      divisor += (*theKernel)[2][2];
      result  += (*theKernel)[2][2] * data[8];
   }
   

   if(divisor > 0)
      result /= divisor;
}

void ossimDiscrete3x3HatFilter::convolveSubImage(const float* data,
                                                 long dataWidth,
                                                 double& result,
                                                 float nullPixel)const
{
   double divisor = 0.0;
   result = 0;
   
   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[0][0];
      result  += (*theKernel)[0][0]*data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[0][1];
      result  += (*theKernel)[0][1]*data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[0][2];
      result  += (*theKernel)[0][2]*data[2];
   }
   
   data +=dataWidth;
   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[1][0];
      result  += (*theKernel)[1][0]*data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[1][1];
      result  += (*theKernel)[1][1]*data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[1][2];
      result  += (*theKernel)[1][2]*data[2];
   }
   
   data +=dataWidth;
   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[2][0];
      result  += (*theKernel)[2][0]*data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[2][1];
      result  += (*theKernel)[2][1]*data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[2][2];
      result  += (*theKernel)[2][2]*data[2];
   }

   if(divisor > 0)
   {
      result /= divisor;
   }
}

void ossimDiscrete3x3HatFilter::convolve(const unsigned char* data,
                                         double& result,
                                         ossim_uint8 nullPixel)const
{
   double divisor = 0;
   result         = 0;

   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[0][0];
      result  += (*theKernel)[0][0] * data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[0][1];
      result  += (*theKernel)[0][1] * data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[0][2];
      result  += (*theKernel)[0][2] * data[2];
   }
   if(data[3] != nullPixel)
   {
      divisor += (*theKernel)[1][0];
      result  += (*theKernel)[1][0] * data[3];
   }
   if(data[4] != nullPixel)
   {
      divisor += (*theKernel)[1][1];
      result  += (*theKernel)[1][1] * data[4];
   }
   if(data[5] != nullPixel)
   {
      divisor += (*theKernel)[1][2];
      result  += (*theKernel)[1][2] * data[5];
   }
   if(data[6] != nullPixel)
   {
      divisor += (*theKernel)[2][0];
      result  += (*theKernel)[2][0] * data[6];
   }
   if(data[7] != nullPixel)
   {
      divisor += (*theKernel)[2][1];
      result  += (*theKernel)[2][1] * data[7];
   }
   if(data[8] != nullPixel)
   {
      divisor += (*theKernel)[2][2];
      result  += (*theKernel)[2][2] * data[8];
   }
   

   if(divisor > 0)
      result /= divisor;
}


void ossimDiscrete3x3HatFilter::convolveSubImage(const unsigned char* data,
                                                 long dataWidth,
                                                 double& result,
                                                 ossim_uint8 nullPixel)const
{
   double divisor = 0.0;
   result = 0;
   
   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[0][0];
      result  += (*theKernel)[0][0]*data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[0][1];
      result  += (*theKernel)[0][1]*data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[0][2];
      result  += (*theKernel)[0][2]*data[2];
   }
   
   data +=dataWidth;
   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[1][0];
      result  += (*theKernel)[1][0]*data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[1][1];
      result  += (*theKernel)[1][1]*data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[1][2];
      result  += (*theKernel)[1][2]*data[2];
   }
   
   data +=dataWidth;
   if(data[0] != nullPixel)
   {
      divisor += (*theKernel)[2][0];
      result  += (*theKernel)[2][0]*data[0];
   }
   if(data[1] != nullPixel)
   {
      divisor += (*theKernel)[2][1];
      result  += (*theKernel)[2][1]*data[1];
   }
   if(data[2] != nullPixel)
   {
      divisor += (*theKernel)[2][2];
      result  += (*theKernel)[2][2]*data[2];
   }

   if(divisor > 0)
   {
      result /= divisor;
   }
}

void ossimDiscrete3x3HatFilter::buildConvolution(double xLocation,
                                                 double yLocation)
{
   NEWMAT::RowVector    row(3);
   NEWMAT::ColumnVector col(3);
   
   row[0] = std::abs(xLocation);
   row[1] = 1;
   row[2] = std::abs(xLocation);
   col[0] = std::abs(yLocation);
   col[1] = 1;
   col[2] = std::abs(yLocation);
  
   (*theKernel) = col*row;
}
