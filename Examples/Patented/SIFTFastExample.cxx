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
//    INPUTS: {ROISpot5.png}
//    OUTPUTS: {ROISpot5SIFTFast.png}
//    3
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{otb}{SiftFastImageFilter}.
// The Scale-Invariant Feature Transform (or SIFT) is an algorithm in
// computer vision to detect and describe local features in
// images. The algorithm was published by David Lowe
// \cite{LoweSIFT}. The detection and description of local image
// features can help in object recognition and image registration. The
// SIFT features are local and based on the appearance of the object
// at particular interest points, and are invariant to image scale and
// rotation. They are also robust to changes in illumination, noise,
// occlusion and minor changes in viewpoint.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSiftFastImageFilter.h"
// Software Guide : EndCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>

int main(int argc, char * argv[])
{
  if (argc != 4)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " InputImage OutputImage scales" << std::endl;
    return 1;
    }
  const char * infname = argv[1];
  const char * outputImageFilename = argv[2];

  const unsigned int scales = atoi(argv[3]);

  const unsigned int Dimension = 2;

// Software Guide : BeginLatex
//
// We will start by defining the required types. We will work with a
// scalar image of float pixels. We also define the corresponding
// image reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef float                           RealType;
  typedef otb::Image<RealType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The SIFT descriptors will be stored in a point set containing the
// vector of features.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::VariableLengthVector<RealType>      RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension> PointSetType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The SIFT filter itself is templated over the input image and the
// generated point set.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SiftFastImageFilter<ImageType, PointSetType>
  ImageToFastSIFTKeyPointSetFilterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We instantiate the reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We instantiate the filter.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageToFastSIFTKeyPointSetFilterType::Pointer filter =
    ImageToFastSIFTKeyPointSetFilterType::New();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We plug the filter and set the number of scales for the SIFT
// computation. We can afterwards run the processing with the
// \code{Update()} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  filter->SetScalesNumber(scales);
  filter->Update();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Once the SIFT are computed, we may want to draw them on top of the
// input image. In order to do this, we will create the following RGB
// image and the corresponding writer:
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef unsigned char                         PixelType;
  typedef itk::RGBPixel<PixelType>              RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>           OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  OutputImageType::Pointer outputImage = OutputImageType::New();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We set the regions of the image by copying the information from the
// input image and we allocate the memory for the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now proceed to copy the input image into the output one
// using region iterators. The input image is a grey level one. The
// output image will be made of color crosses for each SIFT on top of
// the grey level input image. So we start by copying the grey level
// values on each of the 3 channels of the color image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  itk::ImageRegionIterator<OutputImageType> iterOutput(
    outputImage,
    outputImage->
    GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> iterInput(reader->GetOutput(),
                                                reader->GetOutput()->
                                                GetLargestPossibleRegion());

  for (iterOutput.GoToBegin(), iterInput.GoToBegin();
       !iterOutput.IsAtEnd();
       ++iterOutput, ++iterInput)
    {
    OutputImageType::PixelType rgbPixel;
    rgbPixel.SetRed(static_cast<PixelType>(iterInput.Get()));
    rgbPixel.SetGreen(static_cast<PixelType>(iterInput.Get()));
    rgbPixel.SetBlue(static_cast<PixelType>(iterInput.Get()));

    iterOutput.Set(rgbPixel);
    }
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We are now going to plot color crosses on the output image. We will
// need to define offsets (top, bottom, left and right) with respect
// to the SIFT position in order to draw the cross segments.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageType::OffsetType t = {{ 0, 1}};
  ImageType::OffsetType b = {{ 0, -1}};
  ImageType::OffsetType l = {{ 1, 0}};
  ImageType::OffsetType r = {{-1, 0}};
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Now, we are going to access the point set generated by the SIFT
// filter. The points are stored into a points container that we are
// going to walk through using an iterator. These are the types needed
// for this task:
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We set the iterator to the beginning of the point set.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We get the information about image size and spacing before drawing
// the crosses.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageType::SpacingType    spacing = reader->GetOutput()->GetSpacing();
  ImageType::PointType      origin = reader->GetOutput()->GetOrigin();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// And we iterate through the SIFT set:
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  while (pIt != filter->GetOutput()->GetPoints()->End())
    {
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We get the pixel coordinates for each SIFT by using the
// \code{Value()} method on the point set iterator. We use the
// information about size and spacing in order to convert the physical
// coordinates of the point into pixel coordinates.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    ImageType::IndexType index;

    index[0] = static_cast<unsigned int>(vcl_floor(
                                           static_cast<double>(
                                             (pIt.Value()[0] -
                                              origin[0]) / spacing[0] + 0.5
                                             )));

    index[1] = static_cast<unsigned int>(vcl_floor(
                                           static_cast<double>(
                                             (pIt.Value()[1] -
                                              origin[1]) / spacing[1] + 0.5
                                             )));
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We create a green pixel.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    OutputImageType::PixelType keyPixel;
    keyPixel.SetRed(0);
    keyPixel.SetGreen(255);
    keyPixel.SetBlue(0);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We draw the crosses using the offsets and checking that we are
// inside the image, since SIFTs on the image borders would cause an
// out of bounds pixel access.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
    if (outputImage->GetLargestPossibleRegion().IsInside(index))
      {
      outputImage->SetPixel(index, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index +
                                                           t))
        outputImage->
        SetPixel(index + t, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index +
                                                           b))
        outputImage->
        SetPixel(index + b, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index +
                                                           l))
        outputImage->
        SetPixel(index + l, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index +
                                                           r))
        outputImage->
        SetPixel(index + r, keyPixel);
      }
    ++pIt;
    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Finally, we write the image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageFilename);
  writer->SetInput(outputImage);
  writer->Update();
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:SIFTFast} shows the result of applying the SIFT
  // point detector to a small patch extracted from a Spot 5 image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.40\textwidth]{ROISpot5SIFTFast.eps}
  // \itkcaption[SIFT Application]{Result of applying the
  // \doxygen{otb}{SiftFastImageFilter} to a Spot 5
  // image.}
  // \label{fig:SIFTFast}
  // \end{figure}
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
