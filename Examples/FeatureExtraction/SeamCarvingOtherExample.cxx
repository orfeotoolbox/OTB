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


// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{AddCarvingPathFilter},
// the opposite of the \doxygen{otb}{RemoveCarvingPathFilter}.
//
// Here, we use this filter combined with the \doxygen{otb}{DrawPathFilter} to
// output the image with the removed seam in white.
//
// Most of the code is similar to the previous example.
//
// Software Guide : EndLatex

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {SeamCarvingOtherExampleOutput.png}
//    50
//  Software Guide : EndCommandLineArgs

#include "otbImage.h"
#include "itkPolyLineParametricPath.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"

#include "otbDrawPathFilter.h"
#include "otbImageToCarvingPathFilter.h"
#include "otbRemoveCarvingPathFilter.h"
#include "otbAddCarvingPathFilter.h"

#include "itkImageDuplicator.h"
#include "otbObjectList.h"

int main(int itkNotUsed(argc), char * argv[])
{

  typedef float         InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  ImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;

  // Software Guide : BeginLatex
  //
  // We need to define a list to keep the path in memory until the end of
  // the seam carving process. This is done using an \doxygen{otb}{ObjectList}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ObjectList<PathType> PathListType;
  PathListType::Pointer pathList = PathListType::New();
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<ImageType>
  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>
  WriterType;
  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputImageType> RescalerType;

  ReaderType::Pointer   reader = ReaderType::New();
  WriterType::Pointer   writer = WriterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  const char * filenamereader = argv[1];
  reader->SetFileName(filenamereader);

  const char * filenamewriter = argv[2];
  writer->SetFileName(filenamewriter);

  int iteration = atoi(argv[3]);

  // Software Guide : BeginLatex
  //
  // We instanciate the different filters of the pipeline as before.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> GradientType;
  GradientType::Pointer gradient = GradientType::New();

  typedef otb::ImageToCarvingPathFilter<ImageType, PathType> CarvingFilterType;
  CarvingFilterType::Pointer carvingFilter = CarvingFilterType::New();

  typedef otb::DrawPathFilter
  <ImageType, PathType, ImageType> DrawPathFilterType;
  DrawPathFilterType::Pointer drawPathFilter = DrawPathFilterType::New();

  typedef otb::RemoveCarvingPathFilter
  <ImageType, PathType, ImageType> RemoveCarvingPathFilterType;
  RemoveCarvingPathFilterType::Pointer removeCarvingPath =
    RemoveCarvingPathFilterType::New();

  typedef otb::AddCarvingPathFilter
  <ImageType, PathType, ImageType> AddCarvingPathFilterType;
  AddCarvingPathFilterType::Pointer addCarvingPath =
    AddCarvingPathFilterType::New();

  typedef itk::ImageDuplicator<ImageType> duplicatorType;
  duplicatorType::Pointer duplicator = duplicatorType::New();
  reader->Update();
  duplicator->SetInputImage(reader->GetOutput());
  duplicator->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The loop to shorten the image is similar to the previous one. Here we
  // decide to remove alternatively one vertical and one horizontal seam. At
  // each iteration, we save the seam on the list using the \code{PushBack()}
  // method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  for (int i = 0; i < iteration; ++i)
    {

    gradient->SetInput(duplicator->GetOutput());

    carvingFilter->SetInput(gradient->GetOutput());
    carvingFilter->SetDirection(i % 2);

    removeCarvingPath->SetInput(duplicator->GetOutput());
    removeCarvingPath->SetInputPath(carvingFilter->GetOutput());
    removeCarvingPath->SetDirection(i % 2);
    removeCarvingPath->UpdateLargestPossibleRegion();

    pathList->PushBack(carvingFilter->GetOutput());
    carvingFilter->GetOutput()->DisconnectPipeline();

    duplicator->SetInputImage(removeCarvingPath->GetOutput());
    duplicator->Update();

    }
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The next loop will put back the seam using the
  // \doxygen{otb}{AddCarvingPathFilter} and drawing it with the
  // \doxygen{otb}{DrawPathFilter}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  for (int i = iteration - 1; i >= 0; i--)
    {

    addCarvingPath->SetInput(duplicator->GetOutput());
    addCarvingPath->SetInputPath(pathList->GetNthElement(i));
    addCarvingPath->SetDirection(i % 2);
    addCarvingPath->UpdateLargestPossibleRegion();

    drawPathFilter->SetInput(addCarvingPath->GetOutput());
    drawPathFilter->SetInputPath(pathList->GetNthElement(i));
    drawPathFilter->UpdateLargestPossibleRegion();

    duplicator->SetInputImage(drawPathFilter->GetOutput());
    duplicator->Update();
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Finally, the resulting image is saved on an image file as usual
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  rescaler->SetInput(duplicator->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:SEAMCARVING2_FILTER} shows the result of applying
  // the seam carving filter to a satellite image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{SeamCarvingOtherExampleOutput.eps}
  // \itkcaption[Seam carving filter application]{Illustration of the minimum
  // seams alternatively vertical and horizontal on a Quickbird
  // image. From left to right : original image, image with the 25 vertical
  // seams and the 25 horizontal seams.}
  // \label{fig:SEAMCARVING2_FILTER}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
