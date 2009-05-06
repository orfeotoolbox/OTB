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
#include "otbImageToLineSegmentVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStreamingImageFileWriter.h"

int otbImageToLineSegmentVectorData( int argc, char * argv[] )
{
  
  typedef float                                              InputPixelType;
  const   unsigned int                                       Dimension = 2;
  
  /** Typedefs */
  typedef otb::Image< InputPixelType,  Dimension >     ImageType;
  typedef otb::ImageFileReader<ImageType>              ReaderType;
 
  typedef otb::ImageToLineSegmentVectorData<ImageType> ImToLSDFilterType;
  typedef ImToLSDFilterType::VectorDataType            VectorDataType;
  typedef otb::VectorDataFileWriter<VectorDataType>    WriterType;

  ImToLSDFilterType::Pointer  lsdFilter = ImToLSDFilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  lsdFilter->SetInput( reader->GetOutput() );
  lsdFilter->SetThreadDistanceThreshold( 10. );
  lsdFilter->Update();

  writer->SetFileName(argv[2]);
  writer->SetInput( lsdFilter->GetOutputVectorData() );
  writer->Update();


  return EXIT_SUCCESS;
}

