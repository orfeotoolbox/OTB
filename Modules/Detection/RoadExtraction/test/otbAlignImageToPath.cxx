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
#include "itkPolyLineParametricPath.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageToPathListAlignFilter.h"

#include "itkPathSource.h"

#include <stdio.h>

int otbAlignImageToPath(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double InputPixelType;
  typedef double OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;

  PathType::Pointer ltoto = PathType::New();

  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  typedef otb::ImageToPathListAlignFilter<InputImageType, PathType> ListAlignFilterType;
  typedef ListAlignFilterType::ValueType                            ValueType;
  typedef ListAlignFilterType::SizeType                             SizeType;
  typedef otb::ImageFileWriter<OutputImageType>                     WriterType;

  ReaderType::Pointer     reader = ReaderType::New();
  WriterType::Pointer     writer = WriterType::New();
  InputImageType::Pointer ImageIn = InputImageType::New();

  ListAlignFilterType::Pointer testList = ListAlignFilterType::New();

  reader->SetFileName(inputFilename);

  //OTB-FA-00010-CS
  testList->SetInput(reader->GetOutput());

  typedef ListAlignFilterType::OutputPathListType ListAlignFilterOutputPathListType;

  otbGenericMsgDebugMacro(<< "Before update");
  testList->Update();
  otbGenericMsgDebugMacro(<< "After update");

  ValueType pathValue;
  pathValue = testList->GetPathValue();
  testList->SetPathValue(pathValue);

  ValueType  backgroundValue;
  backgroundValue = testList->GetBackgroundValue();
  testList->SetBackgroundValue(backgroundValue);

  SizeType   size;
  size = testList->GetSize();
  testList->SetSize(size);

  bool isMeaningfulSegment;
  isMeaningfulSegment = testList->GetisMeaningfulSegment();
  testList->SetisMeaningfulSegment(isMeaningfulSegment);

  int NbGradDirection;
  NbGradDirection = testList->GetNbGradDirection();
  testList->SetNbGradDirection(NbGradDirection);

  int NbLineDirection;
  NbLineDirection = testList->GetNbLineDirection();
  testList->SetNbLineDirection(NbLineDirection);

  double MinGradNorm;
  MinGradNorm = testList->GetMinGradNorm();
  testList->SetMinGradNorm(MinGradNorm);

  double Eps;
  Eps = testList->GetEps();
  testList->SetEps(Eps);

  ListAlignFilterOutputPathListType * sortiePath = testList->GetOutput();

  otbGenericMsgDebugMacro(<< "Writing :");

  FILE *file = fopen(outputFilename, "w");
  if (file == NULL)
    {
    fprintf(stderr, "Error, can't open file");
    exit(-1);
    }
  typedef itk::ContinuousIndex<double, 2>            VertexType;
  typedef itk::VectorContainer<unsigned, VertexType> VertexListType;
  typedef VertexListType::ConstPointer               VertexListTypePointer;
  VertexListTypePointer vertexList;
  VertexType            cindex;
  double                x1, y1;
  //double                x2, y2;

  int nbPath = sortiePath->Size();
  otbGenericMsgDebugMacro(<< "NbSegment: " << nbPath);
  fprintf(file, "Nb Segment: %d\n", nbPath);
  for (int i = 0; i < nbPath; ++i)
    {
    vertexList = sortiePath->GetNthElement(i)->GetVertexList();
    cindex = vertexList->GetElement(0);
    x1 = cindex[0];
    y1 = cindex[1];
    cindex = vertexList->GetElement(1);
    //x2 = cindex[0];
    //y2 = cindex[1];
    fprintf(file, "%8.3f %8.3f\n", x1, y1);
    }
  fclose(file);

  //        writer->Update();

  return EXIT_SUCCESS;
}
