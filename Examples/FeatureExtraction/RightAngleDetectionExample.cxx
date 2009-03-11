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
  typedef otb::LineSegmentDetector<ImageType , PixelType>   LsdFilterType;

  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can finally define the type for the right angle detection
// filter. This filter is templated over the input image type, the
// type of the lines provided by the line segment detector, and the
// output pointset type containing the detected right angles.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  typedef otb::LineSpatialObjectListToRightAnglePointSetFilter<ImageType,
                                            LinesListType, PointSetType>
                                                    RightAngleFilterType;
  
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We instantiate the line segment detector and the right angle detector.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  LsdFilterType::Pointer         lsdFilter         = LsdFilterType::New();
  RightAngleFilterType::Pointer  rightAngleFilter  =
                                               RightAngleFilterType::New();


// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We plug the pipeline. The right angle detector has 2 inputs: the
// image to be processed and the previously detected lines.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    

  lsdFilter->SetInput(reader->GetOutput());  
  rightAngleFilter->SetInputImage(reader->GetOutput());
  rightAngleFilter->SetInput(lsdFilter->GetOutput());
  rightAngleFilter->Update();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We will now draw the right angles on top of the inout image. For
// this, we get the output of the right angle detector.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  PointSetType::Pointer          segmentOrtho      = PointSetType::New();     
  segmentOrtho = rightAngleFilter->GetOutput();
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We will iterate through the pointset and get the lines which define
// each right angle stored inside each point of the pointset. The
// lines will be stored into a line list.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
  PointSetType::PointType   pRight;
  LineVectorType            outputVectorLines;
  LinesListType::Pointer    outputLinesList = LinesListType::New();
  
  for (unsigned int i = 0; i<segmentOrtho->GetNumberOfPoints(); i++)
    {
      // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Even if we do not use it in this example, we show here how to get
// the coordinates of the right angle.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet    
      segmentOrtho->GetPoint(i, &pRight);
      // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The lines associated to a given angle are obtained using the
// \code{GetPointData} method of the pointset. Then they are stored
// into the list of lines.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet          
      segmentOrtho->GetPointData(i, &outputVectorLines);
      outputLinesList->push_back(outputVectorLines[0]);
      outputLinesList->push_back(outputVectorLines[1]);
    }
      // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We will use the \doxygen{otb}{DrawLineSpatialObjectListFilter} to
// draw the list of lines on top of the input image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet          
  typedef otb::DrawLineSpatialObjectListFilter< ImageType,
                                       ImageType > DrawLineListType;
  DrawLineListType::Pointer drawLineFilter =   DrawLineListType::New();

  drawLineFilter->SetInput(reader->GetOutput());
  drawLineFilter->SetInputLineSpatialObjectList(outputLinesList);


  writer->SetInput(drawLineFilter->GetOutput());
  writer->SetFileName(outfname);

    
  reader->GenerateOutputInformation();
  writer->Update();
  // Software Guide : EndCodeSnippet
    //  Software Guide : BeginLatex
  // Figure~\ref{fig:RIGHTANGLE_FILTER} shows the result of applying
  // the right angle detection filter to an image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{PrettyRighAngleInput.eps}
  // \includegraphics[width=0.25\textwidth]{PrettyRighAngleOutput.eps}
  // \itkcaption[Right Angle Detection Filter]{Result of applying the
  // \doxygen{otb}{LineSpatialObjectListToRightAnglePointSetFilter} to an image. From left to right :
  // original image, detected right angles.}
  // \label{fig:RIGHTANGLE_FILTER}
  // \end{figure}
  //
  //  Software Guide : EndLatex


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

