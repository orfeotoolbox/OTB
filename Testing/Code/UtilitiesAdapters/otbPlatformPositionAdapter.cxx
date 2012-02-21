/*=========================================================================

   Copyright 2011 Emmanuel Christophe
   Contributed to ORFEO Toolbox under license Apache 2

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

=========================================================================*/
#include <iomanip>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbPlatformPositionAdapter.h"

int otbPlatformPositionComputeBaselineNewTest(int argc, char* argv[])
{
  typedef otb::PlatformPositionAdapter PlatformType;
  PlatformType::Pointer masterPlatform = PlatformType::New();
  return EXIT_SUCCESS;
}

int otbPlatformPositionComputeBaselineTest(int argc, char* argv[])
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

  typedef otb::PlatformPositionAdapter PlatformType;
  PlatformType::Pointer masterPlatform = PlatformType::New();
  PlatformType::Pointer slavePlatform = PlatformType::New();

  masterPlatform->CreateSensorModel(master->GetOutput()->GetImageKeywordlist());
  slavePlatform->CreateSensorModel(slave->GetOutput()->GetImageKeywordlist());

  std::vector<double> masterPosition;
  std::vector<double> masterSpeed;
  masterPosition.resize(3);
  masterSpeed.resize(3);

  std::vector<double> slavePosition;
  std::vector<double> slaveSpeed;
  slavePosition.resize(3);
  slaveSpeed.resize(3);

  masterPlatform->GetPlatformPosition(0, masterPosition, masterSpeed);
  slavePlatform->GetPlatformPosition(0, slavePosition, slaveSpeed);

  std::ofstream file;
  file.open(argv[3]);

  file << std::setprecision(15);

  file << "Master image:\n";
  file << "Pos X: " << masterPosition[0] << " m\n";
  file << "Pos Y: " << masterPosition[1] << " m\n";
  file << "Pos Z: " << masterPosition[2] << " m\n";
  file << "Spd X: " << masterSpeed[0] << " m/s\n";
  file << "Spd Y: " << masterSpeed[1] << " m/s\n";
  file << "Spd Z: " << masterSpeed[2] << " m/s\n";

  file << "Slave image:\n";
  file << "Pos X: " << slavePosition[0] << " m\n";
  file << "Pos Y: " << slavePosition[1] << " m\n";
  file << "Pos Z: " << slavePosition[2] << " m\n";
  file << "Spd X: " << slaveSpeed[0] << " m/s\n";
  file << "Spd Y: " << slaveSpeed[1] << " m/s\n";
  file << "Spd Z: " << slaveSpeed[2] << " m/s\n";

  double baselineLength = vcl_sqrt(
      (masterPosition[0] - slavePosition[0]) * (masterPosition[0] - slavePosition[0]) +
      (masterPosition[1] - slavePosition[1]) * (masterPosition[1] - slavePosition[1]) +
      (masterPosition[2] - slavePosition[2]) * (masterPosition[2] - slavePosition[2]));
  file << "\nBaseline length:\n";
  file << baselineLength << " m \n";

  file.close();
  return EXIT_SUCCESS;
}
