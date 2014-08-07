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

#include "otbImageMetadataCorrectionParameters.h"
#include "otbSpectralSensitivityReader.h"

namespace otb
{


/***********************      ImageMetadataCorrectionParameters **************************/
/**
 * Constructor
 */

ImageMetadataCorrectionParameters
::ImageMetadataCorrectionParameters()
{
  m_SolarZenithalAngle   = 361.;
  m_SolarAzimutalAngle   = 361.;
  m_ViewingZenithalAngle = 361.;
  m_ViewingAzimutalAngle = 361.;
  m_Month                = 0;
  m_Day                  = 0;
  m_Year                 = 0;
  m_FilterFunctionValuesFileName = "";
  
  m_WavelengthSpectralBand = InternalWavelengthSpectralBandVectorType::New();
  m_WavelengthSpectralBand->Clear();
}


/** Get data from filter function file*/
void
ImageMetadataCorrectionParameters
::LoadFilterFunctionValue(const std::string& filename)
{
  m_WavelengthSpectralBand->Clear();
  SpectralSensitivityReader::Pointer spectralSensitivityReader = SpectralSensitivityReader::New();
  spectralSensitivityReader->SetFileName(filename);
  spectralSensitivityReader->Update();
  m_WavelengthSpectralBand = spectralSensitivityReader->GetOutput();
}

/**PrintSelf method */
void
ImageMetadataCorrectionParameters
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << "Solar zenithal angle  : " << m_SolarZenithalAngle << std::endl;
  os << "Solar azimutal angle  : " << m_SolarAzimutalAngle << std::endl;
  os << "Viewing zenithal angle: " << m_ViewingZenithalAngle << std::endl;
  os << "Viewing azimutal angle: " << m_ViewingAzimutalAngle << std::endl;
  os << "Month                 : " << m_Month << std::endl;
  os << "Day                   : " << m_Day << std::endl;
  os << "Year                  : " << m_Year << std::endl;

  // Function values print :
  os << "Filter function values: " << std::endl;
  for (unsigned int i = 0; i < m_WavelengthSpectralBand->Size(); ++i)
    {
    os << indent << "Channel " << i + 1 << " : " << std::endl;
    os << indent << m_WavelengthSpectralBand->GetNthElement(i) << std::endl;
    }
}
} // end namespace otb
