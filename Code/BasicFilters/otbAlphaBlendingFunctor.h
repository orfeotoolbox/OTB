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
#ifndef __otbAlphaBlendingFunctor_h
#define __otbAlphaBlendingFunctor_h

#include "itkRGBAPixel.h"
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

template<class TInputPixel1, class TInputPixel2, class TOutputPixel>
class ITK_EXPORT AlphaBlendingFunctorBase
{
public:
  AlphaBlendingFunctorBase(): m_Alpha(1.0) {}
  ~AlphaBlendingFunctorBase(){}

  typedef TInputPixel1                        InputPixel1Type;
  typedef TInputPixel2                        InputPixel2Type;
  typedef TOutputPixel                        OutputPixelType;

  virtual void SetAlpha(double a)
  {
    //Keep alpha between 0 and 1
    m_Alpha = a<0.0?0.0:(a>1.0?1.0:a);
  }

  virtual double GetAlpha() const
  {
    return m_Alpha;
  }

  virtual inline OutputPixelType operator ()(InputPixel1Type input1, InputPixel2Type input2) const = 0;

protected:
  double m_Alpha;
};

template<class TInputPixel1, class TInputPixel2, class TOutputPixel>
class ITK_EXPORT AlphaBlendingFunctor
  :public AlphaBlendingFunctorBase< TInputPixel1, TInputPixel2, TOutputPixel >
{
public:
  AlphaBlendingFunctor(){}
  ~AlphaBlendingFunctor(){}

  typedef TInputPixel1                        InputPixel1Type;
  typedef TInputPixel2                        InputPixel2Type;
  typedef TOutputPixel                        OutputPixelType;

  inline OutputPixelType operator ()(InputPixel1Type input1, InputPixel2Type input2) const
  {
    OutputPixelType resp;
    double alpha = this->GetAlpha();

    resp = static_cast<OutputPixelType>(vcl_floor(0.5 + (1.0 - alpha) * static_cast<double>(input1)
                                                       + alpha * static_cast<double>(input2)));
    return resp;
  }

protected:
  double m_Alpha;
};

template<class TInputInternalPixel1, class TInputInternalPixel2, class TOutputInternalPixel>
class ITK_EXPORT AlphaBlendingFunctor< itk::RGBAPixel<TInputInternalPixel1>,
                                       itk::RGBAPixel<TInputInternalPixel2>,
                                       itk::RGBAPixel<TOutputInternalPixel> > :
  public AlphaBlendingFunctorBase<itk::RGBAPixel<TInputInternalPixel1>,
                                  itk::RGBAPixel<TInputInternalPixel2>,
                                  itk::RGBAPixel<TOutputInternalPixel> >
{
public:
  AlphaBlendingFunctor(){}
  ~AlphaBlendingFunctor(){}

  typedef TInputInternalPixel1                     InternalInputPixel1Type;
  typedef itk::RGBAPixel<InternalInputPixel1Type>  InputPixel1Type;
  typedef TInputInternalPixel2                     InternalInputPixel2Type;
  typedef itk::RGBAPixel<InternalInputPixel2Type>  InputPixel2Type;
  typedef TOutputInternalPixel                     InternalOutputPixelType;
  typedef itk::RGBAPixel<InternalOutputPixelType>  OutputPixelType;

  inline OutputPixelType operator ()(InputPixel1Type input1, InputPixel2Type input2) const
  {
    OutputPixelType resp;
    resp.Fill(itk::NumericTraits<InternalOutputPixelType>::max());
    double alpha = static_cast<double>(input2.GetAlpha()) / 255.0 * this->GetAlpha();

    resp.SetRed(static_cast<InternalOutputPixelType>(vcl_floor(0.5 +
                                                       (1.0 - alpha) * static_cast<double>(input1.GetRed())
                                                       + alpha * static_cast<double>(input2.GetRed())
                                                       )));
    resp.SetGreen(static_cast<InternalOutputPixelType>(vcl_floor(0.5 +
                                                         (1.0 - alpha) * static_cast<double>(input1.GetGreen())
                                                         + alpha * static_cast<double>(input2.GetGreen())
                                                         )));
    resp.SetBlue(static_cast<InternalOutputPixelType>(vcl_floor(0.5 +
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

#endif
