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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png QB_Suburb.png}
//    OUTPUTS: {MarkovRestauration.png}
//    10.0 30 1.0
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
//
// This example applies the \doxygen{otb}{MarkovRandomFieldFilter} for
// image restauration. The structure of the example is similar to the first example
// ones. The original image is assumed to be coded in one byte, thus 256 states
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


int main(int argc, char* argv[] ) 
{
  
  if( argc != 7 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage inputInitialization output lambda iterations optimizerTemperature" << std::endl;
    return 1;
  }
  
  
  const unsigned int Dimension = 2;
  
  typedef double InternalPixelType;
  typedef unsigned char LabelledPixelType;
  typedef otb::Image<InternalPixelType, Dimension>  InputImageType;
  typedef otb::Image<LabelledPixelType, Dimension>    LabelledImageType;


  
  //  Software Guide : BeginLatex
  //
  //  We need to declare an additional reader for the initial state of the
  // MRF. This reader has to be instantiated on the LabelledImageType.
  //
  //  Software Guide : EndLatex 


  typedef otb::ImageFileReader< InputImageType >  ReaderType;
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader< LabelledImageType >  ReaderLabelledType;
  // Software Guide : EndCodeSnippet
  typedef otb::ImageFileWriter< LabelledImageType >  WriterType;
  
  ReaderType::Pointer reader = ReaderType::New();
  // Software Guide : BeginCodeSnippet
  ReaderLabelledType::Pointer reader2 = ReaderLabelledType::New();
  // Software Guide : EndCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  
  const char * inputFilename  = argv[1];
  const char * labelledFilename  = argv[2];
  const char * outputFilename = argv[3];
  
  reader->SetFileName( inputFilename );
  // Software Guide : BeginCodeSnippet
  reader2->SetFileName( labelledFilename );
  // Software Guide : EndCodeSnippet
  writer->SetFileName( outputFilename );



  typedef otb::MarkovRandomFieldFilter
	  <InputImageType,LabelledImageType> MarkovRandomFieldFilterType;

  typedef otb::MRFSamplerRandom< InputImageType, LabelledImageType> SamplerType;

  typedef otb::MRFOptimizerMetropolis OptimizerType;

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The regularization and the fidelity energy are declared and instanciated:
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet

  typedef otb::MRFEnergyEdgeFidelity
		  <LabelledImageType, LabelledImageType>  EnergyRegularizationType;
  typedef otb::MRFEnergyGaussian
		  <InputImageType, LabelledImageType>  EnergyFidelityType;

  // Software Guide : EndCodeSnippet
  

  MarkovRandomFieldFilterType::Pointer markovFilter = MarkovRandomFieldFilterType::New();
  // Software Guide : BeginCodeSnippet
  EnergyRegularizationType::Pointer energyRegularization = EnergyRegularizationType::New();
  EnergyFidelityType::Pointer energyFidelity = EnergyFidelityType::New();
  // Software Guide : EndCodeSnippet
  OptimizerType::Pointer optimizer = OptimizerType::New();
  SamplerType::Pointer sampler = SamplerType::New();

  
  
  // Software Guide : BeginLatex
  //
  // The number of possible states for each pixel is 256 as the image is assumed
  // to be coded on one byte:
  //
  // Software Guide : EndLatex
  
  // Overpass random calculation (for test only):
  sampler->InitializeSeed(0);
  optimizer->InitializeSeed(0);
  markovFilter->InitializeSeed(0);
  
  // Software Guide : BeginCodeSnippet
  
  unsigned int nClass = 256;
  
  // Software Guide : EndCodeSnippet
  
  
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
  
  
  // Software Guide : BeginLatex
  //
  // The original state of the MRF filter is passed through the 
  // \code{SetTrainingInput()} method:
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  markovFilter->SetTrainingInput(reader2->GetOutput());
 // Software Guide : EndCodeSnippet
  
  markovFilter->SetInput(reader->GetOutput());
    
  typedef itk::RescaleIntensityImageFilter
      < LabelledImageType, LabelledImageType > RescaleType;
  RescaleType::Pointer rescaleFilter = RescaleType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  
  rescaleFilter->SetInput( markovFilter->GetOutput() );
  
  writer->SetInput( rescaleFilter->GetOutput() );
  
  writer->Update();  
  
  
  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:MRF_RESTAURATION} shows the output of the Markov Random
  // Field restauration.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{MarkovRestauration.eps}
  // \itkcaption[MRF restauration]{Result of applying
  // the \doxygen{otb}{MarkovRandomFieldFilter} to an extract from a PAN Quickbird
  // image for restauration. From left to right : original image, restaured image 
  // with edge preservation.}  
  // \label{fig:MRF_RESTAURATION} 
  // \end{figure}
  //
  // Software Guide : EndLatex
  
  return EXIT_SUCCESS;
  
}

