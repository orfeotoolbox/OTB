/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbGenericRSResampleImageFilter.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include <ogr_spatialref.h>

// Extract ROI
#include "otbMultiChannelExtractROI.h"

// Images definition
const unsigned int Dimension = 2;
typedef double                                      PixelType;
typedef otb::VectorImage<PixelType, Dimension>      ImageType;
typedef ImageType::SizeType                         SizeType;

typedef otb::GenericRSResampleImageFilter<ImageType,
                                          ImageType> ImageResamplerType;
typedef ImageResamplerType::OriginType              OriginType;
typedef ImageResamplerType::SpacingType             SpacingType;

typedef otb::ImageFileReader<ImageType>             ReaderType;
typedef otb::ImageFileWriter<ImageType>    WriterType;


int otbGenericRSResampleImageFilter(int itkNotUsed(argc), char* argv[])
{

  // SmartPointer instantiation
  ImageResamplerType::Pointer resampler = ImageResamplerType::New();

  const char * infname = argv[1];
  const char * outfname = argv[6];
  unsigned int isize    = atoi(argv[2]);
  double iGridSpacing    = atof(argv[3]);
  int    useInRpc          = atoi(argv[4]);
  int    useOutRpc          = atoi(argv[5]);


  ReaderType::Pointer         reader    = ReaderType::New();

  // Read the input image
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  // Fill the output size with the user selection
  SizeType      size;
  size.Fill(isize);

  // Set the origin & the spacing of the output
  OriginType  origin;
  origin[0] = 367340;
  origin[1] = 4.83467e+06;

  SpacingType  spacing;
  spacing[0] = 0.6;
  spacing[1] = -0.6;

  // Build the output projection ref : UTM ref
  OGRSpatialReference    oSRS;
  oSRS.SetProjCS("UTM");
  oSRS.SetUTM(31, true);
  char * utmRef = nullptr;
  oSRS.exportToWkt(&utmRef);

  // Displacement Field spacing
  SpacingType  gridSpacing;
  gridSpacing[0] = iGridSpacing;
  gridSpacing[1] = -iGridSpacing;

  // Default value builder
  ImageType::PixelType defaultValue;
  itk::NumericTraits<ImageType::PixelType>::SetLength(defaultValue, reader->GetOutput()->GetNumberOfComponentsPerPixel());

  // Set the Resampler Parameters
  resampler->SetInput(reader->GetOutput());
  resampler->SetDisplacementFieldSpacing(gridSpacing);
  resampler->SetOutputOrigin(origin);
  resampler->SetOutputSize(size);
  resampler->SetOutputSpacing(spacing);
  resampler->SetOutputProjectionRef(utmRef);
  resampler->SetEdgePaddingValue(defaultValue);
  if (useInRpc)
    {
    resampler->SetInputRpcGridSize(20);
    resampler->EstimateInputRpcModelOn();
    }

  if (useOutRpc)
    {
    resampler->SetOutputRpcGridSize(20);
    resampler->EstimateOutputRpcModelOn();
    }


  // Write the resampled image
  WriterType::Pointer writer= WriterType::New();
  writer->SetNumberOfDivisionsTiledStreaming(4);
  writer->SetFileName(outfname);
  writer->SetInput(resampler->GetOutput());
  writer->Update();

  std::cout << resampler << std::endl;

  return EXIT_SUCCESS;
}


int otbGenericRSResampleImageFilterFromMap(int itkNotUsed(argc), char* argv[])
{
  typedef otb::MultiChannelExtractROI<PixelType, PixelType>  ExtractROIType;

  // SmartPointer instantiation
  ExtractROIType::Pointer extractor = ExtractROIType::New();
  ImageResamplerType::Pointer resampler = ImageResamplerType::New();

  const char * infname   = argv[1];
  const char * outfname  = argv[4];
  double iGridSpacing    = atof(argv[2]);
  int    useInRpc        = atoi(argv[3]);

  // Reader Instantiation
  ReaderType::Pointer         reader    = ReaderType::New();
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();

  SpacingType  spacing;
  spacing[0] =  2.5;
  spacing[1] = -2.5;

  // Displacement Field spacing
  SpacingType  gridSpacing;
  gridSpacing[0] = iGridSpacing;
  gridSpacing[1] = -iGridSpacing;

  // Default value builder
  ImageType::PixelType defaultValue;
  itk::NumericTraits<ImageType::PixelType>::SetLength(defaultValue, reader->GetOutput()->GetNumberOfComponentsPerPixel());

  // Extract a roi centered on the input center
  ImageType::RegionType roi;
  ImageType::IndexType  roiIndex;
  SizeType              roiSize;

  // Fill the size
  roiSize.Fill(250);

  // Fill the start index
  roiIndex[0] = (unsigned int)((reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0] - roiSize[0]) /2);
  roiIndex[1] = (unsigned int)((reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1] - roiSize[1]) /2);

  roi.SetIndex(roiIndex);
  roi.SetSize(roiSize);

  extractor->SetExtractionRegion(roi);
  extractor->SetInput(reader->GetOutput());
  extractor->UpdateOutputInformation();

  // Set the Resampler Parameters
  resampler->SetInput(extractor->GetOutput());
  resampler->SetDisplacementFieldSpacing(gridSpacing);
  resampler->SetOutputParametersFromMap("UTM", spacing);

  if (useInRpc)
    {
    resampler->SetInputRpcGridSize(20);
    resampler->EstimateInputRpcModelOn();
    }

  // Write the resampled image
  WriterType::Pointer writer= WriterType::New();
  writer->SetAutomaticTiledStreaming();
  writer->SetFileName(outfname);
  writer->SetInput(resampler->GetOutput());
  writer->Update();

  std::cout << resampler << std::endl;

  return EXIT_SUCCESS;
}
