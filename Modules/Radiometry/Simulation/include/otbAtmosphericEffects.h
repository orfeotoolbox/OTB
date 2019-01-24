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

#ifndef otbAtmosphericEffects_h
#define otbAtmosphericEffects_h

#include "otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.h"

namespace otb
{
/** \class AtmosphericEffects
   * \brief This class applies atmospheric effects, computed from otbAtmosphericCorrectionParametersTo6SRadiativeTerms  to an input spectrum (otbSpectralResponse) using the otbSurfaceReflectanceToReflectanceFilter.
   *
   * The two templates indicate:
   * - the SpectralResponse input (from the JPL ASTER spectral library for example see : http://speclib.jpl.nasa.gov/)
   * - the SatelliteRSR relatives spectral response of a sensor (RSR).
   *
   * \sa itk::DataObject
 *
 * \ingroup OTBSimulation
 */
template <class TSpectralResponse , class TRSR>
class AtmosphericEffects
  : public itk::DataObject
      {
        public:
           /** Standard class typedefs */
           typedef AtmosphericEffects Self;
           typedef itk::DataObject Superclass;
           typedef itk::SmartPointer<Self> Pointer;
           typedef itk::SmartPointer<const Self> ConstPointer;

           /** Template parameters typedef. */
           /** Spectral response of a material (from the JPL ASTER spectral library for example)*/
           typedef TSpectralResponse InputSpectralResponseType;
           /** Template parameters typedef. */
           /** The relatives spectral response of a sensor (RSR)*/
           typedef TRSR InputRSRType;

           typedef typename InputRSRType::Pointer InputRSRPointerType;
           typedef typename InputSpectralResponseType::Pointer InputSpectralResponsePointerType;

           typedef typename InputRSRType::PrecisionType PrecisionType;
           typedef typename InputRSRType::ValuePrecisionType ValuePrecisionType;
           typedef typename InputSpectralResponseType::PairType PairType;


          typedef otb::AtmosphericRadiativeTerms                                    AtmosphericRadiativeTermsType;
          typedef typename AtmosphericRadiativeTermsType::Pointer                  AtmosphericRadiativeTermsPointerType;

            /** Standard macros */
           itkNewMacro(Self);
           itkTypeMacro(AtmosphericEffects, DataObject);

           itkGetConstObjectMacro(InputSatRSR, InputRSRType);
           itkSetObjectMacro(InputSatRSR, InputRSRType);

           itkGetConstObjectMacro(InputSpectralResponse, InputSpectralResponseType);
           itkSetObjectMacro(InputSpectralResponse, InputSpectralResponseType);

            /** Get/Set Atmospheric Radiative Terms. */
            void SetAtmosphericRadiativeTerms(AtmosphericRadiativeTermsPointerType atmoRadTerms)
            {
              m_AtmosphericRadiativeTerms = atmoRadTerms;
              this->Modified();
            }
           itkGetConstObjectMacro(AtmosphericRadiativeTerms, AtmosphericRadiativeTermsType);
           //itkSetObjectMacro(AtmosphericRadiativeTerms, AtmosphericRadiativeTermsType);

           itkGetObjectMacro(CorrectedSpectralResponse, InputSpectralResponseType);


//         void  LoadFilterFunctionAtmosphericCorrectionParameters( double step = 0.0025);
           void  Process(/*const unsigned int numBand*/);

        protected:
           /** Constructor */
           AtmosphericEffects();
           /** Constructor from a ASCII file */
           //AtmosphericEffects( const std::string & filename );
           /** Destructor */
           ~AtmosphericEffects() override {};
           /** PrintSelf method */
           //void PrintSelf(std::ostream& os, itk::Indent indent) const;


        private:
           AtmosphericEffects(const Self&) = delete;
           void operator=(const Self&) = delete;

           AtmosphericRadiativeTermsPointerType m_AtmosphericRadiativeTerms;
           InputSpectralResponsePointerType m_InputSpectralResponse;
           InputSpectralResponsePointerType m_CorrectedSpectralResponse;
           InputRSRPointerType m_InputSatRSR;

           bool m_IsSetAtmosphericRadiativeTerms;

      };
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAtmosphericEffects.hxx"
#endif

#endif
