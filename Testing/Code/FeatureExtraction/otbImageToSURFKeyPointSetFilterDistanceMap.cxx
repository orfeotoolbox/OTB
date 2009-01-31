/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) CS Systemes d'information. All rights reserved.
See CSCopyright.txt for details.

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

#include "otbImageToSURFKeyPointSetFilter.h"
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

OutputImageType::Pointer surf(ImageType::Pointer input,
                              const unsigned int octaves,
                              const unsigned int scales,
                              const char* surfFileName)
{
  typedef otb::ImageToSURFKeyPointSetFilter<ImageType,PointSetType> SurfFilterType;
  typedef itk::PointSetToImageFilter<PointSetType, OutputImageType> PointSetFilterType;

  SurfFilterType::Pointer surf = SurfFilterType::New();
  PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();

  surf->SetInput(0,input);
  surf->SetOctavesNumber(octaves);
  surf->SetScalesNumber(scales);


  pointSetFilter->SetInput(surf->GetOutput());
  pointSetFilter->SetOutsideValue(0);
  pointSetFilter->SetInsideValue(255);
  pointSetFilter->SetSize(input->GetLargestPossibleRegion().GetSize());
  pointSetFilter->SetSpacing(input->GetSpacing());
  pointSetFilter->SetOrigin(input->GetOrigin());
  pointSetFilter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(surfFileName);
  writer->SetInput(pointSetFilter->GetOutput());
  writer->Update();

  return pointSetFilter->GetOutput();
}

OutputImageType::Pointer ddm( OutputImageType::Pointer input,
                              const char* ddmFileName)
{
  typedef itk::DanielssonDistanceMapImageFilter <OutputImageType, OutputImageType> DDMFilterType;
  DDMFilterType::Pointer ddmFilter = DDMFilterType::New();

  ddmFilter->SetInput(input);
  ddmFilter->InputIsBinaryOn();
  ddmFilter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(ddmFileName);
  writer->SetInput(ddmFilter->GetOutput());
  writer->Update();

  return ddmFilter->GetOutput();
}

ImageType::Pointer rotate( ImageType::Pointer input,
                           const unsigned int rotation)
{
  typedef itk::AffineTransform<double, 2> TransformType;
  typedef itk::ResampleImageFilter<ImageType, ImageType>
  ResampleFilterType;
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  TransformType::Pointer transform = TransformType::New();
  TransformType::OutputVectorType translation1;
  TransformType::OutputVectorType translation2;

  const ImageType::SpacingType& spacing = input->GetSpacing();
  const ImageType::PointType& origin  = input->GetOrigin();
  ImageType::SizeType size = input->GetLargestPossibleRegion().GetSize();

  const double imageCenterX = origin[0] + spacing[0] * size[0] / 2.0;
  const double imageCenterY = origin[1] + spacing[1] * size[1] / 2.0;
  const double degreesToRadians = atan(1.0) / 45.0;
  const double angle = rotation * degreesToRadians;

  translation1[0] = -imageCenterX;
  translation1[1] = -imageCenterY;
  translation2[0] = imageCenterX;
  translation2[1] = imageCenterY;

  transform->Translate( translation1 );
  transform->Rotate2D( -angle, false );
  transform->Translate( translation2 );

  resampler->SetOutputOrigin( origin );
  resampler->SetOutputSpacing( spacing );
  resampler->SetSize( size );
  resampler->SetTransform(transform);
  resampler->SetInput(input);
  resampler->Update();
  return resampler->GetOutput();
}

OutputImageType::Pointer invRotate(OutputImageType::Pointer input,
                                   const unsigned int rotation)
{
  typedef itk::AffineTransform<double, 2> TransformType;
  typedef itk::ResampleImageFilter<OutputImageType, OutputImageType>
  ResampleFilterType;
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  TransformType::Pointer transform = TransformType::New();
  TransformType::OutputVectorType translation1;
  TransformType::OutputVectorType translation2;

  const ImageType::SpacingType& spacing = input->GetSpacing();
  const ImageType::PointType& origin  = input->GetOrigin();
  ImageType::SizeType size = input->GetLargestPossibleRegion().GetSize();

  const double imageCenterX = origin[0] + spacing[0] * size[0] / 2.0;
  const double imageCenterY = origin[1] + spacing[1] * size[1] / 2.0;

  const double degreesToRadians = atan(1.0) / 45.0;
  const double angle = rotation * degreesToRadians;

  translation1[0] = -imageCenterX;
  translation1[1] = -imageCenterY;
  translation2[0] = imageCenterX;
  translation2[1] = imageCenterY;

  transform->Translate( translation1 );
  transform->Rotate2D( angle, false );
  transform->Translate( translation2 );

  resampler->SetOutputOrigin( origin );
  resampler->SetOutputSpacing( spacing );
  resampler->SetSize( size );
  resampler->SetTransform(transform);
  resampler->SetInput(input);
  resampler->Update();
  return resampler->GetOutput();
}

ImageType::Pointer zoom( ImageType::Pointer input,
                         const unsigned int zoomFactor)
{
  typedef itk::ShrinkImageFilter<ImageType, ImageType> ShrinkFilterType;
  ShrinkFilterType::Pointer shrink = ShrinkFilterType::New();

  shrink->SetInput(input);
  shrink->SetShrinkFactors(zoomFactor);
  shrink->Update();

  return shrink->GetOutput();
}

OutputImageType::Pointer invZoom( OutputImageType::Pointer input,
                                  const unsigned int zoomFactor)
{
  typedef itk::ExpandImageFilter<OutputImageType, OutputImageType> ExpandFilterType;
  ExpandFilterType::Pointer expand = ExpandFilterType::New();

  expand->SetInput(input);
  expand->SetExpandFactors(zoomFactor);
  expand->Update();

  return expand->GetOutput();
}

ImageType::Pointer contrast(ImageType::Pointer input,
                            const unsigned int contrastMin,
                            const unsigned int contrastMax)
{
  typedef itk::RescaleIntensityImageFilter<ImageType, ImageType> RescaleFilterType;
  RescaleFilterType::Pointer rescaler= RescaleFilterType::New();

  rescaler->SetInput(input);
  rescaler->SetOutputMinimum(static_cast<RescaleFilterType::OutputPixelType>(contrastMin));
  rescaler->SetOutputMaximum(static_cast<RescaleFilterType::OutputPixelType>(contrastMax));
  rescaler->Update();
  return rescaler->GetOutput();
}

OutputImageType::Pointer invContrast( OutputImageType::Pointer input,
                                      const unsigned int contrastMin,
                                      const unsigned int contrastMax)
{
  return input;
}

void subtract(OutputImageType::Pointer image1,
              OutputImageType::Pointer image2,
              const char* subtractFileName)
{
  typedef itk::SubtractImageFilter<OutputImageType, OutputImageType, ImageType> SubtractFilterType;
  typedef itk::MinimumMaximumImageCalculator<ImageType> MaximumCalculatorType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> OutputRescaleFilterType;

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
  writer->SetFileName(subtractFileName);
  writer->SetInput(rescaler->GetOutput());
  writer->Update();

  maximumCalculator->SetImage(subtract->GetOutput());
  maximumCalculator->Compute();

  std::cout << "Mix(sub)= " << maximumCalculator->GetMinimum() << " ";
  std::cout << "Max(sub)= " << maximumCalculator->GetMaximum() << std::endl;
}

int otbImageToSURFKeyPointSetFilterDistanceMap(int argc, char * argv[])
{
  // Input Image file name
  const char * infname = argv[1];

  const unsigned int octaves = atoi(argv[2]);
  const unsigned int scales = atoi(argv[3]);

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

  ImageType::Pointer _rotated =
    rotate(reader->GetOutput(), rotation);
  ImageType::Pointer _zoomed =
    zoom(reader->GetOutput(), zoomFactor);
  ImageType::Pointer _contrasted =
    contrast(reader->GetOutput(), contrastMin, contrastMax);

  ImageType::Pointer _combined1 = zoom(_rotated, zoomFactor);
  ImageType::Pointer _combined2 = contrast(_combined1, contrastMin, contrastMax);

  OutputImageType::Pointer surf_base =
    surf(reader->GetOutput(), octaves, scales, "surf_base.png");

  OutputImageType::Pointer _surf_rotated =
    surf(_rotated, octaves, scales ,"surf_rotated.png");

  OutputImageType::Pointer _surf_zoomed =
    surf(_zoomed, octaves, scales, "surf_zoomed.png");

  OutputImageType::Pointer _surf_contrasted =
    surf(_contrasted, octaves, scales,  "surf_contrasted.png");

  OutputImageType::Pointer _surf_combined =
    surf(_combined2, octaves, scales, "surf_combined.png");

  OutputImageType::Pointer surf_rotated =
    invRotate(_surf_rotated, rotation);

  OutputImageType::Pointer surf_zoomed =
    invZoom(_surf_zoomed, zoomFactor);

  OutputImageType::Pointer surf_contrasted =
    invContrast(_surf_contrasted, contrastMin, contrastMax);

  OutputImageType::Pointer _surf_combined1 =
    invContrast(_surf_combined, contrastMin, contrastMax);
  OutputImageType::Pointer _surf_combined2 =
    invRotate(_surf_combined1, rotation);
  OutputImageType::Pointer surf_combined =
    invZoom(_surf_combined2, zoomFactor);

  OutputImageType::Pointer ddm_base = ddm(surf_base, "ddm_base.png");
  OutputImageType::Pointer ddm_rotated = ddm(surf_rotated, "ddm_rotated.png");
  OutputImageType::Pointer ddm_contrasted = ddm(surf_contrasted, "ddm_contrasted.png");
  OutputImageType::Pointer ddm_zoomed = ddm(surf_zoomed, "ddm_zoomed.png");
  OutputImageType::Pointer ddm_combined = ddm(surf_combined, "ddm_combined.png");

  subtract(ddm_base, ddm_rotated,
           "subtract_rotated.png");

  subtract(ddm_base, ddm_zoomed,
           "subtract_zoomed.png");

  subtract(ddm_base, ddm_contrasted,
           "subtract_contrasted.png");

  subtract(ddm_base, ddm_combined,
           "subtract_combined.png");

  return EXIT_SUCCESS;
}
