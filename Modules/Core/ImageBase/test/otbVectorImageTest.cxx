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
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int otbVectorImageLegacyTest(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 3)
    {
    std::cout << argv[0] << "<image> <output information> [dataset number]" << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputFilename  = argv[1];
  const char * outputAsciiFilename  = argv[2];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  InputImageType::Pointer image = InputImageType::New();
  image = reader->GetOutput();

  std::ofstream file;
  file.open(outputAsciiFilename);

  std::cout << "------ IMAGE --------" << std::endl;
  std::cout << image << std::endl;
  std::cout << "---------------------" << std::endl;

  file << "------ IMAGE --------" << std::endl;
  file << "Spacing " << image->GetSpacing() << std::endl;
  file << "Origin " << image->GetOrigin() << std::endl;
  file << "Projection REF " << image->GetProjectionRef() << std::endl;
  file << "GCP Projection " << image->GetGCPProjection() << std::endl;

  unsigned int GCPCount = image->GetGCPCount();
  file << "GCP Count " << image->GetGCPCount() << std::endl;

  for (unsigned int GCPnum = 0; GCPnum < GCPCount; GCPnum++)
    {
    file << "GCP[" << GCPnum << "] Id " << image->GetGCPId(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Info " << image->GetGCPInfo(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Row " << image->GetGCPRow(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Col " << image->GetGCPCol(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] X " << image->GetGCPX(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Y " << image->GetGCPY(GCPnum) << std::endl;
    file << "GCP[" << GCPnum << "] Z " << image->GetGCPZ(GCPnum) << std::endl;
    file << "----------------" << std::endl;
    }

  InputImageType::VectorType tab = image->GetGeoTransform();

  file << "Geo Transform " << std::endl;
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " " << i << " -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetUpperLeftCorner();
  file << "Corners " << std::endl;
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " UL[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetUpperRightCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " UR[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetLowerLeftCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " LL[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  tab = image->GetLowerRightCorner();
  for (unsigned int i = 0; i < tab.size(); ++i)
    {
    file << " LR[" << i << "] -> " << tab[i] << std::endl;
    }
  tab.clear();

  file.close();

  return EXIT_SUCCESS;
}

int otbVectorImageTest(int argc, char* argv[])
{
  if (argc < 3)
    {
    std::cout << argv[0] << "<image> <output information>" << std::endl;
    return EXIT_FAILURE;
    }
  const char * inputFilename  = argv[1];
  const char * outputAsciiFilename  = argv[2];

  typedef double PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  std::ofstream file;
  ImageType::Pointer image = ImageType::New();
  image = reader->GetOutput();
  std::cout << image << std::endl;
  file.open(outputAsciiFilename);
  file << std::setprecision(15);
  file << image;
  file.close();

  return EXIT_SUCCESS;
}
