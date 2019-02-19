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


// The \doxygen{itk}{ImageLinearIteratorWithIndex} is designed for line-by-line
// processing of an image.  It walks a linear path along a selected image
// direction parallel to one of the coordinate axes of the image. This
// iterator conceptually breaks an image into a set of parallel lines
// that span the selected image dimension.
//
// \index{Iterators!and image lines}
//
// Like all image iterators, movement of the
// ImageLinearIteratorWithIndex is constrained within an
// image region $R$.  The line $\ell$ through which the iterator moves is
// defined by selecting a direction and an origin.   The line $\ell$
// extends from the origin to the upper boundary of $R$. The origin can be
// moved to any position along the lower boundary of $R$.
//
// Several additional methods are defined for this iterator to control movement
// of the iterator along the line $\ell$ and movement of the origin of $\ell$.
//
// %Might need a figure here to describe this iterator.
//
// \begin{itemize}
//
// \index{itk::ImageLinearIteratorWithIndex!NextLine()}
//
// \item \textbf{\code{NextLine()}} Moves the iterator to the beginning pixel
// location of the next line in the image.  The origin of the next line is
// determined by incrementing the current origin along the fastest increasing
// dimension of the subspace of the image that excludes the selected dimension.
//
//
// \index{itk::ImageLinearIteratorWithIndex!PreviousLine()}
//
// \item \textbf{\code{PreviousLine()}} Moves the iterator to the \emph{last valid
// pixel location} in the previous line. The origin of the previous line is
// determined by decrementing the current origin along the fastest increasing
// dimension of the subspace of the image that excludes the selected dimension.
//
// \index{itk::ImageLinearIteratorWithIndex!GoToBeginOfLine()}
// \index{itk::ImageLinearIteratorWithIndex!GoToReverseBeginOfLine()}
//
// \item \textbf{\code{GoToBeginOfLine()}} Moves the iterator to the beginning
// pixel of the current line.
//
// \item \textbf{\code{GoToEndOfLine()}}  Move the iterator to
// \emph{one past} the last valid pixel of the current line.
//
//
// \index{itk::ImageLinearIteratorWithIndex!IsAtReverseEndOfLine()}
// \index{itk::ImageLinearIteratorWithIndex!IsAtEndOfLine()}
//
// \item \textbf{\code{IsAtReverseEndOfLine()}}
// Returns true if the iterator points
// to \emph{one position before} the beginning pixel of the current line.
//
// \item \textbf{\code{IsAtEndOfLine()}}
// Returns true if the iterator points to
// \emph{one position past} the last valid pixel of the current line.
// \end{itemize}
//
// The following code example shows how to use the
// ImageLinearIteratorWithIndex.  It implements the same algorithm as
// in the previous example, flipping an image across its $x$-axis.  Two line
// iterators are iterated in opposite directions across the $x$-axis.
// After each line is traversed, the iterator origins are stepped along
// the $y$-axis to the
// next line.
//
// \index{itk::ImageLinearIteratorWithIndex!example of using|(}
//
// Headers for both the const and non-const versions are needed.

#include "otbImage.h"
#include "itkRGBPixel.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  // Verify the number of parameters on the command line.
  if (argc < 3)
  {
    std::cerr << "Missing parameters. " << std::endl;
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile" << std::endl;
    return -1;
  }

  // The RGB image and pixel types are defined as in the previous example.  The
  // ImageLinearIteratorWithIndex class and its const version each have
  // single template parameters, the image type.

  const unsigned int Dimension = 2;

  typedef itk::RGBPixel<unsigned char>        RGBPixelType;
  typedef otb::Image<RGBPixelType, Dimension> ImageType;

  typedef itk::ImageLinearIteratorWithIndex<ImageType>      IteratorType;
  typedef itk::ImageLinearConstIteratorWithIndex<ImageType> ConstIteratorType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ImageType::ConstPointer inputImage;
  ReaderType::Pointer     reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  try
  {
    reader->Update();
    inputImage = reader->GetOutput();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught a !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  // After reading the input image, we allocate an output image that of the same
  // size, spacing, and origin.

  ImageType::Pointer outputImage = ImageType::New();
  outputImage->SetRegions(inputImage->GetRequestedRegion());
  outputImage->CopyInformation(inputImage);
  outputImage->Allocate();

  // Next we create the two iterators.  The const iterator walks the input image,
  // and the non-const iterator walks the output image.  The iterators are
  // initialized over the same region.  The direction of iteration is set to 0,
  // the $x$ dimension.

  ConstIteratorType inputIt(inputImage, inputImage->GetRequestedRegion());
  IteratorType      outputIt(outputImage, inputImage->GetRequestedRegion());

  inputIt.SetDirection(0);
  outputIt.SetDirection(0);

  // Software Guide: BeginLatex
  //
  // Each line in the input is copied to the output.  The input iterator moves
  // forward across columns while the output iterator moves backwards.

  for (inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd(); outputIt.NextLine(), inputIt.NextLine())
  {
    inputIt.GoToBeginOfLine();
    outputIt.GoToEndOfLine();
    --outputIt;
    while (!inputIt.IsAtEndOfLine())
    {
      outputIt.Set(inputIt.Get());
      ++inputIt;
      --outputIt;
    }
  }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(outputImage);
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

  // Running this example on \code{ROI\_QB\_MUL\_1.tif} produces
  // the same output image shown in
  // Figure~\ref{fig:ImageRegionIteratorWithIndexExample}.
  //
  // \index{itk::ImageLinearIteratorWithIndex!example of using|)}

  return EXIT_SUCCESS;
}
