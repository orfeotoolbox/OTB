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
#include <list>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbLineSpatialObjectList.h"

#include "otbDrawLineSpatialObjectListFilter.h"

int otbDrawLineSpatialObjectList(int argc, char* argv[])
{
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef double        InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::DrawLineSpatialObjectListFilter<InputImageType, OutputImageType> FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::LineSpatialObjectList LinesListType;
  typedef LinesListType::LineType    LineType;
  LinesListType::Pointer list = LinesListType::New();

  LineType::PointListType pointList;
  LineType::LinePointType point;

  // Definition of the first line
  double Ux, Uy, Vx, Vy;
  Ux = 10.;
  Uy = 12.;
  Vx = 35.;
  Vy = 29.;

  point.SetPosition(Ux, Uy);
  pointList.push_back(point);
  point.SetPosition(Vx, Vy);
  pointList.push_back(point);

  LineType::Pointer line = LineType::New();
  line->SetId(0);
  line->SetPoints(pointList);
  line->ComputeBoundingBox();

  list->push_back(line);

  pointList.clear();

  // Definition of a second line
  Ux = 8.;
  Uy = 7.;
  Vx = 8.;
  Vy = 46.;

  point.SetPosition(Ux, Uy);
  pointList.push_back(point);
  point.SetPosition(Vx, Vy);
  pointList.push_back(point);

  LineType::Pointer line2 = LineType::New();
  line2->SetId(0);
  line2->SetPoints(pointList);
  line2->ComputeBoundingBox();

  list->push_back(line2);

  pointList.clear();

  // Definition of a third line
  Ux = 52.;
  Uy = 15.;
  Vx = 22.;
  Vy = 38.;

  point.SetPosition(Ux, Uy);
  pointList.push_back(point);
  point.SetPosition(Vx, Vy);
  pointList.push_back(point);

  LineType::Pointer line3 = LineType::New();
  line3->SetId(0);
  line3->SetPoints(pointList);
  line3->ComputeBoundingBox();

  list->push_back(line3);

  /*        LinesListType::const_iterator it;
      std::cout<<list.size()<<std::endl;
      for (it=list.begin(); it!=list.end(); ++it)
      std::cout<< (*it) <<std::endl; */

  filter->SetInputLineSpatialObjectList(list);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
