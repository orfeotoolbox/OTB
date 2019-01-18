/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */




#include "itkMacro.h"

#include <fstream>
#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageMetadataInterfaceFactory.h"

int otbImageMetadataInterfaceBaseTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename  = argv[2];

  typedef otb::VectorImage<double,  2>         InputImageType;
  typedef otb::ImageFileReader<InputImageType> ImageReaderType;

  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  otb::ImageMetadataInterfaceBase::Pointer lImageMetadata = otb::ImageMetadataInterfaceFactory::CreateIMI(
    reader->GetOutput()->GetMetaDataDictionary());

  std::ofstream file;
  file.open(outputFilename);

  std::vector<unsigned int> defaultDisplay = lImageMetadata->GetDefaultDisplay();
  file << "Default RGB Display: ["
               << defaultDisplay[0] << ", "
               << defaultDisplay[1] << ", "
               << defaultDisplay[2] << "]" << std::endl;
  file << "ProjectionRef:   " <<  lImageMetadata->GetProjectionRef() << std::endl;
  file << "GCPProjection:   " << lImageMetadata->GetGCPProjection( ) << std::endl;
  file << "GCPCount:        " << lImageMetadata->GetGCPCount( ) << std::endl;
  for(unsigned int gcpIdx = 0; gcpIdx  < lImageMetadata->GetGCPCount(); ++ gcpIdx)
  {
    file << "GCPId:           " << lImageMetadata->GetGCPId(gcpIdx) << std::endl;
    file << "GCPInfo:         " << lImageMetadata->GetGCPInfo(gcpIdx) << std::endl;
  }
  std::string sensorId;
  lImageMetadata->GetSensorID( sensorId );
  file << "SensorID:        " << sensorId << std::endl;
  file << "NumberOfBands:   " << lImageMetadata->GetNumberOfBands( ) << std::endl;
  file << "XPixelSpacing:   " << lImageMetadata->GetXPixelSpacing( ) << std::endl;
  file << "YPixelSpacing:   " << lImageMetadata->GetYPixelSpacing( ) << std::endl;
  file << "Day:             " << lImageMetadata->GetDay( ) << std::endl;
  file << "Month:           " << lImageMetadata->GetMonth( ) << std::endl;
  file << "Year:            " << lImageMetadata->GetYear( ) << std::endl;
  file << "Hour:            " << lImageMetadata->GetHour( ) << std::endl;
  file << "Minute:          " << lImageMetadata->GetMinute( ) << std::endl;
  file << "ProductionDay:   " << lImageMetadata->GetProductionDay( ) << std::endl;
  file << "ProductionMonth: " << lImageMetadata->GetProductionMonth( ) << std::endl;
  file << "ProductionYear:  " << lImageMetadata->GetProductionYear( ) << std::endl;

  std::vector<std::string> bandNameList = lImageMetadata->GetBandName();
  if (bandNameList.size() == 1)
    {
    file << "BandName:        " << bandNameList[0] << std::endl;
    }
  else
    if (bandNameList.size() > 1)
      {
      file << "BandNameList: ";
      for (std::vector<std::string>::iterator it = bandNameList.begin(); it != bandNameList.end(); ++it)
        file << *it << ", ";
      file << std::endl;
      }

  std::vector<std::string> bandNameList2 = lImageMetadata->GetEnhancedBandNames();
  if (bandNameList2.size() == 1)
    {
    file << "ENH BandName:        " << bandNameList2[0] << std::endl;
    }
  else
    if (bandNameList2.size() > 1)
      {
      file << "ENH BandNameList: ";
      for (std::vector<std::string>::iterator it = bandNameList2.begin(); it != bandNameList2.end(); ++it)
        file << *it << ", ";
      file << std::endl;
      }
  file.close();

  std::cout << lImageMetadata << std::endl;

  return EXIT_SUCCESS;

}
