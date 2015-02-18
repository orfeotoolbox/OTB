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

#include "otbSpectralSensitivityReader.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include "itksys/SystemTools.hxx"

#include "otbSpotImageMetadataInterface.h"
#include "itkMacro.h"

namespace otb
{
SpectralSensitivityReader
::SpectralSensitivityReader() :
  m_FileName(""),
  m_DataPath("")
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0, WavelengthSpectralBandVectorType::New().GetPointer());
  m_Image = ImageType::New();
}

SpectralSensitivityReader
::~SpectralSensitivityReader()
{
}

SpectralSensitivityReader::WavelengthSpectralBandVectorType *
SpectralSensitivityReader
::GetOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<WavelengthSpectralBandVectorType *> (this->ProcessObject::GetOutput(0));
}

const SpectralSensitivityReader::WavelengthSpectralBandVectorType *
SpectralSensitivityReader
::GetOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const WavelengthSpectralBandVectorType *> (this->ProcessObject::GetOutput(0));
}

void
SpectralSensitivityReader
::FindFileName()
{
  if (m_Image.IsNull())
    {
    itkExceptionMacro(<< "An input image has to be set or set directly the filename");
    }

  std::string        sensor("");
  std::ostringstream oss;

  try
    {
    SpotImageMetadataInterface::Pointer lImageMetadata = SpotImageMetadataInterface::New();
    lImageMetadata->SetMetaDataDictionary(m_Image->GetMetaDataDictionary());
    sensor = lImageMetadata->GetSensorID();
    sensor = itksys::SystemTools::UpperCase(sensor);
    // Suppress spaces
    sensor.erase(std::remove_if(sensor.begin(), sensor.end(), isspace), sensor.end());

    oss.str("");
    oss << lImageMetadata->GetInstrument();
    oss << lImageMetadata->GetInstrumentIndex();
    }
  catch (itk::ExceptionObject& err)
    {
    itkExceptionMacro(<< "Invalid input image" << err.GetDescription());
    }

  std::string instrument = oss.str();

  oss.str("");
  oss << m_DataPath;
  oss << "/";
  oss << sensor;
  oss << "/";
  oss << instrument;
  oss << "/rep6S.dat";

  m_FileName = oss.str();
}

void
SpectralSensitivityReader
::GenerateData()
{
  if (m_FileName == "") this->FindFileName();

  WavelengthSpectralBandVectorType * wavelengthSpectralBand = this->GetOutput();

  if ( !itksys::SystemTools::FileExists(m_FileName.c_str()) )
    {
    itkExceptionMacro(<< m_FileName << " does not exist.");
    }

  std::ifstream file(m_FileName.c_str());
  if (!file) itkExceptionMacro(<< "Enable to read " <<  m_FileName << " file.");

  std::string line;
  double      mini = 0;
  double      maxi = 0.;
  bool        firstLine = true;

  unsigned int nbBands = 0;
  // used to store the coef
  std::vector<ValuesVectorType> valuesVector;

  while (std::getline(file, line))
    {
    // Replace any tabulation by a space
    std::string::size_type tabpos = line.find('\t');
    while (tabpos != std::string::npos)
      {
      line.at(tabpos) = ' ';
      tabpos = line.find('\t');
      }

    // Replace multiple spaces by a unique space
    std::unique(line.begin(), line.end(), SpectralSensitivityReader::BothAre(' '));

    // if the first character is a space, erase it
    boost::trim(line);
    std::vector<std::string> keywordStrings;
    boost::split(keywordStrings, line, boost::is_any_of(" "));

    if (keywordStrings.size() < 3) itkExceptionMacro(<< "Invalid file format");

    // Store min wavelength
    if (firstLine)
      {
      mini = atof(keywordStrings[0].c_str());
      nbBands = keywordStrings.size() - 2;

      for (unsigned int j = 0; j < nbBands; ++j)
        {
        wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
        ValuesVectorType temp;
        valuesVector.push_back(temp);
        }
      firstLine = false;
      }

    if (nbBands != keywordStrings.size() - 2) itkExceptionMacro(<< "Invalid file format");

    for (unsigned int i = 0; i < nbBands; ++i)
      {
      valuesVector[i].push_back(atof(keywordStrings[i + 2].c_str()));
      }

    maxi = atof(keywordStrings[0].c_str());
    } //while ( std::getline( file, line ) )

  for (unsigned int j = 0; j < nbBands; ++j)
    {
    wavelengthSpectralBand->GetNthElement(j)->SetFilterFunctionValues(valuesVector[j]);
    wavelengthSpectralBand->GetNthElement(j)->SetMinSpectralValue(mini);
    wavelengthSpectralBand->GetNthElement(j)->SetMaxSpectralValue(maxi);
    }
}

/**PrintSelf method */
void
SpectralSensitivityReader
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << "DataPath: " << m_DataPath << std::endl;
  if (m_FileName != "") os << "FileName: " << m_FileName << std::endl;

  // Function values print :
  const WavelengthSpectralBandVectorType * wavelengthSpectralBand = this->GetOutput();
  os << "Filter function values: " << std::endl;
  for (unsigned int i = 0; i < wavelengthSpectralBand->Size(); ++i)
    {
    os << indent << "Channel " << i + 1 << " : " << std::endl;
    os << indent << wavelengthSpectralBand->GetNthElement(i) << std::endl;
    }
}

} // end namespace otb
