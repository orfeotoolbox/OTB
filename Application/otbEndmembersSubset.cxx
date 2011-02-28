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
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->SetProgramDescription("Select a subset of pixels in an endmembers image (assumed to be one-line, with each vector pixel being an endmember)");
  parser->AddInputImage();
  parser->AddOptionNParams("--Select", "the endmembers indicies to select (incompatible with Remove)", "-s", false);
  parser->AddOptionNParams("--Remove", "the endmembers indicies to remove (incompatible with Select)", "-r", false);
  parser->AddOutputImage();

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
  const char * outputImageName = parseResult->GetOutputImage().c_str();


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName);
  reader->Update();
  VectorImageType::Pointer input = reader->GetOutput();

  const unsigned int nbEndmembers = input->GetLargestPossibleRegion().GetNumberOfPixels();

  std::vector<unsigned int> indicies;
  if (parseResult->IsOptionPresent("--Select"))
    {
    int n = parseResult->GetNumberOfParameters("--Select");
    for (int i = 0; i < n; i++)
      {
      indicies.push_back( parseResult->GetParameterUInt("--Select", i) );
      }
    }
  else if (parseResult->IsOptionPresent("--Remove"))
    {
    std::vector<unsigned int> indicies_removed;
    int n = parseResult->GetNumberOfParameters("--Remove");
    for (int i = 0; i < n; i++)
      {
      indicies_removed.push_back( parseResult->GetParameterUInt("--Remove", i) );
      }
    for (int i = 0; i < nbEndmembers; i++)
      {
      if ( 0 == std::count(indicies_removed.begin(), indicies_removed.end(), i) )
        indicies.push_back( i );
      }
    }
  else
    {
    std::cerr << "--Select or --Remove must be specified" << std::endl;
    }

  std::sort(indicies.begin(), indicies.end());


  VectorImageType::Pointer output = VectorImageType::New();
  VectorImageType::RegionType region;
  VectorImageType::IndexType index;
  VectorImageType::SizeType  size;
  index.Fill(0);
  size[0] = indicies.size();
  size[1] = 1;
  region.SetIndex(index);
  region.SetSize(size);
  output->SetRegions(region);
  output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
  output->Allocate();

  VectorImageType::IndexType indexIn;
  VectorImageType::IndexType indexOut;
  indexIn.Fill(0);
  indexOut.Fill(0);
  for(unsigned int i = 0; i < nbEndmembers; i++)
    {
    indexIn[0] = i;
    if ( 0 != std::count(indicies.begin(), indicies.end(), i) )
      {
      std::cout << "Writing " << indexIn << " in " << indexOut << std::endl;
      output->SetPixel(indexOut, input->GetPixel(indexIn));
      indexOut[0]++;
      }
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageName);
  writer->SetInput(output);
  writer->Update();

  return EXIT_SUCCESS;
}
