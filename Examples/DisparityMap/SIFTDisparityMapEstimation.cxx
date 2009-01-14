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
#include "otbSiftFastImageFilter.h"
#include "itkDeformationFieldSource.h"
#include "itkWarpImageFilter.h"

// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkPointSet.h"


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
  EuclideanDistanceMatchingFilterType::Pointer euclideanMatcher =
                           EuclideanDistanceMatchingFilterType::New();
  // Software Guide : EndCodeSnippet
  // Software Guide : BeginLatex
  //
  // We plug the pipeline and set the parameters.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet

  double secondOrderThreshold = 0.5;
  bool useBackMatching = 0;

  filter1->SetInput( fixedReader->GetOutput() );
  filter2->SetInput( movingReader->GetOutput() );
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
  itk::ImageRegionIterator<RGBImageType> outIt2(rgbimage2,rgbimage2->GetLargestPossibleRegion());
  itk::ImageRegionIterator<OutputImageType> inIt2(printable2->GetOutput(),printable2->GetOutput()->GetLargestPossibleRegion());
  outIt2.GoToBegin();
  inIt2.GoToBegin();
  while(!inIt2.IsAtEnd() && !outIt2.IsAtEnd())
    {
      itk::RGBPixel<unsigned char> pixel;
      pixel.SetRed(inIt2.Get());
      pixel.SetGreen(inIt2.Get());
      pixel.SetBlue(inIt2.Get());
      outIt2.Set(pixel);
      ++inIt2;
      ++outIt2;
    }



  typedef   float          VectorComponentType;
  typedef   itk::Vector< VectorComponentType, Dimension >    VectorType;
  typedef   otb::Image< VectorType,  Dimension >   DeformationFieldType;

  typedef itk::DeformationFieldSource<
                                DeformationFieldType 
                                             >  DeformationSourceType;

  DeformationSourceType::Pointer deformer = DeformationSourceType::New();

  ImageType::ConstPointer fixedImage = fixedReader->GetOutput();


  deformer->SetOutputSpacing( fixedImage->GetSpacing() );
  deformer->SetOutputOrigin(  fixedImage->GetOrigin() );
  deformer->SetOutputRegion(  fixedImage->GetLargestPossibleRegion() );

  typedef DeformationSourceType::LandmarkContainerPointer
                                              LandmarkContainerPointer;
  typedef DeformationSourceType::LandmarkContainer
                                                 LandmarkContainerType;
  typedef DeformationSourceType::LandmarkPointType          LandmarkPointType;

  LandmarkContainerType::Pointer sourceLandmarks = LandmarkContainerType::New();
  LandmarkContainerType::Pointer targetLandmarks = LandmarkContainerType::New();

  LandmarkPointType sourcePoint;
  LandmarkPointType targetPoint;

  
  unsigned int pointId = 0;


    for(LandmarkListType::Iterator it = landmarkList->Begin(); it != landmarkList->End();++it)
    {
      PointType point1 = it.Get()->GetPoint1();
      PointType point2 = it.Get()->GetPoint2();
      
      sourcePoint[0] = point1[0];
      sourcePoint[1] = point1[1];



      targetPoint[0] = point2[0];
      targetPoint[1] = point2[1];
    
      sourceLandmarks->InsertElement( pointId, sourcePoint );
      targetLandmarks->InsertElement( pointId, targetPoint );

      ++pointId;
    }


  deformer->SetSourceLandmarks( sourceLandmarks.GetPointer() );
  deformer->SetTargetLandmarks( targetLandmarks.GetPointer() );

  try
    {
    deformer->UpdateLargestPossibleRegion();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  DeformationFieldType::ConstPointer deformationField = deformer->GetOutput();

  deformer->Update();

  std::cout << "Image Resampling" << std::endl;
  typedef itk::WarpImageFilter< ImageType, 
                                ImageType, 
                                DeformationFieldType  >  FilterType;

  FilterType::Pointer warper = FilterType::New();

  typedef itk::LinearInterpolateImageFunction< 
                       ImageType, double >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  warper->SetInterpolator( interpolator );


  warper->SetOutputSpacing( deformationField->GetSpacing() );
  warper->SetOutputOrigin(  deformationField->GetOrigin() );

  warper->SetDeformationField( deformationField );

  warper->SetInput( movingReader->GetOutput() );


  typedef otb::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer movingWriter = WriterType::New();

  movingWriter->SetFileName( argv[6] );
  movingWriter->SetInput( warper->GetOutput() );


  
  try
    {
    movingWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::ImageFileWriter< DeformationFieldType > DeformationWriterType;

  DeformationWriterType::Pointer defWriter = DeformationWriterType::New();

  defWriter->SetFileName( "deformation.mhd" );
  defWriter->SetInput( deformationField );
  defWriter->Update();
  



  return EXIT_SUCCESS;
}

