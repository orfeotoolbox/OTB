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

#include "otbFusionOGRTileFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

int otbFusionOGRTileFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * inOGRfname = argv[2];
  unsigned int size = atoi(argv[3]);
  
  /** Typedefs */
  const unsigned int Dimension = 2;
  typedef float PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef otb::FusionOGRTileFilter<ImageType>   FilterType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();
  
  reader->SetFileName(infname);
  reader->UpdateOutputInformation();
  
  ImageType::SizeType streamSize;
  streamSize.Fill(size);
  
  filter->SetInput(reader->GetOutput());
  filter->SetInputFileName(inOGRfname);
  filter->SetStreamSize(streamSize);
  filter->GenerateData();

  return EXIT_SUCCESS;
}
