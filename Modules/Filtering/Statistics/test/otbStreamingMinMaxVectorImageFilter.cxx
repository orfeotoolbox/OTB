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

#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbImageFileReader.h"
#include <fstream>
#include "otbStreamingTraits.h"
#include "otbStandardFilterWatcher.h"

int otbStreamingMinMaxVectorImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::VectorImage<PixelType, Dimension>               ImageType;
  typedef otb::ImageFileReader<ImageType>                      ReaderType;
  typedef otb::StreamingMinMaxVectorImageFilter<ImageType> StreamingMinMaxVectorImageFilterType;

  // Instantiating object
  StreamingMinMaxVectorImageFilterType::Pointer filter = StreamingMinMaxVectorImageFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  filter->GetStreamer()->SetNumberOfLinesStrippedStreaming( 10 );
  filter->SetInput(reader->GetOutput());
  otb::StandardFilterWatcher watcher(filter, "Min Max Computation");
  filter->Update();

  std::ofstream file;
  file.open(outfname);
  file << "Minimum: " << filter->GetMinimum() << std::endl;
  file << "Maximum: " << filter->GetMaximum() << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
