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
#include "otbBandMath.h"
#include <iostream>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"
#include "otbStreamingImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"
#include "otbBandMathImageFilter.h"

namespace otb
{

int BandMath::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("BandMath");
  descriptor->SetDescription("Perform a mathematical operation on monoband images");
  descriptor->AddOptionNParams("InputImages", "Input images file names", "ims", true, ApplicationDescriptor::InputImage);
  descriptor->AddOutputImage();
  descriptor->AddOption("Expression", "The mathematical expression to apply. \nUse im1b1 for the first band, im1b2 for the second one...",
                        "exp", 1, true, ApplicationDescriptor::String);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);
  return EXIT_SUCCESS;
}

int BandMath::Execute(otb::ApplicationOptionsResult* parseResult)
{
  const unsigned int NbImages = parseResult->GetNumberOfParameters("InputImages");

  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension> ImageType;
  typedef otb::VectorImage<double, Dimension> VectorImageType;
  typedef otb::ImageFileReader<VectorImageType> ImageReaderType;
  typedef otb::ObjectList<ImageReaderType> ReaderListType;
  typedef otb::MultiToMonoChannelExtractROI<ImageType::PixelType, ImageType::PixelType> ExtractROIFilterType;
  typedef otb::ObjectList<ExtractROIFilterType> ExtractROIFilterListType;
  typedef otb::BandMathImageFilter<ImageType> BandMathImageFilterType;

  ReaderListType::Pointer readerList = ReaderListType::New();
  ExtractROIFilterListType::Pointer channelExtractorList = ExtractROIFilterListType::New();
  BandMathImageFilterType::Pointer filter = BandMathImageFilterType::New();

  unsigned int bandId = 0;
  unsigned int imageId = 0;

  for (unsigned int i = 0; i < NbImages; i++)
    {
    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetFileName(parseResult->GetParameterString("InputImages", i).c_str());

    imageReader->UpdateOutputInformation();

    otbMsgDevMacro( << "Image #" << i + 1 << " has " << imageReader->GetOutput()->GetNumberOfComponentsPerPixel()
        << " components" << std::endl );

    for (unsigned int j = 0; j < imageReader->GetOutput()->GetNumberOfComponentsPerPixel(); j++)
      {
      std::ostringstream tmpParserVarName;
      tmpParserVarName << "im" << imageId + 1 << "b" << j + 1;

      ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
      extractROIFilter->SetInput(imageReader->GetOutput());
      extractROIFilter->SetChannel(j + 1);
      extractROIFilter->GetOutput()->UpdateOutputInformation();
      channelExtractorList->PushBack(extractROIFilter);
      filter->SetNthInput(bandId, channelExtractorList->Back()->GetOutput(), tmpParserVarName.str());

      bandId++;
      }
    imageId++;
    readerList->PushBack(imageReader);
    }

  filter->SetExpression(parseResult->GetParameterString("Expression", 0));

  typedef otb::StreamingImageFileWriter<ImageType> ImageWriterType;
  ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  imageWriter->SetFileName(parseResult->GetOutputImage());
  imageWriter->SetInput(filter->GetOutput());

  unsigned int ram = 256;
  if (parseResult->IsOptionPresent("AvailableMemory"))
    {
    ram = parseResult->GetParameterUInt("AvailableMemory");
    }
  imageWriter->SetAutomaticTiledStreaming(ram);

  imageWriter->Update();

  return EXIT_SUCCESS;
}

}

