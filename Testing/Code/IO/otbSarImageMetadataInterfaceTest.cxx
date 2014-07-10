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



#include "itkMacro.h"

#include <fstream>
#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbSarImageMetadataInterface.h"
#include "otbSarImageMetadataInterfaceFactory.h"

void printPointSet(otb::SarImageMetadataInterface::PointSetPointer pointSet, std::ostream& stream )
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
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef otb::VectorImage<double,  2>         InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  otb::SarImageMetadataInterface::Pointer lImageMetadata = otb::SarImageMetadataInterfaceFactory::CreateIMI(reader->GetOutput()->GetMetaDataDictionary());

  std::ofstream file;
  file.open(outputFilename);
  file << "GetRadiometricCalibrationScale                      : " << lImageMetadata->GetRadiometricCalibrationScale() << std::endl;

  file << "CalibrationNoise : " << std::endl;
  file << "Polynomial degree : " << lImageMetadata->GetRadiometricCalibrationNoisePolynomialDegree() << std::endl;
  printPointSet(lImageMetadata->GetRadiometricCalibrationNoise(), file);

  file << "CalibrationAntennaPatternNewGain : " << std::endl;
  file << "Polynomial degree : " << lImageMetadata->GetRadiometricCalibrationAntennaPatternNewGainPolynomialDegree() << std::endl;
  printPointSet(lImageMetadata->GetRadiometricCalibrationAntennaPatternNewGain(), file);

  file << "CalibrationAntennaPatternOldGain : " << std::endl;
  file << "Polynomial degree : " << lImageMetadata->GetRadiometricCalibrationAntennaPatternOldGainPolynomialDegree() << std::endl;
  printPointSet(lImageMetadata->GetRadiometricCalibrationAntennaPatternOldGain(), file);

  file << "CalibrationIncidenceAngle : " << std::endl;
  file << "Polynomial degree : " << lImageMetadata->GetRadiometricCalibrationIncidenceAnglePolynomialDegree() << std::endl;
  printPointSet(lImageMetadata->GetRadiometricCalibrationIncidenceAngle(), file);

  file << "CalibrationRangeSpreadLoss : " << std::endl;
  file << "Polynomial degree : " << lImageMetadata->GetRadiometricCalibrationRangeSpreadLossPolynomialDegree() << std::endl;
  printPointSet(lImageMetadata->GetRadiometricCalibrationRangeSpreadLoss(), file);

  file.close();

  std::cout << lImageMetadata << std::endl;

  return EXIT_SUCCESS;

}
