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
#ifndef __otbMultiplyBlendingFunction_h
#define __otbMultiplyBlendingFunction_h

#include "otbBlendingFunction.h"
#include "otbMath.h"

namespace otb
{
namespace Function
{
/**
 * \class MultiplyBlendingFunction
 * \brief Implements simple blending
 * For each channel the blending function is as follows:
 * \f[ P_{o} = P_{i1} * P_{i2} / 255 \f]
 */
  template <class TInputRGBPixel1, class TInputRGBPixel2 = TInputRGBPixel1, class TOutputRGBPixel = TInputRGBPixel1>
      class MultiplyBlendingFunction
    : public BlendingFunction<TInputRGBPixel1,TInputRGBPixel2,TOutputRGBPixel>
  {
    public:
      /** Standard class typedefs */
      typedef MultiplyBlendingFunction                 Self;
      typedef BlendingFunction<TInputRGBPixel1,
      TInputRGBPixel2,
      TOutputRGBPixel>            Superclass;
      typedef itk::SmartPointer<Self>                      Pointer;
      typedef itk::SmartPointer<const Self>                ConstPointer;

      /** type macro */
      itkTypeMacro(MultiplyBlendingFunction,BlendingFunction);

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
        resp.SetRed(  static_cast<OutputValueType>(
                      vcl_floor( static_cast<double>(input1.GetRed())
                               * static_cast<double>(input2.GetRed())/255  +0.5)
                                                  ));
        resp.SetGreen(static_cast<OutputValueType>(
                      vcl_floor( static_cast<double>(input1.GetGreen())
                               * static_cast<double>(input2.GetGreen())/255+0.5)
                                                  ));
        resp.SetBlue( static_cast<OutputValueType>(
                      vcl_floor( static_cast<double>(input1.GetBlue())
                               * static_cast<double>(input2.GetBlue())/255 +0.5)
                                                  ));
        return resp;
      }

    protected:
      /** Constructor */
      MultiplyBlendingFunction() {}
      /** Destructor */
      ~MultiplyBlendingFunction() {}
    private:
      MultiplyBlendingFunction(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented


  };
} // end namespace Functor
} // end namespace otb

#endif


