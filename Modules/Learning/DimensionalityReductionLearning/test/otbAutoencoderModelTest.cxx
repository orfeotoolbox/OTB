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

#include "otbAutoencoderModel.h"
#include "otbReadDataFile.h"
#include "itkMacro.h"

typedef otb::AutoencoderModel<double, shark::LogisticNeuron> LogAutoencoderModel;
typedef LogAutoencoderModel::InputListSampleType  InputListSampleType;
typedef LogAutoencoderModel::TargetListSampleType TargetListSampleType;


int otbAutoencoderModelCanRead(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <model>" << std::endl;
    return EXIT_FAILURE;
  }

  LogAutoencoderModel::Pointer model = LogAutoencoderModel::New();
  std::string                  filename(argv[1]);
  if (!model->CanReadFile(filename))
  {
    std::cerr << "Failed to read model file : " << filename << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int otbAutoencoderModeTrain(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " letter.scale  model.out" << std::endl;
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

  itk::Array<unsigned int> nb_neuron;
  itk::Array<float>        noise;
  itk::Array<float>        regularization;
  itk::Array<float>        rho;
  itk::Array<float>        beta;

  nb_neuron.SetSize(1);
  noise.SetSize(1);
  regularization.SetSize(1);
  rho.SetSize(1);
  beta.SetSize(1);

  nb_neuron[0]      = 14;
  noise[0]          = 0.0;
  regularization[0] = 0.01;
  rho[0]            = 0.0;
  beta[0]           = 0.0;

  LogAutoencoderModel::Pointer model = LogAutoencoderModel::New();
  model->SetNumberOfHiddenNeurons(nb_neuron);
  model->SetNumberOfIterations(50);
  model->SetNumberOfIterationsFineTuning(0);
  model->SetEpsilon(0.0);
  model->SetInitFactor(1.0);
  model->SetRegularization(regularization);
  model->SetNoise(noise);
  model->SetRho(rho);
  model->SetBeta(beta);
  model->SetWriteWeights(true);
  model->SetInputListSample(samples);
  model->Train();
  model->Save(std::string(argv[2]));

  return EXIT_SUCCESS;
}
