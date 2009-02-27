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
#ifndef __otb_9_7_Operator__h
#define __otb_9_7_Operator__h

#include "otbWaveletOperator.h"

namespace otb
{

/**
 * \class LowPass_9_7_Operator
 *
 * \brief A NeighborhoodOperator for performing a 9/7 based filtering
 * at a pixel location.
 *
 * LowPass_9_7_Operator is a NeighborhoodOperator that should be applied a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 * The _9_7_ Operator is defiend in 1D as
 * \$ H(z) = 0.026748757411 z^{-4} -0.016864118443 z^{-3} -0.078223266529 z^{-2}
 *           + 0.266864118443 z^{-1} + 0.602949018236 + 0.266864118443 z
 *           -0.078223266529 z^2 -0.016864118443 z^3 + 0.026748757411 z^4 \$.
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
template<class TPixel, unsigned int VDimension,
class TAllocator = itk::NeighborhoodAllocator< TPixel > >
class ITK_EXPORT LowPass_9_7_Operator
      : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef LowPass_9_7_Operator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(LowPass_9_7_Operator, WaveletOperator);

  LowPass_9_7_Operator()
  {
    this->SetRadius(4);
    this->CreateToRadius(4);
  }

  LowPass_9_7_Operator(const Self& other)
      : WaveletOperator<TPixel, VDimension, TAllocator>(other)
  {
    /* ... */
  }

  /**
   * Assignment operator
   */
  Self &operator=(const Self& other)
  {
    Superclass::operator=(other);
    return *this;
  }
  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, itk::Indent i) const
  {
    Superclass::PrintSelf(os, i.GetNextIndent());
    os << i << "LowPass_9_7_Operator {this=" << this << "}" << std::endl;
  }

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
  CoefficientVector GenerateCoefficients()
  {
    CoefficientVector coeff;
    coeff.push_back( 0.026748757411);
    coeff.push_back(-0.016864118443);
    coeff.push_back(-0.078223266529);
    coeff.push_back( 0.266864118443);
    coeff.push_back( 0.602949018236);
    coeff.push_back( 0.266864118443);
    coeff.push_back(-0.078223266529);
    coeff.push_back(-0.016864118443);
    coeff.push_back( 0.026748757411);

    return Superclass::UpSamplingCoefficients( coeff );
  }

  /** Arranges coefficients spatially in the memory buffer. */
  void Fill(const CoefficientVector& coeff)
  {
    this->FillCenteredDirectional(coeff);
  }
};

/**
 * \class HighPass_9_7_Operator
 *
 * \brief A NeighborhoodOperator for performing a 9/7 based filtering
 * at a pixel location.
 *
 * HighPass_9_7_Operator is a NeighborhoodOperator that should be applied a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 * The _9_7_ Operator is defiend in 1D as
 * \$ H(z) = 0.045635881557 z^{-3} -0.028771763114 z^{-2} -0.295635881557 z^{-1}
 *            + 0.557543526229  - 0.295635881557 z -0.028771763114 z^2 + 0.045635881557 z^3 \$.
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
template<class TPixel, unsigned int VDimension,
class TAllocator = itk::NeighborhoodAllocator< TPixel > >
class ITK_EXPORT HighPass_9_7_Operator
      : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef HighPass_9_7_Operator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(HighPass_9_7_Operator, WaveletOperator);

  HighPass_9_7_Operator()
  {
    this->SetRadius(3);
    this->CreateToRadius(3);
  }

  HighPass_9_7_Operator(const Self& other)
      : WaveletOperator<TPixel, VDimension, TAllocator>(other)
  {
    /* ... */
  }

  /**
   * Assignment operator
   */
  Self &operator=(const Self& other)
  {
    Superclass::operator=(other);
    return *this;
  }
  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, itk::Indent i) const
  {
    Superclass::PrintSelf(os, i.GetNextIndent());
    os << i << "HighPass_9_7_Operator {this=" << this << "}" << std::endl;
  }

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
  CoefficientVector GenerateCoefficients()
  {
    CoefficientVector coeff;
    coeff.push_back( 0.045635881557 );
    coeff.push_back(-0.028771763114 );
    coeff.push_back(-0.295635881557 );
    coeff.push_back( 0.557543526229 );
    coeff.push_back(-0.295635881557 );
    coeff.push_back(-0.028771763114 );
    coeff.push_back( 0.045635881557 );

    return Superclass::UpSamplingCoefficients( coeff );
  }

  /** Arranges coefficients spatially in the memory buffer. */
  void Fill(const CoefficientVector& coeff)
  {
    this->FillCenteredDirectional(coeff);
  }
};


} // end of namespace otb

#endif

