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
#ifndef __otbReduceSpectralResponse_txx
#define __otbReduceSpectralResponse_txx

#include "otbReduceSpectralResponse.h"

#include <algorithm>

namespace otb
{

template <class TSpectralResponse , class TRSR>
ReduceSpectralResponse <TSpectralResponse , TRSR>
::ReduceSpectralResponse() : m_ReflectanceMode(false)
{
    m_ReduceResponse = InputSpectralResponseType::New();
}

template <class TSpectralResponse , class TRSR>
bool
ReduceSpectralResponse<TSpectralResponse , TRSR>
::Clear()
{
   return ( m_InputSatRSR->Clear() & m_InputSpectralResponse->Clear() );
}

template <typename T>
inline
T trapezoid_area(T x1, T x2, T y1, T y2)
{
        /* We compute the area of the trapezoid
            by computing the lower square and the
            upper triangle.
                         /- |y2
                       /-   |
                     /-     |
                   /-       |
            y1   +----------+
                 |          |
                 |          |
                 |          |
                 +----------+
                x1          x2

          (x2-x1)*min(y1,y2) + (x2-x1)*abs(y2-y1)/2
        */
  return (x2-x1)*std::min(y1,y2) + (x2-x1)*fabs(y2-y1)*0.5;
}

template <class TSpectralResponse , class TRSR>
inline typename ReduceSpectralResponse<TSpectralResponse , TRSR>::ValuePrecisionType
ReduceSpectralResponse<TSpectralResponse , TRSR>
::operator()(const unsigned int numBand)
{
  if (numBand >= m_InputSatRSR->GetNbBands())
    {
    itkExceptionMacro(<< "There is no band num " << numBand
                      << " in the RSR vector!(Size of the current RSR vector is "
                      << m_InputSatRSR->GetNbBands() << ")" );
    }
  else
    {
    ValuePrecisionType res = itk::NumericTraits<ValuePrecisionType>::ZeroValue();
    typename InputRSRType::SpectralResponseType* solarIrradiance;
    if(m_ReflectanceMode)
      solarIrradiance = this->m_InputSatRSR->GetSolarIrradiance();
    typename VectorPairType::const_iterator it;
    VectorPairType pairs = (m_InputSatRSR->GetRSR())[numBand]->GetResponse();
    it = pairs.begin();
    ValuePrecisionType totalArea(0);
    //start with the second value for the numerical integration
    ++it;
    while (it != pairs.end())
      {
      PrecisionType lambda1 = (*(it-1)).first;
      PrecisionType lambda2 = (*it).first;
      PrecisionType deltaLambda = lambda2-lambda1;
      ValuePrecisionType rsr1 = (*(it-1)).second;
      ValuePrecisionType rsr2 = (*it).second;
      ValuePrecisionType spectrum1 = (*m_InputSpectralResponse)(lambda1);
      ValuePrecisionType spectrum2 = (*m_InputSpectralResponse)(lambda2);
      /*
        In order to simplify the calculations for the reflectance mode,
        we introduce the solar irradiance in the general formula with
        a value of 1.0 for the luminance case.
      */
      ValuePrecisionType solarIrradiance1(1.0);
      ValuePrecisionType solarIrradiance2(1.0);
      if(m_ReflectanceMode)
        {
        solarIrradiance1 = (*solarIrradiance)(lambda1);
        solarIrradiance2 = (*solarIrradiance)(lambda2);
        }
      rsr1 *= solarIrradiance1;
      rsr2 *= solarIrradiance2;
      res += trapezoid_area(lambda1, lambda2,
                            rsr1*spectrum1,
                            rsr2*spectrum2);
      totalArea += trapezoid_area(lambda1, lambda2,
                                  rsr1,
                                  rsr2);
      ++it;
      }    
    return res / totalArea;
    }
}

template <class TSpectralResponse , class TRSR>
void
ReduceSpectralResponse<TSpectralResponse , TRSR>
::CalculateResponse()
{
   m_ReduceResponse->Clear();
   //Compute the reduce response for each band of the sensor
   for (unsigned int i=0; i<m_InputSatRSR->GetNbBands(); ++i)
   {
    m_InputSpectralResponse->SetPosGuessMin((this->m_InputSatRSR->GetRSR())[i]->GetInterval().first);
    m_InputSpectralResponse->SetUsePosGuess(true);
    PairType pair;
    //pair.first = center wavelength of the band
    pair.first = ((this->m_InputSatRSR->GetRSR())[i]->GetInterval().first
        + (this->m_InputSatRSR->GetRSR())[i]->GetInterval().second);
    pair.first = pair.first / 2.0;
    pair.second = (*this)(i);
    m_ReduceResponse->GetResponse().push_back(pair);
    m_InputSpectralResponse->SetUsePosGuess(false);
    }

}


template <class TSpectralResponse , class TRSR>
void
ReduceSpectralResponse<TSpectralResponse , TRSR>
::LoadInputsFromFiles (const std::string & spectralResponseFile, const std::string & RSRFile, const unsigned int nbRSRBands, ValuePrecisionType coefNormSpectre, ValuePrecisionType coefNormRSR)
{
   //Instantiation
   m_InputSpectralResponse=InputSpectralResponseType::New();
   /** Load the spectral response file*/
   m_InputSpectralResponse->Load(spectralResponseFile, coefNormSpectre);

   m_InputSatRSR=InputRSRType::New();
   /** Set the satelite number of bands */
   m_InputSatRSR->SetNbBands(nbRSRBands);
   /** Load the satelite response file*/
   m_InputSatRSR->Load(RSRFile, coefNormRSR);
}


template <class TSpectralResponse , class TRSR>
void
ReduceSpectralResponse<TSpectralResponse , TRSR>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os, indent);
   os<<std::endl;
   os << "spectre " << m_InputSpectralResponse <<std::endl;
   os << "Sat RSR " << m_InputSatRSR <<std::endl;
   os<<std::endl;

   if(m_ReflectanceMode)
     {
     os << "Solar irradiance " << std::endl;
     this->m_InputSatRSR->GetSolarIrradiance()->PrintSelf(os, indent);
     os<<std::endl;
     os <<indent << "[Center Wavelength (micrometers), Reflectance (percent)]" << std::endl;
     }
   else{
   os <<indent << "[Center Wavelength (micrometers), Luminance (percent)]" << std::endl;
   }

   for(typename VectorPairType::const_iterator it = m_ReduceResponse->GetResponse().begin(); it != m_ReduceResponse->GetResponse().end(); ++it)
   {
     os <<indent << "Band Nb : "<< it - m_ReduceResponse->GetResponse().begin() << ": [" << (*it).first << ","<< (*it).second << "]" << std::endl;
   }
}


} // end namespace otb

#endif
