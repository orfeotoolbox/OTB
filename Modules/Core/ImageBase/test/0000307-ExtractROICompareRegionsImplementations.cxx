/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "itkExtractImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"


typedef unsigned char PixelType;

typedef otb::Image<PixelType>       ImageType;
typedef otb::VectorImage<PixelType> VectorImageType;

typedef ImageType::RegionType     RegionType;
typedef ImageType::IndexType      IndexType;
typedef IndexType::IndexValueType IndexValueType;
typedef ImageType::SizeType       SizeType;
typedef SizeType::SizeValueType   SizeValueType;

// ITK filters
typedef itk::ExtractImageFilter<ImageType, ImageType>                        ExtractImageFilterType;
typedef itk::VectorIndexSelectionCastImageFilter<VectorImageType, ImageType> VectorIndexSelectionCastImageFilterType;

// OTB filters
typedef otb::ExtractROI<PixelType, PixelType>                   ExtractROIType;
typedef otb::MultiChannelExtractROI<PixelType, PixelType>       MultiChannelExtractROIType;
typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType> MultiToMonoChannelExtractROIType;


ImageType::Pointer makeImage(IndexValueType startX, IndexValueType startY, SizeValueType sizeX, SizeValueType sizeY)
{
  ImageType::Pointer im = ImageType::New();

  RegionType region;

  region.SetIndex(0, startX);
  region.SetIndex(1, startY);
  region.SetSize(0, sizeX);
  region.SetSize(1, sizeY);

  im->SetRegions(region);
  im->Allocate();

  return im;
}

VectorImageType::Pointer makeVectorImage(IndexValueType startX, IndexValueType startY, SizeValueType sizeX, SizeValueType sizeY, int nbChannel = 3)
{
  VectorImageType::Pointer im = VectorImageType::New();

  RegionType region;

  region.SetIndex(0, startX);
  region.SetIndex(1, startY);
  region.SetSize(0, sizeX);
  region.SetSize(1, sizeY);

  im->SetRegions(region);
  im->SetNumberOfComponentsPerPixel(nbChannel);
  im->Allocate();

  return im;
}

int otbITKExtractImageFilterROITestRegion(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ImageType::Pointer              in;
  RegionType                      extractionRegion;
  ExtractImageFilterType::Pointer extract;

  in = makeImage(0, 0, 100, 100);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = ExtractImageFilterType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  in = makeImage(20, 20, 100, 100);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = ExtractImageFilterType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int otbITKVectorIndexSelectionCastTestRegion(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  VectorImageType::Pointer                         in;
  RegionType                                       extractionRegion;
  VectorIndexSelectionCastImageFilterType::Pointer extract;

  in = makeVectorImage(0, 0, 100, 100, 3);

  extract = VectorIndexSelectionCastImageFilterType::New();
  extract->SetInput(in);
  extract->SetIndex(1);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != in->GetLargestPossibleRegion())
  {
    return EXIT_FAILURE;
  }

  in = makeVectorImage(50, 50, 100, 100, 3);

  extract = VectorIndexSelectionCastImageFilterType::New();
  extract->SetInput(in);
  extract->SetIndex(1);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != in->GetLargestPossibleRegion())
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int otbMultiToMonoChannelExtractROITestRegion(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  VectorImageType::Pointer                  in;
  RegionType                                extractionRegion;
  MultiToMonoChannelExtractROIType::Pointer extract;

  in = makeVectorImage(0, 0, 100, 100, 3);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = MultiToMonoChannelExtractROIType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  in = makeVectorImage(20, 20, 100, 100, 3);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = MultiToMonoChannelExtractROIType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int otbMultiChannelExtractROITestRegion(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  VectorImageType::Pointer            in;
  RegionType                          extractionRegion;
  MultiChannelExtractROIType::Pointer extract;

  in = makeVectorImage(0, 0, 100, 100, 3);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = MultiChannelExtractROIType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  in = makeVectorImage(20, 20, 100, 100, 3);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = MultiChannelExtractROIType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int otbExtractROITestRegion(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ImageType::Pointer      in;
  RegionType              extractionRegion;
  ExtractROIType::Pointer extract;

  in = makeImage(0, 0, 100, 100);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = ExtractROIType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  in = makeImage(20, 20, 100, 100);

  extractionRegion.SetIndex(0, 50);
  extractionRegion.SetIndex(1, 50);
  extractionRegion.SetSize(0, 10);
  extractionRegion.SetSize(1, 10);

  extract = ExtractROIType::New();
  extract->SetInput(in);
  extract->SetExtractionRegion(extractionRegion);
  extract->UpdateOutputInformation();

  std::cout << "Input LargestPossibleRegion : " << in->GetLargestPossibleRegion() << std::endl;
  std::cout << "ExtractionRegion : " << extract->GetExtractionRegion() << std::endl;
  std::cout << "Output LargestPossibleRegion : " << extract->GetOutput()->GetLargestPossibleRegion() << std::endl;

  if (extract->GetOutput()->GetLargestPossibleRegion() != extract->GetExtractionRegion())
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int main(int argc, char* argv[])
{
  int unitaryResult;
  int finalResult = EXIT_SUCCESS;

  std::cout << "*******************************" << std::endl;
  std::cout << "Starting test itk::ExtractImageFilter" << std::endl;
  unitaryResult = otbITKExtractImageFilterROITestRegion(argc, argv);
  if (unitaryResult == EXIT_FAILURE)
  {
    std::cout << "-> Testing itk::ExtractImageFilter FAILED" << std::endl;
    finalResult = EXIT_FAILURE;
  }
  else
  {
    std::cout << "-> Testing itk::ExtractImageFilter OK" << std::endl;
  }


  std::cout << "*******************************" << std::endl;
  std::cout << "Starting test itk::VectorIndexSelectionCast" << std::endl;
  unitaryResult = otbITKVectorIndexSelectionCastTestRegion(argc, argv);
  if (unitaryResult == EXIT_FAILURE)
  {
    std::cout << "-> Testing itk::VectorIndexSelectionCast FAILED" << std::endl;
    finalResult = EXIT_FAILURE;
  }
  else
  {
    std::cout << "-> Testing itk::VectorIndexSelectionCast OK" << std::endl;
  }


  std::cout << "*******************************" << std::endl;
  std::cout << "Starting test otb::MultiToMonoChannelExtractROI" << std::endl;
  unitaryResult = otbMultiToMonoChannelExtractROITestRegion(argc, argv);
  if (unitaryResult == EXIT_FAILURE)
  {
    std::cout << "-> Testing otb::MultiToMonoChannelExtractROI FAILED" << std::endl;
    finalResult = EXIT_FAILURE;
  }
  else
  {
    std::cout << "-> Testing otb::MultiToMonoChannelExtractROI OK" << std::endl;
  }


  std::cout << "*******************************" << std::endl;
  std::cout << "Starting test otb::MultiChannelExtractROI" << std::endl;
  unitaryResult = otbMultiChannelExtractROITestRegion(argc, argv);
  if (unitaryResult == EXIT_FAILURE)
  {
    std::cout << "-> Testing otb::MultiChannelExtractROI FAILED" << std::endl;
    finalResult = EXIT_FAILURE;
  }
  else
  {
    std::cout << "-> Testing otb::MultiChannelExtractROI OK" << std::endl;
  }


  std::cout << "*******************************" << std::endl;
  std::cout << "Starting test otb::ExtractROI" << std::endl;
  unitaryResult = otbExtractROITestRegion(argc, argv);
  if (unitaryResult == EXIT_FAILURE)
  {
    std::cout << "-> Testing otb::ExtractROI FAILED" << std::endl;
    finalResult = EXIT_FAILURE;
  }
  else
  {
    std::cout << "-> Testing otb::ExtractROI OK" << std::endl;
  }

  return finalResult;
}
