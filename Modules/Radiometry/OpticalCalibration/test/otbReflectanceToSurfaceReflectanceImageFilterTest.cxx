/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include <fstream>
#include <iostream>

#include "otbReflectanceToSurfaceReflectanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbOpticalImageMetadataInterfaceFactory.h"


int otbReflectanceToSurfaceReflectanceImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  const char* inputFileName  = argv[1];
  const char* outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<InputImageType, OutputImageType> ReflectanceToSurfaceReflectanceImageFilterType;

  typedef otb::AtmosphericRadiativeTerms::DataVectorType DataVectorType;
  otb::AtmosphericRadiativeTerms::Pointer                atmoRadTerms = otb::AtmosphericRadiativeTerms::New();


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();
  unsigned int nbChannel = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  DataVectorType intrinsic;
  DataVectorType albedo;
  DataVectorType gaseous;
  DataVectorType downTrans;
  DataVectorType upTrans;

  for (unsigned int j = 0; j < nbChannel; ++j)
  {
    intrinsic.push_back(static_cast<double>(atof(argv[3 + j])));
    albedo.push_back(static_cast<double>(atof(argv[3 + j + nbChannel])));
    gaseous.push_back(static_cast<double>(atof(argv[3 + j + 2 * nbChannel])));
    downTrans.push_back(static_cast<double>(atof(argv[3 + j + 3 * nbChannel])));
    upTrans.push_back(static_cast<double>(atof(argv[3 + j + 4 * nbChannel])));
  }

  atmoRadTerms->SetIntrinsicAtmosphericReflectances(intrinsic);
  atmoRadTerms->SetSphericalAlbedos(albedo);
  atmoRadTerms->SetTotalGaseousTransmissions(gaseous);
  atmoRadTerms->SetDownwardTransmittances(downTrans);
  atmoRadTerms->SetUpwardTransmittances(upTrans);

  // Instantiating object
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer filter = ReflectanceToSurfaceReflectanceImageFilterType::New();
  filter->SetAtmosphericRadiativeTerms(atmoRadTerms);
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}

// Check the correct generation of the atmospheric parameters
int otbReflectanceToSurfaceReflectanceImageFilterTest2(int itkNotUsed(argc), char* argv[])
{
  const char* inputFileName  = argv[1];
  const char* outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double     PixelType;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ReflectanceToSurfaceReflectanceImageFilter<InputImageType, OutputImageType> ReflectanceToSurfaceReflectanceImageFilterType;

  typedef otb::AtmosphericCorrectionParameters           AtmoCorrectionParametersType;
  typedef AtmoCorrectionParametersType::AerosolModelType AerosolModelType;
  AtmoCorrectionParametersType::Pointer                  paramAtmo = AtmoCorrectionParametersType::New();


  paramAtmo->SetAtmosphericPressure(static_cast<double>(atof(argv[3])));
  paramAtmo->SetWaterVaporAmount(static_cast<double>(atof(argv[4])));
  paramAtmo->SetOzoneAmount(static_cast<double>(atof(argv[5])));
  paramAtmo->SetAerosolModel(static_cast<AerosolModelType>(atoi(argv[6])));
  paramAtmo->SetAerosolOptical(static_cast<double>(atof(argv[7])));


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  reader->UpdateOutputInformation();

  // Instantiating object
  ReflectanceToSurfaceReflectanceImageFilterType::Pointer filter = ReflectanceToSurfaceReflectanceImageFilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetAtmoCorrectionParameters(paramAtmo);
  writer->SetInput(filter->GetOutput());

  writer->Update();

  /*filter->GenerateParameters();
  otb::AtmosphericRadiativeTerms::Pointer terms = filter->GetAtmosphericRadiativeTerms();

  std::ofstream fout (outputFileName);
  fout << terms;
  fout.close(); */

  return EXIT_SUCCESS;
}
