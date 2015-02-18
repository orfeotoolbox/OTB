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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

int otbGDALDriverDoubleWritingTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname1 = argv[1];
  const char * infname2 = argv[2];
  const char * outfname1 = argv[3];
  const char * outfname2 = argv[4];
  const char * outfname3 = argv[5];
  const char * outfname4 = argv[6];

  typedef otb::VectorImage<double, 2>              ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType>          WriterType;
  typedef otb::ImageFileWriter<ImageType> StreamingWriterType;

  ReaderType::Pointer          reader = ReaderType::New();
  WriterType::Pointer          writer = WriterType::New();
  StreamingWriterType::Pointer streamingWriter = StreamingWriterType::New();

  reader->SetFileName(infname1);
  writer->SetFileName(outfname1);
  writer->SetInput(reader->GetOutput());
  writer->Update();
  reader->SetFileName(infname2);
  writer->SetFileName(outfname2);
  writer->Update();

  reader->SetFileName(infname1);
  streamingWriter->SetFileName(outfname3);
  streamingWriter->SetInput(reader->GetOutput());
  streamingWriter->Update();

  reader->SetFileName(infname2);
  streamingWriter->SetFileName(outfname4);
  streamingWriter->SetInput(reader->GetOutput());
  streamingWriter->Update();

  return EXIT_SUCCESS;
}
