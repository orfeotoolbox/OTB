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


/* Example usage:
./MarkovClassification2Example Input/QB_Suburb.png Output/MarkovRandomField2.png 1.0 5 1
*/


// Using a similar structure as the previous program and the same energy
// function, we are now going to slightly alter the program to use a
// different sampler and optimizer. The proposed sample is proposed
// randomly according to the MAP probability and the optimizer is the
// ICM which accept the proposed sample if it enable a reduction of
// the energy.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

// First, we need to include header specific to these class:

#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"

#include "otbMRFSamplerRandomMAP.h"
#include "otbMRFOptimizerICM.h"

int main(int argc, char* argv[])
{

  if (argc != 6)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage output lambda iterations" << std::endl;
    std::cerr << " useRandomValue" << std::endl;
    return 1;
  }

  const unsigned int Dimension = 2;

  using InternalPixelType = double;
  using LabelledPixelType = unsigned char;
  using InputImageType    = otb::Image<InternalPixelType, Dimension>;
  using LabelledImageType = otb::Image<LabelledPixelType, Dimension>;

  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<LabelledImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  using MarkovRandomFieldFilterType = otb::MarkovRandomFieldFilter<InputImageType, LabelledImageType>;

  //  And to declare these new type:

  using SamplerType = otb::MRFSamplerRandomMAP<InputImageType, LabelledImageType>;
  //   using SamplerType = otb::MRFSamplerRandom< InputImageType, LabelledImageType>;

  using OptimizerType = otb::MRFOptimizerICM;

  using EnergyRegularizationType = otb::MRFEnergyPotts<LabelledImageType, LabelledImageType>;
  using EnergyFidelityType       = otb::MRFEnergyGaussianClassification<InputImageType, LabelledImageType>;

  MarkovRandomFieldFilterType::Pointer markovFilter         = MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer    energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer          energyFidelity       = EnergyFidelityType::New();
  OptimizerType::Pointer               optimizer            = OptimizerType::New();
  SamplerType::Pointer                 sampler              = SamplerType::New();

  if ((bool)(atoi(argv[5])) == true)
  {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    markovFilter->InitializeSeed(1);
  }

  unsigned int nClass = 4;
  energyFidelity->SetNumberOfParameters(2 * nClass);
  EnergyFidelityType::ParametersType parameters;
  parameters.SetSize(energyFidelity->GetNumberOfParameters());
  parameters[0] = 10.0;  // Class 0 mean
  parameters[1] = 10.0;  // Class 0 stdev
  parameters[2] = 80.0;  // Class 1 mean
  parameters[3] = 10.0;  // Class 1 stdev
  parameters[4] = 150.0; // Class 2 mean
  parameters[5] = 10.0;  // Class 2 stdev
  parameters[6] = 220.0; // Class 3 mean
  parameters[7] = 10.0;  // Class 3 stde
  energyFidelity->SetParameters(parameters);

  // As the \doxygen{otb}{MRFOptimizerICM} does not have any parameters,
  // the call to \code{optimizer->SetParameters()} must be removed

  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[4]));
  markovFilter->SetErrorTolerance(0.0);
  markovFilter->SetLambda(atof(argv[3]));
  markovFilter->SetNeighborhoodRadius(1);

  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);

  markovFilter->SetInput(reader->GetOutput());

  using RescaleType                  = itk::RescaleIntensityImageFilter<LabelledImageType, LabelledImageType>;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());

  writer->Update();

  // Apart from these, no further modification is required.

  // Figure~\ref{fig:MRF_CLASSIFICATION2} shows the output of the Markov Random
  // Field classification after 5 iterations with a
  // MAP random sampler and an ICM optimizer.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRandomField2.eps}
  // \itkcaption[MRF restoration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for classification. The result is obtained after 5 iterations with a
  // MAP random sampler and an ICM optimizer. From left to right : original image,
  // classification.}
  // \label{fig:MRF_CLASSIFICATION2}
  // \end{figure}

  return EXIT_SUCCESS;
}
