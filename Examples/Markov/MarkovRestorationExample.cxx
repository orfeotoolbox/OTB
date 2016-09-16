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
//    INPUTS: {QB_Suburb.png}, {QB_Suburb.png}
//    OUTPUTS: {MarkovRestoration.png}
//    10.0 30 1.0 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
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
// The first step toward the use of this filter is the inclusion of the proper
// header files:
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMRFEnergyEdgeFidelity.h"
#include "otbMRFEnergyGaussian.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"
// Software Guide : EndCodeSnippet

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

//  Software Guide : BeginLatex
//
//  We declare the usual types:
//
//  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;

  typedef double                                   InternalPixelType;
  typedef unsigned char                            LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension> LabelledImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We need to declare an additional reader for the initial state of the
  // MRF. This reader has to be instantiated on the LabelledImageType.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We declare all the necessary types for the MRF:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MarkovRandomFieldFilter
  <InputImageType, LabelledImageType> MarkovRandomFieldFilterType;

  typedef otb::MRFSamplerRandom<InputImageType, LabelledImageType> SamplerType;

  typedef otb::MRFOptimizerMetropolis OptimizerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The regularization and the fidelity energy are declared and instantiated:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFEnergyEdgeFidelity
  <LabelledImageType, LabelledImageType>  EnergyRegularizationType;
  typedef otb::MRFEnergyGaussian
  <InputImageType, LabelledImageType>  EnergyFidelityType;
  // Software Guide : EndCodeSnippet

// Software Guide : BeginCodeSnippet
  MarkovRandomFieldFilterType::Pointer markovFilter =
    MarkovRandomFieldFilterType::New();

  EnergyRegularizationType::Pointer energyRegularization =
    EnergyRegularizationType::New();
  EnergyFidelityType::Pointer energyFidelity = EnergyFidelityType::New();

  OptimizerType::Pointer optimizer = OptimizerType::New();
  SamplerType::Pointer   sampler = SamplerType::New();
  // Software Guide : EndCodeSnippet

  if ((bool) (atoi(argv[7])) == true)
    {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(2);
    }

  // Software Guide : BeginLatex
  //
  // The number of possible states for each pixel is 256 as the image is assumed
  // to be coded on one byte and we pass the parameters to the markovFilter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The original state of the MRF filter is passed through the
  // \code{SetTrainingInput()} method:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  markovFilter->SetTrainingInput(reader2->GetOutput());
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // And we plug the pipeline:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  markovFilter->SetInput(reader->GetOutput());

  typedef itk::RescaleIntensityImageFilter
  <LabelledImageType, LabelledImageType> RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput(markovFilter->GetOutput());

  writer->SetInput(rescaleFilter->GetOutput());

  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
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
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
