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
#include <iostream>
#include <fstream>

#include "itkPointSet.h"
#include "itkImageRegionIterator.h"
#include "itkVariableLengthVector.h"
#include "itkResampleImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkShrinkImageFilter.h"
#include "itkExpandImageFilter.h"
#include "itkPointSetToImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

typedef itk::VariableLengthVector<float> RealVectorType;
typedef itk::PointSet<RealVectorType,2> PointSetType;
typedef otb::Image<float,2> ImageType;
typedef otb::Image<unsigned char, 2> OutputImageType;

// PointSet iterator types
typedef PointSetType::PointsContainer PointsContainerType;
typedef PointsContainerType::Iterator PointsIteratorType;
typedef PointSetType::PointDataContainer PointDataContainerType;
typedef PointDataContainerType::Iterator PointDataIteratorType;

// Filter
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::ImageFileWriter<ImageType> WriterInputType;
typedef otb::ImageToSIFTKeyPointSetFilter<ImageType,PointSetType> SiftFilterType;
typedef itk::DanielssonDistanceMapImageFilter <OutputImageType, OutputImageType> DDMFilterType;
typedef itk::ResampleImageFilter<ImageType, ImageType> ResampleFilterType1;
typedef itk::ResampleImageFilter<OutputImageType, OutputImageType> ResampleFilterType2;
typedef itk::AffineTransform<double, 2> TransformType;  
typedef itk::SubtractImageFilter<OutputImageType, OutputImageType, ImageType> SubtractFilterType;
typedef itk::MinimumMaximumImageCalculator<ImageType> MaximumCalculatorType;
typedef itk::ShrinkImageFilter<ImageType, ImageType> ShrinkFilterType;
typedef itk::ExpandImageFilter<OutputImageType, OutputImageType> ExpandFilterType;
typedef itk::PointSetToImageFilter<PointSetType, OutputImageType> PointSetFilterType;
typedef itk::RescaleIntensityImageFilter<ImageType, ImageType> RescaleFilterType;
typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> OutputRescaleFilterType;

OutputImageType::Pointer base( ImageType::Pointer input,
			       const unsigned octaves,
			       const unsigned int scales,
			       const float threshold,
			       const float ratio)
{
  SiftFilterType::Pointer sift = SiftFilterType::New();
  DDMFilterType::Pointer ddmFilter = DDMFilterType::New();
  PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();
  
  sift->SetInput(0,input);
  sift->SetOctavesNumber(octaves);
  sift->SetScalesNumber(scales);
  sift->SetDoGThreshold(threshold);
  sift->SetEdgeThreshold(ratio);
  
  pointSetFilter->SetInput(sift->GetOutput());
  pointSetFilter->SetInsideValue(255);
  pointSetFilter->SetOutsideValue(0);
  pointSetFilter->SetSize(input->GetLargestPossibleRegion().GetSize());
  pointSetFilter->SetSpacing(input->GetSpacing());
  pointSetFilter->SetOrigin(input->GetOrigin());
  
  ddmFilter->SetInput(pointSetFilter->GetOutput());
  ddmFilter->InputIsBinaryOn();
  ddmFilter->Update();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("base_keys.png");
  writer->SetInput(pointSetFilter->GetOutput());
  writer->Update();
  
  std::cout << "Image base   -> sift key points -> ddm1" << std::endl;
  
  return ddmFilter->GetOutput();
}

OutputImageType::Pointer rotate( ImageType::Pointer input,
				 const unsigned int octaves,
				 const unsigned int scales,
				 const float threshold,
				 const float ratio,
				 const unsigned int rotation)
{
  SiftFilterType::Pointer sift = SiftFilterType::New();
  DDMFilterType::Pointer ddmFilter = DDMFilterType::New();
  PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();
  
  ResampleFilterType1::Pointer resampler1 = ResampleFilterType1::New();
  ResampleFilterType2::Pointer resampler2 = ResampleFilterType2::New();
  
  TransformType::Pointer transform1 = TransformType::New();
  TransformType::Pointer transform2 = TransformType::New();
  
  TransformType::OutputVectorType translation1;
  TransformType::OutputVectorType translation2;
  TransformType::OutputVectorType translation3;
  TransformType::OutputVectorType translation4;

  const ImageType::SpacingType& spacing1 = input->GetSpacing();
  const ImageType::PointType& origin1  = input->GetOrigin();
  ImageType::SizeType size1 = input->GetLargestPossibleRegion().GetSize();
  
  const double imageCenterX1 = origin1[0] + spacing1[0] * size1[0] / 2.0;
  const double imageCenterY1 = origin1[1] + spacing1[1] * size1[1] / 2.0;
  const double degreesToRadians = atan(1.0) / 45.0;
  const double angle = rotation * degreesToRadians;

  translation1[0] = -imageCenterX1;
  translation1[1] = -imageCenterY1;
  translation2[0] = imageCenterX1;
  translation2[1] = imageCenterY1;

  transform1->Translate( translation1 );
  transform1->Rotate2D( -angle, false );
  transform1->Translate( translation2 );

  resampler1->SetOutputOrigin( origin1 );
  resampler1->SetOutputSpacing( spacing1 );
  resampler1->SetSize( size1 );
  resampler1->SetTransform(transform1);
  resampler1->SetInput(input);
  resampler1->Update();
  
  sift->SetInput(0,resampler1->GetOutput());
  sift->SetOctavesNumber(octaves);
  sift->SetScalesNumber(scales);
  sift->SetDoGThreshold(threshold);
  sift->SetEdgeThreshold(ratio);
  
  pointSetFilter->SetInput(sift->GetOutput());
  pointSetFilter->SetInsideValue(255);
  pointSetFilter->SetOutsideValue(0);
  pointSetFilter->SetSize(resampler1->GetOutput()->GetLargestPossibleRegion().GetSize());
  pointSetFilter->SetSpacing(resampler1->GetOutput()->GetSpacing());
  pointSetFilter->SetOrigin(resampler1->GetOutput()->GetOrigin());
  
  pointSetFilter->Update();
  
  const ImageType::SpacingType& spacing2 = pointSetFilter->GetOutput()->GetSpacing();
  const ImageType::PointType& origin2  = pointSetFilter->GetOutput()->GetOrigin();
  ImageType::SizeType size2 = pointSetFilter->GetOutput()->GetLargestPossibleRegion().GetSize();
  
  const double imageCenterX2 = origin2[0] + spacing2[0] * size2[0] / 2.0;
  const double imageCenterY2 = origin2[1] + spacing2[1] * size2[1] / 2.0;

  translation3[0] = -imageCenterX2;
  translation3[1] = -imageCenterY2;
  translation4[0] = imageCenterX2;
  translation4[1] = imageCenterY2;
  
  transform2->Translate( translation3 );
  transform2->Rotate2D( angle, false );
  transform2->Translate( translation4 );
  
  resampler2->SetOutputOrigin( origin2 );
  resampler2->SetOutputSpacing( spacing2 );
  resampler2->SetSize( size2 );
  resampler2->SetTransform(transform2);  
  resampler2->SetInput(pointSetFilter->GetOutput());
  
  ddmFilter->SetInput(resampler2->GetOutput());
  ddmFilter->InputIsBinaryOn();
  ddmFilter->Update();
 
  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetFileName("rotate_keys_a.png");
  writer1->SetInput(pointSetFilter->GetOutput());
  writer1->Update();
  
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName("rotate_keys_b.png");
  writer2->SetInput(resampler2->GetOutput());
  writer2->Update();
  
  std::cout << "Image rotate   -> sift key points -> ddm2" << std::endl;
  return ddmFilter->GetOutput();
}

OutputImageType::Pointer zoom( ImageType::Pointer input,
			       const unsigned int octaves,
			       const unsigned int scales,
			       const float threshold,
			       const float ratio,
			       const unsigned int zoomFactor)
{
  SiftFilterType::Pointer sift = SiftFilterType::New();
  DDMFilterType::Pointer ddmFilter = DDMFilterType::New();
  PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();
  
  ShrinkFilterType::Pointer shrink = ShrinkFilterType::New();
  ExpandFilterType::Pointer expand = ExpandFilterType::New();
  
  shrink->SetInput(input);
  shrink->SetShrinkFactors(zoomFactor);
  shrink->Update();
  
  sift->SetInput(0,shrink->GetOutput());
  sift->SetOctavesNumber(octaves);
  sift->SetScalesNumber(scales);
  sift->SetDoGThreshold(threshold);
  sift->SetEdgeThreshold(ratio);
  
  pointSetFilter->SetInput(sift->GetOutput());
  pointSetFilter->SetInsideValue(255);
  pointSetFilter->SetOutsideValue(0);
  pointSetFilter->SetSize(shrink->GetOutput()->GetLargestPossibleRegion().GetSize());
  pointSetFilter->SetSpacing(shrink->GetOutput()->GetSpacing());
  pointSetFilter->SetOrigin(shrink->GetOutput()->GetOrigin());
  pointSetFilter->Update();
  
  expand->SetInput(pointSetFilter->GetOutput());
  expand->SetExpandFactors(zoomFactor);
  expand->Update();
  
  ddmFilter->SetInput(expand->GetOutput());
  ddmFilter->InputIsBinaryOn();
  ddmFilter->Update();
  
  WriterType::Pointer writer1 = WriterType::New();
  writer1->SetFileName("zoom_keys_a.png");
  writer1->SetInput(pointSetFilter->GetOutput());
  writer1->Update();
  
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName("zoom_keys_b.png");
  writer2->SetInput(expand->GetOutput());
  writer2->Update();
  
  std::cout << "Image zoom   -> sift key points -> ddm3" << std::endl;
  return ddmFilter->GetOutput();
}

OutputImageType::Pointer contrast(ImageType::Pointer input,
				  const unsigned int octaves,
				  const unsigned int scales,
				  const float threshold,
				  const float ratio,
				  const unsigned int contrastMin,
				  const unsigned int contrastMax)
{
  SiftFilterType::Pointer sift = SiftFilterType::New();
  DDMFilterType::Pointer ddmFilter = DDMFilterType::New();
  PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();
  RescaleFilterType::Pointer rescaler= RescaleFilterType::New();
  
  rescaler->SetInput(input);
  rescaler->SetOutputMinimum(static_cast<RescaleFilterType::OutputPixelType>(contrastMin));
  rescaler->SetOutputMaximum(static_cast<RescaleFilterType::OutputPixelType>(contrastMax));
  
  sift->SetInput(0,rescaler->GetOutput());
  sift->SetOctavesNumber(octaves);
  sift->SetScalesNumber(scales);
  sift->SetDoGThreshold(threshold);
  sift->SetEdgeThreshold(ratio);
  
  pointSetFilter->SetInput(sift->GetOutput());
  pointSetFilter->SetInsideValue(255);
  pointSetFilter->SetOutsideValue(0);
  pointSetFilter->SetSize(input->GetLargestPossibleRegion().GetSize());
  pointSetFilter->SetSpacing(input->GetSpacing());
  pointSetFilter->SetOrigin(input->GetOrigin());

  ddmFilter->SetInput(pointSetFilter->GetOutput());
  ddmFilter->InputIsBinaryOn();
  ddmFilter->Update();
  
  std::cout << "Image contrast   -> sift key points -> ddm4" << std::endl;
  return ddmFilter->GetOutput();
}

void combine()
{
  std::cout << "Image combine   -> sift key points -> ddm5" << std::endl;
}

void subtract(OutputImageType::Pointer image1,
	      OutputImageType::Pointer image2)
{
  SubtractFilterType::Pointer subtract = SubtractFilterType::New();
  MaximumCalculatorType::Pointer maximumCalculator = MaximumCalculatorType::New();
  
  subtract->SetInput1(image1);
  subtract->SetInput2(image2);
  subtract->Update();
  
  OutputRescaleFilterType::Pointer rescaler = OutputRescaleFilterType::New();
  rescaler->SetInput(subtract->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("subtract.png");
  writer->SetInput(rescaler->GetOutput());
  writer->Update();
  
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetFileName("ddm1.png");
  writer2->SetInput(image1);
  writer2->Update();

  WriterType::Pointer writer3 = WriterType::New();
  writer3->SetFileName("ddm2.png");
  writer3->SetInput(image2);
  writer3->Update();
  
  maximumCalculator->SetImage(subtract->GetOutput());
  maximumCalculator->ComputeMaximum();
  
  std::cout << "Max(sub)= " << maximumCalculator->GetMaximum() << std::endl;
}

int otbImageToSIFTKeyPointSetFilterDistanceMap(int argc, char * argv[])
{
  // Input Image file name
  const char * infname = argv[1];
  
  const unsigned int octaves = atoi(argv[2]);
  const unsigned int scales = atoi(argv[3]);
  const float threshold = atof(argv[4]);
  const float ratio = atof(argv[5]);
  
  // Rotation angle [0,360[
  const unsigned int rotation = atoi(argv[6]);
  
  // Zoom factor
  const unsigned int zoomFactor = atoi(argv[7]);
  
  // contrast factor
  const unsigned int contrastMin = atoi(argv[8]);
  const unsigned int contrastMax = atoi(argv[9]);
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  
  OutputImageType::Pointer ddm1 = base(reader->GetOutput(),
				       octaves, scales,
				       threshold, ratio);
  
  OutputImageType::Pointer ddm2 = rotate(reader->GetOutput(),
					 octaves, scales,
					 threshold, ratio,
					 rotation);
  
  OutputImageType::Pointer ddm3 = zoom(reader->GetOutput(),
				       octaves, scales,
				       threshold, ratio,
				       zoomFactor);
  
  OutputImageType::Pointer ddm4 = contrast(reader->GetOutput(),
					   octaves, scales,
					   threshold, ratio,
					   contrastMin, contrastMax);
  
  subtract(ddm1, ddm2);
  subtract(ddm1, ddm3);
  subtract(ddm1, ddm4);
  
  return EXIT_SUCCESS;
}
