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
./SIFTDisparityMapEstimation Input/ROISpot5.png Input/ROISpot5Warped.png Output/SIFTdeformationFieldOutput.png
*/


// This example demonstrates the use of the
// \doxygen{otb}{KeyPointSetsMatchingFilter} for disparity map
// estimation. The idea here is to match SIFTs extracted from both the
// fixed and the moving images. The use of SIFTs is demonstrated in
// section \ref{sec:SIFTDetector}. The
// \doxygen{itk}{DisplacementFieldSource} will be used
// to generate a deformation field by using
// interpolation on the deformation values from the point set. More
// advanced methods for deformation field interpolation are also
// available.
//
// The first step toward the use of these filters is to include the
// appropriate header files.

#include "otbKeyPointSetsMatchingFilter.h"
#include "otbSiftFastImageFilter.h"
#include "itkLandmarkDisplacementFieldSource.h"


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkPointSet.h"
#include "otbMultiToMonoChannelExtractROI.h"

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "fixedFileName movingFileName fieldOutName" << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 2;

  // Then we must decide what pixel type to use for the image. We choose to do
  // all the computations in floating point precision and rescale the results
  // between 0 and 255 in order to export PNG images.

  using RealType        = double;
  using OutputPixelType = unsigned char;

  using ImageType       = otb::Image<RealType, Dimension>;
  using OutputImageType = otb::Image<OutputPixelType, Dimension>;
  // The SIFTs obtained for the matching will be stored in vector
  // form inside a point set. So we need the following types:

  using RealVectorType = itk::VariableLengthVector<RealType>;
  using PointSetType   = itk::PointSet<RealVectorType, Dimension>;
  // The filter for computing the SIFTs has a type defined as follows:

  using ImageToSIFTKeyPointSetFilterType = otb::SiftFastImageFilter<ImageType, PointSetType>;
  // Although many choices for evaluating the distances during the
  // matching procedure exist, we choose here to use a simple
  // Euclidean distance. We can then define the type for the matching filter.

  using DistanceType                              = itk::Statistics::EuclideanDistanceMetric<RealVectorType>;
  using EuclideanDistanceMetricMatchingFilterType = otb::KeyPointSetsMatchingFilter<PointSetType, DistanceType>;
  // The following types are needed for dealing with the matched points.

  using PointType        = PointSetType::PointType;
  using MatchType        = std::pair<PointType, PointType>;
  using MatchVectorType  = std::vector<MatchType>;
  using LandmarkListType = EuclideanDistanceMetricMatchingFilterType::LandmarkListType;

  // We define the type for the image reader.

  using ReaderType = otb::ImageFileReader<ImageType>;

  // Two readers are instantiated : one for the fixed image, and one
  // for the moving image.

  ReaderType::Pointer fixedReader  = ReaderType::New();
  ReaderType::Pointer movingReader = ReaderType::New();

  fixedReader->SetFileName(argv[1]);
  movingReader->SetFileName(argv[2]);
  fixedReader->UpdateOutputInformation();
  movingReader->UpdateOutputInformation();
  // We will now instantiate the 2 SIFT filters and the filter used
  // for the matching of the points.

  ImageToSIFTKeyPointSetFilterType::Pointer          filter1          = ImageToSIFTKeyPointSetFilterType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer          filter2          = ImageToSIFTKeyPointSetFilterType::New();
  EuclideanDistanceMetricMatchingFilterType::Pointer euclideanMatcher = EuclideanDistanceMetricMatchingFilterType::New();
  // We plug the pipeline and set the parameters.

  double secondOrderThreshold = 0.5;
  bool   useBackMatching      = 0;

  filter1->SetInput(0, fixedReader->GetOutput());
  filter1->SetScalesNumber(3);
  filter2->SetInput(0, movingReader->GetOutput());
  filter2->SetScalesNumber(3);

  filter1->SetNumberOfThreads(1);
  filter2->SetNumberOfThreads(1);

  euclideanMatcher->SetInput1(filter1->GetOutput());
  euclideanMatcher->SetInput2(filter2->GetOutput());
  euclideanMatcher->SetDistanceThreshold(secondOrderThreshold);
  euclideanMatcher->SetUseBackMatching(useBackMatching);
  euclideanMatcher->Update();
  // The matched points will be stored into a landmark list.

  LandmarkListType::Pointer landmarkList;
  landmarkList = euclideanMatcher->GetOutput();
  MatchVectorType trueSecondOrder;

  for (LandmarkListType::Iterator it = landmarkList->Begin(); it != landmarkList->End(); ++it)
  {
    PointType point1 = it.Get()->GetPoint1();
    PointType point2 = it.Get()->GetPoint2();

    trueSecondOrder.push_back(MatchType(point1, point2));
  }

  // Displaying the matches
  using PrintableFilterType = itk::RescaleIntensityImageFilter<ImageType, OutputImageType>;

  PrintableFilterType::Pointer printable1 = PrintableFilterType::New();
  PrintableFilterType::Pointer printable2 = PrintableFilterType::New();

  printable1->SetInput(fixedReader->GetOutput());
  printable1->SetOutputMinimum(0);
  printable1->SetOutputMaximum(255);
  printable1->Update();

  printable2->SetInput(movingReader->GetOutput());
  printable2->SetOutputMinimum(0);
  printable2->SetOutputMaximum(255);
  printable2->Update();

  using RGBImageType = otb::Image<itk::RGBPixel<unsigned char>, 2>;

  RGBImageType::Pointer rgbimage1 = RGBImageType::New();
  rgbimage1->SetRegions(printable1->GetOutput()->GetLargestPossibleRegion());
  rgbimage1->Allocate();
  itk::ImageRegionIterator<RGBImageType>    outIt1(rgbimage1, rgbimage1->GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType> inIt1(printable1->GetOutput(), printable1->GetOutput()->GetLargestPossibleRegion());
  outIt1.GoToBegin();
  inIt1.GoToBegin();
  while (!inIt1.IsAtEnd() && !outIt1.IsAtEnd())
  {
    itk::RGBPixel<unsigned char> pixel;
    pixel.SetRed(inIt1.Get());
    pixel.SetGreen(inIt1.Get());
    pixel.SetBlue(inIt1.Get());
    outIt1.Set(pixel);
    ++inIt1;
    ++outIt1;
  }

  RGBImageType::Pointer rgbimage2 = RGBImageType::New();
  rgbimage2->SetRegions(printable2->GetOutput()->GetLargestPossibleRegion());
  rgbimage2->Allocate();
  itk::ImageRegionIterator<RGBImageType>    outIt2(rgbimage2, rgbimage2->GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType> inIt2(printable2->GetOutput(), printable2->GetOutput()->GetLargestPossibleRegion());
  outIt2.GoToBegin();
  inIt2.GoToBegin();
  while (!inIt2.IsAtEnd() && !outIt2.IsAtEnd())
  {
    itk::RGBPixel<unsigned char> pixel;
    pixel.SetRed(inIt2.Get());
    pixel.SetGreen(inIt2.Get());
    pixel.SetBlue(inIt2.Get());
    outIt2.Set(pixel);
    ++inIt2;
    ++outIt2;
  }

  // The landmarks are used for building a deformation field. The
  // deformation field is an image of vectors created by the
  // \doxygen{itk}{DisplacementFieldSource} class.

  using VectorType            = itk::Vector<RealType, Dimension>;
  using DisplacementFieldType = otb::Image<VectorType, Dimension>;

  using DisplacementSourceType = itk::LandmarkDisplacementFieldSource<DisplacementFieldType>;

  DisplacementSourceType::Pointer deformer = DisplacementSourceType::New();
  // The deformation field needs information about the extent and
  // spacing of the images on which it is defined.
  ImageType::ConstPointer fixedImage = fixedReader->GetOutput();

  deformer->SetOutputSpacing(fixedImage->GetSignedSpacing());
  deformer->SetOutputOrigin(fixedImage->GetOrigin());
  deformer->SetOutputRegion(fixedImage->GetLargestPossibleRegion());
  // We will need some intermediate variables in order to pass the
  // matched SIFTs to the deformation field source.
  using LandmarkContainerType = DisplacementSourceType::LandmarkContainer;
  using LandmarkPointType     = DisplacementSourceType::LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks = LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks = LandmarkContainerType::New();

  LandmarkPointType sourcePoint;
  LandmarkPointType targetPoint;
  // We can now iterate through the list of matched points and store
  // them in the intermediate landmark sets.

  unsigned int pointId = 0;

  for (LandmarkListType::Iterator it = landmarkList->Begin(); it != landmarkList->End(); ++it)
  {
    PointType point1 = it.Get()->GetPoint1();
    PointType point2 = it.Get()->GetPoint2();

    sourcePoint[0] = point1[0];
    sourcePoint[1] = point1[1];

    targetPoint[0] = point2[0];
    targetPoint[1] = point2[1];

    sourceLandmarks->InsertElement(pointId, sourcePoint);
    targetLandmarks->InsertElement(pointId, targetPoint);

    ++pointId;
  }
  // We pass the landmarks to the deformer and we run it.
  deformer->SetSourceLandmarks(sourceLandmarks.GetPointer());
  deformer->SetTargetLandmarks(targetLandmarks.GetPointer());

  deformer->UpdateLargestPossibleRegion();

  DisplacementFieldType::ConstPointer deformationField = deformer->GetOutput();

  deformer->Update();

  ImageType::Pointer outdf = ImageType::New();
  outdf->SetRegions(fixedReader->GetOutput()->GetLargestPossibleRegion());
  outdf->Allocate();

  itk::ImageRegionIterator<ImageType> outIt(outdf, outdf->GetLargestPossibleRegion());

  itk::ImageRegionIterator<DisplacementFieldType> inIt(deformer->GetOutput(), deformer->GetOutput()->GetLargestPossibleRegion());
  outIt.GoToBegin();
  inIt.GoToBegin();

  while (!inIt.IsAtEnd() && !outIt.IsAtEnd())
  {
    // std::cout << inIt.Get() << std::endl;

    outIt.Set(inIt.Get()[1]);

    ++inIt;
    ++outIt;
  }

  using RescaleType = itk::RescaleIntensityImageFilter<ImageType, OutputImageType>;

  RescaleType::Pointer rescaler = RescaleType::New();
  rescaler->SetInput(outdf);
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  using WriterType           = otb::ImageFileWriter<OutputImageType>;
  WriterType::Pointer writer = WriterType::New();

  writer->SetInput(rescaler->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  // Figure~\ref{fig:SIFTDME} shows the result of applying the SIFT
  // disparity map estimation. Only the horizontal component of the
  // deformation is shown.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{ROISpot5.eps}
  // \includegraphics[width=0.40\textwidth]{ROISpot5Warped.eps}
  // \includegraphics[width=0.40\textwidth]{SIFTdeformationFieldOutput.eps}
  // \itkcaption[Displacement field from SIFT disparity map estimation]{From left
  // to right and top to bottom: fixed input image, moving image with a deformation,
  // estimated deformation field in the horizontal direction.}
  // \label{fig:SIFTDME}
  // \end{figure}

  return EXIT_SUCCESS;
}
