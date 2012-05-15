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

#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbGeometricSarSensorModelAdapter.h"

int otbGeometricSarSensorModelAdapterNewTest(int argc, char* argv[])
{
  typedef otb::GeometricSarSensorModelAdapter SarSensorModelType;
  SarSensorModelType::Pointer masterPlatform = SarSensorModelType::New();
  return EXIT_SUCCESS;
}

int otbGeometricSarSensorModelAdapterTest(int argc, char* argv[])
{
 if (argc != 4)
    {
    std::cerr << "Usage: " << argv[0] << " masterImageFile slaveImageFile output" << std::endl;
    return EXIT_FAILURE;
    }

  typedef std::complex<double> PixelType;
  typedef otb::Image<PixelType,2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer master = ReaderType::New();
  ReaderType::Pointer slave = ReaderType::New();

  master->SetFileName(argv[1]);
  slave->SetFileName(argv[2]);

  master->UpdateOutputInformation();
  slave->UpdateOutputInformation();

  typedef otb::GeometricSarSensorModelAdapter SarSensorModelType;
  SarSensorModelType::Pointer masterSarModel = SarSensorModelType::New();
  SarSensorModelType::Pointer slaveSarModel = SarSensorModelType::New();

  masterSarModel->CreateSensorModel(master->GetOutput()->GetImageKeywordlist());
  slaveSarModel->CreateSensorModel(slave->GetOutput()->GetImageKeywordlist());

  std::vector<double> masterPosition;
  std::vector<double> masterSpeed;
  masterPosition.resize(3);
  masterSpeed.resize(3);

  std::vector<double> slavePosition;
  std::vector<double> slaveSpeed;
  slavePosition.resize(3);
  slaveSpeed.resize(3);

  masterSarModel->GetPlatformPositionAtLine(0, masterPosition, masterSpeed);
  slaveSarModel->GetPlatformPositionAtLine(0, slavePosition, slaveSpeed);

  std::ofstream file;
  file.open(argv[3]);

  file << std::setprecision(15);

  file << "Master image:\n";
  //file << "Start Time: " << masterSarModel->getTime(0)<< " m\n";
  file << "Pos X: " << masterPosition[0] << " m\n";
  file << "Pos Y: " << masterPosition[1] << " m\n";
  file << "Pos Z: " << masterPosition[2] << " m\n";
  file << "Spd X: " << masterSpeed[0] << " m/s\n";
  file << "Spd Y: " << masterSpeed[1] << " m/s\n";
  file << "Spd Z: " << masterSpeed[2] << " m/s\n";

  file << "Slave image:\n";
  //file << "Start Time: " << slaveSarModel->getTime(0)<< " m\n";
  file << "Pos X: " << slavePosition[0] << " m\n";
  file << "Pos Y: " << slavePosition[1] << " m\n";
  file << "Pos Z: " << slavePosition[2] << " m\n";
  file << "Spd X: " << slaveSpeed[0] << " m/s\n";
  file << "Spd Y: " << slaveSpeed[1] << " m/s\n";
  file << "Spd Z: " << slaveSpeed[2] << " m/s\n";

  file.close();
  return EXIT_SUCCESS;
}
