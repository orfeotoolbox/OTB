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
// This example illustrates the details of the seam carving operation.
// References to this method can be found in \cite{Avidan07}. This example
// details the use of \doxygen{otb}{ImageToCarvingPathFilter} and
// \doxygen{otb}{RemoveCarvingPathFilter}.
//
// In this example, a loop is defined to remove a vertical or horizontal seam
// at each step of the algorithm. The seam with the minimum energy is chosen.
//
// Software Guide : EndLatex

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {SeamCarvingExampleOutput.png}
//    50
//  Software Guide : EndCommandLineArgs

#include "otbImage.h"
#include "itkPolyLineParametricPath.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbDrawPathFilter.h"
#include "otbImageToCarvingPathFilter.h"
#include "otbRemoveCarvingPathFilter.h"

#include "itkImageDuplicator.h"

int main(int itkNotUsed(argc), char * argv[])
{

  typedef float         InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType, Dimension>  ImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef itk::PolyLineParametricPath<Dimension> PathType;

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

  //  Software Guide : BeginLatex
  //
  // Energy is computed according to the gradient of the image, thus an
  // \doxygen{itk}{GradientMagnitudeImageFilter} is instanciated
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> GradientType;
  GradientType::Pointer gradient = GradientType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{ImageToCarvingPathFilter} compute the seam of minimum
  // energy according to lines or columns of the image. Later, as we will
  //  choose the best option between the two, we need two of these filters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageToCarvingPathFilter<ImageType, PathType> CarvingFilterType;
  CarvingFilterType::Pointer carvingFilterVert = CarvingFilterType::New();
  CarvingFilterType::Pointer carvingFilterHor = CarvingFilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The \doxygen{otb}{RemoveCarvingPathFilter} will really resize the image
  // deleting the path.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::RemoveCarvingPathFilter
  <ImageType, PathType, ImageType> RemoveCarvingPathFilterType;
  RemoveCarvingPathFilterType::Pointer removeCarvingPath =
    RemoveCarvingPathFilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // As we are going to iterate through the filters, we need to disconnect the
  // pipeline at one point and store the image somewhere. For that purpose, we
  // use an \doxygen{itk}{ImageDuplicator}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::ImageDuplicator<ImageType> duplicatorType;
  duplicatorType::Pointer duplicator = duplicatorType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Now that all elements have been instanciated, we start to plug the pipeline
  // and to define the loop.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->Update();
  duplicator->SetInputImage(reader->GetOutput());
  duplicator->Update();

  double energyVert, energyHor;

  for (int i = 0; i < iteration; ++i)
    {

    gradient->SetInput(duplicator->GetOutput());
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // Two carving filters processed the gradient image to find the minimum
    // vertical seam and the minimum horizontal seam. Note that the
    // \code{UpdateLargestPossibleRegion()} need to be used as the size of the
    // input image will be different in each loop.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    carvingFilterVert->SetInput(gradient->GetOutput());
    carvingFilterVert->SetDirection(0);
    carvingFilterVert->UpdateLargestPossibleRegion();
    energyVert = carvingFilterVert->GetEnergyPerPix();

    carvingFilterHor->SetInput(gradient->GetOutput());
    carvingFilterHor->SetDirection(1);
    carvingFilterHor->UpdateLargestPossibleRegion();
    energyHor = carvingFilterHor->GetEnergyPerPix();
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // The vertical or the horizontal seam with the minimal energy is chosen.
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    if (energyVert < energyHor)
      {
      removeCarvingPath->SetInput(duplicator->GetOutput());
      removeCarvingPath->SetInputPath(carvingFilterVert->GetOutput());
      removeCarvingPath->SetDirection(0);
      removeCarvingPath->UpdateLargestPossibleRegion();
      }
    else
      {
      removeCarvingPath->SetInput(duplicator->GetOutput());
      removeCarvingPath->SetInputPath(carvingFilterHor->GetOutput());
      removeCarvingPath->SetDirection(1);
      removeCarvingPath->UpdateLargestPossibleRegion();
      }
    // Software Guide : EndCodeSnippet

    //  Software Guide : BeginLatex
    //
    // The duplicator filter keep the results for the next loop
    //
    //  Software Guide : EndLatex

    // Software Guide : BeginCodeSnippet
    duplicator->SetInputImage(removeCarvingPath->GetOutput());
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
  // Figure~\ref{fig:SEAMCARVING_FILTER} shows the result of applying
  // the seam carving filter to a satellite image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.38\textwidth]{SeamCarvingExampleOutput.eps}
  // \itkcaption[Seam carving filter application]{Result of applying
  // the \doxygen{otb}{ImageToCarvingPathFilter} followed by the
  // \doxygen{otb}{RemoveCarvingPathFilter} to Quickbird
  // image. From left to right : original image, reduced image removing the 50
  // seams with the lowest energy.}
  // \label{fig:SEAMCARVING_FILTER}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
