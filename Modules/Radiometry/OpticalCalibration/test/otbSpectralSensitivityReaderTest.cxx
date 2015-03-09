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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

using namespace otb;

int otbSpectralSensitivityReaderNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{

  SpectralSensitivityReader::Pointer spectSen = SpectralSensitivityReader::New();

  std::cout << spectSen << std::endl;

  return EXIT_SUCCESS;
}

int otbSpectralSensitivityReaderTest(int itkNotUsed(argc), char* argv[])
{
  const char * dataPath  = argv[1];
  const char * imageName = argv[2];
  const char * output    = argv[3];

  typedef VectorImage<double>        ImageType;
  typedef ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageName);
  reader->UpdateOutputInformation();

  SpectralSensitivityReader::Pointer spectSen = SpectralSensitivityReader::New();

  spectSen->SetDataPath(dataPath);
  spectSen->SetImage(reader->GetOutput());

  spectSen->Update();

  std::ofstream fout(output);
  fout << spectSen << std::endl;
  fout.close();

  return EXIT_SUCCESS;
}

int otbSpectralSensitivityReaderGenericTest(int itkNotUsed(argc), char* argv[])
{
  const char * sensitivityFileName = argv[1];
  const char * imageName = argv[2];
  const char * output    = argv[3];

  typedef VectorImage<double>        ImageType;
  typedef ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageName);
  reader->UpdateOutputInformation();

  SpectralSensitivityReader::Pointer spectSen = SpectralSensitivityReader::New();

  spectSen->SetFileName(sensitivityFileName);
//   std::cout << "test : " << "filemane " << spectSen->GetFileName() <<std::endl;
  spectSen->SetImage(reader->GetOutput());

  spectSen->Update();

  std::ofstream fout(output);
  fout << spectSen << std::endl;
  fout.close();

  return EXIT_SUCCESS;
}
