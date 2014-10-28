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
//    INPUTS: {ROISpot5.png}, {ROISpot5Warped.png}
//    OUTPUTS: {SIFTdeformationFieldOutput.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
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
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbKeyPointSetsMatchingFilter.h"
#include "otbSiftFastImageFilter.h"
#include "itkLandmarkDisplacementFieldSource.h"
// Software Guide : EndCodeSnippet


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
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

  // Software Guide : BeginLatex
  //
  // Then we must decide what pixel type to use for the image. We choose to do
  // all the computations in floating point precision and rescale the results
  // between 0 and 255 in order to export PNG images.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double        RealType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<RealType, Dimension>        ImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The SIFTs obtained for the matching will be stored in vector
// form inside a point set. So we need the following types:
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::VariableLengthVector<RealType>      RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension> PointSetType;
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The filter for computing the SIFTs has a type defined as follows:
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::SiftFastImageFilter<ImageType, PointSetType>
  ImageToSIFTKeyPointSetFilterType;
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Although many choices for evaluating the distances during the
// matching procedure exist, we choose here to use a simple
// Euclidean distance. We can then define the type for the matching filter.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Statistics::EuclideanDistanceMetric<RealVectorType> DistanceType;
  typedef otb::KeyPointSetsMatchingFilter<PointSetType, DistanceType>
  EuclideanDistanceMetricMatchingFilterType;
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The following types are needed for dealing with the matched points.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef PointSetType::PointType         PointType;
  typedef std::pair<PointType, PointType> MatchType;
  typedef std::vector<MatchType>          MatchVectorType;
  typedef EuclideanDistanceMetricMatchingFilterType::LandmarkListType
  LandmarkListType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We define the type for the image reader.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Two readers are instantiated : one for the fixed image, and one
  // for the moving image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer fixedReader = ReaderType::New();
  ReaderType::Pointer movingReader = ReaderType::New();

  fixedReader->SetFileName(argv[1]);
  movingReader->SetFileName(argv[2]);
  fixedReader->UpdateOutputInformation();
  movingReader->UpdateOutputInformation();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We will now instantiate the 2 SIFT filters and the filter used
  // for the matching of the points.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageToSIFTKeyPointSetFilterType::Pointer filter1 =
    ImageToSIFTKeyPointSetFilterType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter2 =
    ImageToSIFTKeyPointSetFilterType::New();
  EuclideanDistanceMetricMatchingFilterType::Pointer euclideanMatcher =
    EuclideanDistanceMetricMatchingFilterType::New();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We plug the pipeline and set the parameters.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double secondOrderThreshold = 0.5;
  bool   useBackMatching = 0;

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
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The matched points will be stored into a landmark list.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  LandmarkListType::Pointer landmarkList;
  landmarkList = euclideanMatcher->GetOutput();
  // Software Guide : EndCodeSnippet
  MatchVectorType trueSecondOrder;

  for (LandmarkListType::Iterator it = landmarkList->Begin();
       it != landmarkList->End(); ++it)
    {
    PointType point1 = it.Get()->GetPoint1();
    PointType point2 = it.Get()->GetPoint2();

    trueSecondOrder.push_back(MatchType(point1, point2));
    }

  // Displaying the matches
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType>
  PrintableFilterType;

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

  typedef otb::Image<itk::RGBPixel<unsigned char>, 2> RGBImageType;

  RGBImageType::Pointer rgbimage1 = RGBImageType::New();
  rgbimage1->SetRegions(printable1->GetOutput()->GetLargestPossibleRegion());
  rgbimage1->Allocate();
  itk::ImageRegionIterator<RGBImageType> outIt1(
    rgbimage1,
    rgbimage1->
    GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType> inIt1(
    printable1->GetOutput(), printable1->GetOutput()->GetLargestPossibleRegion());
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
  itk::ImageRegionIterator<RGBImageType>
  outIt2(rgbimage2, rgbimage2->GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType>
  inIt2(printable2->GetOutput(),
        printable2->GetOutput()->GetLargestPossibleRegion());
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

  // Software Guide : BeginLatex
  //
  // The landmarks are used for building a deformation field. The
  // deformation field is an image of vectors created by the
  // \doxygen{itk}{DisplacementFieldSource} class.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef   itk::Vector<RealType, Dimension>   VectorType;
  typedef   otb::Image<VectorType,  Dimension> DisplacementFieldType;

  typedef itk::LandmarkDisplacementFieldSource<DisplacementFieldType>  DisplacementSourceType;

  DisplacementSourceType::Pointer deformer = DisplacementSourceType::New();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // The deformation field needs information about the extent and
  // spacing of the images on which it is defined.
  //
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  ImageType::ConstPointer fixedImage = fixedReader->GetOutput();

  deformer->SetOutputSpacing(fixedImage->GetSpacing());
  deformer->SetOutputOrigin(fixedImage->GetOrigin());
  deformer->SetOutputRegion(fixedImage->GetLargestPossibleRegion());
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We will need some intermediate variables in order to pass the
  // matched SIFTs to the deformation field source.
  //
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef DisplacementSourceType::LandmarkContainer
  LandmarkContainerType;
  typedef DisplacementSourceType::LandmarkPointType LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks =
    LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks =
    LandmarkContainerType::New();

  LandmarkPointType sourcePoint;
  LandmarkPointType targetPoint;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now iterate through the list of matched points and store
// them in the intermediate landmark sets.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  unsigned int pointId = 0;

  for (LandmarkListType::Iterator it = landmarkList->Begin();
       it != landmarkList->End(); ++it)
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
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We pass the landmarks to the deformer and we run it.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  deformer->SetSourceLandmarks(sourceLandmarks.GetPointer());
  deformer->SetTargetLandmarks(targetLandmarks.GetPointer());

  deformer->UpdateLargestPossibleRegion();

  DisplacementFieldType::ConstPointer deformationField =
    deformer->GetOutput();

  deformer->Update();
  // Software Guide : EndCodeSnippet

  ImageType::Pointer outdf = ImageType::New();
  outdf->SetRegions(fixedReader->GetOutput()->GetLargestPossibleRegion());
  outdf->Allocate();

  itk::ImageRegionIterator<ImageType> outIt(outdf,
                                            outdf->GetLargestPossibleRegion());

  itk::ImageRegionIterator<DisplacementFieldType> inIt(
    deformer->GetOutput(), deformer->GetOutput()->GetLargestPossibleRegion());
  outIt.GoToBegin();
  inIt.GoToBegin();

  while (!inIt.IsAtEnd() && !outIt.IsAtEnd())
    {
    //std::cout << inIt.Get() << std::endl;

    outIt.Set(inIt.Get()[1]);

    ++inIt;
    ++outIt;
    }

  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputImageType> RescaleType;

  RescaleType::Pointer rescaler = RescaleType::New();
  rescaler->SetInput(outdf);
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetInput(rescaler->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  // Software Guide : BeginLatex
  //
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
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
