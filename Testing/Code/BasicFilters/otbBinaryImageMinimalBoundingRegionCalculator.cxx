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
#include "otbBinaryImageMinimalBoundingRegionCalculator.h"
#include "otbImageFileReader.h"
#include "otbImageList.h"

int otbBinaryImageMinimalBoundingRegionCalculator(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;

  int    nbImages  = atoi(argv[1]);
  char * outfile   = argv[2];

  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::BinaryImageMinimalBoundingRegionCalculator<ImageType>
  BoundingRegionCalculatorType;
  typedef BoundingRegionCalculatorType::RegionType RegionType;
  typedef otb::ImageList<ImageType>                ImageListType;
  typedef ImageListType::Iterator                  IteratorType;
  // reference image list
  ImageListType::Pointer images = ImageListType::New();

  // Reading input images
  std::ofstream out;
  out.open(outfile, std::ios::out);
  out << "Test results from otbBinaryImageBoundingRegionCalculator test." << std::endl;
  for (int i = 1; i <= nbImages; ++i)
    {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[2 + i]);
    reader->Update();
    images->PushBack(reader->GetOutput());
    }

  // Declaration
  BoundingRegionCalculatorType::Pointer brct;
  // Computing bounding region for each image
  for (IteratorType it = images->Begin(); it != images->End(); ++it)
    {
    brct = BoundingRegionCalculatorType::New();
    brct->SetInput(it.Get());
    brct->Update();
    RegionType region = brct->GetRegion();
    out << region.GetIndex() << "\t" << region.GetSize() << std::endl;
    }
  out << std::endl << "Testing the pad option" << std::endl << std::endl;
  for (IteratorType it = images->Begin(); it != images->End(); ++it)
    {
    brct = BoundingRegionCalculatorType::New();
    brct->SetPad(1);
    brct->SetInput(it.Get());
    brct->Update();
    RegionType region = brct->GetRegion();
    out << region.GetIndex() << "\t" << region.GetSize() << std::endl;
    }

  out.close();

  return EXIT_SUCCESS;
}
