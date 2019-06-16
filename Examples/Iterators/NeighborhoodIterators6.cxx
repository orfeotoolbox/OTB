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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkFastMarchingImageFilter.h"
#include "itkAddImageFilter.h"

/* Example usage:
./NeighborhoodIterators6 Output/NeighborhoodIterators6a.png 100 100
*/


/* Example usage:
./NeighborhoodIterators6 Output/NeighborhoodIterators6b.png 50 150
*/


/* Example usage:
./NeighborhoodIterators6 Output/NeighborhoodIterators6c.png 150 50
*/


// Some image processing routines do not need to visit every pixel in an
// image. Flood-fill and connected-component algorithms, for example, only
// visit pixels that are locally connected to one another.  Algorithms
// such as these can be efficiently written using the random access
// capabilities of the neighborhood iterator.
//
// The following example finds local minima.  Given a seed point, we can search
// the neighborhood of that point and pick the smallest value $m$.  While $m$
// is not at the center of our current neighborhood, we move in the direction
// of $m$ and repeat the analysis.  Eventually we discover a local minimum and
// stop.  This algorithm is made trivially simple in ND using an ITK
// neighborhood iterator.
//
// To illustrate the process, we create an image that descends everywhere to a
// single minimum:  a positive distance transform to a point.  The details of
// creating the distance transform are not relevant to the discussion of
// neighborhood iterators, but can be found in the source code of this
// example. Some noise has been added to the distance transform image for
// additional interest.

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " outputImageFile startX startY" << std::endl;
    return -1;
  }

  using PixelType                = float;
  using ImageType                = otb::Image<PixelType, 2>;
  using NeighborhoodIteratorType = itk::NeighborhoodIterator<ImageType>;

  using FastMarchingFilterType = itk::FastMarchingImageFilter<ImageType, ImageType>;

  FastMarchingFilterType::Pointer fastMarching = FastMarchingFilterType::New();

  using NodeContainer = FastMarchingFilterType::NodeContainer;
  using NodeType      = FastMarchingFilterType::NodeType;

  NodeContainer::Pointer seeds = NodeContainer::New();

  ImageType::IndexType seedPosition;

  seedPosition[0]              = 128;
  seedPosition[1]              = 128;
  const double initialDistance = 1.0;

  NodeType node;

  const double seedValue = -initialDistance;

  ImageType::SizeType size = {{256, 256}};

  node.SetValue(seedValue);
  node.SetIndex(seedPosition);
  seeds->Initialize();
  seeds->InsertElement(0, node);

  fastMarching->SetTrialPoints(seeds);
  fastMarching->SetSpeedConstant(1.0);

  itk::AddImageFilter<ImageType, ImageType, ImageType>::Pointer adder = itk::AddImageFilter<ImageType, ImageType, ImageType>::New();

  // Allocate the noise image
  ImageType::Pointer    noise = ImageType::New();
  ImageType::RegionType noiseRegion;
  noiseRegion.SetSize(size);
  noise->SetRegions(noiseRegion);
  noise->Allocate();

  // Fill the noise image
  itk::ImageRegionIterator<ImageType> itNoise(noise, noiseRegion);
  itNoise.GoToBegin();

  // Random number seed
  unsigned int sample_seed = 12345;
  double       u           = 0.;
  double       rnd         = 0.;
  double       dMin        = -.7;
  double       dMax        = .8;

  while (!itNoise.IsAtEnd())
  {
    sample_seed = (sample_seed * 16807) % 2147483647L;
    u           = static_cast<double>(sample_seed) / 2147483711UL;
    rnd         = (1.0 - u) * dMin + u * dMax;

    itNoise.Set((PixelType)rnd);
    ++itNoise;
  }

  adder->SetInput1(noise);
  adder->SetInput2(fastMarching->GetOutput());

  try
  {
    fastMarching->SetOutputSize(size);
    fastMarching->Update();

    adder->Update();
  }
  catch (itk::ExceptionObject& excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }

  ImageType::Pointer input = adder->GetOutput();

  // The variable \code{input} is the pointer to the distance transform image.
  // The local minimum algorithm is initialized with a seed point read from the
  // command line.

  ImageType::IndexType index;
  index[0] = ::atoi(argv[2]);
  index[1] = ::atoi(argv[3]);

  // Next we create the neighborhood iterator and position it at the seed point.

  NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType it(radius, input, input->GetRequestedRegion());

  it.SetLocation(index);

  // Searching for the local minimum involves finding the minimum in the current
  // neighborhood, then shifting the neighborhood in the direction of that
  // minimum.  The \code{for} loop below records the \doxygen{itk}{Offset} of the
  // minimum neighborhood pixel.  The neighborhood iterator is then moved using
  // that offset.  When a local minimum is detected, \code{flag} will remain
  // false and the \code{while} loop will exit.  Note that this code is
  // valid for an image of any dimensionality.

  bool flag = true;
  while (flag == true)
  {
    NeighborhoodIteratorType::OffsetType nextMove;
    nextMove.Fill(0);

    flag = false;

    PixelType min = it.GetCenterPixel();
    for (unsigned i = 0; i < it.Size(); ++i)
    {
      if (it.GetPixel(i) < min)
      {
        min      = it.GetPixel(i);
        nextMove = it.GetOffset(i);
        flag     = true;
      }
    }
    it.SetCenterPixel(255.0);
    it += nextMove;
  }

  // Figure~\ref{fig:NeighborhoodExample6} shows the results of the algorithm
  // for several seed points.  The white line is the path of the iterator from
  // the seed point to the minimum in the center of the image.  The effect of the
  // additive noise is visible as the small perturbations in the paths.
  //
  // \begin{figure} \centering
  // \includegraphics[width=0.3\textwidth]{NeighborhoodIterators6a.eps}
  // \includegraphics[width=0.3\textwidth]{NeighborhoodIterators6b.eps}
  // \includegraphics[width=0.3\textwidth]{NeighborhoodIterators6c.eps}
  // \itkcaption[Finding local minima]{Paths traversed by the neighborhood
  // iterator from different seed points to the local minimum.
  // The true minimum is at the center
  // of the image.  The path of the iterator is shown in white. The effect of
  // noise in the image is seen as small perturbations in each path. }
  // \protect\label{fig:NeighborhoodExample6} \end{figure}

  using WritePixelType = unsigned char;
  using WriteImageType = otb::Image<WritePixelType, 2>;
  using WriterType     = otb::ImageFileWriter<WriteImageType>;

  using RescaleFilterType = itk::RescaleIntensityImageFilter<ImageType, WriteImageType>;

  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  rescaler->SetInput(input);

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[1]);
  writer->SetInput(rescaler->GetOutput());
  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }
  return EXIT_SUCCESS;
}
