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
  parser->AddOption("--InputReference","The reference image","-inR", 1, true);
  parser->AddOption("--InputMeasured","The measured image","-inM", 1, true);
  parser->AddOption("--NumBandRefImage","The channel number to compare in the reference image (between 1 and number of channels)","-chR", 1, true);
  parser->AddOption("--NumBandMeasuredImage","The channel number to compare in the measured image (between 1 and number of channels)","-chM", 1, true);
  parser->AddOption("--StartX", "first point in x-axis ", "-x0", 1, false);
  parser->AddOption("--StartY", "first point in y-axis ", "-y0", 1, false);
  parser->AddOption("--SizeX", "size in x-axis ", "-Nx", 1, false);
  parser->AddOption("--SizeY", "size in y-axis ", "-Ny", 1, false);
  
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
  //typedef otb::StreamingStatisticsImageFilter<ExtractROIMonoFilterType::OutputImageType> StreamingCompareImageFilterType;
  // Read input images information
  ReaderType::Pointer reader1= ReaderType::New();
  reader1->SetFileName(parseResult->GetParameterString("--InputReference"));
  reader1->GenerateOutputInformation();

  ReaderType::Pointer reader2= ReaderType::New();
  reader2->SetFileName(parseResult->GetParameterString("--InputMeasured"));
  reader2->GenerateOutputInformation();

  unsigned int layer1 = parseResult->GetParameterUInt("--NumBandRefImage");
  unsigned int layer2 = parseResult->GetParameterUInt("--NumBandMeasuredImage");

  
  
 
  ExtractROIMonoFilterType::Pointer extractROIMonoFilter1= ExtractROIMonoFilterType::New();
  extractROIMonoFilter1->SetInput(reader1->GetOutput());
  extractROIMonoFilter1->SetChannel( layer1 );

  ExtractROIMonoFilterType::Pointer extractROIMonoFilter2= ExtractROIMonoFilterType::New();
  extractROIMonoFilter2->SetInput(reader2->GetOutput());
  extractROIMonoFilter2->SetChannel( layer2 );

  if (parseResult->IsOptionPresent("--StartX")) 
    {
    extractROIMonoFilter1->SetStartX(parseResult->GetParameterULong("--StartX"));
    extractROIMonoFilter2->SetStartX(parseResult->GetParameterULong("--StartX"));
    }
  if (parseResult->IsOptionPresent("--StartY")) 
    {
    extractROIMonoFilter1->SetStartY(parseResult->GetParameterULong("--StartY"));
    extractROIMonoFilter2->SetStartY(parseResult->GetParameterULong("--StartY"));
    }

  if (parseResult->IsOptionPresent("--SizeX"))
    {
    extractROIMonoFilter1->SetSizeX(parseResult->GetParameterULong("--SizeX"));
    extractROIMonoFilter2->SetSizeX(parseResult->GetParameterULong("--SizeX"));
    }
  if (parseResult->IsOptionPresent("--SizeY"))
    {
    extractROIMonoFilter1->SetSizeY(parseResult->GetParameterULong("--SizeY"));
    extractROIMonoFilter2->SetSizeY(parseResult->GetParameterULong("--SizeY"));
    }
    
  StreamingCompareImageFilterType::Pointer filter = StreamingCompareImageFilterType::New();

  filter->SetInput1(extractROIMonoFilter1->GetOutput());
  filter->SetInput2(extractROIMonoFilter2->GetOutput());

  filter->Update();

  std::cout << "MSE: " << filter->GetMSE() << std::endl;
  std::cout << "MAE: " << filter->GetMAE() << std::endl;
  std::cout << "PSNR: " << filter->GetPSNR() << std::endl;

  return EXIT_SUCCESS;
}
