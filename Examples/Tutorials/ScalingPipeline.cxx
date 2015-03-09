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
//    OUTPUTS: {TutorialsScalingPipelineOutput.png}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example illustrates the use of the
// \doxygen{itk}{RescaleIntensityImageFilter} to convert
// the result for proper display.
//
// We include the required header including the header
// for the \doxygen{itk}{CannyEdgeImageFilter} and the
// \doxygen{itk}{RescaleIntensityImageFilter}.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char * argv[])
{
  if (argc != 3)
    {
    std::cerr << "Usage: "
        << argv[0]
        << " <input_filename> <output_filename>"
        << std::endl;
    }
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We need to declare two different image types, one for the internal
  // processing and one to output the results:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double                   PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef unsigned char                  OutputPixelType;
  typedef otb::Image<OutputPixelType, 2> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We declare the reader with the image template using the pixel type
  // double. It is worth noticing that this instanciation does not imply
  // anything about the type of the input image. The original image can be
  // anything, the reader will just convert the result to double.
  //
  //  The writer is templated with the unsigned char image to be able to save
  // the result on one byte images (like png for example).
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Now we are declaring the edge detection filter which is going to work with
  // double input and output.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::CannyEdgeDetectionImageFilter
  <ImageType, ImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Here comes the interesting part: we declare the
  // \doxygen{itk}{RescaleIntensityImageFilter}. The input
  // image type is the output type of the edge detection
  // filter. The output type is the same as the input type
  // of the writer.
  //
  // Desired minimum and maximum values for the output are
  // specified by the methods \code{SetOutputMinimum()} and
  // \code{SetOutputMaximum()}.
  //
  // This filter will actually rescale all the pixels of
  // the image but also cast the type of these pixels.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::RescaleIntensityImageFilter
  <ImageType, OutputImageType> RescalerType;
  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Let's plug the pipeline:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // And finally, we trigger the pipeline execution calling the Update()
  // method on the writer
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->Update();

  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet
