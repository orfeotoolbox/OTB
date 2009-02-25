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
//    2
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
  
  /** Typedefs */
  typedef otb::Image< InputPixelType,  Dimension >    InputImageType;
  typedef otb::ImageFileReader<InputImageType>        ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  
  
  

  typedef otb::LineSegmentDetector<InputImageType,
                                                 PrecisionType> lsdFilterType;


  /** Instanciation of smart pointer*/
  lsdFilterType::Pointer  lsdFilter = lsdFilterType::New();


  typedef otb::DrawLineSpatialObjectListFilter< InputImageType,
                                       InputImageType > DrawLineListType;
  DrawLineListType::Pointer drawLineFilter =   DrawLineListType::New();


  reader->GenerateOutputInformation();
  lsdFilter->SetInput(reader->GetOutput());
  drawLineFilter->SetInput(reader->GetOutput());
  drawLineFilter->SetInputLineSpatialObjectList(lsdFilter->GetOutput());

 
  typedef otb::ImageFileWriter<InputImageType>        WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(drawLineFilter->GetOutput());
  writer->Update();

  
  return EXIT_SUCCESS;
}

