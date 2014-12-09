/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


// Software Guide : BeginCommandLineArgs
// INPUTS:  {ROISpot5.png}
// OUTPUTS: {CannyEdgeDetectorImageFilterOutput.png}
// 2
// Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example introduces the use of the
//  \doxygen{itk}{CannyEdgeDetectionImageFilter}. This filter is widely used for
//  edge detection since it is the optimal solution satisfying the constraints
//  of good sensitivity, localization and noise robustness.
//
//  \index{itk::CannyEdgeDetectionImageFilter}
//
//  Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginLatex
//
//  The first step required for using this filter is to include its header file
//
//  \index{itk::CannyEdgeDetectionImageFilter!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkCannyEdgeDetectionImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [variance]" << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  float        variance = 2.0;

  if (argc > 3)
    {
    variance = atof(argv[3]);
    }

  typedef unsigned char CharPixelType;     //  IO
  typedef double        RealPixelType;     //  Operations
  const unsigned int Dimension = 2;

  typedef otb::Image<CharPixelType, Dimension> CharImageType;
  typedef otb::Image<RealPixelType, Dimension> RealImageType;

  //  Software Guide : BeginLatex
  //
  // As the Canny filter works with real values, we can instanciated the reader using
  // an image with pixels as double. This does not imply anything on the real image
  // coding format which will be cast into double.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<RealImageType> ReaderType;
  // Software Guide : EndCodeSnippet
  typedef otb::ImageFileWriter<CharImageType> WriterType;

  typedef itk::RescaleIntensityImageFilter<RealImageType,
      CharImageType> RescaleFilter;

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{itk}{CannyEdgeDetectionImageFilter} is instantiated using the float image type.
  //
  //  \index{itk::CannyEdgeDetectionImageFilter}
  //
  //  Software Guide : EndLatex

  typedef itk::CannyEdgeDetectionImageFilter<RealImageType,
      RealImageType> CannyFilter;

  //Setting the IO

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  RescaleFilter::Pointer rescale = RescaleFilter::New();

  //Setting the ITK pipeline filter

  CannyFilter::Pointer cannyFilter = CannyFilter::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  //The output of an edge filter is 0 or 1
  rescale->SetOutputMinimum(0);
  rescale->SetOutputMaximum(255);

  cannyFilter->SetInput(reader->GetOutput());
  cannyFilter->SetVariance(variance);
  rescale->SetInput(cannyFilter->GetOutput());
  writer->SetInput(rescale->GetOutput());

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.44\textwidth]{CannyEdgeDetectorImageFilterOutput.eps}
  // \itkcaption[CannyEdgeDetectorImageFilter output]{Effect of the
  // CannyEdgeDetectorImageFilter on a ROI of a Spot 5 image.}
  // \label{fig:CannyEdgeDetectorImageFilterInputOutput}
  // \end{figure}
  //
  //  Figure \ref{fig:CannyEdgeDetectorImageFilterInputOutput} illustrates the
  //  effect of this filter on a ROI of a Spot 5 image of an
  //  agricultural area.
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;

}
