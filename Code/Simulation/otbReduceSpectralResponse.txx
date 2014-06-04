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


template <class TSpectralResponse , class TRSR>
inline typename ReduceSpectralResponse<TSpectralResponse , TRSR>::ValuePrecisionType
ReduceSpectralResponse<TSpectralResponse , TRSR>
::operator()(const unsigned int numBand)
{
  if (numBand >= m_InputSatRSR->GetNbBands())
    {
    itkExceptionMacro(<< "There is no band num " << numBand << " in the RSR vector!(Size of the current RSR vector is " << m_InputSatRSR->GetNbBands() << ")" );
    }
  else
    {
    ValuePrecisionType res = itk::NumericTraits<ValuePrecisionType>::ZeroValue();
    ValuePrecisionType response1;
    ValuePrecisionType response2;
    ValuePrecisionType inputSatRSR1;
    ValuePrecisionType inputSatRSR2;

    ValuePrecisionType inputRSR1;
    ValuePrecisionType inputRSR2;
    PrecisionType lambda1;
    PrecisionType lambda2;


    typename VectorPairType::const_iterator it;
    VectorPairType pairs = (m_InputSatRSR->GetRSR())[numBand]->GetResponse();
    it = pairs.begin();

    PrecisionType lambdaMin=(this->m_InputSatRSR->GetRSR())[numBand]->GetInterval().first;
    PrecisionType lambdaMax=(this->m_InputSatRSR->GetRSR())[numBand]->GetInterval().second;

    ValuePrecisionType totalArea = static_cast<ValuePrecisionType> (lambdaMax - lambdaMin);
    if (totalArea == 0) return static_cast<ValuePrecisionType> (0.0);

    typename InputRSRType::SpectralResponseType* solarIrradiance;
    if(m_ReflectanceMode)
      {
      // In the case of reflectances, the normalization is done using the solar irradiance integrated over the spectral band
      totalArea = 0.0;
      solarIrradiance = this->m_InputSatRSR->GetSolarIrradiance();
      typename VectorPairType::const_iterator pit = pairs.begin();
      while (pit != pairs.end())
        {
        totalArea += ((*pit).second)*(*solarIrradiance)((*pit).first);
        ++pit;
        }
      }

    while (it != pairs.end() - 1)
      {

      lambda1 = (*it).first;
      lambda2 = (*(it + 1)).first;
      if((lambda1<lambdaMax) && (lambda2>lambdaMin))
        {
        inputSatRSR1 = (*it).second;
        inputSatRSR2 = (*(it + 1)).second;
        if(m_ReflectanceMode)
          {
          // We multiply the spectral sensitivity by the solar irradiance
          inputSatRSR1 *= (*solarIrradiance)((*it).first);
          inputSatRSR2 *= (*solarIrradiance)((*(it + 1)).first);
          }

        inputRSR1 = (*m_InputSpectralResponse)(lambda1);
        inputRSR2 = (*m_InputSpectralResponse)(lambda2);


        // lambda1 need to be resampled
        /*

                                /------+ inputRSR2
                            /---       |
                           /           |
                          |            |
                          |            |
                          |            |
                          |            |
         inputRSR1=0+------------------+
         lambda1            lambda2
                          ^
                          |
         first non zero val in inputRSR

         - after resampling

                          /------+ inputRSR2
                      /---       |
                     /           |
         inputRSR1  +            |
                    |            |
                    |            |
                    |            |
                    +------------+
                 lambda1      lambda2
         */
        if ((inputRSR1 == 0) && (inputRSR2 != 0))
          {
          PrecisionType lambdaRSRmin = m_InputSpectralResponse->GetInterval().first;
          if ((lambdaRSRmin > lambda2) || (lambdaRSRmin < lambda1))
            {
            itkExceptionMacro(<<"Spectral response problem");
            }

          PrecisionType lambdaDist = lambdaRSRmin - lambda1;
          PrecisionType ratio = lambdaDist / (lambda2 - lambda1);
          lambda1 = lambdaRSRmin;
          inputSatRSR1 = ratio * inputSatRSR1 + (1 - ratio) * inputSatRSR2;

          inputRSR1=(*m_InputSpectralResponse)(lambda1);

          }

        // lambda2 need to be resampled
        /*

         inputRSR1  +---\
                    |    \
                    |     ------\
                    |            |
                    |            |
                    |            |
                    +----------------------------+ inputRSR2=0
                 lambda1                       lambda2
                                 ^
                                 |
         first non zero val in inputRSR

         - after resampling


         inputRSR1  +---\
                    |    \
                    |     ------\
                    |            +inputRSR2
                    |            |
                    |            |
                    +------------+
                 lambda1        lambda2

         */
        if ((inputRSR1 != 0) && (inputRSR2 == 0))
          {
          PrecisionType lambdaRSRmax = m_InputSpectralResponse->GetInterval().second;
          if ((lambdaRSRmax > lambda2) || (lambdaRSRmax < lambda1))
            {
            itkExceptionMacro(<<"Spectral response problem");
            }
          PrecisionType lambdaDist = lambdaRSRmax - lambda1;
          PrecisionType ratio = lambdaDist / (lambda2 - lambda1);
          lambda2 = lambdaRSRmax;
          inputSatRSR2 = ratio * inputSatRSR1 + (1 - ratio) * inputSatRSR2;

          inputRSR2=(*m_InputSpectralResponse)(lambda2);

          }

        response1 = inputRSR1 * inputSatRSR1;
        response2 = inputRSR2 * inputSatRSR2;

        ValuePrecisionType rmin = std::min(response1, response2);
        ValuePrecisionType rmax = std::max(response1, response2);

        /*
          rmax +\
            | ---\
            |     --| rmin
            |       |
            |       |
            |       |
            |       |
            |-------+
           lambda1   lambda2
         */

        //Compute the surface of the trapezoid

        ValuePrecisionType area = (lambda2 - lambda1) * (rmax + rmin) / 2.0;
        res += area;
        }
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
