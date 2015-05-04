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
//    OUTPUTS: {MarkovRandomField1.png}
//    1.0 20 1.0 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the details of the \doxygen{otb}{MarkovRandomFieldFilter}.
// This filter is an application of the Markov Random Fields for classification,
// segmentation or restauration.
//
// This example applies the \doxygen{otb}{MarkovRandomFieldFilter} to
// classify an image into four classes defined by their mean and variance. The
// optimization is done using an Metropolis algorithm with a random sampler. The
// regularization energy is defined by a Potts model and the fidelity by a
// Gaussian model.
//
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
// header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyGaussianClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{

  if (argc != 7)
    {
    std::cerr << "Missing Parameters " << argc << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage output lambda iterations optimizerTemperature" <<
    std::endl;
    std::cerr << " useRandomValue" << std::endl;
    return 1;
    }

  //  Software Guide : BeginLatex
  //
  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with double precision.
  //  The labelled image is of type unsigned char which allows up to 256 different
  //  classes.
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
  //  We define a reader for the image to be classified, an initialization for the
  //  classification (which could be random) and a writer for the final
  //  classification.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType>    ReaderType;
  typedef otb::ImageFileWriter<LabelledImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Finally, we define the different classes necessary for the Markov classification.
  //  A \doxygen{otb}{MarkovRandomFieldFilter} is instanciated, this is the
  // main class which connect the other to do the Markov classification.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MarkovRandomFieldFilter
  <InputImageType, LabelledImageType> MarkovRandomFieldFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{MRFSamplerRandomMAP}, which derives from the
  // \doxygen{otb}{MRFSampler}, is instanciated. The sampler is in charge of
  // proposing a modification for a given site. The
  // \doxygen{otb}{MRFSamplerRandomMAP}, randomly pick one possible value
  // according to the MAP probability.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFSamplerRandom<InputImageType, LabelledImageType> SamplerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  An \doxygen{otb}{MRFOptimizerMetropoli}, which derives from the
  // \doxygen{otb}{MRFOptimizer}, is instanciated. The optimizer is in charge
  // of accepting or rejecting the value proposed by the sampler. The
  // \doxygen{otb}{MRFSamplerRandomMAP}, accept the proposal according to the
  // variation of energy it causes and a temperature parameter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFOptimizerMetropolis OptimizerType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Two energy, deriving from the \doxygen{otb}{MRFEnergy} class need to be instanciated. One energy
  // is required for the regularization, taking into account the relashionship between neighborhing pixels
  // in the classified image. Here it is done with the \doxygen{otb}{MRFEnergyPotts} which implement
  // a Potts model.
  //
  // The second energy is for the fidelity to the original data. Here it is done with an
  // \doxygen{otb}{MRFEnergyGaussianClassification} class, which defines a gaussian model for the data.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFEnergyPotts
  <LabelledImageType, LabelledImageType>  EnergyRegularizationType;
  typedef otb::MRFEnergyGaussianClassification
  <InputImageType, LabelledImageType>  EnergyFidelityType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The different filters composing our pipeline are created by invoking their
  // \code{New()} methods, assigning the results to smart pointers.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MarkovRandomFieldFilterType::Pointer markovFilter =
    MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer energyRegularization =
    EnergyRegularizationType::New();
  EnergyFidelityType::Pointer energyFidelity = EnergyFidelityType::New();
  OptimizerType::Pointer      optimizer = OptimizerType::New();
  SamplerType::Pointer        sampler = SamplerType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Parameter for the \doxygen{otb}{MRFEnergyGaussianClassification} class, meand
  // and standard deviation are created.
  //
  // Software Guide : EndLatex

  if ((bool) (atoi(argv[6])) == true)
    {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(2);
    }

  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Parameters are given to the different class an the sampler, optimizer and
  // energies are connected with the Markov filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The pipeline is connected. An \doxygen{itk}{RescaleIntensityImageFilter}
  // rescale the classified image before saving it.
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
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Finally, the pipeline execution is trigerred.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:MRF_CLASSIFICATION1} shows the output of the Markov Random
  // Field classification after 20 iterations with a
  // random sampler and a Metropolis optimizer.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRandomField1.eps}
  // \itkcaption[MRF restauration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for classification. The result is obtained after 20 iterations with a
  // random sampler and a Metropolis optimizer. From left to right : original image,
  // classification.}
  // \label{fig:MRF_CLASSIFICATION1}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
