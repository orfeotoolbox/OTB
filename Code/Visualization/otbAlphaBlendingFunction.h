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
#ifndef __otbAlphaBlendingFunction_h
#define __otbAlphaBlendingFunction_h

#include "otbBlendingFunction.h"
#include "otbMath.h"

namespace otb
{
namespace Function
{
/**
 * \class AlphaBlendingFunction
 * \brief Implements simple blending
 * For each channel the blending function is as follows:
 * \f[ P_{o} = \alpha P_{i1} + (1 - \alpha) P_{i2} \f]
 * \alpha is retrieved from the second pixel RGBA
 *  \ingroup Visualization
 */
template <class TInputRGBPixel1, class TInputRGBAPixel2, class TOutputRGBPixel = TInputRGBPixel1>
class AlphaBlendingFunction
  : public BlendingFunction<TInputRGBPixel1,TInputRGBAPixel2,TOutputRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef AlphaBlendingFunction                 Self;
  typedef BlendingFunction<TInputRGBPixel1,
                           TInputRGBAPixel2,
                           TOutputRGBPixel>            Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(AlphaBlendingFunction,BlendingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TInputRGBPixel1                        InputPixel1Type;
  typedef TInputRGBAPixel2                        InputPixel2Type;
  typedef TOutputRGBPixel                        OutputRGBPixelType;
  typedef typename OutputRGBPixelType::ValueType OutputValueType;

  /** Evaluate method  */
  inline virtual const OutputRGBPixelType Evaluate(const InputPixel1Type& input1, const InputPixel2Type & input2)
  {
    OutputRGBPixelType resp;
    double alpha = static_cast<double>(input2.GetAlpha())/255.0;

    resp.SetRed(  static_cast<OutputValueType>(vcl_floor((1-alpha) * static_cast<double>(input1.GetRed())   +alpha*static_cast<double>(input2.GetRed())  +0.5)));
    resp.SetGreen(static_cast<OutputValueType>(vcl_floor((1-alpha) * static_cast<double>(input1.GetGreen()) +alpha*static_cast<double>(input2.GetGreen())+0.5)));
    resp.SetBlue( static_cast<OutputValueType>(vcl_floor((1-alpha) * static_cast<double>(input1.GetBlue())  +alpha*static_cast<double>(input2.GetBlue()) +0.5)));
    return resp;
  }

protected:
  /** Constructor */
  AlphaBlendingFunction() {}
  /** Destructor */
  ~AlphaBlendingFunction() {}
private:
  AlphaBlendingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
} // end namespace Functor
} // end namespace otb

#endif


