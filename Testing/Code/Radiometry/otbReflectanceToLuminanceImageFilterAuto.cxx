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

#include "otbReflectanceToLuminanceImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVariableLengthVector.h"
#include "otbMultiChannelExtractROI.h"

int otbReflectanceToLuminanceImageFilterAuto(int itkNotUsed(argc), char * argv[])
{

const char * inputFileName  = argv[1];
  const char * outputFileName  = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                            InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                            OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                              ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                             WriterType;
  typedef otb::ReflectanceToLuminanceImageFilter<OutputImageType,
      OutputImageType> ReflectanceToLuminanceImageFilterType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  // Instantiating object
  ReflectanceToLuminanceImageFilterType::Pointer filterToLuminance = ReflectanceToLuminanceImageFilterType::New();

  filterToLuminance->SetInput(reader->GetOutput());
  writer->SetInput(filterToLuminance->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;

}
