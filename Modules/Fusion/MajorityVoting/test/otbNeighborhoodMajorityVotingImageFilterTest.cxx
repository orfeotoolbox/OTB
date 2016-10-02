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
#include "otbImage.h"
#include <iostream>

#include <otbImageFileReader.h>
#include "otbImageFileWriter.h"

#include "otbNeighborhoodMajorityVotingImageFilter.h"

#include "itkTimeProbe.h"


int otbNeighborhoodMajorityVotingImageFilterTest(int argc, char* argv[])
{
  typedef unsigned char IOLabelPixelType; // 8 bits
  const unsigned int Dimension = 2;

  typedef otb::Image<IOLabelPixelType, Dimension> IOLabelImageType;

  typedef otb::ImageFileReader<IOLabelImageType> ReaderType;
  typedef otb::ImageFileWriter<IOLabelImageType> WriterType;


  // Neighborhood majority voting filter type
  typedef otb::NeighborhoodMajorityVotingImageFilter<IOLabelImageType> NeighborhoodMajorityVotingFilterType;

  // Binary ball Structuring Element type
  typedef NeighborhoodMajorityVotingFilterType::KernelType StructuringType;
  typedef StructuringType::RadiusType RadiusType;


  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);

  // Neighborhood majority voting filter
  NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter;
  NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();

  NeighMajVotingFilter->SetInput(reader->GetOutput());

  StructuringType seBall;
  RadiusType rad;


  std::string KeepOriginalLabelBoolStr = argv[3];
  if (KeepOriginalLabelBoolStr.compare("true") == 0)
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
  }
  else
  {
    NeighMajVotingFilter->SetKeepOriginalLabelBool(false);
  }


  if (argc >= 5)
  {
    rad[0] = atoi(argv[4]);
    rad[1] = atoi(argv[5]);

    if (argc >= 6)
    {
      NeighMajVotingFilter->SetLabelForNoDataPixels(atoi(argv[6]));

      if (argc >= 7)
      {
        NeighMajVotingFilter->SetLabelForUndecidedPixels(atoi(argv[7]));
      }
    }
  }
  else
  {
    rad[0] = 1;
    rad[1] = 1;
  }


  seBall.SetRadius(rad);
  seBall.CreateStructuringElement();
  NeighMajVotingFilter->SetKernel(seBall);

  if(argc==9)
    {
    NeighMajVotingFilter->SetOnlyIsolatedPixels(static_cast<bool>(atoi(argv[8])));
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFileName);
  writer->SetInput(NeighMajVotingFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}

int otbNeighborhoodMajorityVotingImageFilterIsolatedTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  typedef unsigned char PixelType; // 8 bits
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension> ImageType;

  ImageType::Pointer image = ImageType::New();
  ImageType::IndexType start;

  start[0] =   0;
  start[1] =   0;

  ImageType::SizeType size;
  size[0]  = 100;
  size[1]  = 100;

  ImageType::RegionType region;

  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(itk::NumericTraits<PixelType>::Zero);

  // Neighborhood majority voting filter type
  typedef otb::NeighborhoodMajorityVotingImageFilter<ImageType> NeighborhoodMajorityVotingFilterType;

  // Binary ball Structuring Element type
  typedef NeighborhoodMajorityVotingFilterType::KernelType StructuringType;
  typedef StructuringType::RadiusType RadiusType;


  // Neighborhood majority voting filter
  NeighborhoodMajorityVotingFilterType::Pointer NeighMajVotingFilter;
  NeighMajVotingFilter = NeighborhoodMajorityVotingFilterType::New();

  NeighMajVotingFilter->SetInput(image);

  StructuringType seBall;
  RadiusType rad;


  NeighMajVotingFilter->SetKeepOriginalLabelBool(true);
    
  rad[0] = 1;
  rad[1] = 1;
  NeighMajVotingFilter->SetLabelForNoDataPixels(10);
  NeighMajVotingFilter->SetLabelForUndecidedPixels(7);
  seBall.SetRadius(rad);
  seBall.CreateStructuringElement();
  NeighMajVotingFilter->SetKernel(seBall);
  NeighMajVotingFilter->SetOnlyIsolatedPixels(true);
  PixelType value = 255;
  ImageType::IndexType coordinate;
  coordinate[0] = 10;
  coordinate[1] = 10;
  image->SetPixel(coordinate, value);
  image->Update();
  NeighMajVotingFilter->SetIsolatedThreshold(1);
  NeighMajVotingFilter->Update();
  PixelType result = NeighMajVotingFilter->GetOutput()->GetPixel(coordinate);
//Should be filtered
  if(result == value)
    {
    std::cout << "one pixel\n";
    return EXIT_FAILURE;
    }
  coordinate[0] = 10;
  coordinate[1] = 11;
  image->SetPixel(coordinate, value);
  image->Update();
  NeighMajVotingFilter->Modified(); //needed for the filter to be updated
  NeighMajVotingFilter->Update();
  result = NeighMajVotingFilter->GetOutput()->GetPixel(coordinate);
  // Should not be filtered
  if( result != value)
    {
    std::cout << "2 pixels thres = 1 result = " << int(result) << '\n';
    return EXIT_FAILURE;
    }
  NeighMajVotingFilter->SetIsolatedThreshold(3);
  NeighMajVotingFilter->Update();
  result = NeighMajVotingFilter->GetOutput()->GetPixel(coordinate);
//Should be filtered
  if(result == value)
    {
    std::cout << "2 pixels thres = 2" << '\n';
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
