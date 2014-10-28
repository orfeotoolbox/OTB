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
#include "itkMacro.h"

#include <iostream>
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


#include "otbMaskMuParserFilter.h"

int otbMaskMuParserFilterTest(int argc, char * argv[])
{

  if (argc != 4)
     {
     std::cerr << "Usage: " << argv[0] <<
     " infname outframe MuParserexpression"
               << std::endl;
     return EXIT_FAILURE;
     }

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  const char * expression =     argv[3];


  typedef float InputPixelType;
  const unsigned int     Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension>      InputVectorImageType;
  typedef otb::ImageFileReader<InputVectorImageType>        ReaderType;
  typedef otb::Image<unsigned int, Dimension>                OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>             WriterType;
  typedef otb::MaskMuParserFilter<InputVectorImageType, OutputImageType>  FilterType;


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);


  filter->SetInput(reader->GetOutput());

  std::string stringExpression(expression);
  filter->SetExpression(stringExpression);
  filter->UpdateOutputInformation();


  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
