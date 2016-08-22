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
#ifndef otbSIXSTraits_h
#define otbSIXSTraits_h

#include "otbAtmosphericCorrectionParameters.h"

namespace otb
{

/** \class SIXSTraits
   * \brief SIXSTraits operations.
   *
   * Call 6S main function. The main method call 6S to calculate atmospheric correction parameters.
   * It use by the OTB Atmospheric correction framework.
   *
   * \ingroup Radiometry
   *
 *
 * \ingroup OTBOpticalCalibration
 */
class ITK_EXPORT SIXSTraits
{
public:

  /** Standard class typedefs. */
  typedef SIXSTraits Self;

  typedef FilterFunctionValues                               WavelengthSpectralType;
  typedef AtmosphericCorrectionParameters::AerosolModelType  AerosolModelType;
  typedef WavelengthSpectralType::WavelengthSpectralBandType WavelengthSpectralBandType;
  typedef WavelengthSpectralType::ValuesVectorType           ValuesVectorType;

  /** Call 6S main function */
  static void ComputeAtmosphericParameters(
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
    double&                UpwardDiffuseTransmittanceForAerosol     /** upward diffuse transmittance for aerosols */
    );

  /**
  * Check the correpondance between the vector value size and
  * the interval number between min and max.
  * If the vector step is not at 0.0025, the new values are computed.
  * The output vector values is store in the m_FilterFunctionValues6S
  * of WavelengthSpectralBand
  *
  */
  static void ComputeWavelengthSpectralBandValuesFor6S(
    const double SIXSStepOfWavelengthSpectralBandValues,
    WavelengthSpectralType* WavelengthSpectralBand
    );

  static void ComputeEnvironmentalContribution(const double diffuseTransmittanceForRayleighScattering,
                                               const double diffuseTransmittanceForAerosolScattering,
                                               const double radiusInKilometers,
                                               const double altitude,
                                               const double cosineOfViewingAngle,
                                               double& rayleighEstimation,
                                               double& aerosolEstimation,
                                               double& globalEstimation);
};

} // namespace otb

#endif
