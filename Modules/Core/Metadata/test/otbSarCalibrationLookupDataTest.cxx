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


#include <iostream>
#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbSarImageMetadataInterfaceFactory.h"

int otbSarCalibrationLookupDataTest(int argc, char* argv[])
{
  typedef double                                  RealType;
  typedef otb::SarImageMetadataInterface             ImageMetadataInterfaceType;
  typedef otb::SarCalibrationLookupData              LookupDataType;
  typedef otb::Image<double,  2>                     InputImageType;
  typedef otb::ImageFileReader<InputImageType>       ImageReaderType;

  if (argc < 3 )
    {
    std::cerr << "Usage: otbSarCalibationLookupDataTest /path/to/input/file /path/to/output/file  !"<< std::endl;
    return EXIT_FAILURE;
    }
  ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  const char *  outFileName = argv[2];
  std::ofstream outfile;
  outfile.open(outFileName);

  ImageMetadataInterfaceType::Pointer imageMetadataInterface =
    otb::SarImageMetadataInterfaceFactory::CreateIMI( reader->GetOutput()->GetMetaDataDictionary() );

  if (!imageMetadataInterface.IsNotNull())
    {
    std::cerr << "cannot create a otb::SarImageMetadataInterface for input image." << std::endl;
    return EXIT_FAILURE;
    }

  std::string sensorId;
  imageMetadataInterface->GetSensorID(sensorId);

  outfile << sensorId << std::endl;

  LookupDataType::Pointer lookupDataObj = imageMetadataInterface->GetCalibrationLookupData(0);

  if (!lookupDataObj.IsNotNull())
    {
    std::cerr << "lookupDataObj is Null"<< std::endl;
    return EXIT_FAILURE;
    }

  RealType lutVal = static_cast<RealType>(lookupDataObj->GetValue(10,19));

  outfile << imageMetadataInterface->HasCalibrationLookupDataFlag() << std::endl;
  outfile << lutVal << std::endl;

  return EXIT_SUCCESS;

}
