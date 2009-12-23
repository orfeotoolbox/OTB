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
  m_CalFactor = itk::NumericTraits<double>::min(); 
}


template <class TInput, class TOutput>
TOutput
TerraSarBrightnessImageFunctor<TInput, TOutput>
::operator() (const TInput & inPix)
{
  double squareInPix = vcl_pow( static_cast<double>(inPix), 2.);
  // Beta naught computation
  double beta = m_CalFactor*squareInPix;
  
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
    m_CalFactor = 1.;
    m_NoiseRangeValidityMin = 0.;
    m_NoiseRangeValidityMax = 0.;
    m_NoiseRangeValidityRef = 0.;
    m_LocalIncidentAngle = 0.;
    m_SinLocalIncidentAngle = 0.;
    m_NoisePolynomialCoefficientsList = DoubleVectorVectorType( 1, DoubleVectorType(1, 0.) );
    m_ImageSize.Fill(0);
    m_UseFastCalibrationMethod = true;
    m_TimeUTC = LIntVectorType(2, 0);
    m_TimeUTC[1] = 1;
    m_PRF = 1.;
}


template <class TInput, class TOutput>
double
TerraSarCalibrationImageFunctor<TInput, TOutput>
::ComputeCurrentNoise( unsigned int colId )
{
  double curRange = 0.;
  double width_2 = static_cast<double>(m_ImageSize[0])/2.;
  
  // Use +1 because image start index is 0
  if( colId < static_cast<unsigned int>(width_2) )
    {
      curRange = m_NoiseRangeValidityMin + ( m_NoiseRangeValidityRef-m_NoiseRangeValidityMin )/width_2 * static_cast<double>(colId+1);
    }
  else
    {
      curRange = m_NoiseRangeValidityRef + ( m_NoiseRangeValidityMax-m_NoiseRangeValidityRef )/width_2 * (static_cast<double>(colId+1) - width_2 );
    }

  return curRange;
}


template <class TInput, class TOutput>
typename TerraSarCalibrationImageFunctor<TInput, TOutput>::DoubleVectorType
TerraSarCalibrationImageFunctor<TInput, TOutput>
::ComputeCurrentCoeffs( unsigned int lineId )
{
  DoubleVectorType curCoeffs;
  if(m_UseFastCalibrationMethod)
    {
      curCoeffs = m_NoisePolynomialCoefficientsList[0];
    }
  else
    {
      // m_ImageSize[1]-(lineId+1) because the first acquisition line is the last image one.
      // line+1 because image starts to 0.
      //double interval =  static_cast<double>(m_ImageSize[1]) / static_cast<double>(m_NoisePolynomialCoefficientsList.size());
      // compute utc time of the line
      double currTimeUTC = m_TimeUTC[0] + static_cast<double>(m_ImageSize[1]-(lineId-1))*m_InvPRF;
      unsigned int id = 0;
      bool go = true;
      // deduct the corresponding noise acquisition index
      
      while( id<m_TimeUTC.size() && go)
	{
	  if( currTimeUTC < m_TimeUTC[id] )
	    go = false;
	  id++;
	}
      id--;
   
      double timeCoef = 1. / (m_TimeUTC[id]- m_TimeUTC[id-1]) * (currTimeUTC-m_TimeUTC[id-1]);
      for(unsigned int j=0; j<m_NoisePolynomialCoefficientsList.size(); j++)
	{
	  curCoeffs.push_back( m_NoisePolynomialCoefficientsList[id-1][j] + (m_NoisePolynomialCoefficientsList[id][j] - m_NoisePolynomialCoefficientsList[id-1][j]) * timeCoef );
	}
    }

  return curCoeffs;
}



template <class TInput, class TOutput>
TOutput
TerraSarCalibrationImageFunctor<TInput, TOutput>
::operator()(const TInput & inPix, IndexType index)
{
  double diffCurRange = ComputeCurrentNoise( static_cast<unsigned int>(index[0]) ) - this->GetNoiseRangeValidityRef();
  DoubleVectorType curCoeff = this->ComputeCurrentCoeffs( static_cast<unsigned int>(index[1]) );

  double outRadBr = this->GetBrightness()( static_cast<double>(inPix) );

  double NEBN = 0.;
  for(unsigned int i=0; i<curCoeff.size(); i++)
    {
      NEBN += curCoeff[i]*vcl_pow( diffCurRange, static_cast<double>(i));
    }
  double sigma = ( outRadBr - this->GetCalFactor()*NEBN ) * this->GetSinLocalIncidentAngle();

  return static_cast<TOutput>(sigma);
}


template <class TInput, class TOutput>
std::complex<TOutput>
TerraSarCalibrationImageFunctor<TInput, TOutput>
::operator()(const std::complex<TInput> & inPix, IndexType index)
{
  double diffCurRange = this->ComputeCurrentNoise( static_cast<unsigned int>(index[0]) ) - this->GetNoiseRangeValidityRef();
  DoubleVectorType curCoeff = this->ComputeCurrentCoeffs( static_cast<unsigned int>(index[1]) );
  
  double modulus = std::abs(inPix);
  double outRadBr = static_cast<double>(this->GetBrightness()( modulus ));
  
  double NEBN = 0.;
  for(unsigned int i=0; i<curCoeff.size(); i++)
    {
      NEBN += curCoeff[i]*vcl_pow( diffCurRange, static_cast<double>(i));
    }
  double sigma = ( outRadBr - this->GetCalFactor()*NEBN ) * this->GetSinLocalIncidentAngle();
  
  double phase = std::arg(inPix);
  
  std::complex<TOutput> out(sigma*vcl_cos(phase), sigma*vcl_sin(phase));
  
  return out;
}


}// namespace Functor

} // namespace otb
#endif
