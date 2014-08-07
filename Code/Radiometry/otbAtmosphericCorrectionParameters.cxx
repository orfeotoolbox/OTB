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

#include <fstream>

#include "otbAeronetFileReader.h"
#include "otbSpectralSensitivityReader.h"
#include "otbAeronetData.h"

namespace otb
{


AtmosphericCorrectionParameters
::AtmosphericCorrectionParameters()
{

  m_AtmosphericPressure  = 1030.;
  m_WaterVaporAmount     = 2.5;
  m_OzoneAmount          = 0.28;
  m_AerosolModel         = CONTINENTAL;
  m_AerosolOptical       = 0.2;
  m_AeronetFileName      = "";
  m_Day                  = 1;
  m_Month                = 1;
 
}

/** Get data from aeronet file*/
void
AtmosphericCorrectionParameters
::UpdateAeronetData(const std::string& file, int year, int month, int day, int hour, int minute, double epsi)
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



/**PrintSelf method */
void
AtmosphericCorrectionParameters
::PrintSelf(std::ostream& os, itk::Indent indent) const
{

  os << "Atmospheric pressure  : " << m_AtmosphericPressure << std::endl;
  os << "Water vapor amount    : " << m_WaterVaporAmount << std::endl;
  os << "Ozone amount          : " << m_OzoneAmount << std::endl;
  os << "Aerosol model         : " << m_AerosolModel << std::endl;
  os << "Aerosol optical       : " << m_AerosolOptical << std::endl;

}
} // end namespace otb
