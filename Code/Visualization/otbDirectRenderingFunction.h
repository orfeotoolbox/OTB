/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbDirectRenderingFunction_h
#define __otbDirectRenderingFunction_h

#include "otbRenderingFunction.h"
#include "otbMacro.h"
#include <assert.h>

namespace otb
{
namespace Function
{

/** \class DirectRenderingFunction
 * \brief Standard rendering.
 *
 * This function just reproduce the input value at the output
 * with the risk of truncated values. Use it if you know that
 * your input values are between 0 and 255.
 *
 *  \ingroup Visualization
 */
template <class TPixelPrecision, class TRGBPixel >
class DirectRenderingFunction
  : public RenderingFunction<TPixelPrecision,TRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef DirectRenderingFunction                   Self;
  typedef RenderingFunction<TPixelPrecision,TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(DirectRenderingFunction,RenderingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TRGBPixel                                  OutputPixelType;
  typedef typename OutputPixelType::ValueType        OutputValueType;
  typedef TPixelPrecision                            PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>       VectorPixelType;
  typedef itk::RGBPixel<ScalarType> RGBPixelType;
  typedef itk::RGBAPixel<ScalarType> RGBAPixelType;

  /** Evaluate method (scalar version) */
  inline virtual const OutputPixelType Evaluate(ScalarType spixel) const
  {
    OutputPixelType resp;
    resp.SetRed(spixel);
    resp.SetGreen(spixel);
    resp.SetBlue(spixel);
    return resp;
  }
  /** Evaluate method (vector version) */
  inline virtual const OutputPixelType Evaluate(const VectorPixelType & vpixel) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    resp.SetRed(vpixel[m_RedChannelIndex]);
    resp.SetGreen(vpixel[m_GreenChannelIndex]);
    resp.SetBlue(vpixel[m_BlueChannelIndex]);
    return resp;
  }
  /** Evaluate method (RGB pixel version) */
  inline virtual const OutputPixelType Evaluate(const RGBPixelType & vpixel) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    resp.SetRed(vpixel[m_RedChannelIndex]);
    resp.SetGreen(vpixel[m_GreenChannelIndex]);
    resp.SetBlue(vpixel[m_BlueChannelIndex]);
    return resp;
  }
  /** Evaluate method (RGBA pixel version) */
  inline virtual const OutputPixelType Evaluate(const RGBAPixelType & vpixel) const
  {
    OutputPixelType resp;
//     resp.Fill(itk::NumericTraits<typename OutputPixelType::ValueType>::max());
    if (OutputPixelType::Length == 4)
    {//Propagate the alpha channel
      resp[3] = vpixel[3];
    }
    resp.SetRed(vpixel[m_RedChannelIndex]);
    resp.SetGreen(vpixel[m_GreenChannelIndex]);
    resp.SetBlue(vpixel[m_BlueChannelIndex]);
    return resp;
  }

  inline const std::string Describe(ScalarType spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"Grayscale [value: "<< static_cast<typename itk::NumericTraits<PixelType>::PrintType>(spixel)<<", displayed: "<< static_cast<unsigned int>(output[0])<<"]";
    return oss.str();
  }

  inline const std::string Describe(const VectorPixelType & vpixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(vpixel);

    for(unsigned int channel = 0; channel < vpixel.Size();++channel)
      {
      if(channel == m_RedChannelIndex)
        {
        oss<<"c= "<<channel<<", R= "<<(int)output[0];
        if(output[0]<100)
          {
          oss<<" ";
          }
        if(output[0]<10)
          {
          oss<<" ";
          }
        oss<<", v= "<<static_cast<typename itk::NumericTraits<PixelType>::PrintType>(vpixel[channel])<<std::endl;
        }
      else if(channel == m_GreenChannelIndex)
        {
        oss<<"c= "<<channel<<", G= "<<(int)output[1];
        if(output[1]<100)
          {
          oss<<" ";
          }
        if(output[1]<10)
          {
          oss<<" ";
          }
        oss<<", v= "<<static_cast<typename itk::NumericTraits<PixelType>::PrintType>(vpixel[channel])<<std::endl;
        }
      else if(channel == m_BlueChannelIndex)
        {
        oss<<"c= "<<channel<<", B= "<<(int)output[2];
        if(output[2]<100)
          {
          oss<<" ";
          }
        if(output[2]<10)
          {
          oss<<" ";
          }
        oss<<", v= "<<static_cast<typename itk::NumericTraits<PixelType>::PrintType>(vpixel[channel])<<std::endl;
        }
      else
        {
        oss<<"c= "<<channel<<",         v= "<<static_cast<typename itk::NumericTraits<PixelType>::PrintType>(vpixel[channel])<<std::endl;
        }
      }
    return oss.str();
  }

  inline const std::string Describe(const RGBPixelType & spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"RGB value: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[0])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[1])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[2])
        << std::endl;
    oss <<"   displayed: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[0])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[1])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[2])
        <<std::endl;
    return oss.str();
  }

  inline const std::string Describe(const RGBAPixelType & spixel) const
  {
    itk::OStringStream oss;
    OutputPixelType output = this->Evaluate(spixel);
    oss<<"RGBA value: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[0])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[1])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[2])
        << " alpha: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(spixel[3])
        << std::endl;
    oss <<"   displayed: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[0])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[1])
        << ", "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[2])
        << " alpha: "<< static_cast<typename itk::NumericTraits<ScalarType>::PrintType>(output[3])
        <<std::endl;
    return oss.str();
  }



protected:
  /** Constructor */
  DirectRenderingFunction() : m_RedChannelIndex(0), m_GreenChannelIndex(1), m_BlueChannelIndex(2)
  {}
  /** Destructor */
  ~DirectRenderingFunction() {}

private:
  DirectRenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Index of the channels to display (vector mode only, has no effet
   *  on scalar mode)
   */
  unsigned int m_RedChannelIndex;
  unsigned int m_GreenChannelIndex;
  unsigned int m_BlueChannelIndex;

};
} // end namespace Functor
} // end namespace otb

#endif


