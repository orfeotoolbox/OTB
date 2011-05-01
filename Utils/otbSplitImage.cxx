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

#include "otbSplitImage.h"

#include <iostream>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbStandardFilterWatcher.h"

namespace otb
{

template<typename PixelType>
int generic_main_split(otb::ApplicationOptionsResult* parseResult)
{
  std::string base;
  std::string extension;
  std::string baseName = parseResult->GetParameterString("OutputImagesBaseName");
  size_t pointPos = baseName.rfind(".");
  if (pointPos != std::string::npos)
    {
    base = baseName.substr(0, pointPos);
    extension = baseName.substr(pointPos);
    }
  else
    {
    base = baseName;
    extension = "";
    }
  
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>   ImageReaderType;
  typename ImageReaderType::Pointer reader = ImageReaderType::New();
  reader->SetFileName(parseResult->GetInputImage().c_str());
  reader->UpdateOutputInformation();

  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType> FilterType;
  typename FilterType::Pointer filter = FilterType::New();
  filter->SetInput(reader->GetOutput());

  typedef otb::Image<PixelType, Dimension>      OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> ImageWriterType;
  typename ImageWriterType::Pointer writer = ImageWriterType::New();
  writer->SetInput(filter->GetOutput());


  for (unsigned int i = 0;
       i < reader->GetOutput()->GetNumberOfComponentsPerPixel();
       ++i )
    {
    std::stringstream filename; // = baseName + "-" + str()
    filename << base << "-" << i << extension;
    filter->SetChannel(i+1); //FIXME change the convention
    writer->SetFileName(filename.str());
    otb::StandardFilterWatcher watcher(writer, "Writing "+filename.str());
    writer->Update();
    }

  return EXIT_SUCCESS;
}

int SplitImage::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("SplitImage");
  descriptor->SetDescription("Split a N multiband image into N images");
  descriptor->AddInputImage();
  descriptor->AddOption("OutputImagesBaseName", "Base name for the output images", "on", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("OutputPixelType",
                    "OutputPixelType: unsigned char (1), short int (2), int (3), float (4),"
                    " double (5), unsigned short int (12), unsigned int (13); default 1",
                    "t", 1, false, ApplicationDescriptor::Integer);
  
  return EXIT_SUCCESS;
}

int SplitImage::Execute(otb::ApplicationOptionsResult* parseResult)
{
  unsigned int type = 1;
  if (parseResult->IsOptionPresent("OutputPixelType"))
    {
    type = parseResult->GetParameterUInt("OutputPixelType");
    }

  switch (type)
    {
    case 1:
      generic_main_split<unsigned char> (parseResult);
      break;
    case 2:
      generic_main_split<short int> (parseResult);
      break;
    case 3:
      generic_main_split<int> (parseResult);
      break;
    case 4:
      generic_main_split<float> (parseResult);
      break;
    case 5:
      generic_main_split<double> (parseResult);
      break;
    case 12:
      generic_main_split<unsigned short int> (parseResult);
      break;
    case 13:
      generic_main_split<unsigned int> (parseResult);
      break;
    default:
      generic_main_split<unsigned char> (parseResult);
      break;
    }
  return EXIT_SUCCESS;
}
}
