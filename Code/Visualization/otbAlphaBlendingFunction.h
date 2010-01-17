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
 *
 * For each channel the blending function is as follows:
 * \f[ P_{o} = (1 - \alpha) P_{i1} + \alpha P_{i2} \f]
 * with
 * \f[ \alpha = \alpha_{pix} * \alpha_{glo} \f]
 * \f$ \alpha_{pix} \f$ is retrieved from the second pixel RGBA
 * \f$ \alpha_{glo} \f$ is provided by the SetAlpha() method (1.0 by default)
 *
 *  \ingroup Visualization
 */
template <class TInputRGBPixel1, class TInputRGBPixel2, class TOutputRGBPixel = TInputRGBPixel1>
class AlphaBlendingFunction
  : public BlendingFunction<TInputRGBPixel1,TInputRGBPixel2,TOutputRGBPixel>
{
public:
  /** Standard class typedefs */
  typedef AlphaBlendingFunction                 Self;
  typedef BlendingFunction<TInputRGBPixel1,
                           TInputRGBPixel2,
                           TOutputRGBPixel>            Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** type macro */
  itkTypeMacro(AlphaBlendingFunction,BlendingFunction);

  /** new macro */
  itkNewMacro(Self);

  /** PixelType macros */
  typedef TInputRGBPixel1                       InputPixel1Type;
  typedef TInputRGBPixel2                        InputPixel2Type;
  typedef TOutputRGBPixel                        OutputRGBPixelType;
  typedef typename OutputRGBPixelType::ValueType OutputValueType;

  /** Evaluate method  */
  inline virtual const OutputRGBPixelType Evaluate(const InputPixel1Type& input1, const InputPixel2Type & input2)
  {
    OutputRGBPixelType resp;
    resp.Fill(itk::NumericTraits<OutputValueType>::max());
    double alpha = static_cast<double>(input2.GetAlpha())/255.0 * m_Alpha;

    resp.SetRed(  static_cast<OutputValueType>(vcl_floor(0.5+
                  (1.0-alpha) * static_cast<double>(input1.GetRed())
                 + alpha * static_cast<double>(input2.GetRed())
                                                        )));
    resp.SetGreen(static_cast<OutputValueType>(vcl_floor(0.5+
                  (1.0-alpha) * static_cast<double>(input1.GetGreen())
                 + alpha * static_cast<double>(input2.GetGreen())
                                                        )));
    resp.SetBlue( static_cast<OutputValueType>(vcl_floor(0.5+
                  (1.0-alpha) * static_cast<double>(input1.GetBlue())
                 + alpha * static_cast<double>(input2.GetBlue())
                                                        )));
    return resp;
  }

  /** Set/Get the alpha value */
  itkSetClampMacro(Alpha,double,0.,1.);
  itkGetMacro(Alpha,double);

protected:
  /** Constructor */
  AlphaBlendingFunction() : m_Alpha(1.0) {}
  /** Destructor */
  virtual ~AlphaBlendingFunction() {}
private:
  AlphaBlendingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Alpha value for blending (should be in the range [0,1] */
  double m_Alpha;

};
} // end namespace Functor
} // end namespace otb

#endif


