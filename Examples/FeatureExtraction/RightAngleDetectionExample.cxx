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
// This filter detects the right angles in an image by exploiting the
// output of a line detection algorithm. Typically the
// \doxygen{otb}{LineSegmentDetector} class will be used. The right
// angle detection algorithm is described in detail in
// \cite{RightAngleDetection}. 
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLineSpatialObjectListToRightAnglePointSetFilter.h"
// Software Guide : EndCodeSnippet

int main( int argc, char * argv[] )
{
  const   char * infname   = argv[1];  
  const   char * outfname  = argv[2];
  const   char * inprettyfname = argv[3];
  const   char * outprettyfname  = argv[4];
  
  const unsigned int           Dimension = 2;
  typedef float                PixelType;
  
  typedef otb::Image<PixelType ,Dimension>   ImageType;
  typedef otb::ImageFileReader<ImageType>    ReaderType;
  typedef otb::ImageFileWriter<ImageType>    WriterType;

  ReaderType::Pointer            reader            = ReaderType::New();
  reader->SetFileName(infname);
  WriterType::Pointer writer = WriterType::New();

// Software Guide : BeginLatex
//
// After defining, as usual, the types for the input image and the
// image reader, we define the specific types needed for this
// example. First of all, we will use a list of line spatial objects
// to store the detected lines which will be provided by the line
// segment detector.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::LineSpatialObjectList         LinesListType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The right angle detector's output is a pointset where each point
// gives the coordinate of the detected angle. In the data field of
// the pointset, the 2 lines which define the right angle are stored
// in a vector. Therefore we define the 2 following types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
  typedef LinesListType::LineType            LineType;
  typedef std::vector<LineType*>             LineVectorType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// And we can now define the pointset type for storing all the
// information related to the detected right angles.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  typedef itk::PointSet<LineVectorType, Dimension>     PointSetType;
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We define the type for the line segment detector. A detailed
// example for this detector can be found in section \ref{sec:LSD}.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  typedef otb::LineSegmentDetector<ImageType , PixelType>   lsdFilterType;

  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can finally define the type for the right angle detection
// filter. This filter is templated over the input image type, the
// type of the lines provided by the line segment detector, and the
// outpu pointset type containing the detected right angles.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  typedef otb::LineSpatialObjectListToRightAnglePointSetFilter<ImageType,
                                            LinesListType, PointSetType>
                                                    RightAngleFilterType;
  

  RightAngleFilterType::Pointer  rightAngleFilter  =
                                               RightAngleFilterType::New();

  lsdFilterType::Pointer         lsdFilter         = lsdFilterType::New();


 



  lsdFilter->SetInput(reader->GetOutput());  
  
  rightAngleFilter->SetInputImage(reader->GetOutput());
  rightAngleFilter->SetInput(lsdFilter->GetOutput());
  rightAngleFilter->Update();


  /** Print the right angles coordinate in the output file*/
  PointSetType::Pointer          segmentOrtho      = PointSetType::New();     
  segmentOrtho = rightAngleFilter->GetOutput();
  PointSetType::PointType   pRight;
  LineVectorType               outputVectorLines;
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

