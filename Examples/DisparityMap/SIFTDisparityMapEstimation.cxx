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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_IKO_PAN_LesHalles_pos_spacing.tif}, {ROI_IKO_PAN_LesHalles_warped_pos_spacing.tif}
//    OUTPUTS: {SIFTdeformationFieldOutput.png},{SIFTresampledMovingOutput.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the
// \doxygen{otb}{KeyPointSetsMatchingFilter} for disparity map
// estimation. The idea here is to match SIFTs extracted from both the
// fixed and the moving images. The use of SIFTs is demonstrated in
// section \ref{sec:SIFTDetector}. The
// \doxygen{otb}{NearestPointDeformationFieldGenerator} will be used
// to generate a deformation field by using nearest neighbor
// interpolation on the deformation values from the point set. More
// advanced methods for deformation field interpolation are also
// available.
//
// The first step toward the use of these filters is to include the proper header files.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet

#include "otbKeyPointSetsMatchingFilter.h"
#include "otbNearestPointDeformationFieldGenerator.h"
#include "itkWarpImageFilter.h"

// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

int main (int argc, char* argv[])
{
  if(argc!= 5)
    {
      std::cerr <<"Usage: "<<argv[0];
      std::cerr<<"fixedFileName movingFileName fieldOutName imageOutName " << std::endl;
      return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;

   // Software Guide : BeginLatex
   //
   // Then we must decide what pixel type to use for the image. We choose to do
   // all the computation in floating point precision and rescale the results
   // between 0 and 255 in order to export PNG images.
   //
   // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef double        RealType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<RealType,Dimension> ImageType;
  typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

  // Software Guide : EndCodeSnippet
 // Software Guide : BeginLatex
   //
   // The SIFTs obtained for the matching will be stored in vector
   // form inside a point set. So we need the following types:
   //
   // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  
  typedef itk::VariableLengthVector<RealType> RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;

    // Software Guide : EndCodeSnippet
 // Software Guide : BeginLatex
   //
   // The filter for computing the SIFTs has a type defined as follows:
   //
   // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef otb::SiftFastImageFilter<ImageType,PointSetType> ImageToSIFTKeyPointSetFilterType;

  // Software Guide : EndCodeSnippet
 // Software Guide : BeginLatex
   //
   // Although many choices for evaluating the distances during the
   // matching procedure exist, we choose here to use a simple
   // Euclidean distance. We can then define the type for the matching filter.
   //
   // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef itk::Statistics::EuclideanDistance<RealVectorType> DistanceType;
  typedef otb::KeyPointSetsMatchingFilter<PointSetType, DistanceType>
                                      EuclideanDistanceMatchingFilterType;
    // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
   //
   // The following types are needed for dealing with the matched points.
   //
   // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef PointSetType::PointType PointType;
  typedef std::pair<PointType,PointType> MatchType;
  typedef std::vector<MatchType> MatchVectorType;
  typedef EuclideanDistanceMatchingFilterType::LandmarkListType LandmarkListType;

  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;
  
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
  // Two readers are instantiated : one for the fixed image, and one for the moving image.
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




  ImageToSIFTKeyPointSetFilterType::Pointer filter1 = ImageToSIFTKeyPointSetFilterType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter2 = ImageToSIFTKeyPointSetFilterType::New();
  EuclideanDistanceMatchingFilterType::Pointer euclideanMatcher = EuclideanDistanceMatchingFilterType::New();


  
  euclideanMatcher->SetInput1(filter1->GetOutput());
  euclideanMatcher->SetInput2(filter2->GetOutput());
  euclideanMatcher->SetDistanceThreshold(secondOrderThreshold);
  euclideanMatcher->SetUseBackMatching(useBackMatching);


    LandmarkListType::Pointer landmarkList;

          euclideanMatcher->Update();

	    MatchVectorType trueSecondOrder;

  for(LandmarkListType::Iterator it = landmarkList->Begin(); it != landmarkList->End();++it)
    {
      PointType point1 = it.Get()->GetPoint1();
      PointType point2 = it.Get()->GetPoint2();

      trueSecondOrder.push_back(MatchType(point1,point2));
    }

    // Displaying the matches
  typedef otb::PrintableImageFilter<VectorImageType> PrintableFilterType;
  typedef PrintableFilterType::OutputImageType OutputImageType;

  PrintableFilterType::Pointer printable1 = PrintableFilterType::New();
  PrintableFilterType::Pointer printable2 = PrintableFilterType::New();
    
  printable1->SetInput(inputImage1);
  printable1->SetChannel(3);
  printable1->SetChannel(2);
  printable1->SetChannel(1);
  printable1->Update();

  printable2->SetInput(inputImage2);
  printable2->SetChannel(3);
  printable2->SetChannel(2);
  printable2->SetChannel(1);
  printable2->Update();

  // Always the same VariableLenghtVector compatibility problem ...

  typedef otb::Image<itk::RGBPixel<unsigned char>,2> RGBImageType;

  RGBImageType::Pointer rgbimage1 = RGBImageType::New();
  rgbimage1->SetRegions(printable1->GetOutput()->GetLargestPossibleRegion());
  rgbimage1->Allocate();
  itk::ImageRegionIterator<RGBImageType> outIt1(rgbimage1,rgbimage1->GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType> inIt1(printable1->GetOutput(),printable1->GetOutput()->GetLargestPossibleRegion());
  outIt1.GoToBegin();
  inIt1.GoToBegin();
  while(!inIt1.IsAtEnd() && !outIt1.IsAtEnd())
    {
      itk::RGBPixel<unsigned char> pixel;
      pixel.SetRed(inIt1.Get()[0]);
      pixel.SetGreen(inIt1.Get()[1]);
      pixel.SetBlue(inIt1.Get()[2]);
      outIt1.Set(pixel);
      ++inIt1;
      ++outIt1;
    }

  RGBImageType::Pointer rgbimage2 = RGBImageType::New();
  rgbimage2->SetRegions(printable2->GetOutput()->GetLargestPossibleRegion());
  rgbimage2->Allocate();
  itk::ImageRegionIterator<RGBImageType> outIt2(rgbimage2,rgbimage2->GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType> inIt2(printable2->GetOutput(),printable2->GetOutput()->GetLargestPossibleRegion());
  outIt2.GoToBegin();
  inIt2.GoToBegin();
  while(!inIt2.IsAtEnd() && !outIt2.IsAtEnd())
    {
      itk::RGBPixel<unsigned char> pixel;
      pixel.SetRed(inIt2.Get()[0]);
      pixel.SetGreen(inIt2.Get()[1]);
      pixel.SetBlue(inIt2.Get()[2]);
      outIt2.Set(pixel);
      ++inIt2;
      ++outIt2;
    }





  
  // Software Guide : BeginLatex
  //
  // Once the estimation has been performed by the \doxygen{otb}{DisparityMapEstimationMethod}, one can generate
  // the associated deformation field (that means translation in first and second image direction).
  // It will be represented as a \doxygen{otb}{VectorImage}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef otb::VectorImage<PixelType,Dimension> DeformationFieldType;

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // For the deformation field estimation, we will use the \doxygen{otb}{NearestPointDeformationFieldGenerator}.
  // This filter will perform a nearest neighbor interpolation on the deformation values in the point set data.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef otb::NearestPointDeformationFieldGenerator<PointSetType,
               DeformationFieldType> GeneratorType;

  // Software GUide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The disparity map estimation filter is instanciated.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  GeneratorType::Pointer generator = GeneratorType::New();

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We must then specify the input point set using the \code{SetPointSet()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  generator->SetPointSet(dmestimator->GetOutput());

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // One must also specify the origin, size and spacing of the output deformation field.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  generator->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  generator->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());
  generator->SetOutputSize(fixedReader->GetOutput()
			   ->GetLargestPossibleRegion().GetSize());

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The local registration process can lead to wrong deformation values and transform parameters. To Select only
  // points in point set for which the registration process was succesful, one can set a threshold on the final metric
  // value : points for which the absolute final metric value is below this threshold will be discarded. This
  // threshold can be set with the \code{SetMetricThreshold()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  generator->SetMetricThreshold(atof(argv[11]));

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // \relatedClasses
  // \begin{itemize}
  // \item \doxygen{otb}{NNearestPointsLinearInterpolateDeformationFieldGenerator}
  // \item \doxygen{otb}{BSplinesInterpolateDeformationFieldGenerator}
  // \item \doxygen{otb}{NearestTransformDeformationFieldGenerator}
  // \item \doxygen{otb}{NNearestTransformsLinearInterpolateDeformationFieldGenerator}
  // \item \doxygen{otb}{BSplinesInterpolateTransformDeformationFieldGenerator}
  // \end{itemize}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginLatex
  //
  // Now we can warp our fixed image according to the estimated deformation field. This will be performed by the
  // \doxygen{itk}{WarpImageFilter}. First, we define this filter.
  //
  // Software Guide : EndLatex

  // Software Guide :  BeginCodeSnippet

  typedef itk::WarpImageFilter<ImageType,ImageType,
               DeformationFieldType> ImageWarperType;

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Then we instantiate it.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  ImageWarperType::Pointer warper = ImageWarperType::New();

  // Software Guide : EndCodeSnippet

  // Software Guide  : BeginLatex
  //
  // We set the input image to warp using the \code{SetInput()} method, and the deformation field
  // using the \code{SetDeformationField()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  warper->SetInput(movingReader->GetOutput());
  warper->SetDeformationField(generator->GetOutput());
  warper->SetOutputOrigin(fixedReader->GetOutput()->GetOrigin());
  warper->SetOutputSpacing(fixedReader->GetOutput()->GetSpacing());

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // In order to write the result to a PNG file, we will rescale it on a proper range.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef itk::RescaleIntensityImageFilter<ImageType,
               OutputImageType> RescalerType;

  RescalerType::Pointer outputRescaler = RescalerType::New();
  outputRescaler->SetInput(warper->GetOutput());
  outputRescaler->SetOutputMaximum(255);
  outputRescaler->SetOutputMinimum(0);

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now write the image to a file. The filters are executed by invoking
  // the \code{Update()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer outputWriter = WriterType::New();
  outputWriter->SetInput(outputRescaler->GetOutput());
  outputWriter->SetFileName(argv[4]);
  outputWriter->Update();

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We also want to write the deformation field along the first direction to a file.
  // To achieve this we will use the \doxygen{otb}{MultiToMonoChannelExtractROI} filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  typedef otb::MultiToMonoChannelExtractROI<PixelType,
               PixelType> ChannelExtractionFilterType;

  ChannelExtractionFilterType::Pointer channelExtractor
    = ChannelExtractionFilterType::New();

  channelExtractor->SetInput(generator->GetOutput());
  channelExtractor->SetChannel(1);

  RescalerType::Pointer fieldRescaler = RescalerType::New();
  fieldRescaler->SetInput(channelExtractor->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);

  WriterType::Pointer fieldWriter = WriterType::New();
  fieldWriter->SetInput(fieldRescaler->GetOutput());
  fieldWriter->SetFileName(argv[3]);
  fieldWriter->Update();

  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:SIMPLEDISPARITYMAPESTIMATIONOUTPUT} shows the result of applying disparity map estimation on
  // a regular point set, followed by deformation field estimation and fixed image resampling on an Ikonos image.
  // The moving image is the fixed image warped with a sinusoidal deformation with a 3-pixels amplitude and a 170-pixels
  // period. Please note that there are more efficient ways to interpolate the deformation field than nearest neighbor,
  // including BSplines fitting.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{ROI_IKO_PAN_LesHalles_pos_spacing.eps}
  // \includegraphics[width=0.40\textwidth]{ROI_IKO_PAN_LesHalles_warped_pos_spacing.eps}
  // \includegraphics[width=0.40\textwidth]{SIFTdeformationFieldOutput.eps}
  // \includegraphics[width=0.40\textwidth]{SIFTresampledMovingOutput.eps}
  // \itkcaption[Deformation field and resampling from disparity map estimation]{From left
  // to right and top to bottom: fixed input image, moving image with a sinusoid deformation,
  // estimated deformation field in the horizontal direction, resampled moving image.}
  // \label{fig:SIMPLEDISPARITYMAPESTIMATIONOUTPUT}
  // \end{figure}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}

