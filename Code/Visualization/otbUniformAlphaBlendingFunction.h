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
#include "itkNumericTraits.h"
#include "otbUniformAlphaBlendingFunctor.h"

namespace otb
{
namespace Function
{
/**
 * \class UniformAlphaBlendingFunction
 * \brief Implements simple blending
 * For each channel the blending function is as follows:
 * \f[ P_{o} = \alpha P_{i1} + (1 - \alpha) P_{i2} \f]
 *
 * This class is not intended to be subclassed
 *
 *  \ingroup Visualization
 */
template <class TInputRGBPixel1, class TInputRGBPixel2 = TInputRGBPixel1, class TOutputRGBPixel = TInputRGBPixel1>
class UniformAlphaBlendingFunction
  : public BlendingFunction<TInputRGBPixel1, TInputRGBPixel2, TOutputRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef UniformAlphaBlendingFunction Self;
  typedef BlendingFunction<TInputRGBPixel1,
      TInputRGBPixel2,
      TOutputRGBPixel>            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** type macro */
  itkTypeMacro(UniformAlphaBlendingFunction, BlendingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TInputRGBPixel1                        InputPixel1Type;
  typedef TInputRGBPixel2                        InputPixel2Type;
  typedef TOutputRGBPixel                        OutputRGBPixelType;
  typedef typename OutputRGBPixelType::ValueType OutputValueType;
  typedef Functor::UniformAlphaBlendingFunctor<InputPixel1Type, InputPixel2Type, OutputRGBPixelType> FunctorType;

  /** Evaluate method  */
  inline const OutputRGBPixelType Evaluate(const InputPixel1Type& input1, const InputPixel2Type& input2) const
  {
    return functor(input1, input2);
  }

  /** Set/Get the alpha value */
  void SetAlpha(double a)
  {
    functor.SetAlpha(a);
  }
  double GetAlpha() const
  {
    return functor.GetAlpha();
  }

protected:
  /** Constructor */
  UniformAlphaBlendingFunction() : functor() {}
  /** Destructor */
  ~UniformAlphaBlendingFunction() {}
private:
  UniformAlphaBlendingFunction(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  FunctorType functor;

};
} // end namespace Functor
} // end namespace otb

#endif
