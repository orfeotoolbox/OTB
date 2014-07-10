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
#include "ossim/elevation/ossimElevManager.h"
#include "ossim/base/ossimFilename.h"

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIteratorWithIndex.h"

int otbOssimElevManagerTest(int argc, char* argv[])
{

  if (argc != 9)
    {
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " srtmDir outfname originX originY spacingX spacingY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
    }

  const ossimFilename srtmDir(argv[1]);
  const char *        outfname = argv[2];

  typedef double PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension>             ImageType;
  typedef otb::ImageFileWriter<ImageType>              WriterType;
  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;

  typedef ImageType::RegionType  RegionType;
  typedef ImageType::SpacingType SpacingType;
  typedef RegionType::IndexType  IndexType;
  typedef RegionType::SizeType   SizeType;
  typedef ImageType::PointType   PointType;

  PointType   origin;
  SpacingType spacing;
  SizeType    size;

  origin[0] = atof(argv[3]);
  origin[1] = atof(argv[4]);
  spacing[0] = atof(argv[5]);
  spacing[1] = atof(argv[6]);
  size[0] =   atoi(argv[7]);
  size[1] =   atoi(argv[8]);

  IndexType index;
  index.Fill(0);

  RegionType region;
  region.SetIndex(index);
  region.SetSize(size);

  ImageType::Pointer image = ImageType::New();
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  image->SetOrigin(origin);
  image->SetSpacing(spacing);

  ossimElevManager * elevManager = ossimElevManager::instance();

  elevManager->loadElevationPath(srtmDir);

  IteratorType it(image, image->GetLargestPossibleRegion());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    PointType point;
    image->TransformIndexToPhysicalPoint(it.GetIndex(), point);
    ossimGpt ossimWorldPoint;
    ossimWorldPoint.lon = point[0];
    ossimWorldPoint.lat = point[1];
    double height = elevManager->getHeightAboveMSL(ossimWorldPoint);

    if (!ossim::isnan(height))
      {
      // Fill the image
      it.Set(height);
      }
    else
      {
      // Back to the MNT default value
      it.Set(0);
      }
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image);
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
