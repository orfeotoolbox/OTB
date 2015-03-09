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
#ifndef __otbLog10ThresholdedImageFilter_h
#define __otbLog10ThresholdedImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace otb
{
/** \class Log10ThresholdedImageFilter
 * \brief Computes the 10 * vcl_log10(x) pixel-wise with a threshold for the lowest values
 *
 * \ingroup OTBImageManipulation
 */
namespace Functor
{
template<class TInput, class TOutput>
class Log10Thresholded
{
public:
  Log10Thresholded()
  {
    m_ThresholdValue = 0.001; // -> -30dB
  }
  ~Log10Thresholded()
  {
  }

  bool operator!=(const Log10Thresholded &) const
  {
    return false;
  }
  bool operator==(const Log10Thresholded & other) const
  {
    return !(*this != other);
  }
  inline TOutput operator()(const TInput & A) const
  {
    if ((static_cast<double> (A) >= m_ThresholdValue) && (A == A))
      {
      return static_cast<TOutput> (10.0 * vcl_log10(static_cast<double> (A)));
      }
    else // If (A != A) then A is a NaN
      {
      return static_cast<TOutput> (10.0 * vcl_log10(m_ThresholdValue));
      }
  }

  /** Set ThresholdValue method */
  void SetThresholdValue(const double value)
  {
    if (value > 0.0)
      {
      m_ThresholdValue = value;
      }
  }

private:
  double m_ThresholdValue;
};
} // End namespace Functor

template <class TInputImage, class TOutputImage>
class ITK_EXPORT Log10ThresholdedImageFilter :
    public itk::UnaryFunctorImageFilter<TInputImage, TOutputImage, Functor::Log10Thresholded<typename TInputImage::PixelType, typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef Log10ThresholdedImageFilter                                Self;
  typedef Functor::Log10Thresholded<  typename TInputImage::PixelType, typename TOutputImage::PixelType> FunctorType;
  typedef itk::UnaryFunctorImageFilter<  TInputImage, TOutputImage, FunctorType>  Superclass;
  typedef itk::SmartPointer<Self>                              Pointer;
  typedef itk::SmartPointer<const Self>                        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(Log10ThresholdedImageFilter,
               itk::UnaryFunctorImageFilter);

  void SetThresholdedValue(double val)
  {
    this->GetFunctor().SetThresholdValue(val);
  }
protected:
  Log10ThresholdedImageFilter() {}
  virtual ~Log10ThresholdedImageFilter() {}

private:
  Log10ThresholdedImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
