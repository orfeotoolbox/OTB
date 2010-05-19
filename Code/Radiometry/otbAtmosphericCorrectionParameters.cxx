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

#include "otbAtmosphericCorrectionParameters.h"

#include "otbAeronetFileReader.h"
#include "otbSpectralSensitivityReader.h"
#include "base/ossimFilename.h"
#include <fstream>

namespace otb
{
/***********************      FilterFunctionValues **************************/
FilterFunctionValues
::FilterFunctionValues()
{
  m_MinSpectralValue = 0;
  m_MaxSpectralValue = 1;
  // Fill with 1. Size 3 for 6S interpolation
  m_FilterFunctionValues = ValuesVectorType(3, 1.);
  m_UserStep = 0.0025;
}

/**PrintSelf method */
void
FilterFunctionValues
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "Minimum spectral value       : " << m_MinSpectralValue << std::endl;
  os << indent << "Maximum spectral value       : " << m_MaxSpectralValue << std::endl;
  os << indent << "Wavelength spectral band step: " << m_UserStep << std::endl;
  os << indent << "Filter function values: " << std::endl;
  for (unsigned int i = 0; i < m_FilterFunctionValues.size(); ++i)
    {
    os << indent << m_FilterFunctionValues[i] << std::endl;
    }
  os << indent << "6S Filter function values: " << std::endl;
  for (unsigned int i = 0; i < m_FilterFunctionValues6S.size(); ++i)
    {
    os << indent << m_FilterFunctionValues6S[i] << std::endl;
    }
}

/***********************      AtmosphericCorrectionParameters **************************/
/**
 * Constructor
 */

AtmosphericCorrectionParameters
::AtmosphericCorrectionParameters()
{
  m_SolarZenithalAngle   = 361.;
  m_SolarAzimutalAngle   = 361.;
  m_ViewingZenithalAngle = 361.;
  m_ViewingAzimutalAngle = 361.;
  m_Month                = 0;
  m_Day                  = 0;
  m_AtmosphericPressure  = 1030.;
  m_WaterVaporAmount     = 2.5;
  m_OzoneAmount          = 0.28;
  m_AerosolModel         = CONTINENTAL;
  m_AerosolOptical       = 0.2;

  m_WavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();
  m_WavelengthSpectralBand->Clear();

}

/** Get data from aeronet file*/
void
AtmosphericCorrectionParameters
::UpdateAeronetData(std::string file, int year, int month, int day, int hour, int minute, double epsi)
{
  if (file == "") itkExceptionMacro(<< "No Aeronet filename specified.");

  AeronetFileReader::Pointer reader = AeronetFileReader::New();
  reader->SetFileName(file);
  reader->SetDay(day);
  reader->SetMonth(month);
  reader->SetYear(year);
  reader->SetHour(hour);
  reader->SetMinute(minute);
  reader->SetEpsilon(epsi);

  reader->Update();

  m_AerosolOptical = reader->GetOutput()->GetAerosolOpticalThickness();
  m_WaterVaporAmount = reader->GetOutput()->GetWater();
}

/** Get data from filter function file*/
void
AtmosphericCorrectionParameters
::LoadFilterFunctionValue(std::string filename)
{
  m_WavelengthSpectralBand->Clear();
  SpectralSensitivityReader::Pointer spectralSensitivityReader = SpectralSensitivityReader::New();
  spectralSensitivityReader->SetFileName(filename);
  spectralSensitivityReader->Update();
  m_WavelengthSpectralBand = spectralSensitivityReader->GetOutput();
}

/**PrintSelf method */
void
AtmosphericCorrectionParameters
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << "Solar zenithal angle  : " << m_SolarZenithalAngle << std::endl;
  os << "Solar azimutal angle  : " << m_SolarAzimutalAngle << std::endl;
  os << "Viewing zenithal angle: " << m_ViewingZenithalAngle << std::endl;
  os << "Viewing azimutal angle: " << m_ViewingAzimutalAngle << std::endl;
  os << "Month                 : " << m_Month << std::endl;
  os << "Day                   : " << m_Day << std::endl;
  os << "Atmospheric pressure  : " << m_AtmosphericPressure << std::endl;
  os << "Water vapor amount    : " << m_WaterVaporAmount << std::endl;
  os << "Ozone amount          : " << m_OzoneAmount << std::endl;
  os << "Aerosol model         : " << m_AerosolModel << std::endl;
  os << "Aerosol optical       : " << m_AerosolOptical << std::endl;

  // Function values print :
  os << "Filter function values: " << std::endl;
  for (unsigned int i = 0; i < m_WavelengthSpectralBand->Size(); ++i)
    {
    os << indent << "Channel " << i + 1 << " : " << std::endl;
    os << indent << m_WavelengthSpectralBand->GetNthElement(i) << std::endl;
    }
}
} // end namespace otb
