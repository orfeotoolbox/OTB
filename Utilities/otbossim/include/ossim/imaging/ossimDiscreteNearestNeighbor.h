//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimDiscreteNearestNeighbor.h 17195 2010-04-23 17:32:18Z dburken $
#ifndef ossimDiscreteNearestNeighbor_HEADER
#define ossimDiscreteNearestNeighbor_HEADER
#include <ossim/imaging/ossimDiscreteConvolutionKernel.h>
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
                                       long /* dataWidth */,
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
                                        long /* dataWidth */,
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
                                       long /* dataWidth */,
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
                                        long /* dataWidth */,
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
                                        long /* dataWidth */,
                                        double& result)const
    {
       result = *data;
    }
    
    virtual void buildConvolution(double /* widthPercent */,
                                  double /* heightPercent */)
    {
    }
};

#endif
