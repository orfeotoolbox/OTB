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
//    INPUTS: {StereoFixed.png}, {StereoMoving.png}
//    OUTPUTS: {deformationFieldOutput-horizontal.png}, {deformationFieldOutput-vertical.png}, {resampledOutput2.png}
//    5 1.0 2
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the \doxygen{otb}{NCCRegistrationFilter}. This filter performs deformation estimation
// by optimising a PDE based on the normalized correlation coefficient. It uses the finite difference solver hierarchy.
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
#include "otbWarpImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageOfVectorsToMonoChannelExtractROI.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"

#include <iostream>

int main(int argc, char** argv)
{

  if (argc != 9)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " fixedFileName movingFileName fieldOutNameHorizontal fieldOutNameVertical imageOutName ";
    std::cerr << "explorationSize bluringSigma nbIterations ";

    return EXIT_FAILURE;
    }

  const unsigned int ImageDimension = 2;

  typedef double                              PixelType;
  typedef itk::Vector<double, ImageDimension> DisplacementPixelType;

  typedef double CoordinateRepresentationType;

  typedef unsigned char                               OutputPixelType;
  typedef otb::Image<OutputPixelType, ImageDimension> OutputImageType;

  // Software Guide : BeginLatex
  //
  // Several type of \doxygen{otb}{Image} are required to represent the reference image (fixed)
  // the image we want to register (moving) and the deformation field.
  //
  // Software Guide : EndLatex

  //Allocate Images
  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, ImageDimension> MovingImageType;
  typedef otb::Image<PixelType, ImageDimension> FixedImageType;
  typedef otb::Image<DisplacementPixelType,
      ImageDimension>         DisplacementFieldType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<FixedImageType> FixedReaderType;
  FixedReaderType::Pointer fReader = FixedReaderType::New();
  fReader->SetFileName(argv[1]);

  typedef otb::ImageFileReader<MovingImageType> MovingReaderType;
  MovingReaderType::Pointer mReader = MovingReaderType::New();
  mReader->SetFileName(argv[2]);

  // Software Guide : BeginLatex
  //
  // To make the correlation estimation more robust, the first
  // required step is to blur the input images. This is done using the
  // \doxygen{itk}{RecursiveGaussianImageFilter}:
  //
  // Software Guide : EndLatex

  //Blur input images
  // Software Guide : BeginCodeSnippet
  typedef itk::RecursiveGaussianImageFilter<FixedImageType,
      FixedImageType> FixedBlurType;

  FixedBlurType::Pointer fBlur = FixedBlurType::New();
  fBlur->SetInput(fReader->GetOutput());
  fBlur->SetSigma(atof(argv[7]));

  typedef itk::RecursiveGaussianImageFilter<MovingImageType,
      MovingImageType> MovingBlurType;

  MovingBlurType::Pointer mBlur = MovingBlurType::New();
  mBlur->SetInput(mReader->GetOutput());
  mBlur->SetSigma(atof(argv[7]));
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Now, we need to instanciate the NCCRegistrationFilter which is going to perform the registration:
  //
  // Software Guide : EndLatex

  //Create the filter
  // Software Guide : BeginCodeSnippet
  typedef otb::NCCRegistrationFilter<FixedImageType,
      MovingImageType,
      DisplacementFieldType>
  RegistrationFilterType;

  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();

  registrator->SetMovingImage(mBlur->GetOutput());
  registrator->SetFixedImage(fBlur->GetOutput());
// Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Some parameters need to be specified to the NCCRegistrationFilter:
  // \begin{itemize}
  // \item The area where the search is performed. This area is defined by its radius:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef RegistrationFilterType::RadiusType RadiusType;

  RadiusType radius;

  radius[0] = atoi(argv[6]);
  radius[1] = atoi(argv[6]);

  registrator->SetNCCRadius(radius);
// Software Guide : EndCodeSnippet

  std::cout << "NCC radius " << registrator->GetNCCRadius() << std::endl;

  // Software Guide : BeginLatex
  //
  // \item The number of iterations for the PDE resolution:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  registrator->SetNumberOfIterations(atoi(argv[8]));
// Software Guide : EndCodeSnippet
// registrator->GetDisplacementField();

  // Software Guide : BeginLatex
  //
  // \end{itemize}
  // The execution of the NCCRegistrationFilter will be triggered by
  // the \code{Update()} call on the writer at the end of the
  // pipeline. Make sure to use a
  // \doxygen{otb}{ImageFileWriter} if you want to benefit
  // from the streaming features.
  //
  // Software Guide : EndLatex

  typedef otb::ImageOfVectorsToMonoChannelExtractROI<DisplacementFieldType,
      MovingImageType>
  ChannelExtractionFilterType;
  ChannelExtractionFilterType::Pointer channelExtractor =
    ChannelExtractionFilterType::New();

  channelExtractor->SetInput(registrator->GetOutput());
  channelExtractor->SetChannel(1);

  typedef itk::RescaleIntensityImageFilter<MovingImageType,
      OutputImageType> RescalerType;
  RescalerType::Pointer fieldRescaler = RescalerType::New();

  fieldRescaler->SetInput(channelExtractor->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);

  typedef otb::ImageFileWriter<OutputImageType> DFWriterType;

  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(argv[3]);
  dfWriter->SetInput(fieldRescaler->GetOutput());
  dfWriter->Update();

  channelExtractor->SetChannel(2);
  dfWriter->SetFileName(argv[4]);
  dfWriter->Update();

  typedef otb::WarpImageFilter<MovingImageType, MovingImageType,
      DisplacementFieldType> WarperType;
  WarperType::Pointer warper = WarperType::New();

  MovingImageType::PixelType padValue = 4.0;

  warper->SetInput(mReader->GetOutput());
  warper->SetDisplacementField(registrator->GetOutput());
  warper->SetEdgePaddingValue(padValue);

  typedef itk::CastImageFilter<MovingImageType, OutputImageType> CastFilterType;
  CastFilterType::Pointer caster =  CastFilterType::New();
  caster->SetInput(warper->GetOutput());

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[5]);
  writer->SetInput(caster->GetOutput());
  writer->Update();

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:NCCRegistrationFilterOUTPUT} shows the result of
  // applying the disparity map estimation.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{StereoFixed.eps}
  // \includegraphics[width=0.40\textwidth]{StereoMoving.eps}
  // \includegraphics[width=0.40\textwidth]{deformationFieldOutput-horizontal.eps}
  // \includegraphics[width=0.40\textwidth]{deformationFieldOutput-vertical.eps}
  // \itkcaption[Displacement field and resampling from NCC registration]{From left
  // to right and top to bottom: fixed input image, moving image with a low stereo angle,
  // estimated deformation field in the horizontal direction, estimated deformation field in the vertical direction.}
  // \label{fig:NCCRegistrationFilterOUTPUT}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;

}
