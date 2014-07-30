/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      2008-05-30
  Version:   2.2.0


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbSIXSTraits2.h"

#include "otb_6S.h"
#include "main_6s.h"
#include "otbMacro.h"

#include <iomanip>

namespace otb
{

void
SIXSTraits2::ComputeAtmosphericParameters(
  const double SolarZenithalAngle,                                        /** The Solar zenithal angle */
  const double SolarAzimutalAngle,                                        /** The Solar azimutal angle */
  const double ViewingZenithalAngle,                                      /** The Viewing zenithal angle */
  const double ViewingAzimutalAngle,                                      /** The Viewing azimutal angle */
  const unsigned int Month,                                               /** The Month */
  const unsigned int Day,                                                 /** The Day (in the month) */
  const double AtmosphericPressure,                                       /** The Atmospheric pressure */
  const double WaterVaporAmount,                                          /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
  const double OzoneAmount,                                               /** The Ozone amount (Stratospheric ozone layer content) */
  const AerosolModelType&      AerosolModel,                              /** The Aerosol model */
  const double AerosolOptical,                                            /** The Aerosol optical (radiative impact of aerosol for the reference wavelength 550-nm) */
  WavelengthSpectralType* WavelengthSpectralBand,                 /** Wavelength for the spectral band definition */
  /** Note : The Max wavelength spectral band value must be updated ! */
  double&                AtmosphericReflectance,                  /** Atmospheric reflectance */
  double&                AtmosphericSphericalAlbedo,              /** atmospheric spherical albedo */
  double&                TotalGaseousTransmission,                /** Total gaseous transmission */
  double&                DownwardTransmittance,                   /** downward transmittance */
  double&                UpwardTransmittance,                     /** upward transmittance */
  double&                UpwardDiffuseTransmittance,              /** upward diffuse transmittance */
  double&                UpwardDirectTransmittance,               /** Upward direct transmittance */
  double&                UpwardDiffuseTransmittanceForRayleigh,   /** upward diffuse transmittance for rayleigh */
  double&                UpwardDiffuseTransmittanceForAerosol     /** supward diffuse transmittance for aerosols */
  )
{
// geometrical conditions
  otb_6s_doublereal asol(static_cast<otb_6s_doublereal>(SolarZenithalAngle));
  otb_6s_doublereal phi0(static_cast<otb_6s_doublereal>(SolarAzimutalAngle));
  otb_6s_doublereal avis(static_cast<otb_6s_doublereal>(ViewingZenithalAngle));
  otb_6s_doublereal phiv(static_cast<otb_6s_doublereal>(ViewingAzimutalAngle));
  otb_6s_integer    month(static_cast<otb_6s_integer>(Month));
  otb_6s_integer    jday(static_cast<otb_6s_integer>(Day));
  otb_6s_doublereal pressure(static_cast<otb_6s_doublereal>(AtmosphericPressure));
  otb_6s_doublereal uw(static_cast<otb_6s_doublereal>(WaterVaporAmount));
  otb_6s_doublereal uo3(static_cast<otb_6s_doublereal>(OzoneAmount));
// atmospheric model
  otb_6s_integer    iaer(static_cast<otb_6s_integer>(AerosolModel));
  otb_6s_doublereal taer55(static_cast<otb_6s_doublereal>(AerosolOptical));

  // Init output parameters
  AtmosphericReflectance = 0.;
  AtmosphericSphericalAlbedo = 0.;
  TotalGaseousTransmission = 0.;
  DownwardTransmittance = 0.;
  UpwardTransmittance = 0.;
  UpwardDiffuseTransmittance = 0.;
  UpwardDirectTransmittance = 0.;
  UpwardDiffuseTransmittanceForRayleigh = 0.;
  UpwardDiffuseTransmittanceForAerosol = 0.;

  otb_6s_doublereal wlinf(0.), wlsup(0.);
  otb_6s_doublereal otb_ratm__(0.), sast(0.), tgasm(0.), sdtott(0.), sutott(0.);
  otb_6s_doublereal tdif_up(0.), tdir_up(0.), tdif_up_ray(0.), tdif_up_aer(0.);

  // 6S official Wavelength Spectral Band step value
  const float SIXSStepOfWavelengthSpectralBandValues = .0025;
  // Generate 6s Wavelength Spectral Band with the offcicial step value
  ComputeWavelengthSpectralBandValuesFor6S(SIXSStepOfWavelengthSpectralBandValues,
                                           WavelengthSpectralBand        // Update
                                           );
  try
    {

    // 6S official tab size Wavelength Spectral
    const otb_6s_integer S_6S_SIZE = 1501;
    // Generate WavelengthSpectralBand  in 6S compatible buffer s[1501]
    wlinf = static_cast<otb_6s_doublereal>(WavelengthSpectralBand->GetMinSpectralValue());
    wlsup = static_cast<otb_6s_doublereal>(WavelengthSpectralBand->GetMaxSpectralValue());
    otb_6s_integer iinf =
      static_cast<otb_6s_integer>((wlinf - (float) .25) / SIXSStepOfWavelengthSpectralBandValues + (float) 1.5);
    otb_6s_integer      cpt = iinf - 1;
    otb_6s_doublereal * s(NULL);
    s = new otb_6s_doublereal[S_6S_SIZE];
    memset(s, 0, S_6S_SIZE * sizeof(otb_6s_doublereal));
    const ValuesVectorType& FilterFunctionValues6S = WavelengthSpectralBand->GetFilterFunctionValues6S();
    // Set the values of FilterFunctionValues6S in s between [iinf-1; isup]
    for (unsigned int i = 0; i < FilterFunctionValues6S.size() && cpt < S_6S_SIZE; ++i)
      {
      s[cpt] = FilterFunctionValues6S[i];
      ++cpt;
      }

    // Call 6s main function
    otbMsgDevMacro(<< "Start call 6S main function ...");
    otb_6s_ssssss_otb_main_function(&asol, &phi0, &avis, &phiv, &month, &jday,
                                    &pressure, &uw, &uo3,
                                    &iaer,
                                    &taer55,
                                    &wlinf, &wlsup,
                                    s,
                                    &otb_ratm__,
                                    &sast,
                                    &tgasm,
                                    &sdtott,
                                    &sutott,
                                    &tdif_up,
                                    &tdir_up,
                                    &tdif_up_ray,
                                    &tdif_up_aer);
    otbMsgDevMacro(<< "Done call 6S main function!");
    delete[] s;
    s = NULL;
    }
  catch (std::bad_alloc& err)
    {
    itkGenericExceptionMacro(<< "Exception bad_alloc in SIXSTraits2 class: " << (char*) err.what());
    }
  catch (...)
    {
    itkGenericExceptionMacro(<< "Unknown exception in SIXSTraits2 class (catch(...)");
    }

  // Set outputs parameters
  AtmosphericReflectance = static_cast<double>(otb_ratm__);
  AtmosphericSphericalAlbedo = static_cast<double>(sast);
  TotalGaseousTransmission = static_cast<double>(tgasm);
  DownwardTransmittance = static_cast<double>(sdtott);
  UpwardTransmittance = static_cast<double>(sutott);
  UpwardDiffuseTransmittance = static_cast<double>(tdif_up);
  UpwardDirectTransmittance = static_cast<double>(tdir_up);
  UpwardDiffuseTransmittanceForRayleigh = static_cast<double>(tdif_up_ray);
  UpwardDiffuseTransmittanceForAerosol = static_cast<double>(tdif_up_aer);
}

void
SIXSTraits2::ComputeWavelengthSpectralBandValuesFor6S(
  const double SIXSStepOfWavelengthSpectralBandValues,
  WavelengthSpectralType*         WavelengthSpectralBand
  )
{
  const double            epsilon(.000001);
  const double            L_min = static_cast<double>(WavelengthSpectralBand->GetMinSpectralValue());
  const double            L_max = static_cast<double>(WavelengthSpectralBand->GetMaxSpectralValue());
  const double            L_userStep = static_cast<double>(WavelengthSpectralBand->GetUserStep());
  const ValuesVectorType& FilterFunctionValues = WavelengthSpectralBand->GetFilterFunctionValues();
  unsigned int            i = 1;
  unsigned int            j = 1;
  const double            invStep = static_cast<double>(1. / L_userStep);
  double                  value(0.);

  if (FilterFunctionValues.size() <= 1)
    {
    itkGenericExceptionMacro(<< "The FilterFunctionValues vector must have more than 1 values !");
    }
  if (! (L_min + static_cast<double>(FilterFunctionValues.size() - 1) * L_userStep < (L_max + epsilon) ) )
    {
    itkGenericExceptionMacro(
      << "The following condition: " << L_min << "+(" << FilterFunctionValues.size() << "-1)*" << L_userStep <<
      " < (" << L_max << "+" << epsilon << ") is not respected !" << "val1 " << L_min + static_cast<double>(FilterFunctionValues.size() - 1) * L_userStep << " val2 " << L_max - epsilon);
    }

  // Generate WavelengthSpectralBand if the step is not the offical 6S step value
  if (vcl_abs(L_userStep - SIXSStepOfWavelengthSpectralBandValues) > epsilon)
    {
    ValuesVectorType values(1, FilterFunctionValues[0]); //vector size 1 with the value vect[0]

    // Stop the interpolation at the max spectral value.
    value = SIXSStepOfWavelengthSpectralBandValues;
    while (L_min + value <= L_max)
      {
      // Search the User interval that surround the StepOfWavelengthSpectralBandValues current value.

      // removed the <= here, might be wrong
      while (j * L_userStep < value)
        {
        ++j;
        }

      // Check if we are not out of bound
      if (j >= FilterFunctionValues.size())
        {
        itkGenericExceptionMacro(
          << "Index " << j << " out of bound for FilterFunctionValues vector (size: " << FilterFunctionValues.size() <<
          ")." << " and value is " << value << " and SIXSStepOfWavelengthSpectralBandValues is " << SIXSStepOfWavelengthSpectralBandValues<< " and i is " << i);
        }

      double valueTemp;
      valueTemp = static_cast<double>(FilterFunctionValues[j - 1])
                  + ((static_cast<double>(FilterFunctionValues[j]) -
                      static_cast<double>(FilterFunctionValues[j - 1])) * invStep)
                  * (value - L_userStep * (j - 1));
      values.push_back(static_cast<WavelengthSpectralBandType>(valueTemp));

      ++i;
      value += SIXSStepOfWavelengthSpectralBandValues;
      }

    if (L_min + (i - 1) * SIXSStepOfWavelengthSpectralBandValues != L_max)
      {
      values.push_back(0);
      }
    // Store this values
    WavelengthSpectralBand->SetFilterFunctionValues6S(values);
    // Store the new Max MaxSpectralValue
    WavelengthSpectralBand->SetMaxSpectralValue(static_cast<WavelengthSpectralBandType>(L_min + i *
                                                                                        SIXSStepOfWavelengthSpectralBandValues));

    }
  else
    {
    // Init with copy of FilterFunctionValues input vector values
    WavelengthSpectralBand->SetFilterFunctionValues6S(FilterFunctionValues);
    }
}

void
SIXSTraits2::ComputeEnvironmentalContribution(const double diffuseTransmittanceForRayleighScattering,
                                             const double diffuseTransmittanceForAerosolScattering,
                                             const double radiusInKilometers,
                                             const double altitude,
                                             const double cosineOfViewingAngle,
                                             double& rayleighEstimation,
                                             double& aerosolEstimation,
                                             double& globalEstimation)
{
  otb_6s_doublereal difr(static_cast<otb_6s_doublereal>(diffuseTransmittanceForRayleighScattering));
  otb_6s_doublereal difa(static_cast<otb_6s_doublereal>(diffuseTransmittanceForAerosolScattering));
  otb_6s_doublereal rad(static_cast<otb_6s_doublereal>(radiusInKilometers));
  otb_6s_doublereal palt(static_cast<otb_6s_doublereal>(altitude));
  otb_6s_doublereal xmuv(static_cast<otb_6s_doublereal>(cosineOfViewingAngle));
  otb_6s_doublereal fra(0.), fae(0.), fr(0.);
  otb_6s_enviro_(&difr, &difa, &rad, &palt, &xmuv, &fra, &fae, &fr);
  rayleighEstimation = static_cast<double>(fra);
  aerosolEstimation = static_cast<double>(fae);
  globalEstimation = static_cast<double>(fr);
}

} // namespace otb
