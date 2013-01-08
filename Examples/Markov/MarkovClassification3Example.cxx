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
//    OUTPUTS: {MarkovRandomField3_gray_value.png}, {MarkovRandomField3_color_value.png}
//    1.0 20 1.0 1
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
//  This example illustrates the details of the MarkovRandomFieldFilter by using the Fisher distribution
//  to model the likelihood energy.
//  This filter is an application of the Markov Random Fields for classification.
//
//  This example applies the MarkovRandomFieldFilter to
//  classify an image into four classes defined by their Fisher distribution parameters L, M and mu.
//  The optimization is done using a Metropolis algorithm with a random sampler. The
//  regularization energy is defined by a Potts model and the fidelity or likelihood energy is modelled by a
//  Fisher distribution.
//  The parameter of the Fisher distribution was determined for each class in a supervised step.
//  ( See the File OtbParameterEstimatioOfFisherDistribution )
//  This example is a contribution from Jan Wegner.
//
// Software Guide : EndLatex


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "otbMarkovRandomFieldFilter.h"
#include "itkRescaleIntensityImageFilter.h"


#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"


#include "otbMRFEnergyPotts.h"
#include "otbMRFEnergyFisherClassification.h"
#include "otbMRFOptimizerMetropolis.h"
#include "otbMRFSamplerRandom.h"


int main(int argc, char* argv[] )
{
  if( argc != 8 )
    {
    std::cerr << "Missing Parameters "<< argc << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage output_gray_label output_color_label lambda iterations "
        "optimizerTemperature useRandomValue " << std::endl;
    return 1;
    }
  // Software Guide : BeginLatex
  //
  //  Then we must decide what pixel type to use for the image. We
  //  choose to make all computations with double precision.
  //  The labeled image is of type unsigned char which allows up to 256 different
  //  classes.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef double        InternalPixelType;
  typedef unsigned char LabelledPixelType;

  typedef otb::Image<InternalPixelType, Dimension>    InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension>    LabelledImageType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  //  We define a reader for the image to be classified, an initialization for the
  //  classification (which could be random) and a writer for the final
  //  classification.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader< InputImageType >              ReaderType;
  typedef otb::ImageFileWriter< LabelledImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  const char * outputRescaledImageFileName = argv[3];

  reader->SetFileName( inputFilename );
  writer->SetFileName( outputFilename );

  // Software Guide : BeginLatex
  //
  //  Finally, we define the different classes necessary for the Markov classification.
  //  A MarkovRandomFieldFilter is instantiated, this is the
  // main class which connect the other to do the Markov classification.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MarkovRandomFieldFilter
      <InputImageType, LabelledImageType> MarkovRandomFieldFilterType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  //  An MRFSamplerRandomMAP, which derives from the
  //  MRFSampler, is instanciated. The sampler is in charge of
  // proposing a modification for a given site. The
  // MRFSamplerRandomMAP, randomly pick one possible value
  // according to the MAP probability.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFSamplerRandom< InputImageType, LabelledImageType> SamplerType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  //  An MRFOptimizerMetropolis, which derives from the
  // MRFOptimizer, is instanciated. The optimizer is in charge
  // of accepting or rejecting the value proposed by the sampler. The
  // MRFSamplerRandomMAP, accept the proposal according to the
  // variation of energy it causes and a temperature parameter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFOptimizerMetropolis OptimizerType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Two energy, deriving from the MRFEnergy class need to be instantiated. One energy
  // is required for the regularization, taking into account the relationship between neighboring pixels
  // in the classified image. Here it is done with the MRFEnergyPotts, which implements
  // a Potts model.
  //
  // The second energy is used for the fidelity to the original data. Here it is done with a
  // MRFEnergyFisherClassification class, which defines a Fisher distribution to model the data.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MRFEnergyPotts
      <LabelledImageType, LabelledImageType>  EnergyRegularizationType;
  typedef otb::MRFEnergyFisherClassification
      <InputImageType, LabelledImageType>     EnergyFidelityType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The different filters composing our pipeline are created by invoking their
  // New() methods, assigning the results to smart pointers.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MarkovRandomFieldFilterType::Pointer markovFilter = MarkovRandomFieldFilterType::New();
  EnergyRegularizationType::Pointer energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer energyFidelity = EnergyFidelityType::New();
  OptimizerType::Pointer optimizer = OptimizerType::New();
  SamplerType::Pointer sampler = SamplerType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Parameter for the MRFEnergyFisherClassification class are created. The shape parameters M, L
  // and the weighting parameter mu are computed in a supervised step
  //
  // Software Guide : EndLatex


  if ((bool)(atoi(argv[6])) == true)
    {
    // Overpass random calculation(for test only):
    sampler->InitializeSeed(0);
    optimizer->InitializeSeed(1);
    markovFilter->InitializeSeed(1);
    }

  // Software Guide : BeginCodeSnippet
  unsigned int nClass =4;
  energyFidelity->SetNumberOfParameters(3*nClass);
  EnergyFidelityType::ParametersType parameters;
  parameters.SetSize(energyFidelity->GetNumberOfParameters());
  //Class 0
  parameters[0] =         12.353042;    //Class 0 mu
  parameters[1] =         2.156422;       //Class 0 L
  parameters[2] =         4.920403;       //Class 0 M
  //Class 1
  parameters[3] =         72.068291;    //Class 1 mu
  parameters[4] =         11.000000;   //Class 1 L
  parameters[5] =         50.950001;    //Class 1 M
  //Class 2
  parameters[6] =         146.665985;   //Class 2 mu
  parameters[7] =         11.000000;   //Class 2 L
  parameters[8] =         50.900002;    //Class 2 M
  //Class 3
  parameters[9]  =      200.010132;     //Class 3 mu
  parameters[10] =      11.000000;      //Class 3 L
  parameters[11] =      50.950001;      //Class 3 M

  energyFidelity->SetParameters(parameters);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Parameters are given to the different classes and the sampler, optimizer and
  // energies are connected with the Markov filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  OptimizerType::ParametersType param(1);
  param.Fill(atof(argv[6]));
  optimizer->SetParameters(param);
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
  // The pipeline is connected. An itkRescaleIntensityImageFilter
  // rescales the classified image before saving it.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  markovFilter->SetInput(reader->GetOutput());

  typedef itk::RescaleIntensityImageFilter
      < LabelledImageType, LabelledImageType > RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);

  rescaleFilter->SetInput( markovFilter->GetOutput() );

  writer->SetInput( rescaleFilter->GetOutput() );
  writer->Update();
  // Software Guide : EndCodeSnippet

  //convert output image to color
  typedef itk::RGBPixel<unsigned char>                          RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>                           RGBImageType;
  typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>  ColorMapFunctorType;

  typedef itk::UnaryFunctorImageFilter< LabelledImageType, RGBImageType, ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();

  colormapper->SetInput( rescaleFilter->GetOutput() );
  // Software Guide : BeginLatex
  //
  // We can now create an image file writer and save the image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<RGBImageType> WriterRescaledType;

  WriterRescaledType::Pointer writerRescaled = WriterRescaledType::New();

  writerRescaled->SetFileName( outputRescaledImageFileName );
  writerRescaled->SetInput( colormapper->GetOutput() );

  writerRescaled->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:MRF_CLASSIFICATION3} shows the output of the Markov Random
  // Field classification into four classes using the
  // Fisher-distribution as likelihood term.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRandomField3_color_value.eps}
  // \itkcaption[MRF restauration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for classification into four classes using the Fisher-distribution as
  // likehood term. From left to right : original image,
  // classification.}
  // \label{fig:MRF_CLASSIFICATION3}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
