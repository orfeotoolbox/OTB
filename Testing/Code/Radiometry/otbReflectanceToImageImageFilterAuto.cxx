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

#include "otbReflectanceToImageImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVariableLengthVector.h"
#include "otbMultiChannelExtractROI.h"

int otbReflectanceToImageImageFilterAuto(int argc, char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * inputFileName2  = argv[2];
  const char * outputFileName = argv[3];
  const char * outputFileName2 = argv[4];

  const unsigned int Dimension = 2;
  typedef double                                                              PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                              InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                              OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                               WriterType;
  typedef otb::ReflectanceToImageImageFilter<InputImageType, OutputImageType> ReflectanceToImageImageFilterType;
  typedef ReflectanceToImageImageFilterType::VectorType                       VectorType;
  typedef otb::MultiChannelExtractROI<PixelType, PixelType>                   RoiFilterType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  ReaderType::Pointer reader2  = ReaderType::New();
  WriterType::Pointer writer2 = WriterType::New();
  reader2->SetFileName(inputFileName2);
  writer2->SetFileName(outputFileName2);
  reader2->UpdateOutputInformation();

  // Instantiating object
  ReflectanceToImageImageFilterType::Pointer filter = ReflectanceToImageImageFilterType::New();
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();

  RoiFilterType::Pointer roiFilter = RoiFilterType::New();
  roiFilter->SetStartX(1000);
  roiFilter->SetStartY(1000);
  roiFilter->SetSizeX(100);
  roiFilter->SetSizeY(100);
  roiFilter->SetInput(reader2->GetOutput());
  writer2->SetInput(roiFilter->GetOutput());
  writer2->Update();

  return EXIT_SUCCESS;
}
