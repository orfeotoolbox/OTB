/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms_h
#define otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms_h

#include "otbAtmosphericRadiativeTerms.h"
#include "otbImageMetadataCorrectionParameters.h"
#include "otbSIXSTraits.h"
#include "otbMacro.h"

namespace otb
{

/** \class RadiometryCorrectionParametersToAtmosphericRadiativeTerms
   * \brief TODO.
   *
   * TODO
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBOpticalCalibration
 */
class RadiometryCorrectionParametersToAtmosphericRadiativeTerms
{
public:
  /** Call the varSol function*/
  static AtmosphericRadiativeTerms::Pointer Compute(AtmosphericCorrectionParameters* paramAtmo, ImageMetadataCorrectionParameters* paramAcqui)
  {
    AtmosphericRadiativeTerms::Pointer radTermsOut = AtmosphericRadiativeTerms::New();

    typedef AtmosphericCorrectionParameters::WavelengthSpectralBandVectorType WavelengthSpectralBandVectorType;
    WavelengthSpectralBandVectorType                                          WavelengthSpectralBandVector = paramAcqui->GetWavelengthSpectralBand();
    unsigned int                                                              NbBand                       = WavelengthSpectralBandVector->Size();

    radTermsOut->ValuesInitialization(NbBand);

    double atmosphericReflectance(0.);
    double atmosphericSphericalAlbedo(0.);
    double totalGaseousTransmission(0.);
    double downwardTransmittance(0.);
    double upwardTransmittance(0.);
    double upwardDiffuseTransmittance(0.);
    double upwardDirectTransmittance(0.);
    double upwardDiffuseTransmittanceForRayleigh(0.);
    double upwardDiffuseTransmittanceForAerosol(0.);

    for (unsigned int i = 0; i < NbBand; ++i)
    {
      atmosphericReflectance                = 0.;
      atmosphericSphericalAlbedo            = 0.;
      totalGaseousTransmission              = 0.;
      downwardTransmittance                 = 0.;
      upwardTransmittance                   = 0.;
      upwardDiffuseTransmittance            = 0.;
      upwardDirectTransmittance             = 0.;
      upwardDiffuseTransmittanceForRayleigh = 0.;
      upwardDiffuseTransmittanceForAerosol  = 0.;
      SIXSTraits::ComputeAtmosphericParameters(
          paramAcqui->GetSolarZenithalAngle(),   /** The Solar zenithal angle */
          paramAcqui->GetSolarAzimutalAngle(),   /** The Solar azimutal angle */
          paramAcqui->GetViewingZenithalAngle(), /** The Viewing zenithal angle */
          paramAcqui->GetViewingAzimutalAngle(), /** The Viewing azimutal angle */
          paramAcqui->GetMonth(),                /** The Month */
          paramAcqui->GetDay(),                  /** The Day (in the month) */
          paramAtmo->GetAtmosphericPressure(),   /** The Atmospheric pressure */
          paramAtmo->GetWaterVaporAmount(),      /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
          paramAtmo->GetOzoneAmount(),           /** The Ozone amount (Stratospheric ozone layer content) */
          paramAtmo->GetAerosolModel(),          /** The Aerosol model */
          paramAtmo->GetAerosolOptical(),        /** The Aerosol optical (radiative impact of aerosol for the reference wavelength 550-nm) */
          paramAcqui->GetWavelengthSpectralBand()->GetNthElement(i), /** Wavelength for the spectral band definition */
          /** Note : The Max wavelength spectral band value must be updated ! */
          atmosphericReflectance,                /** Atmospheric reflectance */
          atmosphericSphericalAlbedo,            /** atmospheric spherical albedo */
          totalGaseousTransmission,              /** Total gaseous transmission */
          downwardTransmittance,                 /** downward transmittance */
          upwardTransmittance,                   /** upward transmittance */
          upwardDiffuseTransmittance,            /** Upward diffuse transmittance */
          upwardDirectTransmittance,             /** Upward direct transmittance */
          upwardDiffuseTransmittanceForRayleigh, /** Upward diffuse transmittance for rayleigh */
          upwardDiffuseTransmittanceForAerosol   /** Upward diffuse transmittance for aerosols */
          );

      radTermsOut->SetIntrinsicAtmosphericReflectance(i, atmosphericReflectance);
      radTermsOut->SetSphericalAlbedo(i, atmosphericSphericalAlbedo);
      radTermsOut->SetTotalGaseousTransmission(i, totalGaseousTransmission);
      radTermsOut->SetDownwardTransmittance(i, downwardTransmittance);
      radTermsOut->SetUpwardTransmittance(i, upwardTransmittance);
      radTermsOut->SetUpwardDiffuseTransmittance(i, upwardDiffuseTransmittance);
      radTermsOut->SetUpwardDirectTransmittance(i, upwardDirectTransmittance);
      radTermsOut->SetUpwardDiffuseTransmittanceForRayleigh(i, upwardDiffuseTransmittanceForRayleigh);
      radTermsOut->SetUpwardDiffuseTransmittanceForAerosol(i, upwardDiffuseTransmittanceForAerosol);
      radTermsOut->SetWavelengthSpectralBand(i, paramAcqui->GetWavelengthSpectralBand()->GetNthElement(i)->GetCenterSpectralValue());
    }

    return radTermsOut;
  }
};
} // end namespace otb
#endif
