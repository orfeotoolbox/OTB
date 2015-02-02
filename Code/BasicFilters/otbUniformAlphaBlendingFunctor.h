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
#ifndef __otbUniformAlphaBlendingFunctor_h
#define __otbUniformAlphaBlendingFunctor_h

#include "itkMacro.h"
#include "itkNumericTraits.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/**
 * \class UniformAlphaBlendingFunctor
 * \brief Implements simple blending
 * For each channel the blending function is as follows:
 * \f[ P_{o} = \alpha P_{i1} + (1 - \alpha) P_{i2} \f]
 */

template<class TInputPixel, class TInputPixel2, class TOutputPixel>
class ITK_EXPORT UniformAlphaBlendingFunctor
{
public:
  UniformAlphaBlendingFunctor(): m_Alpha(0.5) {}
  ~UniformAlphaBlendingFunctor(){}

  typedef TInputPixel                         InputPixelType;
  typedef TInputPixel2                        InputPixel2Type;
  typedef TOutputPixel                        OutputPixelType;
  typedef typename OutputPixelType::ValueType OutputInternalPixelType;

  void SetAlpha(double a)
  {
    //Keep alpha between 0 and 1
    m_Alpha = a<0.0?0.0:(a>1.0?1.0:a);
  }
  double GetAlpha() const
  {
    return m_Alpha;
  }

  inline OutputPixelType operator ()(InputPixelType input1, InputPixel2Type input2) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<OutputInternalPixelType>::max());
    resp.SetRed(static_cast<OutputInternalPixelType>(vcl_floor(m_Alpha * static_cast<double>(input1.GetRed())   +
                                                       (1 - m_Alpha) * static_cast<double>(input2.GetRed())  + 0.5)));
    resp.SetGreen(static_cast<OutputInternalPixelType>(vcl_floor(m_Alpha * static_cast<double>(input1.GetGreen()) +
                                                         (1 - m_Alpha) * static_cast<double>(input2.GetGreen()) + 0.5)));
    resp.SetBlue(static_cast<OutputInternalPixelType>(vcl_floor(m_Alpha * static_cast<double>(input1.GetBlue())  +
                                                        (1 - m_Alpha) * static_cast<double>(input2.GetBlue()) + 0.5)));
    return resp;
  }

protected:
  double m_Alpha;
};

}
}
#endif
