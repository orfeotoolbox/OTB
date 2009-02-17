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
#ifndef __otbRGBRenderingFunction_h
#define __otbRGBRenderingFunction_h

#include "otbRenderingFunction.h"

namespace otb
{
namespace Function
{
/**\class RGBRenderingFunction
 * \brief RGB rendering.
 * If the input image is an Image, the function
 * renders it with R, G and B channels all equals. 
 * If it is a VectorImage, the function renders
 * the selected channels.
 */
template <class TPixelPrecision, class TRGBPixel>
class RGBRenderingFunction
  : public RenderingFunction<TPixelPrecision,TRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef RGBRenderingFunction                   Self;
  typedef RenderingFunction<TPixelPrecision,TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(RGBRenderingFunction,RenderingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TRGBPixel                                  OutputPixelType;
  typedef typename OutputPixelType::ValueType        OutputValueType;
  typedef TPixelPrecision                            ScalarPixelType;
  typedef itk::VariableLengthVector<ScalarPixelType> VectorPixelType;
  /** Extrema vector */
  typedef std::vector<ScalarPixelType>               ExtremaVectorType;

  /** Evaluate method (scalar version) */
  virtual const OutputPixelType Evaluate(ScalarPixelType spixel)
  {
    OutputPixelType resp;
    resp.Fill(this->Evaluate(spixel,this->m_Minimum[0],this->m_Maximum[0]));
    return resp;
  }
  /** Evaluate method (vector version) */
  virtual const OutputPixelType Evaluate(const VectorPixelType & vpixel)
  {
    OutputPixelType resp;
    resp.SetRed(this->Evaluate(vpixel[m_RedChannelIndex],this->m_Minimum[m_RedChannelIndex],this->m_Maximum[m_RedChannelIndex]));
    resp.SetBlue(this->Evaluate(vpixel[m_BlueChannelIndex],this->m_Minimum[m_BlueChannelIndex],this->m_Maximum[m_BlueChannelIndex]));
    resp.SetGreen(this->Evaluate(vpixel[m_GreenChannelIndex],this->m_Minimum[m_GreenChannelIndex],this->m_Maximum[m_GreenChannelIndex]));
    return resp;
  }

  /** Set the red channel index (vector mode only) */
  void SetRedChannelIndex(unsigned int index)
  {
    m_RedChannelIndex = index;
  }
  
  /** Get the red channel index (vector mode only) */
  unsigned int GetRedChannelIndex(void)
  {
    return m_RedChannelIndex;
  }

  /** Set the blue channel index (vector mode only) */
  void SetBlueChannelIndex(unsigned int index)
  {
    m_BlueChannelIndex = index;
  }
  
  /** Get the blue channel index (vector mode only) */
  unsigned int GetBlueChannelIndex(void)
  {
    return m_BlueChannelIndex;
  }

  /** Set the green channel index (vector mode only) */
  void SetGreenChannelIndex(unsigned int index)
  {
    m_GreenChannelIndex = index;
  }
  
  /** Get the green channel index (vector mode only) */
  unsigned int GetGreenChannelIndex(void)
  {
    return m_GreenChannelIndex;
  }

protected:
  /** Constructor */
  RGBRenderingFunction() : m_RedChannelIndex(0), m_BlueChannelIndex(1), m_GreenChannelIndex(2) {}
  /** Destructor */
  ~RGBRenderingFunction() {}
  /** Perform the computation for a single value (this is done in
   * order to have the same code for vector and scalar version)
   */
  const OutputValueType Evaluate(ScalarPixelType input, ScalarPixelType min, ScalarPixelType max)
  {
    if(input > max)
      {
      return 255;
      }
    else if(input < min)
      {
      return 0;
      }
    else
      {
      return static_cast<OutputValueType>(vcl_floor(255.*(static_cast<double>(input)-static_cast<double>(min))/(static_cast<double>(max)-static_cast<double>(min))+0.5));
      }
  }

private:
  RGBRenderingFunction(const Self&); //purposely not implemented
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


