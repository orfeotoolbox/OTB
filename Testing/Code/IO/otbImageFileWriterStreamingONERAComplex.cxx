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



#include <iostream>

#include "itkMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageFileWriterStreamingONERAComplex(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<float>                      PixelType;
  typedef otb::Image<PixelType,  2>                ImageType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer complexReader = ReaderType::New();
  WriterType::Pointer complexWriter = WriterType::New();

  complexReader->SetFileName(inputFilename);

  complexWriter->SetNumberOfDivisionsStrippedStreaming(100);
  complexWriter->SetFileName(outputFilename);
  complexWriter->SetInput(complexReader->GetOutput());

  complexWriter->Update();

  return EXIT_SUCCESS;
}
