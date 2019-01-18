/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbReduceSpectralResponse_h
#define otbReduceSpectralResponse_h


#include "itkDataObject.h"
#include <itkObjectFactory.h>
#include <vector>
#include <utility>
#include <limits>

namespace otb
{
/** \class ReduceSpectralResponse
   * \brief This class computes the reduced spectral response of each band of a sensor
   *
   * It takes the spectral response of an object and the relative
   * spectral response of a satellite (one response per band) to
   * compute the satellite spectral bands.
   *
   * There are 2 operating modes:
   * - radiance mode (default): integrates the spectral response over each band
   * - reflectance mode (needs to call SetReflectanceMode(true)): takes into account
   *   the solar irradiance in the integration
   *
   * The two templates indicate:
   * - the SpectralResponse input
   * - the relative spectral response (RSR) of a sensor
   *
   * The computation assumes that the RSR of the sensor has a
   * wavelength resolution similar or better than the spectral
   * response to be reduced. If this is not the case, it is wise to
   * oversample it (by linear interpolation, for instance).
   *
   * The method CalculateResponse must be called.
   *
   * \sa itk::DataObject
   * \sa SpectralResponse
   * \sa SatelliteRSR
   *
 *
 * \ingroup OTBSimulation
 */
template <class TSpectralResponse , class TRSR>
class ReduceSpectralResponse : public itk::DataObject
{

public:

  /** Standard class typedefs */
  typedef ReduceSpectralResponse Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Template parameters typedef */
  typedef TSpectralResponse InputSpectralResponseType;
  typedef TRSR InputRSRType;

  typedef typename InputSpectralResponseType::PairType PairType;
  typedef typename InputRSRType::Pointer InputRSRPointerType;
  typedef typename InputSpectralResponseType::Pointer InputSpectralResponsePointerType;

  typedef typename InputRSRType::PrecisionType PrecisionType;
  typedef typename InputRSRType::ValuePrecisionType ValuePrecisionType;

  typedef typename InputSpectralResponseType::VectorPairType VectorPairType;

  //TODO need a specific class for the integration of stectral responses (now it is in the functor)
  typedef typename InputRSRType::RSRVectorType RSRVectorType;
  typedef typename std::vector<ValuePrecisionType> ReduceSpectralResponseVectorType;
  /** Standard macros */
  itkNewMacro(Self);
; itkTypeMacro(ReduceSpectralResponse, DataObject);
;

  itkGetConstObjectMacro(InputSatRSR, InputRSRType);
; itkSetObjectMacro(InputSatRSR, InputRSRType);
;

  itkGetConstObjectMacro(InputSpectralResponse, InputSpectralResponseType);
; itkSetObjectMacro(InputSpectralResponse, InputSpectralResponseType);
;

  /** The GetReduceResponse method gives the output. The first value in the pair is the central wavelength of the band (see SpectralResponse). */
  itkGetObjectMacro(ReduceResponse, InputSpectralResponseType);
;

  itkSetMacro(ReflectanceMode, bool);
  itkGetConstMacro(ReflectanceMode, bool);
  
  /** Clear the vector data  */
  virtual bool Clear();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /**
   * \param PrecisionType
   * \return The integrated value of the RSR for the input spectral response.
   */
  inline ValuePrecisionType operator()(const unsigned int numBand);

  /** Calculate the vector response for each band of the sensor*/
  void CalculateResponse();

  /** Create and load Spectral response and satellite RSR from files*/
  void LoadInputsFromFiles(const std::string & spectralResponseFile, const std::string & RSRFile,
                           const unsigned int nbRSRBands, ValuePrecisionType coefNormSpectre = 1.0,
                           ValuePrecisionType coefNormRSR = 1.0);

protected:
  /** Constructor */
  ReduceSpectralResponse();
  /** Constructor from a ASCII file */
  //ReduceSpectralResponse( const std::string & filename );
  /** Destructor */
  ~ReduceSpectralResponse() override
  {
  }
;
  /** PrintSelf method */
  //void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Input relative spectral sensor model of response */
  InputRSRPointerType m_InputSatRSR;
  /** Input spectral response (usually from a library) */
  InputSpectralResponsePointerType m_InputSpectralResponse;

private:
  ReduceSpectralResponse(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** the computed response */
  InputSpectralResponsePointerType m_ReduceResponse;

  /** Choose between reflectance or radiance mode */
  bool m_ReflectanceMode;
};

}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReduceSpectralResponse.hxx"
#endif

#endif

