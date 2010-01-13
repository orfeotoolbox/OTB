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
#ifndef __otbTerraSarBrightnessFunctor_txx
#define __otbTerraSarBrightnessFunctor_txx

#include "otbTerraSarBrightnessFunctor.h"


namespace otb
{
namespace Functor
{

template <class TInput, class TOutput>
TerraSarBrightnessFunctor<TInput, TOutput>
::TerraSarBrightnessFunctor()
{
  m_CalibrationFactor = itk::NumericTraits<double>::Zero; 
}


template <class TInput, class TOutput>
TOutput
TerraSarBrightnessFunctor<TInput, TOutput>
::operator() (const TInput & inPix)
{
  // Formula: Beta^0 = Ks * |DN|²

  // First, square the input pixel
  double squareInPix = vcl_pow( static_cast<double>(inPix), 2.);

  // Then apply the calibration factor
  double beta = m_CalibrationFactor*squareInPix;
  
//   if(beta > 0.)
//     {
//     beta = 10 * vcl_log10(beta);
//     }
//   else
//     {
//     beta  = -50;
//     }

  return static_cast<TOutput>(beta); 
}


template <class TInput, class TOutput>
std::complex<TOutput>
TerraSarBrightnessFunctor<TInput, TOutput>
::operator() (const std::complex<TInput> & inPix)
{
  // First, extract modulus and phase
  double modulus = vcl_sqrt(inPix.real()*inPix.real() + inPix.imag()*inPix.imag());
  double phase   = vcl_atan2(inPix.imag(),inPix.real());

  if(modulus > vcl_sqrt(1/m_CalibrationFactor))
    {
    std::cout<<"Odd pixel: "<<inPix<<" (modulus: "<<modulus<<", 1/sqrt(CalFac): "<<1/vcl_sqrt(m_CalibrationFactor)<<") "<<std::endl;
    }


  //std::cout<<"Modulus: "<<modulus<<", phase: "<<phase<<std::endl;
  //std::cout<<"Calibration factor: "<<m_CalibrationFactor<<std::endl;

  // Then, calibrate the modulus
  double beta = this->operator()(modulus);
  
  //std::cout<<"Beta: "<<beta<<std::endl;

  // Last, put back the phase
  std::complex<TOutput> out(std::polar(beta,phase));

  //std::cout<<"Out: "<<out<<std::endl;

  return out;
}

}// namespace Functor

} // namespace otb
#endif
