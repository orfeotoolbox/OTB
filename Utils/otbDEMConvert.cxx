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

#include "otbDEMConvert.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbDEMConvertAdapter.h"

namespace otb
{

template<typename TPixelType>
int generic_convert_to_tif(otb::ApplicationOptionsResult* parseResult, std::string tempFilename)
{
  typedef otb::VectorImage<TPixelType>             ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType> WriterType;

  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(parseResult->GetInputImage().c_str());
  writer->SetFileName(tempFilename);

  writer->SetInput(reader->GetOutput());\
  writer->Update();

  return EXIT_SUCCESS;
}


int DEMConvert::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("DEMConvertApplication");
  descriptor->SetDescription("Convert a DEM file into a general raster (.ras, .geom and .omd). To be used, those files have to be in a stand alone directory");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputPath", "The filename (or path filename) of the output. It generates a Output.geom, Output.omd and Output.ras file.","out", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("KeepTif", "Keep the temporary generate tif file.","ktif", 0, false, ApplicationDescriptor::Boolean);
  
  return EXIT_SUCCESS;
}

int temporaryMethod(std::string tempFilename, std::string output)
{
}


/* The main is simple : read image using OTB and write it as a tif.
* Read this tif using OSSIM and convert it as a general raster file (.ras, .geom and . omd)
* first write the image as a tif (supported by OSSIM) allows to not only be abble to work
* with OSSIM supported format but any more.
*/

int DEMConvert::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Load input and output filename
  const char * input_file = parseResult->GetInputImage().c_str();

  std::string tempFilename(parseResult->GetParameterString("OutputPath"));
  tempFilename += "_DEMConvert.tif";
  
  // Search for the input
  typedef otb::VectorImage<double, 2> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader=ReaderType::New();
  reader->SetFileName(input_file);
  reader->UpdateOutputInformation();

  // Generate the tif image using OTB
  // We keep the same pixel type
  std::string componentTypeInfo(reader->GetImageIO()->GetComponentTypeInfo().name());
  if( componentTypeInfo == typeid(unsigned char).name())
    {
    generic_convert_to_tif<unsigned char>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(char).name())
    {
    generic_convert_to_tif<char>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(unsigned short).name())
    {
    generic_convert_to_tif<unsigned short>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(short).name())
    {
    generic_convert_to_tif<short>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(unsigned int).name())
    {
    generic_convert_to_tif<unsigned int>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(int).name())
    {
    generic_convert_to_tif<int>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(unsigned long).name())
    {
    generic_convert_to_tif<unsigned long>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(long).name())
    {
    generic_convert_to_tif<long>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(float).name())
    {
    generic_convert_to_tif<float>(parseResult, tempFilename);
    }
  else if( componentTypeInfo == typeid(double).name())
    {
    generic_convert_to_tif<double>(parseResult, tempFilename);
    }
  else
    {
    itkExceptionMacro("This application doesn't support image pixel type " << componentTypeInfo);
    return EXIT_FAILURE;
    }
  std::string output = parseResult->GetParameterString("OutputPath")+".ras";

  DEMConvertAdapter::Pointer DEMConverter = DEMConvertAdapter::New();
  return DEMConverter->Convert(tempFilename, output);

  if ( parseResult->IsOptionPresent("KeepTif") == false)
    {
    bool resRemove = itksys::SystemTools::RemoveFile(tempFilename.c_str());
    if( resRemove == false )
      {
      std::cout<<"Enable to erase the output temporary file "<<tempFilename<<"."<<std::endl;
      }
    }

  return EXIT_SUCCESS;
}


} // namespace otb
