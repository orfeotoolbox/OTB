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
#include "otbDrawLineSpatialObjectListFilter.h"
#include "otbLineSpatialObjectList.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {QB_SuburbLSD.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{LineSegmentDetector}\cite{LSD}, also known as {\em Lucy in the
// Sky with Diamonds}.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbLineSegmentDetector.h"
// Software Guide : EndCodeSnippet

int main( int argc, char * argv[] )
{
  const   char * infname  = argv[1];
  const   char * outfname  = argv[2];
  
  typedef unsigned char   InputPixelType;
  typedef unsigned char   PrecisionType;
  const   unsigned int    Dimension = 2;
  

// Software Guide : BeginLatex
//
// As usual, we start by defining the types for the input image and
// the image file reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
  typedef otb::Image< InputPixelType,  Dimension >    ImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
// Software Guide : EndCodeSnippet    
// Software Guide : BeginLatex
//
// We instantiate the reader and set the file name for the input image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
// Software Guide : EndCodeSnippet    
// Software Guide : BeginLatex
//
// We define now the type for the segment detector filter. It is
// templated over the input image type and the precision with which
// the coordinates of the detected segments will be given. It is
// recommended to set this precision to a real type. The output of the
// filter will be a list of \doxygen{itk}{LineSpatialObject}s.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  
    
  typedef otb::LineSegmentDetector<ImageType,
                                                 PrecisionType> LsdFilterType;


  
  LsdFilterType::Pointer  lsdFilter = LsdFilterType::New();

  // Software Guide : EndCodeSnippet    
// Software Guide : BeginLatex
//
// In order to be able to display the results, we will draw the
// detected segments on top of the input image. For this matter, we
// will use the \doxygen{otb}{DrawLineSpatialObjectListFilter} which
// is templated over the input and output image types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  


  typedef otb::DrawLineSpatialObjectListFilter< ImageType,
                                       ImageType > DrawLineListType;
  DrawLineListType::Pointer drawLineFilter =   DrawLineListType::New();


  // Software Guide : EndCodeSnippet    
// Software Guide : BeginLatex
//
// We can now define the type for the writer, instantiate it and set
// the file name for the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  

  typedef otb::ImageFileWriter<ImageType>        WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);

  // Software Guide : EndCodeSnippet    
// Software Guide : BeginLatex
//
// We plug the pipeline.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  

  lsdFilter->SetInput(reader->GetOutput());
  writer->SetInput(drawLineFilter->GetOutput());

  drawLineFilter->SetInput(reader->GetOutput());
  drawLineFilter->SetInputLineSpatialObjectList(lsdFilter->GetOutput());

  // Software Guide : EndCodeSnippet    
// Software Guide : BeginLatex
//
// Before calling the \code{Update()} method of the writer in order to
// trigger the pipeline execution, we call the
// \doxygen{GenerateOutputInformation()} of the reader, so the LSD
// filter gets the information about image size and spacing.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet  


  reader->GenerateOutputInformation();
  writer->Update();

  // Software Guide : EndCodeSnippet      

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:LSD} shows the result of applying the line segment
  // detection to a small patch extracted from a VHR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.35\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.35\textwidth]{QB_SuburbLSD.eps}
  // \itkcaption[LSD Application]{Result of applying the
  // \doxygen{otb}{LineSegmentDetector} to a VHR image of a suburb.}
  // \label{fig:LSD}
  // \end{figure}
  //
  //  Software Guide : EndLatex

  
  return EXIT_SUCCESS;
}

