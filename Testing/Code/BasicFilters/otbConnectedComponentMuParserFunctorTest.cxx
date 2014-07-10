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

#include "otbConnectedComponentMuParserFunctor.h"
#include <itkConnectedComponentFunctorImageFilter.h>

#include "otbParser.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"



int otbConnectedComponentMuParserFunctorTest(int argc, char *argv[])
{

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];
  const char * expression =     argv[3];
  const char * maskFilename;

   // maskFilename option is present
   if (argc == 5)
   {
       maskFilename = argv[4];
   }
   else
       {
       maskFilename = NULL;
       }

  typedef float InputPixelType;
  const unsigned int     Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension>      InputVectorImageType;
  typedef otb::Image<unsigned int, Dimension>                InputMaskImageType;
  typedef otb::ImageFileReader<InputVectorImageType>        ReaderType;
  typedef otb::ImageFileReader<InputMaskImageType>          MaskReaderType;
  typedef otb::Image<unsigned int, Dimension>               OutputImageType;

  typedef otb::ImageFileWriter<OutputImageType>             WriterType;

  typedef otb::Functor::ConnectedComponentMuParserFunctor<InputVectorImageType::PixelType>  FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<InputVectorImageType, OutputImageType, FunctorType, InputMaskImageType> FilterType;


  MaskReaderType::Pointer maskReader;
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();

  reader->SetFileName(inputFilename);
  reader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);

 filter->SetInput(reader->GetOutput());

 if(NULL != maskFilename)
 {
  maskReader = MaskReaderType::New();
  maskReader->SetFileName(maskFilename);
  maskReader->GenerateOutputInformation();

  filter->SetMaskImage(maskReader->GetOutput());
 }


  std::string stringExpression(expression);

 filter->GetFunctor().SetExpression(stringExpression);
 filter->Update();

  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
