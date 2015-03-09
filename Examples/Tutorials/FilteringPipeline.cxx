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
//    OUTPUTS: {TutorialsFilteringPipelineOutput.png}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//
//  We are going to use the \doxygen{itk}{GradientMagnitudeImageFilter}
// to compute the gradient of the image. The begining of the file is
// similar to the Pipeline.cxx.
//
// We include the required headers, without forgetting to add the header
// for the \doxygen{itk}{GradientMagnitudeImageFilter}.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientMagnitudeImageFilter.h"

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
  //  We declare the image type, the reader and the writer as
  //  before:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned char, 2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Now we have to declare the filter. It is templated with the
  // input image type and the output image type like many filters
  // in OTB. Here we are using the same type for the input and the
  // output images:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::GradientMagnitudeImageFilter
  <ImageType, ImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Let's plug the pipeline:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // And finally, we trigger the pipeline execution calling the \code{Update()}
  // method on the writer
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->Update();

  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet
