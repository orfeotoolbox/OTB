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
#include "otbImage.h"
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"
#include "otbPathListSource.h"
#include "otbImageToPathListAlignFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbImageFileWriter.h"

int otbDrawPathAlign(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType> PathListType;
  typedef PathListType::OutputPathListType                          OutputPathListType;

  typedef otb::DrawPathListFilter<InputImageType, PathType, OutputImageType> DrawPathFilterType;

  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();

  DrawPathFilterType::Pointer DrawPath = DrawPathFilterType::New();
  PathType::Pointer           VertexList     = PathType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType> PathListAlignType;
  PathListAlignType::Pointer testList = PathListAlignType::New();

  //OTB-FA-00010-CS
  testList->SetInput(reader->GetOutput());
  testList->Update();

  OutputPathListType * sortiePath = testList->GetOutput();
  int                  nbPath = sortiePath->Size();
  std::cout << "NbPath: " << nbPath << std::endl;

  InputImageType::ConstPointer imageIn   = reader->GetOutput();

  DrawPath->SetInput(imageIn);
  DrawPath->SetInputPath(sortiePath);

  writer->SetInput(DrawPath->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
