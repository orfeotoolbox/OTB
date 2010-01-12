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
//    INPUTS: {ROISpot5.png}, {ROISpot5Moving.png}
//    OUTPUTS: {SIFTResampling.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the
// \doxygen{otb}{KeyPointSetsMatchingFilter} for disparity map
// estimation. The idea here is to match SIFTs extracted from both the
// fixed and the moving images. The use of SIFTs is demonstrated in
// section \ref{sec:SIFTDetector}. The
// \doxygen{itk}{DeformationFieldSource} will be used
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
// Disabling deprecation warning if on visual
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
#include "itkDeformationFieldSource.h"
// Enabling remaining deprecation warning
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "itkWarpImageFilter.h"

// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkPointSet.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbLeastSquareAffineTransformEstimator.h"
#include "itkResampleImageFilter.h"

int main (int argc, char* argv[])
{
  if (argc!= 4)
  {
    std::cerr <<"Usage: "<<argv[0];
    std::cerr<<"fixedFileName movingFileName resamplingImageFileName" << std::endl;
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
//typedef float        RealType;
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

  typedef otb::SiftFastImageFilter<ImageType,PointSetType>
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
  typedef EuclideanDistanceMatchingFilterType::LandmarkListType
  LandmarkListType;

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

  filter1->SetInput(0, fixedReader->GetOutput() );
  filter1->SetScalesNumber(3);
  filter2->SetInput(0, movingReader->GetOutput() );
  filter2->SetScalesNumber(3);

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

 // Software Guide : BeginLatex
  //
  // Apply Mean square algorithm
  //
  // Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef itk::Point<double,2>                                 MyPointType;
  typedef otb::LeastSquareAffineTransformEstimator<MyPointType> EstimatorType;
  
  // instantiation
  EstimatorType::Pointer estimator = EstimatorType::New();

  for (LandmarkListType::Iterator it = landmarkList->Begin();
     it != landmarkList->End(); ++it)
  {
          estimator->AddTiePoints(it.Get()->GetPoint1(),it.Get()->GetPoint2());
  }

  // Trigger computation
  estimator->Compute();
  // meanSquarestimator->SetInput(euclideanMatcher->GetOutput());
// Software Guide : EndCodeSnippet

 // Software Guide : BeginLatex
  //
  // Resample the initial image with the transformation evaluated
  //
  // Software Guide : EndLatex
//  Software Guide : BeginLatex
  //
  //  It is common, as the last step of a registration task, to use
  //  the resulting transform to map the moving image into the fixed
  //  image space.  This is easily done with the
  //  \doxygen{itk}{ResampleImageFilter}. First, a ResampleImageFilter
  //  type is instantiated using the image types. It is convenient to
  //  use the fixed image type as the output type since it is likely
  //  that the transformed moving image will be compared with the
  //  fixed image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::ResampleImageFilter<
  ImageType,
  OutputImageType >    ResampleFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  A resampling filter is created and the moving image is connected as
  //  its input.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  resampler->SetInput( movingReader->GetOutput() );

  //typedef itk::ImageRegistrationMethod<
  //ImageType,
  //ImageType >    RegistrationType;
  
  //RegistrationType::Pointer   registration  = RegistrationType::New();

  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The Transform that is produced as output need to be inversed to 
  //  We apply here the resampling algorithm to the "fixed" image 
  // to produce the moving image. Or apply to the moving image
  //
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  // Get the output transform 
  //EstimatorType::AffineTransformPointerType transform = estimator->GetAffineTransform();
  resampler->SetTransform( estimator->GetAffineTransform() );
  resampler->SetSize( fixedReader->GetOutput()->GetLargestPossibleRegion().GetSize() );
  resampler->SetOutputOrigin( fixedReader->GetOutput()->GetOrigin() );
  resampler->SetOutputSpacing( fixedReader->GetOutput()->GetSpacing() );
  resampler->SetDefaultPixelValue( 100 );

  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetInput( resampler->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  return EXIT_SUCCESS;
}

