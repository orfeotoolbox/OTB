/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



// This example illustrates the details of the seam carving operation.
// References to this method can be found in \cite{Avidan07}. This example
// details the use of \doxygen{otb}{ImageToCarvingPathFilter} and
// \doxygen{otb}{RemoveCarvingPathFilter}.
//
// In this example, a loop is defined to remove a vertical or horizontal seam
// at each step of the algorithm. The seam with the minimum energy is chosen.

/* Example usage:
./SeamCarvingExample Input/QB_Suburb.png Output/SeamCarvingExampleOutput.png 50
*/


#include "otbImage.h"
#include "itkPolyLineParametricPath.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
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

  // Energy is computed according to the gradient of the image, thus an
  // \doxygen{itk}{GradientMagnitudeImageFilter} is instantiated

  typedef itk::GradientMagnitudeImageFilter<ImageType, ImageType> GradientType;
  GradientType::Pointer gradient = GradientType::New();

  // The \doxygen{otb}{ImageToCarvingPathFilter} compute the seam of minimum
  // energy according to lines or columns of the image. Later, as we will
  //  choose the best option between the two, we need two of these filters.

  typedef otb::ImageToCarvingPathFilter<ImageType, PathType> CarvingFilterType;
  CarvingFilterType::Pointer carvingFilterVert = CarvingFilterType::New();
  CarvingFilterType::Pointer carvingFilterHor = CarvingFilterType::New();

  // The \doxygen{otb}{RemoveCarvingPathFilter} will really resize the image
  // deleting the path.

  typedef otb::RemoveCarvingPathFilter
  <ImageType, PathType, ImageType> RemoveCarvingPathFilterType;
  RemoveCarvingPathFilterType::Pointer removeCarvingPath =
    RemoveCarvingPathFilterType::New();

  // As we are going to iterate through the filters, we need to disconnect the
  // pipeline at one point and store the image somewhere. For that purpose, we
  // use an \doxygen{itk}{ImageDuplicator}

  typedef itk::ImageDuplicator<ImageType> duplicatorType;
  duplicatorType::Pointer duplicator = duplicatorType::New();

  // Now that all elements have been instantiated, we start to plug the pipeline
  // and to define the loop.

  reader->Update();
  duplicator->SetInputImage(reader->GetOutput());
  duplicator->Update();

  double energyVert, energyHor;

  for (int i = 0; i < iteration; ++i)
    {

    gradient->SetInput(duplicator->GetOutput());

    // Two carving filters processed the gradient image to find the minimum
    // vertical seam and the minimum horizontal seam. Note that the
    // \code{UpdateLargestPossibleRegion()} need to be used as the size of the
    // input image will be different in each loop.

    carvingFilterVert->SetInput(gradient->GetOutput());
    carvingFilterVert->SetDirection(0);
    carvingFilterVert->UpdateLargestPossibleRegion();
    energyVert = carvingFilterVert->GetEnergyPerPix();

    carvingFilterHor->SetInput(gradient->GetOutput());
    carvingFilterHor->SetDirection(1);
    carvingFilterHor->UpdateLargestPossibleRegion();
    energyHor = carvingFilterHor->GetEnergyPerPix();

    // The vertical or the horizontal seam with the minimal energy is chosen.

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

    // The duplicator filter keep the results for the next loop

    duplicator->SetInputImage(removeCarvingPath->GetOutput());
    duplicator->Update();

    }

  // Finally, the resulting image is saved on an image file as usual

  rescaler->SetInput(duplicator->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

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

  return EXIT_SUCCESS;
}
