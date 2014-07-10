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

#include "otbImportGeoInformationImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageKeywordlist.h"

int otbImportGeoInformationImageFilterWithKeywordList(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outputAsciiFilenameOtbImage = argv[2];

  const unsigned int Dimension = 2;
  typedef unsigned int                                               PixelType;
  typedef otb::Image<PixelType, Dimension>                           ImageType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef otb::ImportGeoInformationImageFilter<ImageType, ImageType> ImportGeoInformationImageFilterType;

  // Instantiating objects
  ReaderType::Pointer                          reader = ReaderType::New();
  ImportGeoInformationImageFilterType::Pointer import = ImportGeoInformationImageFilterType::New();

  reader->SetFileName(infname);
  reader->GenerateOutputInformation();
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::SizeType size;
  size.Fill(500);
  ImageType::RegionType region;
  region.SetIndex(index);
  region.SetSize(size);
  ImageType::Pointer black = ImageType::New();
  black->SetRegions(region);
  black->Allocate();
  black->FillBuffer(128);
  std::cout << "black: " << black->GetLargestPossibleRegion() << std::endl;

  import->SetInput(black);
  import->SetSource(reader->GetOutput());
  import->GetOutput()->UpdateOutputInformation();

  otb::ImageKeywordlist otb_tmp_image;
  otb_tmp_image = import->GetOutput()->GetImageKeywordlist();

  std::cout << " -> otbImage Ossim key word list copy : " << otb_tmp_image << std::endl;

  std::ofstream file;
  file.open(outputAsciiFilenameOtbImage);
  file << "--- OSSIM KEYWORDLIST ---" << std::endl;
  file << otb_tmp_image;
  file.close();

  return EXIT_SUCCESS;
}
