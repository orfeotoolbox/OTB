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
#include "otbCommandLineArgumentParser.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "otbMatrixMultiplyImageFilter.h"
#include "otbStreamingImageFileWriter.h"

const unsigned int Dimension = 2;
typedef float PixelType;

typedef otb::VectorImage<PixelType, Dimension> VectorImageType;
typedef otb::ImageFileReader<VectorImageType> ReaderType;
typedef otb::VectorImageToMatrixImageFilter<VectorImageType> VectorImageToMatrixImageFilterType;
typedef otb::MatrixMultiplyImageFilter<VectorImageType,VectorImageType,PixelType> MatrixMultiplyImageFilterType;
typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;

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
  parser->AddInputImage();
  parser->AddOption( "--Endmembers", "Endmembers image", "-e", 1, true );
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

  const char * inputImageName = parseResult->GetInputImage().c_str();
  std::string endmembersImageName = parseResult->GetParameterString("--Endmembers");
  const char * outputImageName = parseResult->GetOutputImage().c_str();

  /*
   *
   * PROCESSING
   *
   */

  ReaderType::Pointer readerAbundance = ReaderType::New();
  readerAbundance->SetFileName(inputImageName);

  ReaderType::Pointer readerEndmembers = ReaderType::New();
  readerEndmembers->SetFileName(endmembersImageName);

  /*
   * Transform Endmembers image to matrix representation
   */
  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(readerEndmembers->GetOutput());
  endMember2Matrix->Update();

  /*
   * Multiply abundance and endmembers
   */
  MatrixMultiplyImageFilterType::Pointer mul = MatrixMultiplyImageFilterType::New();
  mul->SetMatrix(endMember2Matrix->GetMatrix());
  mul->SetInput(readerAbundance->GetOutput());

  /*
   * Write output
   */
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageName);
  writer->SetInput(mul->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
