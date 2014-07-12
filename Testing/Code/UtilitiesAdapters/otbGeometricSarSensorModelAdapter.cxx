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

int otbGeometricSarSensorModelAdapterNewTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef otb::GeometricSarSensorModelAdapter SarSensorModelType;
  SarSensorModelType::Pointer masterPlatform = SarSensorModelType::New();
  return EXIT_SUCCESS;
}

int otbGeometricSarSensorModelAdapterTest(int argc, char* argv[])
{
 if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " imageFile output" << std::endl;
    return EXIT_FAILURE;
    }

  typedef std::complex<double> PixelType;
  typedef otb::Image<PixelType,2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer image = ReaderType::New();

  image->SetFileName(argv[1]);

  image->UpdateOutputInformation();

  typedef otb::GeometricSarSensorModelAdapter SarSensorModelType;
  SarSensorModelType::Pointer imageSarModel = SarSensorModelType::New();

  imageSarModel->CreateSensorModel(image->GetOutput()->GetImageKeywordlist());

  std::vector<double> imagePosition;
  std::vector<double> imageSpeed;
  imagePosition.resize(3);
  imageSpeed.resize(3);


  imageSarModel->GetPlatformPositionAtLine(0, imagePosition, imageSpeed);

  std::ofstream file;
  file.open(argv[2]);

  file << std::setprecision(15);

  file << "Image:\n";
  //file << "Start Time: " << masterSarModel->getTime(0)<< " m\n";
  file << "Pos X: " << imagePosition[0] << " m\n";
  file << "Pos Y: " << imagePosition[1] << " m\n";
  file << "Pos Z: " << imagePosition[2] << " m\n";
  file << "Spd X: " << imageSpeed[0] << " m/s\n";
  file << "Spd Y: " << imageSpeed[1] << " m/s\n";
  file << "Spd Z: " << imageSpeed[2] << " m/s\n";
  file.close();

  return EXIT_SUCCESS;
}
