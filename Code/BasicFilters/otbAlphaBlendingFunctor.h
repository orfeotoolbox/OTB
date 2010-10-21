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

namespace otb
{
namespace Functor
{
/**
 * \class AlphaBlendingFunctor
 * \brief Implements simple blending
 *
 * For each channel the blending function is as follows:
 * \f[ P_{o} = (1 - \alpha) P_{i1} + \alpha P_{i2} \f]
 * with
 * \f[ \alpha = \alpha_{pix} * \alpha_{glo} \f]
 * \f$ \alpha_{pix} \f$ is retrieved from the second pixel RGBA
 * \f$ \alpha_{glo} \f$ is provided by the SetAlpha() method (1.0 by default)
 *
 */

template<class TInputPixel, class TInputPixel2, class TOutputPixel>
class ITK_EXPORT AlphaBlendingFunctor
{
public:
  AlphaBlendingFunctor(): m_Alpha(1.0) {}
  ~AlphaBlendingFunctor(){}

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
    double alpha = static_cast<double>(input2.GetAlpha()) / 255.0 * m_Alpha;

    resp.SetRed(static_cast<OutputInternalPixelType>(vcl_floor(0.5 +
                                                       (1.0 - alpha) * static_cast<double>(input1.GetRed())
                                                       + alpha * static_cast<double>(input2.GetRed())
                                                       )));
    resp.SetGreen(static_cast<OutputInternalPixelType>(vcl_floor(0.5 +
                                                         (1.0 - alpha) * static_cast<double>(input1.GetGreen())
                                                         + alpha * static_cast<double>(input2.GetGreen())
                                                         )));
    resp.SetBlue(static_cast<OutputInternalPixelType>(vcl_floor(0.5 +
                                                        (1.0 - alpha) * static_cast<double>(input1.GetBlue())
                                                        + alpha * static_cast<double>(input2.GetBlue())
                                                        )));
    return resp;
  }

protected:
  double m_Alpha;
};

}
}
