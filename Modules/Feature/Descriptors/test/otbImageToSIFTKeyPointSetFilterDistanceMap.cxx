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
#include "itkVariableLengthVector.h"
#include "itkResampleImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkPointSetToImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkAffineTransform.h"

typedef itk::VariableLengthVector<float> RealVectorType;
typedef itk::PointSet<RealVectorType, 2> PointSetType;
typedef otb::Image<float, 2>             ImageType;
typedef otb::Image<unsigned char, 2>     OutputImageType;

// PointSet iterator types
typedef PointSetType::PointsContainer    PointsContainerType;
typedef PointsContainerType::Iterator    PointsIteratorType;
typedef PointSetType::PointDataContainer PointDataContainerType;
typedef PointDataContainerType::Iterator PointDataIteratorType;

// Filter
typedef otb::ImageFileReader<ImageType>       ReaderType;
typedef otb::ImageFileWriter<OutputImageType> WriterType;
typedef otb::ImageFileWriter<ImageType>       WriterInputType;

OutputImageType::Pointer sift(ImageType::Pointer input,
                              const unsigned int octaves,
                              const unsigned int scales,
                              const float threshold,
                              const float ratio,
                              const char* siftFileName)
{
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType> SiftFilterType;
  typedef itk::PointSetToImageFilter<PointSetType, OutputImageType>  PointSetFilterType;

  SiftFilterType::Pointer     sift = SiftFilterType::New();
  PointSetFilterType::Pointer pointSetFilter = PointSetFilterType::New();

  sift->SetInput(input);
  sift->SetOctavesNumber(octaves);
  sift->SetScalesNumber(scales);
  sift->SetDoGThreshold(threshold);
  sift->SetEdgeThreshold(ratio);

  pointSetFilter->SetInput(sift->GetOutput());
  pointSetFilter->SetOutsideValue(0);
  pointSetFilter->SetInsideValue(255);
  pointSetFilter->SetSize(input->GetLargestPossibleRegion().GetSize());
  pointSetFilter->SetSpacing(input->GetSpacing());
  pointSetFilter->SetOrigin(input->GetOrigin());
  pointSetFilter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(siftFileName);
  writer->SetInput(pointSetFilter->GetOutput());
  //writer->Update();

  return pointSetFilter->GetOutput();
}

OutputImageType::Pointer ddm(OutputImageType::Pointer input,
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
  //writer->Update();

  return ddmFilter->GetOutput();
}

ImageType::Pointer rotate(ImageType::Pointer input,
                          const unsigned int rotation)
{
  typedef itk::AffineTransform<double, 2> TransformType;
  typedef itk::ResampleImageFilter<ImageType, ImageType>
  ResampleFilterType;
  ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  TransformType::Pointer          transform = TransformType::New();
  TransformType::OutputVectorType translation1;
  TransformType::OutputVectorType translation2;

  const ImageType::SpacingType& spacing = input->GetSpacing();
  const ImageType::PointType&   origin  = input->GetOrigin();
  ImageType::SizeType           size = input->GetLargestPossibleRegion().GetSize();

  const double imageCenterX = origin[0] + spacing[0] * size[0] / 2.0;
  const double imageCenterY = origin[1] + spacing[1] * size[1] / 2.0;
  const double degreesToRadians = atan(1.0) / 45.0;
  const double angle = rotation * degreesToRadians;

  translation1[0] = -imageCenterX;
  translation1[1] = -imageCenterY;
  translation2[0] = imageCenterX;
  translation2[1] = imageCenterY;

  transform->Translate(translation1);
  transform->Rotate2D(-angle, false);
  transform->Translate(translation2);

  resampler->SetOutputOrigin(origin);
  resampler->SetOutputSpacing(spacing);
  resampler->SetSize(size);
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

  TransformType::Pointer          transform = TransformType::New();
  TransformType::OutputVectorType translation1;
  TransformType::OutputVectorType translation2;

  const ImageType::SpacingType& spacing = input->GetSpacing();
  const ImageType::PointType&   origin  = input->GetOrigin();
  ImageType::SizeType           size = input->GetLargestPossibleRegion().GetSize();

  const double imageCenterX = origin[0] + spacing[0] * size[0] / 2.0;
  const double imageCenterY = origin[1] + spacing[1] * size[1] / 2.0;

  const double degreesToRadians = atan(1.0) / 45.0;
  const double angle = rotation * degreesToRadians;

  translation1[0] = -imageCenterX;
  translation1[1] = -imageCenterY;
  translation2[0] = imageCenterX;
  translation2[1] = imageCenterY;

  transform->Translate(translation1);
  transform->Rotate2D(angle, false);
  transform->Translate(translation2);

  resampler->SetOutputOrigin(origin);
  resampler->SetOutputSpacing(spacing);
  resampler->SetSize(size);
  resampler->SetTransform(transform);
  resampler->SetInput(input);
  resampler->Update();
  return resampler->GetOutput();
}

ImageType::Pointer zoom(ImageType::Pointer input,
                        const unsigned int zoomFactor)
{
  typedef itk::ShrinkImageFilter<ImageType, ImageType> ShrinkFilterType;
  ShrinkFilterType::Pointer shrink = ShrinkFilterType::New();

  shrink->SetInput(input);
  shrink->SetShrinkFactors(zoomFactor);
  shrink->Update();

  return shrink->GetOutput();
}

OutputImageType::Pointer invZoom(OutputImageType::Pointer input,
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
  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();

  rescaler->SetInput(input);
  rescaler->SetOutputMinimum(static_cast<RescaleFilterType::OutputPixelType>(contrastMin));
  rescaler->SetOutputMaximum(static_cast<RescaleFilterType::OutputPixelType>(contrastMax));
  rescaler->Update();
  return rescaler->GetOutput();
}

OutputImageType::Pointer invContrast(OutputImageType::Pointer input,
                                     const unsigned int itkNotUsed(contrastMin),
                                     const unsigned int itkNotUsed(contrastMax))
{
  return input;
}

void subtract(OutputImageType::Pointer image1,
              OutputImageType::Pointer image2,
              const char* subtractFileName)
{
  typedef itk::SubtractImageFilter<OutputImageType, OutputImageType, ImageType> SubtractFilterType;
  typedef itk::MinimumMaximumImageCalculator<ImageType>                         MaximumCalculatorType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType>          OutputRescaleFilterType;

  SubtractFilterType::Pointer    subtract = SubtractFilterType::New();
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
  //writer->Update();

  maximumCalculator->SetImage(subtract->GetOutput());
  maximumCalculator->Compute();

  std::cout << "Mix(sub)= " << maximumCalculator->GetMinimum() << " ";
  std::cout << "Max(sub)= " << maximumCalculator->GetMaximum() << std::endl;
}

int otbImageToSIFTKeyPointSetFilterDistanceMap(int argc, char * argv[])
{

  if (argc < 10)
    {
    std::cout << "Missing arguments " << std::endl;
    return EXIT_FAILURE;
    }

  // Input Image file name
  const char * infname = argv[1];
  const char * outfname = argv[10];

  const unsigned int octaves = atoi(argv[2]);
  const unsigned int scales = atoi(argv[3]);
  const float        threshold = atof(argv[4]);
  const float        ratio = atof(argv[5]);

  // Rotation angle [0, 360[
  const unsigned int rotation = atoi(argv[6]);

  // Zoom factor
  const unsigned int zoomFactor = atoi(argv[7]);

  // contrast factor
  const unsigned int contrastMin = atoi(argv[8]);
  const unsigned int contrastMax = atoi(argv[9]);

  //redirect cout to a file
  std::ofstream   file(outfname);
  std::streambuf* strm_buffer = std::cout.rdbuf(); //save the cout to put it
                                             //back later
  std::cout.rdbuf(file.rdbuf());

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

  OutputImageType::Pointer sift_base =
    sift(reader->GetOutput(), octaves, scales, threshold, ratio, "sift_base.png");

  OutputImageType::Pointer _sift_rotated =
    sift(_rotated, octaves, scales, threshold, ratio, "sift_rotated.png");

  OutputImageType::Pointer _sift_zoomed =
    sift(_zoomed, octaves, scales, threshold, ratio, "sift_zoomed.png");

  OutputImageType::Pointer _sift_contrasted =
    sift(_contrasted, octaves, scales, threshold, ratio, "sift_contrasted.png");

  OutputImageType::Pointer _sift_combined =
    sift(_combined2, octaves, scales, threshold, ratio, "sift_combined.png");

  OutputImageType::Pointer sift_rotated =
    invRotate(_sift_rotated, rotation);

  OutputImageType::Pointer sift_zoomed =
    invZoom(_sift_zoomed, zoomFactor);

  OutputImageType::Pointer sift_contrasted =
    invContrast(_sift_contrasted, contrastMin, contrastMax);

  OutputImageType::Pointer _sift_combined1 =
    invContrast(_sift_combined, contrastMin, contrastMax);
  OutputImageType::Pointer _sift_combined2 =
    invRotate(_sift_combined1, rotation);
  OutputImageType::Pointer sift_combined =
    invZoom(_sift_combined2, zoomFactor);

  OutputImageType::Pointer ddm_base = ddm(sift_base, "ddm_base.png");
  OutputImageType::Pointer ddm_rotated = ddm(sift_rotated, "ddm_rotated.png");
  OutputImageType::Pointer ddm_contrasted = ddm(sift_contrasted, "ddm_contrasted.png");
  OutputImageType::Pointer ddm_zoomed = ddm(sift_zoomed, "ddm_zoomed.png");
  OutputImageType::Pointer ddm_combined = ddm(sift_combined, "ddm_combined.png");

  subtract(ddm_base, ddm_rotated,
           "subtract_rotated.png");

  subtract(ddm_base, ddm_zoomed,
           "subtract_zoomed.png");

  subtract(ddm_base, ddm_contrasted,
           "subtract_contrasted.png");

  subtract(ddm_base, ddm_combined,
           "subtract_combined.png");

  std::cout.rdbuf(strm_buffer);
  file.close();

  return EXIT_SUCCESS;
}
