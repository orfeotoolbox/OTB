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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbCommandLineArgumentParser.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbStreamingCompareImageFilter.h"

int main(int argc, char* argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Estimator between 2 images");
  parser->AddOption("--Input1","The first input image","-in1", 1,true);
  parser->AddOption("--Input2","The first input image","-in2", 1,true);
  parser->AddOption("--NumBandImage1","Layer number to compare in the input image 1 (between 1 and number of layers)","-layer1",1,true);
  parser->AddOption("--NumBandImage2","Layer number to compare in the input image 2 (between 1 and number of layers)","-layer2",1,true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine(argc,argv,parseResult);
  }
  catch ( itk::ExceptionObject & err )
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

  typedef double PixelType;

  typedef otb::VectorImage<PixelType, 2>                                              ImageType;
  typedef otb::ImageFileReader<ImageType>                                             ReaderType;
  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType>                     ExtractROIMonoFilterType;
  typedef otb::StreamingCompareImageFilter<ExtractROIMonoFilterType::OutputImageType> StreamingCompareImageFilterType;

  // Read input images information
  ReaderType::Pointer reader1= ReaderType::New();
  reader1->SetFileName(parseResult->GetParameterString("--Input1"));
  reader1->GenerateOutputInformation();

  ReaderType::Pointer reader2= ReaderType::New();
  reader2->SetFileName(parseResult->GetParameterString("--Input2"));
  reader2->GenerateOutputInformation();

  unsigned int layer1 = parseResult->GetParameterUInt("--NumBandImage1");
  unsigned int layer2 = parseResult->GetParameterUInt("--NumBandImage2");

  ExtractROIMonoFilterType::Pointer extractROIMonoFilter1= ExtractROIMonoFilterType::New();
  extractROIMonoFilter1->SetInput(reader1->GetOutput());
  extractROIMonoFilter1->SetChannel( layer1 );

  ExtractROIMonoFilterType::Pointer extractROIMonoFilter2= ExtractROIMonoFilterType::New();
  extractROIMonoFilter2->SetInput(reader2->GetOutput());
  extractROIMonoFilter2->SetChannel( layer2 );

  StreamingCompareImageFilterType::Pointer filter = StreamingCompareImageFilterType::New();

  filter->SetInput1(extractROIMonoFilter1->GetOutput());
  filter->SetInput2(extractROIMonoFilter2->GetOutput());

  filter->Update();

  std::cout << "MSE: " << filter->GetMSE() << std::endl;
  std::cout << "MAE: " << filter->GetMAE() << std::endl;
  std::cout << "PSNR: " << filter->GetPSNR() << std::endl;

  return EXIT_SUCCESS;
}
