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
./MarkovRestorationExample Input/QB_Suburb.png Input/QB_Suburb.png Output/MarkovRestoration.png 10.0 30 1.0 1
*/


// The Markov Random Field framework can be used to apply an edge preserving
// filtering, thus playing a role of restoration.
//
// This example applies the \doxygen{otb}{MarkovRandomFieldFilter} for
// image restoration. The structure of the example is similar to the other MRF example.
// The original image is assumed to be coded in one byte, thus 256 states
// are possible for each pixel. The only other modifications reside in the energy
// function chosen for the fidelity and for the regularization.
//
// For the regularization energy function, we choose an edge preserving function:
//
// \begin{equation}
// \Phi(u) = \frac{u^2}{1+u^2}
// \end{equation}
//
// and for the fidelity function, we choose a gaussian model.
//
// The starting state of the Markov Random Field is given by the image itself
// as the final state should not be too far from it.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

// The first step toward the use of this filter is the inclusion of the proper
// header files:

#include "otbMRFEnergyEdgeFidelity.h"
#include "otbMRFEnergyGaussian.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"

int main(int argc, char* argv[])
{

  if (argc != 8)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputImage inputInitialization output lambda iterations optimizerTemperature"
              << std::endl;
    std::cerr << " useRandomValue" << std::endl;
    return 1;
    }

//  We declare the usual types:

  const unsigned int Dimension = 2;

  typedef double                                   InternalPixelType;
  typedef unsigned char                            LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension> LabelledImageType;

  //  We need to declare an additional reader for the initial state of the
  // MRF. This reader has to be instantiated on the LabelledImageType.

  typedef otb::ImageFileReader<InputImageType>    ReaderType;
  typedef otb::ImageFileReader<LabelledImageType> ReaderLabelledType;
  typedef otb::ImageFileWriter<LabelledImageType> WriterType;

  ReaderType::Pointer         reader = ReaderType::New();
  ReaderLabelledType::Pointer reader2 = ReaderLabelledType::New();
  WriterType::Pointer         writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * labelledFilename  = argv[2];
  const char * outputFilename = argv[3];

  reader->SetFileName(inputFilename);
  reader2->SetFileName(labelledFilename);
  writer->SetFileName(outputFilename);

  // We declare all the necessary types for the MRF:

  typedef otb::MarkovRandomFieldFilter
  <InputImageType, LabelledImageType> MarkovRandomFieldFilterType;

  typedef otb::MRFSamplerRandom<InputImageType, LabelledImageType> SamplerType;

  typedef otb::MRFOptimizerMetropolis OptimizerType;

  // The regularization and the fidelity energy are declared and instantiated:

  typedef otb::MRFEnergyEdgeFidelity
  <LabelledImageType, LabelledImageType>  EnergyRegularizationType;
  typedef otb::MRFEnergyGaussian
  <InputImageType, LabelledImageType>  EnergyFidelityType;

  MarkovRandomFieldFilterType::Pointer markovFilter =
    MarkovRandomFieldFilterType::New();

  EnergyRegularizationType::Pointer energyRegularization =
    EnergyRegularizationType::New();
  EnergyFidelityType::Pointer energyFidelity = EnergyFidelityType::New();

  OptimizerType::Pointer optimizer = OptimizerType::New();
  SamplerType::Pointer   sampler = SamplerType::New();

  if ((bool) (atoi(argv[7])) == true)
    {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(2);
    }

  // The number of possible states for each pixel is 256 as the image is assumed
  // to be coded on one byte and we pass the parameters to the markovFilter.

  unsigned int nClass = 256;

  optimizer->SetSingleParameter(atof(argv[6]));
  markovFilter->SetNumberOfClasses(nClass);
  markovFilter->SetMaximumNumberOfIterations(atoi(argv[5]));
  markovFilter->SetErrorTolerance(0.0);
  markovFilter->SetLambda(atof(argv[4]));
  markovFilter->SetNeighborhoodRadius(1);

  markovFilter->SetEnergyRegularization(energyRegularization);
  markovFilter->SetEnergyFidelity(energyFidelity);
  markovFilter->SetOptimizer(optimizer);
  markovFilter->SetSampler(sampler);

  // The original state of the MRF filter is passed through the
  // \code{SetTrainingInput()} method:

  markovFilter->SetTrainingInput(reader2->GetOutput());

  // And we plug the pipeline:

  markovFilter->SetInput(reader->GetOutput());

  typedef itk::RescaleIntensityImageFilter
  <LabelledImageType, LabelledImageType> RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return -1;
    }

  // Figure~\ref{fig:MRF_RESTORATION} shows the output of the Markov Random
  // Field restoration.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRestoration.eps}
  // \itkcaption[MRF restoration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for restoration. From left to right : original image, restaured image
  // with edge preservation.}
  // \label{fig:MRF_RESTORATION}
  // \end{figure}

  return EXIT_SUCCESS;

}
