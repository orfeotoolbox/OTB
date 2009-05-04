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
#ifndef __otbHaarOperator_h
#define __otbHaarOperator_h

#include "otbWaveletOperator.h"

namespace otb {

/**
 * \class LowPassHaarOperator
 *
 * \brief <b>EXPERIMENTAL FEATURE</b> A NeighborhoodOperator for performing a Haar based filtering
 * at a pixel location.
 *
 * LowPassHaarOperator is a NeighborhoodOperator that should be applied to a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 * The Haar Operator is defiend in 1D as \f$ H(z) = ( 1 + z^{-1} ) / 2 \f$.
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
class ITK_EXPORT LowPassHaarOperator
  : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef LowPassHaarOperator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(LowPassHaarOperator, WaveletOperator);

  typedef InverseOrForwardTransformationEnum DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation,DirectionOfTransformationEnumType,TDirectionOfTransformation);

  LowPassHaarOperator()
  {
    this->SetRadius(1);
    this->CreateToRadius(1);
    this->SetWavelet( "Haar" );
  }

  LowPassHaarOperator(const Self& other)
    : WaveletOperator<TPixel, VDimension, TAllocator>(other)
  {
    this->SetWavelet( "Haar" );
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
    os << i << "LowPassHaarOperator {this=" << this << "}" << std::endl;
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
    // stands for z^{-1}
    coeff.push_back(0.5);
    // stands for z^0
    coeff.push_back(0.5);
    // stands for z^1
    coeff.push_back(0.0);

#if 0
  std::cerr << "Coeff H(" << this->GetWavelet();
  if ( (int) DirectionOfTransformation == (int) FORWARD )
    std::cerr << " Forward ) = ";
  else
    std::cerr << " Inverse ) = ";
  for ( typename CoefficientVector::const_iterator iter = coeff.begin(); iter < coeff.end(); ++iter )
    std::cerr << *iter << " ";
  std::cerr << "\n";
#endif

    return Superclass::UpSamplingCoefficients( coeff );
  }

  /** Arranges coefficients spatially in the memory buffer. */
  void Fill(const CoefficientVector& coeff)
  {
    this->FillCenteredDirectional(coeff);
  }
};

/**
 * \class HighPassHaarOperator
 *
 * \brief A NeighborhoodOperator for performing a Haar based filtering
 * at a pixel location.
 *
 * HighPassHaarOperator is a NeighborhoodOperator that should be applied a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 * The Haar Operator is defiend in 1D as \f$ G(z) = ( 1 - z^{-1} ) / 2 \f$.
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
template< InverseOrForwardTransformationEnum TDirectionOfTransformation,
  class TPixel, unsigned int VDimension,
  class TAllocator = itk::NeighborhoodAllocator< TPixel > >
class ITK_EXPORT HighPassHaarOperator
  : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef HighPassHaarOperator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(HighPassHaarOperator, WaveletOperator);

  typedef InverseOrForwardTransformationEnum DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation,DirectionOfTransformationEnumType,TDirectionOfTransformation);

  HighPassHaarOperator()
  {
    this->SetRadius(1);
    this->CreateToRadius(1);
    this->SetWavelet( "Haar" );
  }

  HighPassHaarOperator(const Self& other)
    : WaveletOperator<TPixel, VDimension, TAllocator>(other)
  {
    this->SetWavelet( "Haar" );
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
    os << i << "HighPassHaarOperator {this=" << this << "}" << std::endl;
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
   * Due to the number of template parameter, we cannot specialized it with regard
   * to the TDirectionOfTransformation value...
   */
  CoefficientVector GenerateCoefficients()
  {
    CoefficientVector coeff;

    typedef LowPassHaarOperator< FORWARD, TPixel, VDimension, TAllocator >
      LowPassOperatorType;
    LowPassOperatorType lowPassOperator;
    lowPassOperator.SetDirection(0);
    lowPassOperator.CreateDirectional();

    CoefficientVector lowPassCoeff;
    lowPassCoeff.resize( lowPassOperator.GetSize()[0] );

    for ( typename LowPassOperatorType::ConstIterator iter = lowPassOperator.Begin();
        iter != lowPassOperator.End(); ++iter )
      lowPassCoeff.push_back( *iter );

    switch ( DirectionOfTransformation )
    {
      case FORWARD:
        coeff = this->GetHighPassFilterFromLowPassFilter( lowPassCoeff );
        break;
      case INVERSE:
        coeff = this->GetInverseHighPassFilterFromForwardLowPassFilter( lowPassCoeff );
        break;
      default:
        itkExceptionMacro(<<"Wavelet operator has to be INVERSE or FORWARD only!!");
        break;
    }

#if 0
    std::cerr << "Coeff G(" << this->GetWavelet();
    if ( (int) DirectionOfTransformation == (int) FORWARD )
      std::cerr << " Forward ) = ";
    else
      std::cerr << " Inverse ) = ";
    for ( typename CoefficientVector::const_iterator iter = coeff.begin(); iter < coeff.end(); ++iter )
      std::cerr << *iter << " ";
    std::cerr << "\n";
#endif

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

