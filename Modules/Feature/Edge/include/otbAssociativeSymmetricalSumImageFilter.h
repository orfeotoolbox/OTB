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
#ifndef __otbAssociativeSymmetricalSumImageFilter_h
#define __otbAssociativeSymmetricalSumImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace otb
{

/** \class AssociativeSymmetricalSumImageFilter
 * \brief Implements an assymetric fusion of line detector image filter.
 *
 * This class implements a fusion of the output images of
 * otb::LineRatioDetector and otb::LineCorrelationDetector
 *
 * The associative symmetrical sum \f$ \sigma(x, y) \f$ is used to merge information from the
 * two detectors:
   \f[ \sigma(x, y)=\frac{xy}{1-x-y+2xy} \f] with \f$ x, y \in [0, 1] \f$.
 *
 *
 *
 * \ingroup OTBEdge
 */

namespace Functor
{
/** \class AssociativeSymmetricalSum
 * \brief Functor used with the AssociativeSymmetricalSumImageFilter.
 *
 * \ingroup OTBEdge
 */
template<class TInput1, class TInput2, class TOutput>
class ITK_EXPORT AssociativeSymmetricalSum
{
public:
  AssociativeSymmetricalSum() {};
  virtual ~AssociativeSymmetricalSum() {}

  inline TOutput operator ()(const TInput1 X,
                             const TInput2 Y)
  {

    TOutput SumXY = 0.;

    SumXY += static_cast<TOutput>((X * Y) / (1. - X - Y + (2 * X * Y)));

    return (SumXY);
  }
};
}

template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT AssociativeSymmetricalSumImageFilter :
  public
  itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage,
      Functor::AssociativeSymmetricalSum<
          typename TInputImage1::PixelType,
          typename TInputImage2::PixelType,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef AssociativeSymmetricalSumImageFilter Self;
  typedef itk::BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage,
      Functor::AssociativeSymmetricalSum<
          typename TInputImage1::PixelType,
          typename TInputImage1::PixelType,
          typename TOutputImage::PixelType>
      > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  AssociativeSymmetricalSumImageFilter() {}
  ~AssociativeSymmetricalSumImageFilter() ITK_OVERRIDE {}

private:
  AssociativeSymmetricalSumImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
