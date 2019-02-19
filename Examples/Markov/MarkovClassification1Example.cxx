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
./MarkovClassification1Example Input/QB_Suburb.png Output/MarkovRandomField1.png 1.0 20 1.0 1
*/


// This example illustrates the details of the \doxygen{otb}{MarkovRandomFieldFilter}.
// This filter is an application of the Markov Random Fields for classification,
// segmentation or restoration.
//
// This example applies the \doxygen{otb}{MarkovRandomFieldFilter} to
// classify an image into four classes defined by their mean and variance. The
// optimization is done using an Metropolis algorithm with a random sampler. The
// regularization energy is defined by a Potts model and the fidelity by a
// Gaussian model.
//

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

// The first step toward the use of this filter is the inclusion of the proper
// header files.

#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"

int main(int argc, char* argv[])
{

  if (argc != 7)
  {
    std::cerr << "Missing Parameters " << argc << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage output lambda iterations optimizerTemperature" << std::endl;
    std::cerr << " useRandomValue" << std::endl;
    return 1;
  }

  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with double precision.
  //  The labelled image is of type unsigned char which allows up to 256 different
  //  classes.

  const unsigned int Dimension = 2;

  typedef double                                   InternalPixelType;
  typedef unsigned char                            LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension> LabelledImageType;

  //  We define a reader for the image to be classified, an initialization for the
  //  classification (which could be random) and a writer for the final
  //  classification.

  typedef otb::ImageFileReader<InputImageType>    ReaderType;
  typedef otb::ImageFileWriter<LabelledImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  //  Finally, we define the different classes necessary for the Markov classification.
  //  A \doxygen{otb}{MarkovRandomFieldFilter} is instantiated, this is the
  // main class which connect the other to do the Markov classification.

  typedef otb::MarkovRandomFieldFilter<InputImageType, LabelledImageType> MarkovRandomFieldFilterType;

  //  An \doxygen{otb}{MRFSamplerRandomMAP}, which derives from the
  // \doxygen{otb}{MRFSampler}, is instantiated. The sampler is in charge of
  // proposing a modification for a given site. The
  // \doxygen{otb}{MRFSamplerRandomMAP}, randomly pick one possible value
  // according to the MAP probability.

  typedef otb::MRFSamplerRandom<InputImageType, LabelledImageType> SamplerType;

  //  An \doxygen{otb}{MRFOptimizerMetropoli}, which derives from the
  // \doxygen{otb}{MRFOptimizer}, is instantiated. The optimizer is in charge
  // of accepting or rejecting the value proposed by the sampler. The
  // \doxygen{otb}{MRFSamplerRandomMAP}, accept the proposal according to the
  // variation of energy it causes and a temperature parameter.

  typedef otb::MRFOptimizerMetropolis OptimizerType;

  // Two energy, deriving from the \doxygen{otb}{MRFEnergy} class need to be instantiated. One energy
  // is required for the regularization, taking into account the relashionship between neighborhing pixels
  // in the classified image. Here it is done with the \doxygen{otb}{MRFEnergyPotts} which implement
  // a Potts model.
  //
  // The second energy is for the fidelity to the original data. Here it is done with an
  // \doxygen{otb}{MRFEnergyGaussianClassification} class, which defines a gaussian model for the data.

  typedef otb::MRFEnergyPotts<LabelledImageType, LabelledImageType>               EnergyRegularizationType;
  typedef otb::MRFEnergyGaussianClassification<InputImageType, LabelledImageType> EnergyFidelityType;

  // The different filters composing our pipeline are created by invoking their
  // \code{New()} methods, assigning the results to smart pointers.

  MarkovRandomFieldFilterType::Pointer markovFilter         = MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer    energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer          energyFidelity       = EnergyFidelityType::New();
  OptimizerType::Pointer               optimizer            = OptimizerType::New();
  SamplerType::Pointer                 sampler              = SamplerType::New();

  // Parameter for the \doxygen{otb}{MRFEnergyGaussianClassification} class, meand
  // and standard deviation are created.

  if ((bool)(atoi(argv[6])) == true)
  {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(2);
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

  // Parameters are given to the different class an the sampler, optimizer and
  // energies are connected with the Markov filter.

  OptimizerType::ParametersType param(1);
  param.Fill(atof(argv[5]));
  optimizer->SetParameters(param);
  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[4]));
  markovFilter->SetErrorTolerance(0.0);
  markovFilter->SetLambda(atof(argv[3]));
  markovFilter->SetNeighborhoodRadius(1);

  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);

  // The pipeline is connected. An \doxygen{itk}{RescaleIntensityImageFilter}
  // rescale the classified image before saving it.

  markovFilter->SetInput(reader->GetOutput());

  typedef itk::RescaleIntensityImageFilter<LabelledImageType, LabelledImageType> RescaleType;
  RescaleType::Pointer                                                           rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());

  // Finally, the pipeline execution is trigerred.

  writer->Update();

  // Figure~\ref{fig:MRF_CLASSIFICATION1} shows the output of the Markov Random
  // Field classification after 20 iterations with a
  // random sampler and a Metropolis optimizer.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRandomField1.eps}
  // \itkcaption[MRF restoration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for classification. The result is obtained after 20 iterations with a
  // random sampler and a Metropolis optimizer. From left to right : original image,
  // classification.}
  // \label{fig:MRF_CLASSIFICATION1}
  // \end{figure}

  return EXIT_SUCCESS;
}
