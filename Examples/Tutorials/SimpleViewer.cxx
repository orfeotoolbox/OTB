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

//  Software Guide : BeginLatex
//
//  Now, we are going to illustrate the use of the \doxygen{otb}{StandardImageViewer}
// to display an image or the result of an algorithm without saving the image.
//
// We include the required header including the header
// for the \doxygen{itk}{GradientMagnitudeImageFilter} and the
// \doxygen{otb}{ImageViewer}.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "otbStandardImageViewer.h"

int main( int argc, char * argv[] )
{
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We need to declare two different image types, one for the internal
  // processing and one to output the results:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader=ReaderType::New();

  reader->SetFileName(argv[1]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Now we are declaring the edge detection filter which is going to work with
  // double input and output.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::GradientMagnitudeImageFilter
  <ImageType,ImageType> FilterType;
  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{StandardImageViewer} is the new, highly versatile
  // component to display images in OTB. There is a system of plugins to
  // fully customize its behavior. But we'll keep it simple for now.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::StandardImageViewer<ImageType> ViewerType;
  ViewerType::Pointer viewer = ViewerType::New();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  // Let's plug the pipeline: for the viewer the method is
  // \code{SetImage()}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  viewer->SetImage(filter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // When you open several reader, this is handy to be able to identify
  // each one. This can be done using
  // \code{SetLabel()}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  viewer->SetLabel(argv[1]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We trigger the pipeline execution and the image display
  // with the \code{Update()} method of the viewer.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  viewer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // A call to \code{Fl::run()} is mandatory to ask the program
  // to listen to mouse and keyword events until the viewer is closed.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  Fl::run();

  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet


