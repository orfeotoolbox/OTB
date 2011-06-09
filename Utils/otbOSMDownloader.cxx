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
  descriptor->AddOption("OSMKey", "OSM key (highway, building...)",
                        "key", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("OSMValue", "OSM Value (motorway, footway...)",
                        "val", 1, false, ApplicationDescriptor::String);
  descriptor->AddOption("DEMDirectory", "DEM directory",
                        "dem", 1, false, ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("Output", "OutputVectorData",
                        "out", 1, false, ApplicationDescriptor::FileName);
  
  std::ostringstream oss;
  oss << "Print the key/value classes available for the bounding box of the input image "<<std::endl;
  oss << "\t\t\t\t  ** If not used : Note that the options OSMKey (-key) and Output (-out) become mandatory";
  descriptor->AddOption("PrintClasses", oss.str().c_str(), "print", 0, false, ApplicationDescriptor::Boolean);
  
  return EXIT_SUCCESS;
}

int OSMDownloader::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // CASE:  when the -print option is not required and the User
  // does not set the option OSMKey or the option Output or does not
  // set both of them
  if (!parseResult->IsOptionPresent("PrintClasses"))
    {
    if( !parseResult->IsOptionPresent("Output") && !parseResult->IsOptionPresent("OSMKey"))
      {
      itkExceptionMacro(<<"Missing Options : Output (-out) and OSMKey (-key)");
      }

    if(!parseResult->IsOptionPresent("OSMKey") )
      {
      itkExceptionMacro(<<"Missing Option : OSMKey (-key)");
      }
    
    if( !parseResult->IsOptionPresent("Output"))
      {
      itkExceptionMacro(<<"Missing Option : Output (-out)");
      }    
    }

  typedef otb::OSMDataToVectorDataGenerator           VectorDataProviderType;
  typedef VectorDataProviderType::KeyMapType          KeyMapType;
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
  if (parseResult->IsOptionPresent("DEMDirectory"))
    {
    envelopeFilter->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory"));
    }
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

  // If the user wants to print the Key/Values present in the XML file
  // downloaded  :
  if (parseResult->IsOptionPresent("PrintClasses"))
    {
    // Print the classes
    KeyMapType  keymap = vdOSMGenerator->GetKeysMap();

    KeyMapType::iterator  it = keymap.begin();

    while(it != keymap.end())
      {
      std::cout << " Key : "<< (*it).first<< " value : ";
      for(unsigned int i = 0; i < (*it).second.size(); i++)
        std::cout<< (*it).second[i]<< " ";
      std::cout<< std::endl;
      ++it;
      }
    return EXIT_SUCCESS;
    }

  // Get the VectorData By name
  if (parseResult->IsOptionPresent("OSMValue"))
    {
    vdWriter->SetInput(vdOSMGenerator->GetVectorDataByName(parseResult->GetParameterString("OSMKey"),
                                                           parseResult->GetParameterString("OSMValue")));

    std::cout << vdOSMGenerator->GetVectorDataByName(parseResult->GetParameterString("OSMKey"),
                                                     parseResult->GetParameterString("OSMValue"))->Size()-3
              << " elements retrieved" << std::endl;
    }
  else
    {
    vdWriter->SetInput(vdOSMGenerator->GetVectorDataByName(parseResult->GetParameterString("OSMKey")));
    std::cout << vdOSMGenerator->GetVectorDataByName(parseResult->GetParameterString("OSMKey"))->Size()-3
              << " elements retrieved" << std::endl;
    }

  vdWriter->SetFileName(parseResult->GetParameterString("Output"));
  vdWriter->Update();

  return EXIT_SUCCESS;
}
}
