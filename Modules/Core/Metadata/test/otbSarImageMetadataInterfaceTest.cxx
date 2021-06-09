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


#include "itkMacro.h"

#include <fstream>
#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbSarImageMetadataInterfaceFactory.h"

void printPointSet(otb::SarImageMetadataInterface::PointSetPointer pointSet, std::ostream& stream)
{
  stream << pointSet << std::endl;
  if (pointSet->GetNumberOfPoints() < 20)
  {
    stream << "PointSet values :" << std::endl;
    otb::SarImageMetadataInterface::PointSetType::PointType point;
    otb::SarImageMetadataInterface::PointSetType::PixelType pointValue;
    for (unsigned int i = 0; i < pointSet->GetNumberOfPoints(); ++i)
    {
      pointSet->GetPoint(i, &point);
      pointSet->GetPointData(i, &pointValue);
      stream << point << " : " << pointValue << std::endl;
    }
  }
}

int otbSarImageMetadataInterfaceTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  typedef otb::VectorImage<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();
// TODO Use new framework !
  otb::SarImageMetadataInterface::Pointer lImageMetadata  = otb::SarImageMetadataInterfaceFactory::CreateIMI(reader->GetOutput()->GetMetaDataDictionary());

  std::ofstream file;
  file.open(outputFilename);
  file << "GetRadiometricCalibrationScale                      : " << lImageMetadata->GetRadiometricCalibrationScale() << "\n"
       << "CalibrationNoise : " << "\n"
       << "Polynomial degree : ";
  for(const auto& s: lImageMetadata->GetRadiometricCalibrationNoisePolynomialDegree())
    file << s << " ";
  //printPointSet(lImageMetadata->GetRadiometricCalibrationNoise(lImageMetadata), file);

  file << "\n"
       << "CalibrationAntennaPatternNewGain : " << "\n"
       << "Polynomial degree : ";
  for(const auto& s: lImageMetadata->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree())
    file << s << " ";
  printPointSet(lImageMetadata->GetRadiometricCalibrationAntennaPatternNewGain(), file);

  file << "\n"
       << "CalibrationAntennaPatternOldGain : " << "\n"
       << "Polynomial degree : ";
  for(const auto& s: lImageMetadata->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree())
    file << s << " ";
  printPointSet(lImageMetadata->GetRadiometricCalibrationAntennaPatternOldGain(), file);

  file << "\n"
       << "CalibrationIncidenceAngle : " << "\n"
       << "Polynomial degree : ";
  for(const auto& s: lImageMetadata->GetRadiometricCalibrationIncidenceAnglePolynomialDegree())
    file << s << " ";
  //printPointSet(lImageMetadata->GetRadiometricCalibrationIncidenceAngle(lImageMetadata), file);

  file << "\n"
       << "CalibrationRangeSpreadLoss : " << "\n"
       << "Polynomial degree : ";
  for(const auto& s: lImageMetadata->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree())
    file << s << " ";
  printPointSet(lImageMetadata->GetRadiometricCalibrationRangeSpreadLoss(), file);

  file.close();

  std::cout << lImageMetadata << std::endl;

  return EXIT_SUCCESS;
}
