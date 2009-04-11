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
#ifndef __otbWaveletOperator_h
#define __otbWaveletOperator_h

#include "itkExceptionObject.h"
#include "itkNeighborhoodOperator.h"

// This include is needed to define InverseOrForwardTransformationEnum only...
#include "otbGenericMapProjection.h"

namespace otb {

/**
 * \class WaveletOperator
 *
 * \brief A NeighborhoodOperator wavelet base class
 *
 * This class is the mother class for any wavelet operator that requires
 * "a-trou" approach for shift-invariant wavelet transform. This class has 
 * to be derived, it cannot be used directly.
 *
 * Any wavelet operator that inherits from WaveletOperator is to be used
 * as a NeighborhoodOperator that should be applied to a 
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 *
 * It is assumed that any wavelet is directional.
 *
 * Set the level of up-sampling though SetUpSampleFactor() before calling 
 * CreateDirectional(). Each class that inherits from WaveletOperator has 
 * to re-implement GenerateCoefficients(). It has to end by:
 * return this->UpSamplingCoefficients( coeff ) to perform the up-sampling
 *  
 * \sa LowPassHaarOperator
 * \sa HighPassHaarOperator
 * \sa NeighborhoodOperator
 * \sa Neighborhood
 *
 * \ingroup Operators
 */
template<class TPixel, unsigned int VDimension,
  class TAllocator = itk::NeighborhoodAllocator< TPixel > >
class ITK_EXPORT WaveletOperator
        : public itk::NeighborhoodOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef WaveletOperator Self;
  typedef itk::NeighborhoodOperator<TPixel, VDimension, TAllocator> Superclass;

  itkTypeMacro(WaveletOperator,NeighborhoodOperator);

  /** Construction */
  WaveletOperator() :
    m_UpSampleFactor( 0 ), m_Wavelet( "Unknown" )
  { }

  /** Construction by copy */
  WaveletOperator( const Self & other ) 
    : itk::NeighborhoodOperator<TPixel, VDimension, TAllocator> (other)
  {
    m_UpSampleFactor = other.GetUpSampleFactor();
    m_Wavelet = other.m_Wavelet;
  }
  virtual ~WaveletOperator() {}

  /** Assignment operator */
  Self &operator=(const Self& other)
  {
    Superclass::operator=(other);
    m_UpSampleFactor = other.GetUpSampleFactor();
    m_Wavelet = other.m_Wavelet;
    return *this;
  }
  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, itk::Indent i) const  
  { 
    Superclass::PrintSelf(os, i.GetNextIndent());
    os << i << "Up-Sampling factor " << this->m_UpSampleFactor << "\n";
    os << i << "Wavelet kind : " << GetWavelet() << std::endl;
  }

  /**
   * Set/Get the level of up sampling of the filter
   */
  unsigned int GetUpSampleFactor () const 
  {
    return this->m_UpSampleFactor;
  }
  void SetUpSampleFactor ( unsigned int upSampleFactor )
  {
    this->m_UpSampleFactor = upSampleFactor;
  }

  /**
   * Set/Get the name of the wavelet when necessary
   */
  virtual void SetWavelet ( const std::string & str ) {
    this->m_Wavelet = str;
  }

  virtual void SetWavelet ( const char * str ) {
    this->m_Wavelet = str; }

  virtual const char * GetWavelet () const {
    return this->m_Wavelet.c_str(); }

protected:
  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;
  typedef typename Superclass::PixelType PixelType;

  /** 
   * Perform the "a-trou" algorithm for shift-invariant transformation.
   * It transforms the filter \f$ H(z) \f$ into \f$ H(z^2) \f$.
   */
  CoefficientVector UpSamplingCoefficients ( CoefficientVector & coeff ) 
  {
    if ( m_UpSampleFactor == 0 )
      return coeff;

    for ( unsigned int up = 0; up < m_UpSampleFactor; ++up )
    {
      unsigned long radius = static_cast<unsigned long>( coeff.size() ) >> 1;

      this->SetRadius( 2*radius );

      CoefficientVector upSampledCoeff;
      upSampledCoeff.push_back( coeff[0] );
      for ( unsigned int i = 1; i < coeff.size(); ++i )
      {
        upSampledCoeff.push_back( 0.0 );
        upSampledCoeff.push_back( coeff[i] );
      }
      coeff = upSampledCoeff;
    }

    return coeff;
  }

  /** 
   * Performs the definition of synthesis filter from analysis one.
   * Input is the forward low pass filter coefficients.
   * It performs \f$ {\tilde G}(z) = -H(-z) \f$.
   */
  CoefficientVector GetInverseHighPassFilterFromForwardLowPassFilter ( CoefficientVector & coeff ) 
  {
    unsigned long radius = static_cast<unsigned long>( coeff.size() );
    this->SetRadius( radius );

    unsigned long medianPosition = radius/2;

    CoefficientVector highPassedCoeff;
    highPassedCoeff = coeff;

    for ( unsigned int i = 0; i < medianPosition; i+=2 )
    {
      highPassedCoeff[medianPosition + i] = - coeff[ medianPosition + i ];
      highPassedCoeff[medianPosition - i] = - coeff[ medianPosition - i ];
    }

    coeff = highPassedCoeff;
    return  coeff;
  }

  /** 
   * Performs the definition of synthesis filter from analysis one.
   * Input is the forward high pass filter coefficients.
   * It performs \f$ {\tilde H}(z) = G(-z) \f$.
   */
  CoefficientVector GetInverseLowPassFilterFromForwardHighPassFilter ( CoefficientVector & coeff ) 
  {
    unsigned long radius = static_cast<unsigned long>( coeff.size() );
    this->SetRadius( radius );

    unsigned long medianPosition = radius/2;

    CoefficientVector lowPassedCoeff;
    lowPassedCoeff = coeff;

    for ( unsigned int i = 1; i < medianPosition; i+=2 )
    {
      lowPassedCoeff[medianPosition + i] = - coeff[ medianPosition + i ];
      lowPassedCoeff[medianPosition - i] = - coeff[ medianPosition - i ];
    }

    coeff = lowPassedCoeff;
    return  coeff;
  }

  /** 
   * Performs the definition of high pass filter from the low pass one
   * in a Quadrature mirror filter bank framework. It is then valid to
   * define high pass filters from orthogonal wavelets...
   * Input is the forward low pass filter coefficients.
   * It performs \f$ G(z) = H(-z) \f$.
   */
  CoefficientVector GetHighPassFilterFromLowPassFilter ( CoefficientVector & coeff ) 
  {
    unsigned long radius = static_cast<unsigned long>( coeff.size() );
    this->SetRadius( radius );

    unsigned long medianPosition = radius/2;

    CoefficientVector highPassedCoeff;
    highPassedCoeff = coeff;

    for ( unsigned int i = 1; i < medianPosition; i+=2 )
    {
      highPassedCoeff[medianPosition + i] = - coeff[ medianPosition + i ];
      highPassedCoeff[medianPosition - i] = - coeff[ medianPosition - i ];
    }

    coeff = highPassedCoeff;
    return  coeff;
  }

  unsigned int m_UpSampleFactor;
  std::string m_Wavelet;
};

} // end of namespace otb

#endif





