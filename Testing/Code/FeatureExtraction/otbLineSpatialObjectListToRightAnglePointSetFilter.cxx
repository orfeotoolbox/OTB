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

#include "otbImageFileReader.h"
#include "otbLineSpatialObjectListToRightAnglePointSetFilter.h"
#include "otbImage.h"
#include "otbLineSpatialObjectList.h"
#include "itkPointSet.h"

int otbLineSpatialObjectListToRightAnglePointSetFilter( int argc, char * argv[] )
{
  const char * infname = argv[1];  
  
  const unsigned int                                                Dimension = 2;
  typedef float                                                     PixelType;
  
  /** Typedefs */
  typedef otb::Image<PixelType ,Dimension >                         ImageType;
  typedef otb::ImageFileReader<ImageType>                           ReaderType;
  typedef otb::LineSpatialObjectList                                LinesListType;
  typedef LinesListType::LineType                                   LineType;
  typedef itk::PointSet<PixelType>                                  PointSetType;
  typedef otb::LineSpatialObjectListToRightAnglePointSetFilter<ImageType ,LinesListType, 
                                                               PointSetType >    RightAngleFilterType;

  /** Creatop, of an instance of the filters */
  RightAngleFilterType::Pointer  rightAngleFilter  = RightAngleFilterType::New();
  ReaderType::Pointer            reader            = ReaderType::New();
  
  /** Creation of  lines */
  LinesListType::Pointer list = LinesListType::New();
  LineType::PointListType pointList;
  LineType::LinePointType point;
  
  // Definition of the first line
  float Ux, Uy, Vx, Vy;
  Ux = 31.7;
  Uy = 11.1;
  Vx = 31.7;
  Vy = 25.1;

  point.SetPosition(Ux,Uy);
  pointList.push_back(point);
  point.SetPosition(Vx,Vy);
  pointList.push_back(point);

  LineType::Pointer line = LineType::New();
  line->SetId(0);
  line->SetPoints( pointList );
  line->ComputeBoundingBox();

  list->push_back(line);

  pointList.clear();

  // Definition of a second line
  Ux = 10.1;
  Uy = 10.7;
  Vx = 30.1;
  Vy = 10.6;

  point.SetPosition(Ux,Uy);
  pointList.push_back(point);
  point.SetPosition(Vx,Vy);
  pointList.push_back(point);


  LineType::Pointer line2 = LineType::New();
  line2->SetId(0);
  line2->SetPoints( pointList );
  line2->ComputeBoundingBox();

  list->push_back(line2);

  pointList.clear();

  // Definition of a third line

  Ux = 20.0;
  Uy = 7.0;
  Vx = 40.0;
  Vy = 17.0;

  point.SetPosition(Ux,Uy);
  pointList.push_back(point);
  point.SetPosition(Vx,Vy);
  pointList.push_back(point);

  LineType::Pointer line3 = LineType::New();
  line3->SetId(0);
  line3->SetPoints( pointList );
  line3->ComputeBoundingBox();

  list->push_back(line3);

  
  // Begin the process
  reader->SetFileName(infname);
  rightAngleFilter->SetInputImage(reader->GetOutput());
  rightAngleFilter->SetInput(list);
  rightAngleFilter->Update();

  
  return EXIT_SUCCESS;
}

