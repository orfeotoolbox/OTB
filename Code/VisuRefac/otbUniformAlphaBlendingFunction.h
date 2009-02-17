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
#ifndef __otbUniformAlphaBlendingFunction_h
#define __otbUniformAlphaBlendingFunction_h

#include "otbBlendingFunction.h"
#include "otbMath.h"

namespace otb
{
namespace Function
{
/**\class UniformAlphaBlendingFunction
 * \brief Implements simple blending
 * For each channel the blending function is as follows:
 * $P_{o} = \alpha P_{i1} + (1 - \alpha P_{i2} $
 */
template <class TInputRGBPixel1, class TInputRGBPixel2 = TInputRGBPixel1, class TOutputRGBPixel = TInputRGBPixel1>
class UniformAlphaBlendingFunction
  : public BlendingFunction<TInputRGBPixel1,TInputRGBPixel2,TOutputRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef UniformAlphaBlendingFunction                 Self;
  typedef BlendingFunction<TInputRGBPixel1,
			   TInputRGBPixel2,
			   TOutputRGBPixel>            Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(UniformAlphaBlendingFunction,BlendingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TInputRGBPixel1                        InputPixel1Type;
  typedef TInputRGBPixel2                        InputPixel2Type;
  typedef TOutputRGBPixel                        OutputRGBPixelType;
  typedef typename OutputRGBPixelType::ValueType OutputValueType;

  /** Evaluate method  */
  virtual const OutputRGBPixelType Evaluate(const InputPixel1Type& input1, const InputPixel2Type & input2)
  {
    OutputRGBPixelType resp;
    resp.SetRed(  static_cast<OutputValueType>(vcl_floor(m_Alpha * static_cast<double>(input1.GetRed())   +(1-m_Alpha)*static_cast<double>(input2.GetRed())  +0.5)));
    resp.SetGreen(static_cast<OutputValueType>(vcl_floor(m_Alpha * static_cast<double>(input1.GetGreen()) +(1-m_Alpha)*static_cast<double>(input2.GetGreen())+0.5)));
    resp.SetBlue( static_cast<OutputValueType>(vcl_floor(m_Alpha * static_cast<double>(input1.GetBlue())  +(1-m_Alpha)*static_cast<double>(input2.GetBlue()) +0.5)));
    return resp;
  }

  /** Set/Get the alpha value */
  itkSetClampMacro(Alpha,double,0.,1.);
  itkGetMacro(Alpha,double);

protected:
  /** Constructor */
  UniformAlphaBlendingFunction() : m_Alpha(0.5) {}
  /** Destructor */
  ~UniformAlphaBlendingFunction() {}
private:
  UniformAlphaBlendingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Alpha value for blending (should be in the range [0,1] */
  double m_Alpha;
  
};
} // end namespace Functor
} // end namespace otb

#endif


