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

#include "otbImage.h"
#include "itkPointSet.h"
#include "otbLineSpatialObjectListToRightAnglePointSetFilter.h"
#include "otbLineSpatialObjectList.h"
#include "otbDrawLineSpatialObjectListFilter.h"
#include "otbLineSegmentDetector.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include <iostream>
#include <fstream>

int main( int argc, char * argv[] )
{
  const   char * infname   = argv[1];  
  const   char * outfname  = argv[2];
  
  const unsigned int           Dimension = 2;
  typedef float                PixelType;
  
  /** Typedefs */
  typedef otb::Image<PixelType ,Dimension>   ImageType;
  typedef otb::ImageFileReader<ImageType>    ReaderType;
  typedef otb::ImageFileWriter<ImageType>    WriterType;
  typedef otb::LineSpatialObjectList         LinesListType;
  typedef LinesListType::LineType            LineType;
  typedef std::vector<LineType*>             VectorLines;
  typedef itk::PointSet<VectorLines , Dimension>     PointSetType;
  typedef otb::LineSegmentDetector<ImageType , PixelType>   lsdFilterType;
  typedef otb::LineSpatialObjectListToRightAnglePointSetFilter<ImageType,
                                            LinesListType, PointSetType>
                                                    RightAngleFilterType;
  
  /** Creatop, of an instance of the filters */
  RightAngleFilterType::Pointer  rightAngleFilter  =
                                               RightAngleFilterType::New();
  ReaderType::Pointer            reader            = ReaderType::New();
  PointSetType::Pointer          segmentOrtho      = PointSetType::New();     
  lsdFilterType::Pointer         lsdFilter         = lsdFilterType::New();


 

  // Begin the process
  reader->SetFileName(infname);
  lsdFilter->SetInput(reader->GetOutput());  
  
  rightAngleFilter->SetInputImage(reader->GetOutput());
  rightAngleFilter->SetInput(lsdFilter->GetOutput());
  rightAngleFilter->Update();


  /** Print the right angles coordinate in the output file*/
  segmentOrtho = rightAngleFilter->GetOutput();
  PointSetType::PointType   pRight;
  VectorLines               outputVectorLines;
  LinesListType::Pointer    outputLinesList = LinesListType::New();
  
  for (unsigned int i = 0; i<segmentOrtho->GetNumberOfPoints() ; i++)
    {
      segmentOrtho->GetPoint(i, &pRight);
      segmentOrtho->GetPointData(i, &outputVectorLines);
      outputLinesList->push_back(outputVectorLines[0]);
      outputLinesList->push_back(outputVectorLines[1]);
    }

  typedef otb::DrawLineSpatialObjectListFilter< ImageType,
                                       ImageType > DrawLineListType;
  DrawLineListType::Pointer drawLineFilter =   DrawLineListType::New();

    drawLineFilter->SetInput(reader->GetOutput());
  drawLineFilter->SetInputLineSpatialObjectList(outputLinesList);

  WriterType::Pointer writer = WriterType::New();
    writer->SetInput(drawLineFilter->GetOutput());
    writer->SetFileName(outfname);

    
  reader->GenerateOutputInformation();
  writer->Update();

  
  return EXIT_SUCCESS;
}

