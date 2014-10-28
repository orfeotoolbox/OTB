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
#include <iostream>
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

int otbImageFileReaderONERATest(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  typedef float InputPixelType;
  typedef float OutputPixelType;

  typedef otb::VectorImage<InputPixelType, 2> InputImageType;
  typedef otb::VectorImage<OutputPixelType, 2> OutputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::MultiChannelExtractROI<OutputPixelType, OutputPixelType> ExtractROIFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  if (argc == 7)
    {
    extractROIFilter->SetStartX(atoi(argv[3]));
    extractROIFilter->SetStartY(atoi(argv[4]));
    extractROIFilter->SetSizeX(atoi(argv[5]));
    extractROIFilter->SetSizeY(atoi(argv[6]));
    extractROIFilter->SetInput(reader->GetOutput());
    writer->SetInput(extractROIFilter->GetOutput());
    }
  else if (argc == 3)
    {
    writer->SetInput(reader->GetOutput());
    }

  writer->Update();

  return EXIT_SUCCESS;
}
