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
#include "otbBinarySpectralAngleImageFilter.h"
#include "otbStreamingImageFileWriter.h"
#include "otbStreamingStatisticsImageFilter.h"

#include "otbCommandLineArgumentParser.h"


const unsigned int Dimension = 2;
typedef float PixelType;

typedef otb::Image<PixelType, Dimension> ImageType;
typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

typedef otb::ImageFileReader<VectorImageType> VectorReaderType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::BinarySpectralAngleImageFilter<VectorImageType, VectorImageType, ImageType> BinarySpectralAngleImageFilterType;
typedef otb::StreamingImageFileWriter<ImageType> WriterType;
typedef otb::StreamingStatisticsImageFilter<ImageType> StatisticsFilterType;

typedef vnl_vector<PixelType> VectorType;
typedef vnl_matrix<PixelType> MatrixType;



int main(int argc, char * argv[])
{
  /*
   *
   * DEFINE APPLICATION
   *
   */
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Constructs an hyperspectral cube from an endmembers image and an abundance map");
  parser->AddOption("--Input1", "first input image file name", "-in1", 1, true);
  parser->AddOption("--Input2", "second input image file name", "-in2", 1, true);
  parser->AddOption("--OutputStats", "ASCII file where the spectral angle statisitics will be stored", "-s", 1, true);
  parser->AddOutputImage();

  /*
   *
   * EXTRACT PARAMETERS
   *
   */
  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine( argc, argv, parseResult );
  }
  catch( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if ( descriptionException.find("ParseCommandLine(): Help Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    if(descriptionException.find("ParseCommandLine(): Version Parser")
        != std::string::npos )
      return EXIT_SUCCESS;
    return EXIT_FAILURE;
  }

  std::string input1 = parseResult->GetParameterString("--Input1");
  std::string input2 = parseResult->GetParameterString("--Input2");
  std::string output = parseResult->GetOutputImage();

  VectorReaderType::Pointer reader1 = VectorReaderType::New();
  reader1->SetFileName(input1);

  VectorReaderType::Pointer reader2 = VectorReaderType::New();
  reader2->SetFileName(input2);

  BinarySpectralAngleImageFilterType::Pointer spectralAngle = BinarySpectralAngleImageFilterType::New();
  spectralAngle->SetInput1(reader1->GetOutput());
  spectralAngle->SetInput2(reader2->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(output);
  writer->SetInput(spectralAngle->GetOutput());
  writer->Update();
  writer = NULL;

  ReaderType::Pointer readerOut = ReaderType::New();
  readerOut->SetFileName(output);

  StatisticsFilterType::Pointer stats = StatisticsFilterType::New();
  stats->SetInput(readerOut->GetOutput());
  stats->Update();

  std::ofstream statsFile(parseResult->GetParameterString("--OutputStats").c_str());
  statsFile << "Mean  : " << stats->GetMean() << std::endl;
  statsFile << "Sigma : " << stats->GetSigma() << std::endl;
  statsFile.close();

  std::cout << "Mean  : " << stats->GetMean() << std::endl;
  std::cout << "Sigma : " << stats->GetSigma() << std::endl;

  return EXIT_SUCCESS;
}
