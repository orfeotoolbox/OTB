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
#ifndef __otbSatelliteRSR_txx
#define __otbSatelliteRSR_txx

#include "otbSatelliteRSR.h"

namespace otb
{

template<class TPrecision, class TValuePrecision>
SatelliteRSR<TPrecision, TValuePrecision>::SatelliteRSR()
{
  //m_RSR = VectorType::New();
  m_SortBands = true;
  m_SolarIrradiance = SpectralResponseType::New();
}

template<class TPrecision, class TValuePrecision>
void SatelliteRSR<TPrecision, TValuePrecision>::Load(const std::string & filename, ValuePrecisionType coefNormalization)
{
  //Parse 6S file Reduce spectral response
  //Begin by getting the number of band of the satellite
  //unsigned int nbBands= this->SetNbBands ( filename );
  std::ifstream fin(filename.c_str());
  if (fin.fail())
    {
    itkExceptionMacro(<<"Error opening file" << filename);
    }

  // For each band
  for (unsigned int i = 0; i < m_NbBands; ++i)
    {
    //Create a SpectralResponse for the band i
    SpectralResponsePointerType RSRBand = SpectralResponseType::New();
    m_RSR.push_back(RSRBand);
    }
  PrecisionType currentLambda;
  PrecisionType solarIrradiance;
  while (!fin.eof())
    {
    //Parse the 6S.txt file
    fin >> currentLambda;
    fin >> solarIrradiance;

    m_SolarIrradiance->GetResponse().push_back(std::make_pair(currentLambda,
                                                              solarIrradiance));
    //for each band add a pair of values (wavelength and % response)
    for (unsigned int i = 0; i < m_NbBands; ++i)
      {
      std::pair<TPrecision, TValuePrecision> currentPair;
      currentPair.first = currentLambda;
      fin >> currentPair.second;
      currentPair.second = currentPair.second / coefNormalization;
      //Include only non null value //TODO
      m_RSR[i]->GetResponse().push_back(currentPair);
      }
    }
  fin.close();

  if (m_SortBands)
    {
    //Sort the vector of SpectralResponse (band order by minimum wavelength first not null response
    std::sort(m_RSR.begin(), m_RSR.end(), sort_band());
    }
}

template<class TPrecision, class TValuePrecision>
void SatelliteRSR<TPrecision, TValuePrecision>::Load(PrecisionType lambdaMin, PrecisionType lambdaMax,
                                                     PrecisionType sampling, ValuePrecisionType coefNormalization)
{
  m_NbBands = 1;
  const double wavelengthPrecision = 0.0025; //in um

  if (0 == sampling)
    {
    sampling = wavelengthPrecision;
    }

  // For each band
  for (unsigned int i = 0; i < m_NbBands; ++i)
    {
    //Create a SpectralResponse for the band i
    SpectralResponsePointerType RSRBand = SpectralResponseType::New();
    m_RSR.push_back(RSRBand);
    }

  PrecisionType currentLambda;
  ValuePrecisionType value = 1.0;

  for (double j = lambdaMin; j < lambdaMax; j = j + sampling)
    {
    currentLambda = j;
    std::pair<TPrecision, TValuePrecision> currentPair;
    currentPair.first = currentLambda;
    currentPair.second = value / coefNormalization;
    m_RSR[0]->GetResponse().push_back(currentPair);
    }

}

template<class TPrecision, class TValuePrecision>
bool SatelliteRSR<TPrecision, TValuePrecision>::Clear()
{
  m_RSR.clear();
  return true;
}

template<class TPrecision, class TValuePrecision>
int SatelliteRSR<TPrecision, TValuePrecision>::Size() const
{
  return m_RSR.size();
}

template<class TPrecision, class TValuePrecision>
inline typename SatelliteRSR<TPrecision, TValuePrecision>::ValuePrecisionType SatelliteRSR<TPrecision, TValuePrecision>::operator()(
                                                                                                                                    const PrecisionType & lambda,
                                                                                                                                    const unsigned int numBand)
{
  //Get the response of the band number numBand at the wavelenght lambda
  if (numBand >= m_NbBands)
    {
    itkExceptionMacro(<<"There is no band num " << numBand<< " in the RSR vector!(Size of the current RSR vector is " << m_NbBands <<")");
    }
  else
    {
    return (*(m_RSR[numBand]))(lambda);
    }

}

template<class TPrecision, class TValuePrecision>
void SatelliteRSR<TPrecision, TValuePrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << std::endl;

  //typename VectorPairType::iterator it = m_RSR.begin();
  //it = m_RSR.at(0);
  for (typename RSRVectorType::const_iterator it = m_RSR.begin(); it != m_RSR.end(); ++it)
    {
    os << indent << "Band Number " << it - m_RSR.begin() << std::endl;
    (*it)->PrintSelf(os, indent);
    os << std::endl;
    }
}

} // end namespace otb

#endif
