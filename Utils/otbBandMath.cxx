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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"
#include "otbStreamingImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"
#include "otbBandMathImageFilter.h"


int generic_main_bandMath(otb::CommandLineArgumentParseResult* parseResult)
{
  const unsigned int NbImages = parseResult->GetNumberOfParameters("--InputImages");

  std::cout << "Compute result considering " << NbImages << " input images" << std::endl;

  const unsigned int Dimension = 2;
  typedef otb::Image<double, Dimension>          ImageType;
  typedef otb::VectorImage<double, Dimension>    VectorImageType;
  typedef otb::ImageFileReader<VectorImageType>  ImageReaderType;
  typedef otb::ObjectList<ImageReaderType>       ReaderListType;
  typedef otb::MultiToMonoChannelExtractROI<ImageType::PixelType, ImageType::PixelType> ExtractROIFilterType;
  typedef otb::ObjectList<ExtractROIFilterType>  ExtractROIFilterListType;
  typedef otb::BandMathImageFilter<ImageType>    BandMathImageFilterType;

  ReaderListType::Pointer readerList = ReaderListType::New();
  ExtractROIFilterListType::Pointer channelExtractorList = ExtractROIFilterListType::New();
  BandMathImageFilterType::Pointer filter = BandMathImageFilterType::New();
  
  unsigned int bandId = 0;
  unsigned int imageId = 0;

  for (unsigned int i = 0; i < NbImages; i++)
    {
    ImageReaderType::Pointer imageReader = ImageReaderType::New();
    imageReader->SetFileName(parseResult->GetParameterString("--InputImages", i).c_str());

    imageReader->UpdateOutputInformation();

    std::cout << "Image #" << i+1 << " has " <<  imageReader->GetOutput()->GetNumberOfComponentsPerPixel()
              << " components" << std::endl;


    for(unsigned int j = 0; j < imageReader->GetOutput()->GetNumberOfComponentsPerPixel(); j++)
      {
      std::ostringstream tmpParserVarName;
      tmpParserVarName << "im" << imageId+1 << "b" << j+1;
      
      ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
      extractROIFilter->SetInput(imageReader->GetOutput());
      extractROIFilter->SetChannel(j+1);
      extractROIFilter->GetOutput()->UpdateOutputInformation();
      channelExtractorList->PushBack(extractROIFilter);
      filter->SetNthInput(bandId, channelExtractorList->Back()->GetOutput(), tmpParserVarName.str());
      
      bandId ++;
      }
    imageId ++;
    readerList->PushBack(imageReader);
    }

  filter->SetExpression(parseResult->GetParameterString("--Expression", 0));

  typedef otb::StreamingImageFileWriter<ImageType> ImageWriterType;
  ImageWriterType::Pointer imageWriter = ImageWriterType::New();
  imageWriter->SetFileName(parseResult->GetOutputImage());
  imageWriter->SetInput(filter->GetOutput());
  imageWriter->Update();

  return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Perform a mathematical operation on monoband images");
  parser->AddOutputImage();
  parser->AddOptionNParams("--InputImages", "input images file names", "-ims");
  parser->AddOption("--Expression", "The mathematical expression to be applied (must be between double quotes)", "-exp");
  
  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer parseResult = ParserResultType::New();

  try
    {
    parser->ParseCommandLine(argc, argv, parseResult);
    }
  catch (itk::ExceptionObject & err)
    {
    std::string descriptionException = err.GetDescription();
    if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
      {
      return EXIT_SUCCESS;
      }
    if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
      {
      return EXIT_SUCCESS;
      }
    return EXIT_FAILURE;
    }

  generic_main_bandMath(parseResult);

  return EXIT_SUCCESS;
}
