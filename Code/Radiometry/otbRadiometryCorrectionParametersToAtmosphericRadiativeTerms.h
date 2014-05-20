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
#ifndef __otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms_h
#define __otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms_h

#include "otbAtmosphericRadiativeTerms2.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbSIXSTraits.h"

namespace otb
{

/** \class RadiometryCorrectionParametersToAtmosphericRadiativeTerms
   * \brief TODO.
   *
   * TODO
   * \ingroup Radiometry
   *
 */
  class ITK_EXPORT RadiometryCorrectionParametersToAtmosphericRadiativeTerms
  {
  public:

    /** Call the varSol function*/
    static AtmosphericRadiativeTerms2::Pointer Compute(AtmosphericCorrectionParameters* paramIn)
    {
      AtmosphericRadiativeTerms2::Pointer radTermsOut = AtmosphericRadiativeTerms2::New();

      typedef AtmosphericCorrectionParameters::WavelengthSpectralBandVectorType WavelengthSpectralBandVectorType;
      WavelengthSpectralBandVectorType WavelengthSpectralBandVector = paramIn->GetWavelengthSpectralBand();
      unsigned int NbBand = WavelengthSpectralBandVector->Size();

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
          atmosphericReflectance = 0.;
          atmosphericSphericalAlbedo = 0.;
          totalGaseousTransmission = 0.;
          downwardTransmittance = 0.;
          upwardTransmittance = 0.;
          upwardDiffuseTransmittance = 0.;
          upwardDirectTransmittance = 0.;
          upwardDiffuseTransmittanceForRayleigh = 0.;
          upwardDiffuseTransmittanceForAerosol = 0.;
          SIXSTraits::ComputeAtmosphericParameters(
            paramIn->GetSolarZenithalAngle(),                  /** The Solar zenithal angle */
            paramIn->GetSolarAzimutalAngle(),                  /** The Solar azimutal angle */
            paramIn->GetViewingZenithalAngle(),                /** The Viewing zenithal angle */
            paramIn->GetViewingAzimutalAngle(),                /** The Viewing azimutal angle */
            paramIn->GetMonth(),                               /** The Month */
            paramIn->GetDay(),                                 /** The Day (in the month) */
            paramIn->GetAtmosphericPressure(),                 /** The Atmospheric pressure */
            paramIn->GetWaterVaporAmount(),                    /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
            paramIn->GetOzoneAmount(),                         /** The Ozone amount (Stratospheric ozone layer content) */
            paramIn->GetAerosolModel(),                        /** The Aerosol model */
            paramIn->GetAerosolOptical(),                      /** The Aerosol optical (radiative impact of aerosol for the reference wavelength 550-nm) */
            paramIn->GetWavelengthSpectralBand()->GetNthElement(i), /** Wavelength for the spectral band definition */
            /** Note : The Max wavelength spectral band value must be updated ! */
            atmosphericReflectance,                          /** Atmospheric reflectance */
            atmosphericSphericalAlbedo,                      /** atmospheric spherical albedo */
            totalGaseousTransmission,                        /** Total gaseous transmission */
            downwardTransmittance,                           /** downward transmittance */
            upwardTransmittance,                             /** upward transmittance */
            upwardDiffuseTransmittance,                      /** Upward diffuse transmittance */
            upwardDirectTransmittance,                       /** Upward direct transmittance */
            upwardDiffuseTransmittanceForRayleigh,           /** Upward diffuse transmittance for rayleigh */
            upwardDiffuseTransmittanceForAerosol             /** Upward diffuse transmittance for aerosols */
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
          radTermsOut->SetWavelengthSpectralBand(i, paramIn->GetWavelengthSpectralBand()->GetNthElement(i)->GetCenterSpectralValue());
        }

      return radTermsOut;
    }
  };
} //end namespace otb
#endif
