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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {MarkovRandomField2.png}
//    1.0 5 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// Using a similar structure as the previous program and the same energy
// function, we are now going to slightly alter the program to use a
// different sampler and optimizer. The proposed sample is proposed
// randomly according to the MAP probability and the optimizer is the
// ICM which accept the proposed sample if it enable a reduction of
// the energy.
//
// Software Guide : EndLatex

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

// Software Guide : BeginLatex
//
// First, we need to include header specific to these class:
//
// Software Guide : EndLatex

#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"

// Software Guide : BeginCodeSnippet
#include "otbMRFSamplerRandomMAP.h"
#include "otbMRFOptimizerICM.h"
// Software Guide : EndCodeSnippet

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

  typedef double                                   InternalPixelType;
  typedef unsigned char                            LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension> LabelledImageType;

  typedef otb::ImageFileReader<InputImageType>    ReaderType;
  typedef otb::ImageFileWriter<LabelledImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::MarkovRandomFieldFilter
  <InputImageType, LabelledImageType> MarkovRandomFieldFilterType;

  //  Software Guide : BeginLatex
  //
  //  And to declare these new type:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFSamplerRandomMAP<InputImageType,
      LabelledImageType> SamplerType;
//   typedef otb::MRFSamplerRandom< InputImageType, LabelledImageType> SamplerType;
// Software Guide : EndCodeSnippet

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFOptimizerICM OptimizerType;
  // Software Guide : EndCodeSnippet

  typedef otb::MRFEnergyPotts
  <LabelledImageType, LabelledImageType>  EnergyRegularizationType;
  typedef otb::MRFEnergyGaussianClassification
  <InputImageType, LabelledImageType>  EnergyFidelityType;

  MarkovRandomFieldFilterType::Pointer markovFilter =
    MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer energyRegularization =
    EnergyRegularizationType::New();
  EnergyFidelityType::Pointer energyFidelity = EnergyFidelityType::New();
  OptimizerType::Pointer      optimizer = OptimizerType::New();
  SamplerType::Pointer        sampler = SamplerType::New();

  if ((bool) (atoi(argv[5])) == true)
    {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    markovFilter->InitializeSeed(1);
    }

  unsigned int nClass = 4;
  energyFidelity->SetNumberOfParameters(2 * nClass);
  EnergyFidelityType::ParametersType parameters;
  parameters.SetSize(energyFidelity->GetNumberOfParameters());
  parameters[0] = 10.0; //Class 0 mean
  parameters[1] = 10.0; //Class 0 stdev
  parameters[2] = 80.0; //Class 1 mean
  parameters[3] = 10.0; //Class 1 stdev
  parameters[4] = 150.0; //Class 2 mean
  parameters[5] = 10.0; //Class 2 stdev
  parameters[6] = 220.0; //Class 3 mean
  parameters[7] = 10.0; //Class 3 stde
  energyFidelity->SetParameters(parameters);

  // Software Guide : BeginLatex
  //
  // As the \doxygen{otb}{MRFOptimizerICM} does not have any parameters,
  // the call to \code{optimizer->SetParameters()} must be removed
  //
  // Software Guide : EndLatex

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

  typedef itk::RescaleIntensityImageFilter
  <LabelledImageType, LabelledImageType> RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());

  writer->Update();

  // Software Guide : BeginLatex
  //
  // Apart from these, no further modification is required.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginLatex
  //
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
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
