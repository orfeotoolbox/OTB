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


#include "otbImage.h"
#include "otbVectorImage.h"

#include "itkExtractImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"


typedef unsigned char PixelType;

typedef otb::Image<PixelType>  ImageType;
typedef otb::VectorImage<PixelType> VectorImageType;

typedef ImageType::RegionType      RegionType;
typedef ImageType::IndexType       IndexType;
typedef IndexType::IndexValueType  IndexValueType;
typedef ImageType::SizeType        SizeType;
typedef SizeType::SizeValueType    SizeValueType;

// ITK filters
typedef itk::ExtractImageFilter<ImageType, ImageType>
  ExtractImageFilterType;
typedef itk::VectorIndexSelectionCastImageFilter<VectorImageType, ImageType>
  VectorIndexSelectionCastImageFilterType;

// OTB filters
typedef otb::ExtractROI<PixelType, PixelType>
  ExtractROIType;
typedef otb::MultiChannelExtractROI<PixelType, PixelType>
  MultiChannelExtractROIType;
typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType>
  MultiToMonoChannelExtractROIType;


ImageType::Pointer makeImage(IndexValueType startX, IndexValueType startY, SizeValueType sizeX, SizeValueType sizeY)
{
  ImageType::Pointer im = ImageType::New();

  RegionType region;

  region.SetIndex(0, startX);
  region.SetIndex(1, startY);
  region.SetSize(0, sizeX);
  region.SetSize(0, sizeY);

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
  region.SetSize(0, sizeY);

  im->SetRegions(region);
  im->SetNumberOfComponentsPerPixel(nbChannel);
  im->Allocate();

  return im;
}

int otbITKExtractImageFilterROITestRegion(int argc, char * argv[])
{
  ImageType::Pointer in;
  RegionType extractionRegion;
  ExtractImageFilterType::Pointer extract;

  in = makeImage(0,0,100,100);

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

  in = makeImage(20,20,100,100);

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


int otbITKVectorIndexSelectionCastTestRegion(int argc, char * argv[])
{
  VectorImageType::Pointer in;
  RegionType extractionRegion;
  VectorIndexSelectionCastImageFilterType::Pointer extract;

  in = makeVectorImage(0,0,100,100,3);

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

  in = makeVectorImage(50,50,100,100,3);

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


int otbMultiToMonoChannelExtractROITestRegion(int argc, char * argv[])
{
  VectorImageType::Pointer in;
  RegionType extractionRegion;
  MultiToMonoChannelExtractROIType::Pointer extract;

  in = makeVectorImage(0,0,100,100,3);

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

  in = makeVectorImage(20,20,100,100,3);

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



int otbMultiChannelExtractROITestRegion(int argc, char * argv[])
{
  VectorImageType::Pointer in;
  RegionType extractionRegion;
  MultiChannelExtractROIType::Pointer extract;

  in = makeVectorImage(0,0,100,100,3);

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

  in = makeVectorImage(20,20,100,100,3);

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


int otbExtractROITestRegion(int argc, char * argv[])
{
  ImageType::Pointer in;
  RegionType extractionRegion;
  ExtractROIType::Pointer extract;

  in = makeImage(0,0,100,100);

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

  in = makeImage(20,20,100,100);

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


