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


#include "otbAtmosphericCorrectionParameters.h"

#include <fstream>

#include "otbAeronetFileReader.h"
#include "otbSpectralSensitivityReader.h"
#include "otbAeronetData.h"

namespace otb
{


AtmosphericCorrectionParameters::AtmosphericCorrectionParameters()
{

  m_AtmosphericPressure = 1030.;
  m_WaterVaporAmount    = 2.5;
  m_OzoneAmount         = 0.28;
  m_AerosolModel        = CONTINENTAL;
  m_AerosolOptical      = 0.2;
  m_AeronetFileName     = "";
  m_Day                 = 1;
  m_Month               = 1;
}

/** Get data from aeronet file*/
void AtmosphericCorrectionParameters::UpdateAeronetData(const std::string& file, int year, int month, int day, int hour, int minute, double epsi)
{
  if (file == "")
    itkExceptionMacro(<< "No Aeronet filename specified.");

  AeronetFileReader::Pointer reader = AeronetFileReader::New();
  reader->SetFileName(file);
  reader->SetDay(day);
  reader->SetMonth(month);
  reader->SetYear(year);
  reader->SetHour(hour);
  reader->SetMinute(minute);
  reader->SetEpsilon(epsi);

  reader->Update();

  m_AerosolOptical   = reader->GetOutput()->GetAerosolOpticalThickness();
  m_WaterVaporAmount = reader->GetOutput()->GetWater();
}


/**PrintSelf method */
void AtmosphericCorrectionParameters::PrintSelf(std::ostream& os, itk::Indent indent) const
{

  os << indent << "Atmospheric pressure  : " << m_AtmosphericPressure << std::endl;
  os << indent << "Water vapor amount    : " << m_WaterVaporAmount << std::endl;
  os << indent << "Ozone amount          : " << m_OzoneAmount << std::endl;
  os << indent << "Aerosol model         : " << m_AerosolModel << std::endl;
  os << indent << "Aerosol optical       : " << m_AerosolOptical << std::endl;
}
} // end namespace otb
