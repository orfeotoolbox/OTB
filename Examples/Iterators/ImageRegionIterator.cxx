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


/* Example usage:
./ImageRegionIterator Input/QB_Suburb.png Output/ImageRegionIteratorOutput.png 10 10 110 140
*/


// \index{Iterators!speed}
// The \doxygen{itk}{ImageRegionIterator} is optimized for
// iteration speed and is the first choice for iterative, pixel-wise operations
// when location in the image is not
// important. ImageRegionIterator is the least specialized of the ITK
// image iterator classes.  It implements all of the methods described in the
// preceding section.
//
// The following example illustrates the use of
// \doxygen{itk}{ImageRegionConstIterator} and ImageRegionIterator.
// Most of the code constructs introduced apply to other ITK iterators as
// well. This simple application crops a subregion from an image by copying
// its pixel values into to a second, smaller image.
//
// \index{Iterators!and image regions}
// \index{itk::ImageRegionIterator!example of using|(}
// We begin by including the appropriate header files.

#include "otbImage.h"
#include "itkImageRegionIterator.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  // Verify the number of parameters on the command line.
  if (argc < 7)
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile startX startY sizeX sizeY" << std::endl;
    return -1;
  }

  // Next we define a pixel type and corresponding image type. ITK iterator
  // classes expect the image type as their template parameter.

  const unsigned int Dimension = 2;

  using PixelType = unsigned char;
  using ImageType = otb::Image<PixelType, Dimension>;

  using ConstIteratorType = itk::ImageRegionConstIterator<ImageType>;
  using IteratorType      = itk::ImageRegionIterator<ImageType>;

  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;

  // Information about the subregion to copy is read from the command line. The
  // subregion is defined by an \doxygen{itk}{ImageRegion} object, with a starting
  // grid index and a size (Section~\ref{sec:ImageSection}).

  ImageType::RegionType inputRegion;

  ImageType::RegionType::IndexType inputStart;
  ImageType::RegionType::SizeType  size;

  inputStart[0] = ::atoi(argv[3]);
  inputStart[1] = ::atoi(argv[4]);

  size[0] = ::atoi(argv[5]);
  size[1] = ::atoi(argv[6]);

  inputRegion.SetSize(size);
  inputRegion.SetIndex(inputStart);

  // The destination region in the output image is defined using the input region
  // size, but a different start index.  The starting index for the destination
  // region is the corner of the newly generated image.

  ImageType::RegionType outputRegion;

  ImageType::RegionType::IndexType outputStart;

  outputStart[0] = 0;
  outputStart[1] = 0;

  outputRegion.SetSize(size);
  outputRegion.SetIndex(outputStart);

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return -1;
  }

  // Check that the region is contained within the input image.
  if (!reader->GetOutput()->GetRequestedRegion().IsInside(inputRegion))
  {
    std::cerr << "Error" << std::endl;
    std::cerr << "The region " << inputRegion << "is not contained within the input image region " << reader->GetOutput()->GetRequestedRegion() << std::endl;
    return -1;
  }

  // After reading the input image and checking that the desired subregion is,
  // in fact, contained in the input, we allocate an output image.  It is
  // fundamental to set valid values to some of the basic image information
  // during the copying process.
  // In particular, the starting index of the output region
  // is now filled up with zero values and the coordinates of the physical
  // origin are computed as a shift from the origin of the input image. This is
  // quite important since it will allow us to later
  // register the extracted region against the original image.

  ImageType::Pointer outputImage = ImageType::New();
  outputImage->SetRegions(outputRegion);
  const ImageType::SpacingType& spacing     = reader->GetOutput()->GetSignedSpacing();
  const ImageType::PointType&   inputOrigin = reader->GetOutput()->GetOrigin();
  double                        outputOrigin[Dimension];

  for (unsigned int i = 0; i < Dimension; ++i)
  {
    outputOrigin[i] = inputOrigin[i] + spacing[i] * inputStart[i];
  }

  outputImage->SetSignedSpacing(spacing);
  outputImage->SetOrigin(outputOrigin);
  outputImage->Allocate();

  // \index{Iterators!construction of} \index{Iterators!and image regions}
  // The necessary images and region definitions are now in place.  All that is
  // left to do is to create the iterators and perform the copy.  Note that image
  // iterators are not accessed via smart pointers so they are light-weight
  // objects that are instantiated on the stack.  Also notice how the input and
  // output iterators are defined over the \emph{same corresponding region}.  Though the
  // images are different sizes, they both contain the same target subregion.

  ConstIteratorType inputIt(reader->GetOutput(), inputRegion);
  IteratorType      outputIt(outputImage, outputRegion);

  for (inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd(); ++inputIt, ++outputIt)
  {
    outputIt.Set(inputIt.Get());
  }

  // \index{Iterators!image dimensionality}
  //  The \code{for} loop above is a common
  // construct in ITK/OTB.  The beauty of these four lines of code is that they are
  // equally valid for one, two, three, or even ten dimensional data, and no
  // knowledge of the size of the image is necessary.  Consider the ugly
  // alternative of ten nested \code{for} loops for traversing an image.

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(outputImage);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return -1;
  }

  // Let's run this example on the image \code{QB\_Suburb.png} found
  // in \code{Examples/Data}.  The command line arguments specify the
  // input and output file names, then the $x$, $y$ origin and the $x$, $y$ size
  // of the cropped subregion.
  //
  // \small
  // \begin{verbatim}
  // ImageRegionIterator QB_Suburb.png ImageRegionIteratorOutput.png 20 70 210 140
  // \end{verbatim}
  // \normalsize
  //
  // The output is the cropped subregion shown in
  // Figure~\ref{fig:ImageRegionIteratorOutput}.
  //
  // \begin{figure}
  // \centering
  // \includegraphics[width=0.4\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.3\textwidth]{ImageRegionIteratorOutput.eps}
  // \itkcaption[Copying an image subregion using ImageRegionIterator]{Cropping a
  // region from an image.  The original image is shown at left.  The image on
  // the right is the result of applying the ImageRegionIterator example code.}
  // \protect\label{fig:ImageRegionIteratorOutput}
  // \end{figure}
  //
  // \index{itk::ImageRegionIterator!example of using|)}

  return EXIT_SUCCESS;
}
