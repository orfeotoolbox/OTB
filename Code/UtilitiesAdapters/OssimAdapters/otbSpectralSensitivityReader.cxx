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

#include "base/ossimFilename.h"

#include "otbSpotImageMetadataInterface.h"
#include "itkExceptionObject.h"

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

  ossimString        sensor("");
  itk::OStringStream oss;

  try
    {
    SpotImageMetadataInterface::Pointer lImageMetadata = SpotImageMetadataInterface::New();
    lImageMetadata->SetMetaDataDictionary(m_Image->GetMetaDataDictionary());
    sensor = ossimString(lImageMetadata->GetSensorID());
    sensor.upcase();
    // Suppress spaces
    for (unsigned int i = 0; i < sensor.size() - 1; i++)
      {
      if (sensor.compare(i, 1, " ") == 0)
        {
        sensor.erase(i, 1);
        i--;
        }
      }

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

  ossimFilename fname(m_FileName);
  if (!fname.exists()) itkExceptionMacro(<< m_FileName << " does not exist.");

  std::ifstream file(fname.c_str());
  if (!file) itkExceptionMacro(<< "Enable to read " << fname << " file.");

  std::string line;
  ossimString separator = " ";
  double      mini = 0;
  double      maxi = 0.;
  bool        firstLine = true;

  unsigned int nbBands = 0;
  // used to store the coef
  std::vector<ValuesVectorType> valuesVector;

  while (std::getline(file, line))
    {
    ossimString osLine(line);

    // Suppress multiple spaces
    for (unsigned int i = 0; i < osLine.size() - 1; i++)
      {
      if (osLine.compare(i, 1, " ") == 0 && osLine.compare(i + 1, 1, " ") == 0)
        {
        osLine.erase(i + 1, 1);
        i--;
        }
      }
    // if the first character is a space, erase it
    if (osLine.compare(0, 1, " ") == 0) osLine.erase(0, 1);

    std::vector<ossimString> keywordStrings = osLine.split(separator);

    if (keywordStrings.size() < 3) itkExceptionMacro(<< "Invalid file format");

    // Store min wavelength
    if (firstLine)
      {
      mini = keywordStrings[0].toDouble();
      nbBands = keywordStrings.size() - 2;
      for (unsigned int j = 0; j < nbBands; j++)
        {
        wavelengthSpectralBand->PushBack(FilterFunctionValues::New());
        ValuesVectorType temp;
        valuesVector.push_back(temp);
        }
      firstLine = false;
      }

    if (nbBands != keywordStrings.size() - 2) itkExceptionMacro(<< "Invalid file format");

    for (unsigned int i = 0; i < nbBands; i++)
      {
      valuesVector[i].push_back(keywordStrings[i + 2].toDouble());
      }

    maxi = keywordStrings[0].toDouble();
    } //while ( std::getline( file, line ) )

  for (unsigned int j = 0; j < nbBands; j++)
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
