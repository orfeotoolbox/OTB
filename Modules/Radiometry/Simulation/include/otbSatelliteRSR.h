/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbSatelliteRSR_h
#define otbSatelliteRSR_h

#include <vector>
#include <utility>
#include <fstream>
#include <limits>

#include "otbSpectralResponse.h"

namespace otb
{
/** \class SatelliteRSR
 * \brief This class represents a hierarchy of vector data.
 *
 * This class contains an std::vector of spectral response, class otb::SpectralResponse (one for each band of the satellite).
 *
 *
 * The two templates indicate:
 * - the precision of the wavelength (X) (by default: double)
 * - the precision of the gain associated to the wavelength (Y) (by default: double)

 * \sa SpectralResponse
 * \sa itk::DataObject
 *
 * \ingroup OTBSimulation
 */
template<class TPrecision = double, class TValuePrecision = double>
class SatelliteRSR: public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef SatelliteRSR Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkNewMacro(Self)
; itkTypeMacro(SatelliteRSR, DataObject)
;

  /** Set the number of band of the satellite from an ASCII file
   * Need to parse first all the file to determine the number of columns */
  itkGetConstMacro(NbBands, unsigned int)
; itkSetMacro(NbBands, unsigned int)
;

  itkSetMacro(SortBands, bool)
;

  /** Template parameters typedef */
  typedef TPrecision PrecisionType;
  typedef TValuePrecision ValuePrecisionType;

  typedef SpectralResponse<TPrecision, TValuePrecision> SpectralResponseType;
  typedef typename SpectralResponseType::Pointer SpectralResponsePointerType;
  typedef std::vector<SpectralResponsePointerType> RSRVectorType;
  typedef typename SpectralResponseType::VectorPairType VectorPairType;

  /** Clear the vector data  */
  virtual bool Clear();

  /** Return the number of element in the vector (nb bands) */
  virtual int Size() const;

  /** Fill from a ASCII file (spectral gauge)*/
  void Load(const std::string & filename, ValuePrecisionType coefNormalization = 1.0);

  /** Fill from user defined criteria (sampling band from lambdaMin to lambdaMax with Bandwidth width) */
  void Load(PrecisionType lambdaMin, PrecisionType lambdaMax, PrecisionType sampling,
            ValuePrecisionType coefNormalization = 1.0);

  /** Compare RSR response (order bands from high to low frequency)*/
  struct sort_band
  {
    bool operator()(SpectralResponsePointerType a, SpectralResponsePointerType b)
    {
      PrecisionType aFirstNotNull;
      PrecisionType bFirstNotNull;

      typename VectorPairType::const_iterator it = a->GetResponse().begin();

      while ((*it).second == 0)
        {
        ++it;
        }
      aFirstNotNull = (*it).first;

      typename VectorPairType::const_iterator it2 = b->GetResponse().begin();

      while ((*it2).second == 0)
        {
        ++it2;
        }
      bFirstNotNull = (*it2).first;

      return aFirstNotNull < bFirstNotNull;
    }
  };

  /**
   * \param PrecisionType
   * \param Band
   * \return The interpolated value of the SR for the numBand (band num 0 to band num (m_NbBands-1)).
   */
  inline ValuePrecisionType operator()(const PrecisionType & lambda, const unsigned int numBand);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** get vector of RSR */
  RSRVectorType & GetRSR()
  {
    return m_RSR;
  }

  /** get the solar irradiance */
  SpectralResponseType* GetSolarIrradiance()
  {
    return m_SolarIrradiance;
  }

protected:
  /** Constructor */
  SatelliteRSR();
  /** Constructor from a ASCII file */
  //SatelliteRSR( const std::string & filename );
  /** Destructor */
  ~SatelliteRSR() override
  {
  }
;

  bool m_SortBands;

private:
  SatelliteRSR(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Vector of SpectralResponse */
  RSRVectorType m_RSR;

  /** Solar irradiance vector */
  SpectralResponsePointerType m_SolarIrradiance;
  /** Number of bands*/
  unsigned int m_NbBands;
};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSatelliteRSR.hxx"
#endif

#endif

