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
  WaveletOperator() 
  {
    m_UpSampleFactor = 0;
  }
  /** Construction by copy */
  WaveletOperator( const Self & other ) 
    : itk::NeighborhoodOperator<TPixel, VDimension, TAllocator> (other)
  {
    m_UpSampleFactor = other.GetUpSampleFactor();
  }
  virtual ~WaveletOperator() {}

  /** Assignment operator */
  Self &operator=(const Self& other)
  {
    Superclass::operator=(other);
    m_UpSampleFactor = other.GetUpSampleFactor();
    return *this;
  }
  /**
   * Prints some debugging information
   */
  virtual void PrintSelf(std::ostream &os, itk::Indent i) const  
  { 
    Superclass::PrintSelf(os, i.GetNextIndent());
    os << i << "Up-Sampling factor " << this->m_UpSampleFactor << "\n";
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

  unsigned int m_UpSampleFactor;
};

} // end of namespace otb

#endif





