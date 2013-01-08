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
#include "itkMacro.h"

#include "otbStreamingResampleImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "itkTranslationTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbStreamingTraits.h"
#include "itkResampleImageFilter.h"


int otbStreamingResampleImageFilterCompareWithITK(int argc, char * argv[])
{
  const char*  inputFilename = argv[1];
  unsigned int sizeXOutputImage = atoi(argv[2]);
  unsigned int sizeYOutputImage = atoi(argv[3]);
  const char*  outputITKFilename = argv[4];
  const char*  outputOTBFilename = argv[5];

  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef double OutputPixelType;
  typedef double InterpolatorPrecisionType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  //      typedef otb::VectorImage<OutputPixelType, Dimension> InputVectorImageType;
  typedef otb::ImageFileReader<InputImageType>                                                    ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                                                   WriterType;
  typedef otb::ImageFileWriter<OutputImageType>                                          StreamingWriterType;
  typedef itk::TranslationTransform<InputPixelType, Dimension>                                    TransformType;
  typedef itk::NearestNeighborInterpolateImageFunction<InputImageType, InterpolatorPrecisionType> NNInterpolatorType;

  typedef itk::ResampleImageFilter<InputImageType, OutputImageType,
      InterpolatorPrecisionType>          ITKResampleImageFilterType;
  typedef otb::StreamingResampleImageFilter<InputImageType, OutputImageType,
      InterpolatorPrecisionType> OTBResampleImageFilterType;

  // Instantiating object
  ReaderType::Pointer          reader = ReaderType::New();
  WriterType::Pointer          writerITKFilter = WriterType::New();
  StreamingWriterType::Pointer writerOTBFilter = StreamingWriterType::New();

  ITKResampleImageFilterType::Pointer resamplerITK = ITKResampleImageFilterType::New();
  OTBResampleImageFilterType::Pointer resamplerOTB = OTBResampleImageFilterType::New();

  TransformType::Pointer transform = TransformType::New();
  // Transformation creation
  TransformType::OutputVectorType translation;
  translation[0] = 30;
  translation[1] = 50;
  transform->SetOffset(translation);

  // Input Image
  reader->SetFileName(inputFilename);

  // Resamplers connected to input image
  resamplerITK->SetInput(reader->GetOutput());
  resamplerOTB->SetInput(reader->GetOutput());

  // Size of output resamplers result
  OTBResampleImageFilterType::SizeType size;
  size[0] = sizeXOutputImage;
  size[1] = sizeYOutputImage;
  resamplerITK->SetSize(size);
  resamplerOTB->SetOutputSize(size);

  // Set Interpolation
  NNInterpolatorType::Pointer interpolator = NNInterpolatorType::New();

  // Resamplers are updated with new interpolation (default is linear interpolation)
  resamplerITK->SetInterpolator(interpolator);
  resamplerOTB->SetInterpolator(interpolator);

  // Resamplers are updated with new transformation (default is identity)
  resamplerITK->SetTransform(transform);
  resamplerOTB->SetTransform(transform);

  // Result of resamplers is written
  writerITKFilter->SetInput(resamplerITK->GetOutput());
  writerITKFilter->SetFileName(outputITKFilename);
  writerITKFilter->Update();

  writerOTBFilter->SetInput(resamplerOTB->GetOutput());
  //      writerOTBFilter->SetAutomaticTiledStreaming();
  writerOTBFilter->SetFileName(outputOTBFilename);
  writerOTBFilter->SetNumberOfDivisionsStrippedStreaming(10);
  writerOTBFilter->Update();

  return EXIT_SUCCESS;
}
