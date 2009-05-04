/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSplieBiOrthogonalOperator_h
#define __otbSplieBiOrthogonalOperator_h

#include "itkMacro.h"
#include "itkExceptionObject.h"

#include "otbWaveletOperator.h"

namespace otb {

/**
 * \class LowPassSplineBiOrthogonalOperator
 *
 * \brief <b>EXPERIMENTAL FEATURE</b> A NeighborhoodOperator for performing a Spline Bi-Orthogonal-based filtering
 * at a pixel location.
 *
 * It may be configure with many filter orders. AT present time, so-called "9/7" filter is
 * implemented. It may be fixed with SetWavelet method
 *
 * The LowPassSplineBiOrthogonalOperator is a NeighborhoodOperator that should be applied a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 * The 9/7 Operator is defiend in 1D as
 * \f$ H(z) = 0.026748757411 z^{-4} -0.016864118443 z^{-3} -0.078223266529 z^{-2}
 *           + 0.266864118443 z^{-1} + 0.602949018236 + 0.266864118443 z
 *           -0.078223266529 z^2 -0.016864118443 z^3 + 0.026748757411 z^4 \f$.
 *
 * In N dimensions, the operator is directional
 *
 * \sa WaveletOperator
 * \sa NeighborhoodOperator
 * \sa Neighborhood
 * \sa ForwardDifferenceOperator
 * \sa BackwardDifferenceOperator
 *
 * \ingroup Operators
 */
template < InverseOrForwardTransformationEnum TDirectionOfTransformation,
  class TPixel, unsigned int VDimension,
  class TAllocator = itk::NeighborhoodAllocator< TPixel > >
class ITK_EXPORT LowPassSplineBiOrthogonalOperator
  : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef LowPassSplineBiOrthogonalOperator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(LowPassSplineBiOrthogonalOperator, WaveletOperator);

  typedef InverseOrForwardTransformationEnum DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation,DirectionOfTransformationEnumType,TDirectionOfTransformation);

  LowPassSplineBiOrthogonalOperator();
  LowPassSplineBiOrthogonalOperator(const Self& other);

  /**
   * Assignment operator
   */
  Self &operator=(const Self& other);

  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, itk::Indent i) const;

protected:
  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;
  typedef typename Superclass::PixelType PixelType;

  /**
   * Set operator coefficients.
   */
  CoefficientVector GenerateCoefficients();

  /** Arranges coefficients spatially in the memory buffer. */
  void Fill(const CoefficientVector& coeff);
};

/**
 * \class HighPassSplineBiOrthogonalOperator
 *
 * \brief A NeighborhoodOperator for performing a Spline Bi-Orthogonal-based filtering
 * at a pixel location.
 *
 * HighPassSplineBiOrthogonalOperator is a NeighborhoodOperator that should be applied a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 * The 9/7 Operator is defiend in 1D as
 * \f$ H(z) = 0.045635881557 z^{-3} -0.028771763114 z^{-2} -0.295635881557 z^{-1}
 *            + 0.557543526229  - 0.295635881557 z -0.028771763114 z^2 + 0.045635881557 z^3 \f$.
 *
 * In N dimensions, the operator is directional
 *
 * \sa WaveletOperator
 * \sa NeighborhoodOperator
 * \sa Neighborhood
 * \sa ForwardDifferenceOperator
 * \sa BackwardDifferenceOperator
 *
 * \ingroup Operators
 */
template < InverseOrForwardTransformationEnum TDirectionOfTransformation,
class TPixel, unsigned int VDimension,
  class TAllocator = itk::NeighborhoodAllocator< TPixel > >
class ITK_EXPORT HighPassSplineBiOrthogonalOperator
  : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef HighPassSplineBiOrthogonalOperator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(HighPassSplineBiOrthogonalOperator, WaveletOperator);

  typedef InverseOrForwardTransformationEnum DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation,DirectionOfTransformationEnumType,TDirectionOfTransformation);

  HighPassSplineBiOrthogonalOperator ();
  HighPassSplineBiOrthogonalOperator(const Self& other);

  /**
   * Assignment operator
   */
  Self &operator=(const Self& other);

  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, itk::Indent i) const;

protected:
  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;
  typedef typename Superclass::PixelType PixelType;

  /**
   * Set operator coefficients.
   */
  CoefficientVector GenerateCoefficients();

  /** Arranges coefficients spatially in the memory buffer. */
  void Fill(const CoefficientVector& coeff);
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSplineBiOrthogonalOperator.txx"
#endif


#endif

