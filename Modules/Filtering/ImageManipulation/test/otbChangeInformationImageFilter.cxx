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
#include "otbChangeInformationImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbChangeInformationImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char *inputFilename(argv[1]);
  const char *outputFilename(argv[2]);
  
  typedef otb::Image<float,2> ImageType;
  typedef otb::ChangeInformationImageFilter<ImageType> FilterType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());
  filter->SetOutputMetaData<std::string>(otb::MetaDataKey::ProjectionRefKey,NULL);
  std::vector<bool> flags;
  flags.push_back(true);
  std::vector<double> nodata;
  nodata.push_back(0.0);
  filter->SetOutputMetaData<std::vector<bool> >(otb::MetaDataKey::NoDataValueAvailable,&flags);
  filter->SetOutputMetaData<std::vector<double> >(otb::MetaDataKey::NoDataValue,&nodata);
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outputFilename);
  writer->Update();
  
  return EXIT_SUCCESS;
}

