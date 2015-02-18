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


#include "otbMetaImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbFlusserMomentsImageFunction.h"
#include "otbImageFunctionAdaptor.h"

typedef unsigned short                                                InputPixelType;
typedef double                                                        PrecisionType;
const unsigned int Dimension =                                        2;

typedef otb::Image<InputPixelType,  Dimension>                        InputImageType;
typedef otb::ImageFileReader<InputImageType>                          ReaderType;
typedef otb::FlusserMomentsImageFunction<InputImageType>              FlusserFunctionType;
typedef otb::ImageFunctionAdaptor<FlusserFunctionType, PrecisionType> FunctionType;

typedef otb::MetaImageFunction<PrecisionType, double>                 MetaImageFunctionType;
typedef MetaImageFunctionType::PointType                              PointType;
typedef MetaImageFunctionType::OutputType                             OutputType;

int otbMetaImageFunctionNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  MetaImageFunctionType::Pointer function = MetaImageFunctionType::New();

  return EXIT_SUCCESS;
}

int otbMetaImageFunction(int itkNotUsed(argc), char * argv[])
{
  // Read the input image
  ReaderType::Pointer   reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  // Build  flusser functions with different radius
  FunctionType::Pointer function1 = FunctionType::New();
  FunctionType::Pointer function2 = FunctionType::New();
  FunctionType::Pointer function3 = FunctionType::New();

  function1->SetInputImage(reader->GetOutput());
  function2->SetInputImage(reader->GetOutput());
  function3->SetInputImage(reader->GetOutput());

  function1->GetInternalImageFunction()->SetNeighborhoodRadius(3);
  function2->GetInternalImageFunction()->SetNeighborhoodRadius(5);
  function3->GetInternalImageFunction()->SetNeighborhoodRadius(7);

  std::ofstream outputStream(argv[2]);

  MetaImageFunctionType::Pointer metaFunction = MetaImageFunctionType::New();
  metaFunction->AddFunction(function1);
  metaFunction->AddFunction(function2);
  metaFunction->AddFunction(function3);

  outputStream<<"Initial number of functions: "<<metaFunction->GetNumberOfFunctions()<<std::endl;

  metaFunction->RemoveNthFunction(2);

  outputStream<<"Number of functions after removing the 2nd: "<<metaFunction->GetNumberOfFunctions()<<std::endl;

  metaFunction->ClearFunctions();

  outputStream<<"Number of functions after clear: "<<metaFunction->GetNumberOfFunctions()<<std::endl;

  metaFunction->AddFunction(function1);
  metaFunction->AddFunction(function2);
  metaFunction->AddFunction(function3);

  outputStream<<"Adding functions again: "<<metaFunction->GetNumberOfFunctions()<<std::endl;

  // For coverage
  metaFunction->GetNthFunction(0);

  PointType p;
  p[0] = atof(argv[3]);
  p[1] = atof(argv[4]);

  OutputType output = metaFunction->Evaluate(p);

  outputStream<<"Evaluate("<<p<<") = "<<output<<std::endl;

  outputStream.close();

  return EXIT_SUCCESS;
}
