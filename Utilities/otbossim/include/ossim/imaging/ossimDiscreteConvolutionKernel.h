//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimDiscreteConvolutionKernel.h 17195 2010-04-23 17:32:18Z dburken $
#ifndef ossimDiscreteConvolutionKernel_HEADER
#define ossimDiscreteConvolutionKernel_HEADER
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>

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
                         float nullPixel=OSSIM_DEFAULT_NULL_PIX_FLOAT)const;
   
   virtual void convolveSubImage(const float* data,
                                 long dataWidth,
                                 double& result,
                                 float nullPixel=OSSIM_DEFAULT_NULL_PIX_FLOAT)const;

   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const double* data,
                         double& result,
                         double nullPixel=OSSIM_DEFAULT_NULL_PIX_DOUBLE)const;

   virtual void convolveSubImage(const double* data,
                                 long dataWidth,
                                 double& result,
                                 double nullPixel=OSSIM_DEFAULT_NULL_PIX_DOUBLE)const;
   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const short* data,
                         double& result,
                         ossim_sint16 nullPixel=OSSIM_DEFAULT_NULL_PIX_SINT16)const;
   
   virtual void convolveSubImage(const short* data,
                                 long dataWidth,
                                 double& result,
                                 ossim_sint16 nullPixel=OSSIM_DEFAULT_NULL_PIX_SINT16)const;

   /*!
    * Will expect a data buffer of size width*height
    * and is row ordered.
    */
   virtual void convolve(const unsigned short* data,
                         double& result,
                         ossim_uint16 nullPixel=OSSIM_DEFAULT_NULL_PIX_UINT16)const;
   
   virtual void convolveSubImage(const unsigned short* data,
                                 long dataWidth,
                                 double& result,
                                 ossim_uint16 nullPixel=OSSIM_DEFAULT_NULL_PIX_UINT16)const;
   
   
   virtual void convolve(const unsigned char* data,
                         double& result,
                         ossim_uint8 nullPixel=OSSIM_DEFAULT_NULL_PIX_UINT8)const;
   /*!
    * this allows you to pass a subImage to
    * the convolution engine.  It needs to know
    * the width of the buffer so it can increment
    * to the next element.
    */
   virtual void convolveSubImage(const unsigned char* data,
                                 long dataWidth,
                                 double& result,
                                 ossim_uint8 nullPixel=OSSIM_DEFAULT_NULL_PIX_UINT8)const;
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
   virtual void buildConvolution(double /* xLocation */,
                                 double /* yLocation */)
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
