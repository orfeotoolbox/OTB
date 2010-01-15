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
#ifndef __otbTerraSarCalibrationFunctor_txx
#define __otbTerraSarCalibrationFunctor_txx

#include "otbTerraSarCalibrationFunctor.h"


namespace otb
{
namespace Functor
{
template <class TInput, class TOutput>
TerraSarCalibrationFunctor<TInput, TOutput>
::TerraSarCalibrationFunctor()
{
  // Initialise values 
  m_CalibrationFactor = itk::NumericTraits<double>::Zero;
  m_OriginalProductSize.Fill(0);
  m_UseFastCalibration = false;
  m_ResultsInDecibels = false;
  m_Brightness.SetResultsInDecibels(false);
  m_DefaultValue = 0.00001; // Default value is 10^-5
}

template <class TInput, class TOutput>
double 
TerraSarCalibrationFunctor<TInput, TOutput>
::ComputeRangePosition(const IndexType & index) const
{
  // First compute the range step for the given noise record
  double rangeStep = (m_NoiseRecord.get_validityRangeMax()-m_NoiseRecord.get_validityRangeMin())
                     /static_cast<double>(m_OriginalProductSize[0]);
  
  // Compute the range position using the rangeStep
  double position = index[0] * rangeStep + m_NoiseRecord.get_validityRangeMin();

  return position;
}

template <class TInput, class TOutput>
double 
TerraSarCalibrationFunctor<TInput, TOutput>
::ComputeNoiseEquivalentBetaNaught(double range) const
{
  // Formula: NEBN = Ks * SUM( coef_i * (tau - tau_ref)^i)
  
  // Retrieve the polynomial degree
  unsigned int polynomialDegree = m_NoiseRecord.get_polynomialDegree();

  // Compute tau - tau_ref
  double deltaTau = range - m_NoiseRecord.get_referencePoint();

  // Get polynomial coefficients
  std::vector<double>  coefficients = m_NoiseRecord.get_polynomialCoefficients();

  // Initialize nebn value
  double nebn = 0.;

  // For each degree
  std::vector<double>::const_iterator coefIt = coefficients.begin();
  unsigned int degree = 0;
  
  while(coefIt != coefficients.end() && degree <= polynomialDegree)
    {
    // Cumulate polynomial
    nebn += (*coefIt) * vcl_pow(deltaTau,static_cast<double>(degree));
    ++degree;
    ++coefIt;
    }

  // Do not forget to multiply by the calibration factor
  nebn*=m_CalibrationFactor;

  // Return the nebn value
  return nebn;
}


template <class TInput, class TOutput>
TOutput
TerraSarCalibrationFunctor<TInput, TOutput>
::operator()(const TInput & inPix, const IndexType& index, double angle)
{
  // Formula: sigma = (Ks.|DN|²-NEBN) * sin Theta_local

  // First, compute the brightness using the brightness functor
  double beta0 = m_Brightness( static_cast<double>(inPix) );

  // Initialise sigma
  double sigma = 0.;

  // If fast calibration is off, compute noise
  if(m_UseFastCalibration)
    {
    sigma = beta0 * vcl_sin(angle); 
    }
  else
    {
    // Compute the range position for this noise record
    double currentRange = this->ComputeRangePosition(index);
    
    // Compute the NEBN
    double NEBN = this->ComputeNoiseEquivalentBetaNaught(currentRange);
    
    // Last, apply formula
    sigma = (beta0 - NEBN) * vcl_sin(angle);

    // Handle negative sigma case
    if(sigma <=0)
      {
      sigma = m_DefaultValue;
      }
    }

  // If results are in dB
  if(m_ResultsInDecibels)
    {
    sigma = 10 * vcl_log10(sigma);
    }

  // Cast and return
  return static_cast<TOutput>(sigma);
}

template <class TInput, class TOutput>
std::complex<TOutput>
TerraSarCalibrationFunctor<TInput, TOutput>
::operator()(const std::complex<TInput> & inPix, const IndexType& index, double angle)
{
  // First, extract modulus and phase
  double modulus = vcl_sqrt(inPix.real()*inPix.real() + inPix.imag()*inPix.imag());
  double phase   = vcl_atan2(inPix.imag(),inPix.real());

  // Then, calibrate the modulus
  double sigma = this->operator()(modulus,index,angle);
  
  // Last, put back the phase
  std::complex<TOutput> out(std::polar(sigma,phase));
  
  // And return the result
  return out;
}

}// namespace Functor

} // namespace otb
#endif
