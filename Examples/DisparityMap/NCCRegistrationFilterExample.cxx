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
//    INPUTS: {StereoFixed.png}, {StereoMoving.png}
//    OUTPUTS: {deformationFieldOutput2.png} {resampledOutput2.png}
//    5 1.0 2
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// This example demonstrates the use of the \doxygen{otb}{NCCRegistrationFilter}. This filter performs deformation estimation
// by optimising a PDE based on correlation. This use the finite difference solver hierarchy.
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbCommandLineArgumentParser.h"

// Software Guide : BeginCodeSnippet
#include "otbNCCRegistrationFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkWarpImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include <iostream>


int main(int argc, char** argv )
{
  
  if(argc!= 8)
  {
    std::cerr <<"Usage: "<<argv[0];
    std::cerr<<" fixedFileName movingFileName fieldOutName imageOutName ";
    std::cerr<<"explorationSize bluringSigma nbIterations ";
      
    return EXIT_FAILURE;
  }
  
  const unsigned int ImageDimension = 2;

  typedef double                                PixelType;
  typedef itk::Vector<double,ImageDimension>    DeformationPixelType;

  typedef double                   CoordinateRepresentationType;

  typedef unsigned char                         OutputPixelType;
  typedef otb::Image<OutputPixelType,ImageDimension> OutputImageType;
  
    // Software Guide : BeginLatex
  //
   // Several type of \doxygen{otb}{Image} are required to represent the reference image (fixed)
  // the image we want to register (moving) and the deformation field.
  //
   // Software Guide : EndLatex
  
  //Allocate Images
  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType,ImageDimension>         MovingImageType;
  typedef otb::Image<PixelType,ImageDimension>         FixedImageType;
  typedef otb::Image<DeformationPixelType,
                               ImageDimension>         DeformationFieldType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader< FixedImageType > FixedReaderType;
  FixedReaderType::Pointer fReader = FixedReaderType::New();
  fReader->SetFileName(argv[1]);  

  typedef otb::ImageFileReader< MovingImageType > MovingReaderType;
  MovingReaderType::Pointer mReader = MovingReaderType::New();
  mReader->SetFileName(argv[2]);  



  // Software Guide : BeginLatex
  //
  // To make the correlation possible and to avoid some local minima the first required step is
  // to blur the input images. This is done using the \doxygen{itk}{RecursiveGaussianImageFilter}:
  //
  // Software Guide : EndLatex
  
  //Blur input images
  // Software Guide : BeginCodeSnippet
  typedef itk::RecursiveGaussianImageFilter< FixedImageType,
    FixedImageType > FixedBlurType;

  FixedBlurType::Pointer fBlur = FixedBlurType::New();
  fBlur->SetInput( fReader->GetOutput() );
  fBlur->SetSigma( atof(argv[6]) );


  typedef itk::RecursiveGaussianImageFilter< MovingImageType,
    MovingImageType > MovingBlurType;

  MovingBlurType::Pointer mBlur = MovingBlurType::New();
  mBlur->SetInput( mReader->GetOutput() );
  mBlur->SetSigma(atof(argv[6]) );
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now, we need to instanciate the NCCRegistrationFilter which is going to perform the registration:
  //
  // Software Guide : EndLatex
  
  //Create the filter
  // Software Guide : BeginCodeSnippet
  typedef otb::NCCRegistrationFilter< FixedImageType, 
                                       MovingImageType,
                                       DeformationFieldType >
                                           RegistrationFilterType;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingImage( mBlur->GetOutput() );
  registrator->SetFixedImage( fBlur->GetOutput() );
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Few parameters need to be specified to the NCCRegistrationFilter:
  // \begin{itemize}
  // \item The area where the search is performed. This area is defined by its radius:
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  typedef RegistrationFilterType::RadiusType RadiusType;

  RadiusType radius;

  radius[0] = atoi(argv[5]);
  radius[1] = atoi(argv[5]);

  registrator->SetNCCRadius( radius );
// Software Guide : EndCodeSnippet
  
  std::cout << "NCC radius " << registrator->GetNCCRadius() << std::endl;
  
  // Software Guide : BeginLatex
  //
  // \item The number of iteration for the PDE resolution:
  //
  // Software Guide : EndLatex
  
   // Software Guide : BeginCodeSnippet
  registrator->SetNumberOfIterations( atoi(argv[7]) );
// Software Guide : EndCodeSnippet
  // registrator->GetDeformationField();

  // Software Guide : BeginLatex
  //
  // \end{itemize}
  // Then we can trigger the \code{Update()} method on the NCCRegistrationFilter:
  //
  // Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet
  registrator->Update();
  // Software Guide : EndCodeSnippet


  typedef otb::ImageOfVectorsToMonoChannelExtractROI<DeformationFieldType, MovingImageType> ChannelExtractionFilterType;
  ChannelExtractionFilterType::Pointer channelExtractor = ChannelExtractionFilterType::New();

  channelExtractor->SetInput(registrator->GetOutput());
  channelExtractor->SetChannel(1);
  
  typedef itk::RescaleIntensityImageFilter<MovingImageType, OutputImageType> RescalerType;
  RescalerType::Pointer fieldRescaler = RescalerType::New();
  
  fieldRescaler->SetInput(channelExtractor->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);
  
  typedef otb::ImageFileWriter< OutputImageType > DFWriterType;

  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput( fieldRescaler->GetOutput() );
  dfWriter->Update();

  
  typedef itk::WarpImageFilter<MovingImageType, MovingImageType, DeformationFieldType> WarperType;
  WarperType::Pointer warper = WarperType::New();
  
  MovingImageType::PixelType padValue = 4.0;

  warper->SetInput( mReader->GetOutput() );
  warper->SetDeformationField( registrator->GetOutput() );
  warper->SetEdgePaddingValue( padValue );
  
  
  
  typedef itk::CastImageFilter< MovingImageType, OutputImageType > CastFilterType;
  CastFilterType::Pointer  caster =  CastFilterType::New();
  caster->SetInput( warper->GetOutput() );
  
  typedef otb::ImageFileWriter< OutputImageType > WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[4]);
  writer->SetInput( caster->GetOutput() );
  writer->Update();
  
  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:NCCRegistrationFilterOUTPUT} shows the result of applying.
  //
  // \begin{figure} 
  // \center
  // \includegraphics[width=0.40\textwidth]{}
  // \includegraphics[width=0.40\textwidth]{}
  // \includegraphics[width=0.40\textwidth]{}
  // \includegraphics[width=0.40\textwidth]{}
  // \itkcaption[Deformation field and resampling from NCC registration]{From left
  // to right and top to bottom: fixed input image, moving image with a low stereo angle, 
  // estimated deformation field in the horizontal direction, resampled moving image.}
  // \label{fig:NCCRegistrationFilterOUTPUT}
  // \end{figure}
  //
  // Software Guide : EndLatex
  
  return EXIT_SUCCESS;

}

