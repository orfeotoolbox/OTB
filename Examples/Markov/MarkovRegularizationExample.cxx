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
./MarkovRegularizationExample Input/ROI_QB_MUL_1_SVN_CLASS_MULTI.png Output/MarkovRegularization.png Output/MarkovRegularization-scaled.png 0.2 20 0.0 1
*/


// This example illustrates the use of the \doxygen{otb}{MarkovRandomFieldFilter}.
// to regularize a classification obtained previously by another classifier. Here
// we will apply the regularization to the output of an SVM classifier presented
// in \ref{ssec:LearningFromImages}.
//
// The reference image and the starting image are both going to be the original
// classification. Both regularization and fidelity energy are defined by Potts model.
//
// The convergence of the Markov Random Field is done with a random sampler
// and a Metropolis model as in example 1. As you should get use to the general
// program structure to use the MRF framework, we are not going to repeat the entire
// example. However, remember you can find the full source code for this example
// in your OTB source directory.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"

#include "otbMRFEnergyPotts.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"

int main(int argc, char* argv[])
{

  if (argc != 8)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputClassificationImage outputClassification outputClassificationScaled lambda iterations temperature " << std::endl;
    std::cerr << " useRandomValue" << std::endl;
    return 1;
  }

  const unsigned int Dimension = 2;

  typedef unsigned char                            LabelledPixelType;
  typedef otb::Image<LabelledPixelType, Dimension> LabelledImageType;

  typedef otb::ImageFileReader<LabelledImageType> ReaderType;
  typedef otb::ImageFileWriter<LabelledImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char* inputFilename        = argv[1];
  const char* outputFilename       = argv[2];
  const char* outputScaledFilename = argv[3];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::MarkovRandomFieldFilter<LabelledImageType, LabelledImageType> MarkovRandomFieldFilterType;

  typedef otb::MRFSamplerRandom<LabelledImageType, LabelledImageType> SamplerType;

  typedef otb::MRFOptimizerMetropolis OptimizerType;

  typedef otb::MRFEnergyPotts<LabelledImageType, LabelledImageType> EnergyRegularizationType;
  typedef otb::MRFEnergyPotts<LabelledImageType, LabelledImageType> EnergyFidelityType;

  MarkovRandomFieldFilterType::Pointer markovFilter         = MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer    energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer          energyFidelity       = EnergyFidelityType::New();
  OptimizerType::Pointer               optimizer            = OptimizerType::New();
  SamplerType::Pointer                 sampler              = SamplerType::New();

  if ((bool)(atoi(argv[7])) == true)
  {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(2);
  }

  // To find the number of classes available in the original image we use the
  // \doxygen{itk}{LabelStatisticsImageFilter} and more particularly the method
  // \code{GetNumberOfLabels()}.

  typedef itk::LabelStatisticsImageFilter<LabelledImageType, LabelledImageType> LabelledStatType;
  LabelledStatType::Pointer                                                     labelledStat = LabelledStatType::New();
  labelledStat->SetInput(reader->GetOutput());
  labelledStat->SetLabelInput(reader->GetOutput());
  labelledStat->Update();

  unsigned int nClass = labelledStat->GetNumberOfLabels();

  optimizer->SetSingleParameter(0.0);
  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[5]));
  markovFilter->SetErrorTolerance(0.0);
  markovFilter->SetLambda(atof(argv[4]));
  markovFilter->SetNeighborhoodRadius(1);

  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);

  markovFilter->SetTrainingInput(reader->GetOutput());
  markovFilter->SetInput(reader->GetOutput());

  writer->SetInput(markovFilter->GetOutput());

  writer->Update();

  typedef itk::RescaleIntensityImageFilter<LabelledImageType, LabelledImageType> RescaleType;
  RescaleType::Pointer                                                           rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetFileName(outputScaledFilename);

  writer->SetInput(rescaleFilter->GetOutput());

  writer->Update();

  // Figure~\ref{fig:MRF_REGULARIZATION} shows the output of the Markov Random
  // Field regularization on the classification output of another method.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{MarkovRegularization.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRegularization-scaled.eps}
  // \itkcaption[MRF restoration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to regularized the result of another
  // classification. From left to right : original classification, regularized
  // classification}
  // \label{fig:MRF_REGULARIZATION}
  // \end{figure}

  return EXIT_SUCCESS;
}
