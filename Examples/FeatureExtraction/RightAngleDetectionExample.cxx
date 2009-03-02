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
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

#include <iostream>
#include <fstream>

#include "otbImage.h"
#include "itkPointSet.h"
#include "otbLineSpatialObjectList.h"
#include "otbDrawLineSpatialObjectListFilter.h"
#include "otbLineSegmentDetector.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {qb_RoadExtract2.tif}
//    OUTPUTS: {RighAngleOutput.tif}, {PrettyRighAngleInput.png}, {PrettyRighAngleOutput.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{LineSpatialObjectListToRightAnglePointSetFilter}. 
// This filter computes a local density of edges on an image and can
// be useful to detect man made objects or urban areas, for
// instance. The filter has been implemented in a generic way, so that
// the way the edges are detected and the way their density is
// computed can be chosen by the user.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLineSpatialObjectListToRightAnglePointSetFilter.h"

int main( int argc, char * argv[] )
{
  const   char * infname   = argv[1];  
  const   char * outfname  = argv[2];
  const   char * inprettyfname = argv[3];
  const   char * outprettyfname  = argv[4];
  
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

  /************** images for printing *********/
  typedef unsigned char       OutputPixelType;

  typedef otb::Image<OutputPixelType ,Dimension>   OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>    OutputWriterType;
  typedef itk::RescaleIntensityImageFilter<ImageType, OutputImageType> RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();
  OutputWriterType::Pointer outwriter = OutputWriterType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  outwriter->SetInput( rescaler->GetOutput() );

  rescaler->SetInput( reader->GetOutput() );
  outwriter->SetFileName( inprettyfname );
  outwriter->Update();
  
  rescaler->SetInput( drawLineFilter->GetOutput() );
  outwriter->SetFileName( outprettyfname );
  outwriter->Update();
  
  return EXIT_SUCCESS;
}

