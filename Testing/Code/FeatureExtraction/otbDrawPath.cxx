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
#include "otbDrawPathFilter.h"
#include "otbImageFileWriter.h"

int otbDrawPathDessinCarre(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef InputImageType::PointType              InputImagePointType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::DrawPathFilter<InputImageType, PathType, OutputImageType> DrawPathFilterType;

  ReaderType::Pointer reader         = ReaderType::New();
  WriterType::Pointer writer         = WriterType::New();
  PathType::Pointer   VertexList     = PathType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  InputImageType::ConstPointer image = reader->GetOutput();

  InputImagePointType           pos;
  PathType::ContinuousIndexType cindex;

  VertexList->Initialize();

  /*1er Segment*/
  pos[0] = 30;
  pos[1] = 30;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);
  pos[0] = 30;
  pos[1] = 130;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  /*2eme Segment*/
  pos[0] = 90;
  pos[1] = 130;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  /*3eme Segment*/
  pos[0] = 90;
  pos[1] = 30;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  /*4eme Segment*/
  pos[0] = 30;
  pos[1] = 30;
  image->TransformPhysicalPointToContinuousIndex(pos, cindex);
  VertexList->AddVertex(cindex);

  DrawPathFilterType::Pointer DrawPath = DrawPathFilterType::New();

  DrawPath->SetImageInput(reader->GetOutput());
  DrawPath->SetInputPath(VertexList);
  writer->SetInput(DrawPath->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
