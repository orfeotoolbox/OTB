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
#include "otbOSMDownloader.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataProperties.h"
#include "otbOSMDataToVectorDataGenerator.h"
#include "otbVectorDataFileWriter.h"

namespace otb
{

int OSMDownloader::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("OSMDownloader");
  descriptor->SetDescription("Generate a vector data from OSM on the input image extend");
  descriptor->AddOption("InputImage", "Support to estimate the models on",
                        "in", 1, true, ApplicationDescriptor::InputImage);
  descriptor->AddOption("OSMKey", "OSM key (highway, buildings...)",
                          "key", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("DEMDirectory", "DEM directory",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("Output", "OutputVectorData",
                        "out", 1, false, ApplicationDescriptor::FileName);
  return EXIT_SUCCESS;
}


int OSMDownloader::Execute(otb::ApplicationOptionsResult* parseResult)
{
  typedef otb::OSMDataToVectorDataGenerator           VectorDataProviderType;
  typedef VectorDataProviderType::VectorDataType      VectorDataType;
  typedef VectorDataType::ValuePrecisionType          PrecisionType;
  typedef VectorDataType::PrecisionType               CoordRepType;

  typedef otb::VectorImage<PrecisionType , 2>         ImageType;
  typedef otb::ImageFileReader<ImageType>             ImageReaderType;

  typedef otb::ImageToEnvelopeVectorDataFilter<ImageType, VectorDataType>
                                                      EnvelopeFilterType;
  typedef otb::VectorDataProperties<VectorDataType>   VectorDataPropertiesType;
  typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataWriterType;


  //Instantiate
  ImageReaderType::Pointer                 imgReader = ImageReaderType::New();
  EnvelopeFilterType::Pointer         envelopeFilter = EnvelopeFilterType::New();
  VectorDataProviderType::Pointer     vdOSMGenerator = VectorDataProviderType::New();
  VectorDataPropertiesType::Pointer     vdProperties = VectorDataPropertiesType::New();
  VectorDataWriterType::Pointer             vdWriter = VectorDataWriterType::New();

  //Read the image
  imgReader->SetFileName(parseResult->GetParameterString("InputImage"));
  imgReader->UpdateOutputInformation();

  //Generate the envelope
  envelopeFilter->SetInput(imgReader->GetOutput()); //->Output in WGS84
  envelopeFilter->Update();

  vdProperties->SetVectorDataObject(envelopeFilter->GetOutput());
  vdProperties->ComputeBoundingRegion();

  double north, south, east, west;
  north = vdProperties->GetBoundingRegion().GetIndex()[1]
           + vdProperties->GetBoundingRegion().GetSize()[1];
  south = vdProperties->GetBoundingRegion().GetIndex()[1];
  east  = vdProperties->GetBoundingRegion().GetIndex()[0]
           + vdProperties->GetBoundingRegion().GetSize()[0];
  west  = vdProperties->GetBoundingRegion().GetIndex()[0];

  /*
  std::cout << "north : " << north << std::endl;
  std::cout << "south : " << south << std::endl;
  std::cout << "east  : " << east  << std::endl;
  std::cout << "west  : " << west  << std::endl;
  */

  vdOSMGenerator->SetNorth(north);
  vdOSMGenerator->SetSouth(south);
  vdOSMGenerator->SetEast(east);
  vdOSMGenerator->SetWest(west);

  try
  {
    vdOSMGenerator->Update();
  }
  catch ( itk::ExceptionObject & err )
  {
    std::cout << "Exception itk::ExceptionObject raised !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << vdOSMGenerator->GetVectorDataByName(parseResult->GetParameterString("OSMKey"))->Size()-3
      << std::endl;

  vdWriter->SetInput(vdOSMGenerator->GetVectorDataByName(parseResult->GetParameterString("OSMKey")));
  vdWriter->SetFileName(parseResult->GetParameterString("Output"));
  vdWriter->Update();

  return EXIT_SUCCESS;
}

}
