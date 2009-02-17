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
#ifndef __otbGrayscaleRenderingFunction_h
#define __otbGrayscaleRenderingFunction_h

#include "otbRenderingFunction.h"

namespace otb
{
namespace Function
{
/**\class GrayscaleRenderingFunction
 * \brief Grayscale rendering.
 * If the input image is an Image, the function
 * renders it. If it is a VectorImage, the function renders
 * the selected channel.
 */
template <class TPixelPrecision, class TRGBPixel>
class GrayscaleRenderingFunction
  : public RenderingFunction<TPixelPrecision,TRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef GrayscaleRenderingFunction                   Self;
  typedef RenderingFunction<TPixelPrecision,TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(GrayscaleRenderingFunction,RenderingFunction);

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
    resp.Fill(this->Evaluate(vpixel[m_ChannelIndex],this->m_Minimum[m_ChannelIndex],this->m_Maximum[m_ChannelIndex]));
    return resp;
  }

  /** Set the channel index (vector mode only) */
  void SetChannelIndex(unsigned int index)
  {
    m_ChannelIndex = index;
  }
  
  /** Get the channel index (vector mode only) */
  unsigned int GetChannelIndex(void)
  {
    return m_ChannelIndex;
  }

protected:
  /** Constructor */
  GrayscaleRenderingFunction() : m_ChannelIndex(0) {}
  /** Destructor */
  ~GrayscaleRenderingFunction() {}
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
  GrayscaleRenderingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Index of the channel to display (vector mode only, has no effet
   *  on scalar mode)
   */
  unsigned int m_ChannelIndex;
};
} // end namespace Functor
} // end namespace otb

#endif


