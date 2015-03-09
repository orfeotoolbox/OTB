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
#include "otbGaborFilterGenerator.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"

int otbGaborFilterGenerator(int argc, char * argv[])
{
  if (argc != 10)
    {
    std::cerr << "Usage: " << argv[0] << "outfname xradius yradius a b theta u0 v0 phi" << std::endl;
    return EXIT_FAILURE;
    }

  const char *       outfname = argv[1];
  const unsigned int xradius = atoi(argv[2]);
  const unsigned int yradius = atoi(argv[3]);
  const double       a = atof(argv[4]);
  const double       b = atof(argv[5]);
  const double       theta = atof(argv[6]);
  const double       u0 = atof(argv[7]);
  const double       v0 = atof(argv[8]);
  const double       phi = atof(argv[9]);

  typedef double                                   PrecisionType;
  typedef otb::GaborFilterGenerator<PrecisionType> GaborGeneratorType;
  typedef GaborGeneratorType::RadiusType           RadiusType;
  typedef GaborGeneratorType::ArrayType            ArrayType;

  typedef otb::Image<PrecisionType, 2>    ImageType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  // Gabor filter generation
  GaborGeneratorType::Pointer gabor = GaborGeneratorType::New();
  RadiusType                  radius;
  radius[0] = xradius;
  radius[1] = yradius;
  gabor->SetRadius(radius);
  gabor->SetA(a);
  gabor->SetB(b);
  gabor->SetTheta(theta);
  gabor->SetU0(u0);
  gabor->SetV0(v0);
  gabor->SetPhi(phi);
  ArrayType filter = gabor->GetFilter();

  // Writing filter mask to an image
  ImageType::Pointer    image = ImageType::New();
  ImageType::RegionType region;
  ImageType::SizeType   size;
  size[0] = 2 * radius[0] + 1;
  size[1] = 2 * radius[1] + 1;
  ImageType::IndexType index;
  index.Fill(0);
  region.SetIndex(index);
  region.SetSize(size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  itk::ImageRegionIterator<ImageType> it(image, region);
  unsigned int                        k = 0;

  for (it.GoToBegin(); !it.IsAtEnd(); ++it, ++k)
    {
    it.Set(filter[k]);
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(image);
  writer->Update();

  return EXIT_SUCCESS;
}
