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
#ifndef __otbHaarOperator__h
#define __otbHaarOperator__h

#include "otbWaveletOperator.h"

namespace otb {

/**
 * \class LowPassHaarOperator
 *
 * \brief A NeighborhoodOperator for performing a Haar based filtering
 * at a pixel location.
 * 
 * LowPassHaarOperator is a NeighborhoodOperator that should be applied to a 
 * NeighborhoodIterator using the NeighborhoodInnerProduct method. 
 * The Haar Operator is defiend in 1D as \$ H(z) = ( 1 + z^{-1} ) / 2 \$.
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
class ITK_EXPORT LowPassHaarOperator
  : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef LowPassHaarOperator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(LowPassHaarOperator, WaveletOperator);
  
  LowPassHaarOperator() : WaveletOperator ()
	{
    this->SetRadius(1);
		this->CreateToRadius(1);
  }

  LowPassHaarOperator(const Self& other)
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
 * The Haar Operator is defiend in 1D as \$ G(z) = ( 1 - z^{-1} ) / 2 \$.
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
class ITK_EXPORT HighPassHaarOperator
  : public WaveletOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef HighPassHaarOperator Self;
  typedef WaveletOperator<TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(HighPassHaarOperator, WaveletOperator);
  
  HighPassHaarOperator() 
	{
    this->SetRadius(1);
		this->CreateToRadius(1);
  }

  HighPassHaarOperator(const Self& other)
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
   */
  CoefficientVector GenerateCoefficients()
  {
    CoefficientVector coeff;
    coeff.push_back(-0.5);  
    coeff.push_back(0.5);  
    coeff.push_back(0.0);  

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

