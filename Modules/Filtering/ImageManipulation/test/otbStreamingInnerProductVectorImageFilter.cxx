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
#include "otbStreamingInnerProductVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbStreamingInnerProductVectorImageFilter(int itkNotUsed(argc), char* argv[])
{
  const char * inputFileName = argv[1];
  const char * outfname = argv[2];
  const bool   centerdata = atoi(argv[3]);

  typedef double PixelType;
  const unsigned int Dimension = 2;

  // Typedef
  typedef otb::VectorImage<PixelType, Dimension>                 ImageType;
  typedef otb::ImageFileReader<ImageType>                        ReaderType;
  typedef otb::StreamingInnerProductVectorImageFilter<ImageType> FilterType;

  ReaderType::Pointer reader     = ReaderType::New();
  reader->SetFileName(inputFileName);

  // Instantiation object
  FilterType::Pointer filter = FilterType::New();
  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming( 10 );
  filter->SetCenterData(centerdata);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file.precision(10);
  file << std::fixed;
  file << "Inner Product: Dim [" << filter->GetInnerProduct().size() << "]:" << std::endl;
  file << filter->GetInnerProduct() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
