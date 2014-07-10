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

#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMultiChannelExtractROI.h"

#include "otbSystem.h"

int otbTestMultiExtractMultiUpdate(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                        InternPixelType;
  typedef otb::VectorImage<InternPixelType, Dimension>                  InputImageType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::ImageFileWriter<InputImageType>                          WriterType;
  typedef otb::MultiChannelExtractROI<InternPixelType, InternPixelType> ExtractInternVectorType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  ExtractInternVectorType::Pointer extractInternVector = ExtractInternVectorType::New();
  //readerMS->UpdateOutputInformation();

  extractInternVector->SetInput(reader->GetOutput());
  extractInternVector->SetStartX(atoi(argv[3]));
  extractInternVector->SetStartY(atoi(argv[4]));
  extractInternVector->SetSizeX(atoi(argv[5]));
  extractInternVector->SetSizeY(atoi(argv[6]));

  extractInternVector->SetFirstChannel(atoi(argv[7]));
  extractInternVector->SetLastChannel(atoi(argv[8]));

  extractInternVector->Update();
  extractInternVector->Update();
  extractInternVector->Update();

  writer->SetInput(extractInternVector->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
