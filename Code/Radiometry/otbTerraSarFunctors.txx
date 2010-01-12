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
#ifndef __otbTerraSarFunctors_txx
#define __otbTerraSarFunctors_txx

#include "otbTerraSarFunctors.h"


namespace otb
{
namespace Functor
{

/******************************************************************/
/***********  TerraSarBrightnessImageFunctor *****************/
/******************************************************************/
template <class TInput, class TOutput>
TerraSarBrightnessImageFunctor<TInput, TOutput>
::TerraSarBrightnessImageFunctor()
{
  m_CalibrationFactor = itk::NumericTraits<double>::min(); 
}


template <class TInput, class TOutput>
TOutput
TerraSarBrightnessImageFunctor<TInput, TOutput>
::operator() (const TInput & inPix)
{
  // Formula: Beta^0 = Ks * |DN|²

  // First, square the input pixel
  double squareInPix = vcl_pow( static_cast<double>(inPix), 2.);

  // Then apply the calibration factor
  double beta = m_CalibrationFactor*squareInPix;
  
  return static_cast<TOutput>(beta);  
}


template <class TInput, class TOutput>
std::complex<TOutput>
TerraSarBrightnessImageFunctor<TInput, TOutput>
::operator() (const std::complex<TInput> & inPix)
{
   // Beta naught computation, will be the Modulus of the result

  double beta = operator()(static_cast<double>(std::abs(inPix)));
  // Phase
  double phase = static_cast<double>(std::arg(inPix));
  
  // We retrieve the complex value from the modulus and the phase.
  std::complex<TOutput> res = std::complex<TOutput>(beta*vcl_cos(phase), beta*vcl_sin(phase) );

  return res;
}


/******************************************************************/
/***********      TerraSarCalibrationImageFunctor  ****************/
/******************************************************************/
template <class TInput, class TOutput>
TerraSarCalibrationImageFunctor<TInput, TOutput>
::TerraSarCalibrationImageFunctor()
{
  // Initialise values 
  m_CalibrationFactor = itk::NumericTraits<double>::min();
  m_LocalIncidentAngle = itk::NumericTraits<double>::Zero;
  m_SinLocalIncidentAngle = itk::NumericTraits<double>::Zero;
  m_ImageSize.Fill(0);
  m_UseFastCalibrationMethod = false;
  m_PRF = 1.;
  m_InvPRF = 1.;
  m_AcquisitionStartTime = itk::NumericTraits<double>::Zero;
}


template <class TInput, class TOutput>
const typename TerraSarCalibrationImageFunctor<TInput, TOutput>
::ImageNoiseType &
TerraSarCalibrationImageFunctor<TInput, TOutput>
::FindClosestNoiseRecord( double utcTime ) const
{  
  // Iterators
  NoiseRecordVectorType::const_iterator it, closest;

  // Initial search values
  it = m_NoiseRecords.begin();
  double delta = vcl_abs(utcTime - it->first);
  closest = it;

  // First increment
  ++it;

  // Iterate on the noise records to find the closest
  while(it != m_NoiseRecords.end())
    {
    double newDelta = vcl_abs(utcTime-it->first);

    // Check if this one is closer
    if(newDelta < delta)
      {
      // If so, update search values
      delta = newDelta;
      closest = it;
      }
    // Increment
    ++it;
    }
  return closest->second;
}

template <class TInput, class TOutput>
double 
TerraSarCalibrationImageFunctor<TInput, TOutput>
::ComputeAzimuthPosition(const IndexType & index) const
{
  double currentTime = m_AcquisitionStartTime + (m_ImageSize[1]-index[1]-1) * m_InvPRF;
  return currentTime;
}

template <class TInput, class TOutput>
double 
TerraSarCalibrationImageFunctor<TInput, TOutput>
::ComputeRangePosition(const ImageNoiseType & noise, const IndexType & index) const
{
  // First compute the range step for the given noise record
  double rangeStep = (noise.get_validityRangeMax()-noise.get_validityRangeMin())
                     /static_cast<double>(m_ImageSize[0]);
  
  // Compute the range position using the rangeStep
  double position = index[0] * rangeStep + noise.get_validityRangeMin();

  return position;
}
template <class TInput, class TOutput>
double 
TerraSarCalibrationImageFunctor<TInput, TOutput>
::ComputeNoiseEquivalentBetaNaught(const ImageNoiseType & noise, double range) const
{
  // Formula: NEBN = Ks * SUM( coef_i * (tau - tau_ref)^i)
  
  // Retrieve the polynomial degree
  unsigned int polynomialDegree = noise.get_polynomialDegree();

  // Compute tau - tau_ref
  double deltaTau = range - noise.get_referencePoint();

  // Get polynomial coefficients
  std::vector<double>  coefficients = noise.get_polynomialCoefficients();

  // Initialize nebn value
  double nebn = 0.;

  // For each degree
  for(unsigned int degree = 0; degree <= polynomialDegree; ++degree)
    {
    // Cumulate polynomial
    nebn += coefficients[degree] * vcl_pow(deltaTau,static_cast<double>(degree));
    }

  // Do not forget to multiply by the calibration factor
  nebn*=m_CalibrationFactor;

  // Return the nebn value
  return nebn;
}


template <class TInput, class TOutput>
TOutput
TerraSarCalibrationImageFunctor<TInput, TOutput>
::operator()(const TInput & inPix, IndexType index)
{
  // Formula: sigma = (Ks.|DN|²-NEBN) * sin Theta_local

  // First, compute the brightness using the brightness functor
  double beta0 = m_Brightness( static_cast<double>(inPix) );

  // Then compute the current acquisition time
  double currentAzimuth = this->ComputeAzimuthPosition(index);

  // Retrieve the associated noise record
  ImageNoiseType currentNoiseRecord;

  // If fast calibration is on, use always the first noise record
  if(m_UseFastCalibrationMethod)
    {
    currentNoiseRecord = m_NoiseRecords[0].second;
    }
  else
    {
    // Else, find the closest noise record
    currentNoiseRecord = this->FindClosestNoiseRecord(currentAzimuth);
    }

  // Compute the range position for this noise record
  double currentRange = this->ComputeRangePosition(currentNoiseRecord,index);
  
  // Compute the NEBN
  double NEBN = this->ComputeNoiseEquivalentBetaNaught(currentNoiseRecord,currentRange);

  // Last, apply formula
  double sigma = ( beta0 - NEBN ) * m_SinLocalIncidentAngle;

  // Cast and return
  return static_cast<TOutput>(sigma);
}

template <class TInput, class TOutput>
std::complex<TOutput>
TerraSarCalibrationImageFunctor<TInput, TOutput>
::operator()(const std::complex<TInput> & inPix, IndexType index)
{
  // First, extract modulus and phase
  double modulus = vcl_abs(inPix);
  double phase   = std::arg(inPix);

  // Then, calibrate the modulus
  double sigma = this->operator()(modulus,index);
  
  // Last, put back the phase
  std::complex<TOutput> out(sigma*vcl_cos(phase), sigma*vcl_sin(phase));
  
  // And return the result
  return out;
}

}// namespace Functor

} // namespace otb
#endif
