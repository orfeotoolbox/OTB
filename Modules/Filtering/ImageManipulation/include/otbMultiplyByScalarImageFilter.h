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
#ifndef __otbMultiplyByScalarImageFilter_h
#define __otbMultiplyByScalarImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class MultiplyByScalarImageFilter
 * \brief TODO
 */

namespace Functor
{

template<class TInput, class TOutput>
class MultiplyByScalar
{
public:
  MultiplyByScalar()
  {
    m_Coef = 1.0;
  }
  virtual ~MultiplyByScalar() {}
  inline TOutput operator ()(const TInput& value)
  {
    TOutput result;

    result = static_cast<TOutput>(m_Coef * value);

    return result;
  }

  void SetCoef(double Coef)
  {
    m_Coef = Coef;
  }
  double GetCoef(void) const
  {
    return (m_Coef);
  }

private:
  double m_Coef;
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT MultiplyByScalarImageFilter :
  public itk::UnaryFunctorImageFilter<
      TInputImage, TOutputImage,
      Functor::MultiplyByScalar<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef MultiplyByScalarImageFilter Self;
  typedef typename itk::UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Functor::MultiplyByScalar<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  void SetCoef(double Coef)
  {
    this->GetFunctor().SetCoef(Coef);
    this->Modified();
  }
  double GetCoef(void) const
  {
    return (this->GetFunctor().GetCoef());
  }
protected:
  MultiplyByScalarImageFilter() {}
  virtual ~MultiplyByScalarImageFilter() {}

private:
  MultiplyByScalarImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
