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

#include "otbSOMModel.h"
#include "otbReadDataFile.h"

typedef otb::SOMModel<double, 2> SOMModel2D;
typedef otb::SOMModel<double, 3> SOMModel3D;
typedef otb::SOMModel<double, 4> SOMModel4D;
typedef otb::SOMModel<double, 5> SOMModel5D;

typedef SOMModel2D::InputListSampleType  InputListSampleType;
typedef SOMModel2D::TargetListSampleType TargetListSampleType;


int otbSOMModelCanRead(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <model2D> <model3D> <model4D> <model5D>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string filename2D(argv[1]);
  std::string filename3D(argv[2]);
  std::string filename4D(argv[3]);
  std::string filename5D(argv[4]);

  SOMModel2D::Pointer model2D = SOMModel2D::New();
  SOMModel3D::Pointer model3D = SOMModel3D::New();
  SOMModel4D::Pointer model4D = SOMModel4D::New();
  SOMModel5D::Pointer model5D = SOMModel5D::New();

  if (!model2D->CanReadFile(filename2D))
  {
    std::cerr << "Failed to read model file : " << filename2D << std::endl;
    return EXIT_FAILURE;
  }
  if (!model3D->CanReadFile(filename3D))
  {
    std::cerr << "Failed to read model file : " << filename3D << std::endl;
    return EXIT_FAILURE;
  }
  if (!model4D->CanReadFile(filename4D))
  {
    std::cerr << "Failed to read model file : " << filename4D << std::endl;
    return EXIT_FAILURE;
  }
  if (!model5D->CanReadFile(filename5D))
  {
    std::cerr << "Failed to read model file : " << filename5D << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int otbSOMModeTrain(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " letter.scale  model2D.out model3D.out model4D.out model5D.out" << std::endl;
    return EXIT_FAILURE;
  }

  // Extract data from letter.scale
  InputListSampleType::Pointer  samples = InputListSampleType::New();
  TargetListSampleType::Pointer target  = TargetListSampleType::New();
  if (!otb::ReadDataFile(argv[1], samples, target))
  {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  SOMModel2D::Pointer model2D = SOMModel2D::New();
  SOMModel3D::Pointer model3D = SOMModel3D::New();
  SOMModel4D::Pointer model4D = SOMModel4D::New();
  SOMModel5D::Pointer model5D = SOMModel5D::New();

  SOMModel2D::SizeType size2D, radius2D;
  size2D.Fill(10);
  radius2D.Fill(3);
  SOMModel3D::SizeType size3D, radius3D;
  size3D.Fill(6);
  radius3D.Fill(3);
  SOMModel4D::SizeType size4D, radius4D;
  size4D.Fill(4);
  radius4D.Fill(2);
  SOMModel5D::SizeType size5D, radius5D;
  size5D.Fill(3);
  radius5D.Fill(2);

  std::cout << "Train 2D model..." << std::endl;
  model2D->SetNumberOfIterations(10);
  model2D->SetBetaInit(1.0);
  model2D->SetWriteMap(true);
  model2D->SetBetaEnd(0.1);
  model2D->SetMaxWeight(10.0);
  model2D->SetMapSize(size2D);
  model2D->SetNeighborhoodSizeInit(radius2D);
  model2D->SetInputListSample(samples);
  model2D->Train();
  model2D->Save(std::string(argv[2]));

  std::cout << "Train 3D model..." << std::endl;
  model3D->SetNumberOfIterations(10);
  model3D->SetBetaInit(1.0);
  model3D->SetWriteMap(true);
  model3D->SetBetaEnd(0.1);
  model3D->SetMaxWeight(10.0);
  model3D->SetMapSize(size3D);
  model3D->SetNeighborhoodSizeInit(radius3D);
  model3D->SetInputListSample(samples);
  model3D->Train();
  model3D->Save(std::string(argv[3]));

  std::cout << "Train 4D model..." << std::endl;
  model4D->SetNumberOfIterations(10);
  model4D->SetBetaInit(1.0);
  model4D->SetWriteMap(true);
  model4D->SetBetaEnd(0.1);
  model4D->SetMaxWeight(10.0);
  model4D->SetMapSize(size4D);
  model4D->SetNeighborhoodSizeInit(radius4D);
  model4D->SetInputListSample(samples);
  model4D->Train();
  model4D->Save(std::string(argv[4]));

  std::cout << "Train 5D model..." << std::endl;
  model5D->SetNumberOfIterations(10);
  model5D->SetBetaInit(1.0);
  model5D->SetWriteMap(true);
  model5D->SetBetaEnd(0.1);
  model5D->SetMaxWeight(10.0);
  model5D->SetMapSize(size5D);
  model5D->SetNeighborhoodSizeInit(radius5D);
  model5D->SetInputListSample(samples);
  model5D->Train();
  model5D->Save(std::string(argv[5]));

  return EXIT_SUCCESS;
}
