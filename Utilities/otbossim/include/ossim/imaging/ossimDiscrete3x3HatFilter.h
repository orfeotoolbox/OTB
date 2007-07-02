//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimDiscrete3x3HatFilter.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimDiscrete3x3HatFilter_HEADER
#define ossimDiscrete3x3HatFilter_HEADER
#include <ossim/imaging/ossimDiscreteConvolutionKernel.h>
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
