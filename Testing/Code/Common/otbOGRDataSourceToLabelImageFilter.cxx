/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"

#include "otbOGRDataSourceWrapper.h"

#include "otbOGRDataSourceToLabelImageFilter.h"
#include "otbStandardOneLineFilterWatcher.h"

typedef otb::Image<unsigned int, 2>                           ImageType;
typedef otb::ImageFileReader<ImageType>                       ReaderType;

typedef otb::StreamingImageFileWriter<ImageType>              WriterType;
typedef otb::OGRDataSourceToLabelImageFilter<ImageType>       RasterizationFilterType;

int otbOGRDataSourceToLabelImageFilterNew(int argc, char* argv[])
{
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  return EXIT_SUCCESS;
}

int otbOGRDataSourceToLabelImageFilter(int argc, char* argv[])
{
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  bool mode = atoi(argv[4]);
  unsigned char background = atoi(argv[5]);
  unsigned char foreground = atoi(argv[6]);

  otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(argv[2], otb::ogr::DataSource::Modes::Read);
  
  // rasterize
  RasterizationFilterType::Pointer  rasterization = RasterizationFilterType::New();
  rasterization->AddOGRDataSource(ogrDS);
  rasterization->SetOutputParametersFromImage(reader->GetOutput());
  rasterization->SetBurnAttribute("DN");
  rasterization->SetBurnAttributeMode(mode);
  rasterization->SetBackgroundValue(background);
  rasterization->SetForegroundValue(foreground);

  /*otb::StandardOneLineFilterWatcher * watch = new otb::StandardOneLineFilterWatcher(rasterization.GetPointer(),
                                                                          "rasterization"); */

  WriterType::Pointer writer  = WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(rasterization->GetOutput());
  writer->Update();

return EXIT_SUCCESS;
}
