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

#include "otbImageToLuminanceImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

//Test the retrieval of parameters from the image metadata
int otbImageToLuminanceImageFilterAuto(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName  = argv[1];
  const char * outputFileName = argv[2];

  const unsigned int Dimension = 2;
  typedef double                                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>                            InputImageType;
  typedef otb::VectorImage<PixelType, Dimension>                            OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                              ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                             WriterType;
  typedef otb::ImageToLuminanceImageFilter<InputImageType, OutputImageType> ImageToLuminanceImageFilterType;
  typedef otb::MultiChannelExtractROI<PixelType, PixelType>                 RoiFilterType;

  ReaderType::Pointer reader  = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);
  reader->UpdateOutputInformation();

  // Instantiating object
  ImageToLuminanceImageFilterType::Pointer filter = ImageToLuminanceImageFilterType::New();
  filter->SetInput(reader->GetOutput());

  RoiFilterType::Pointer roiFilter = RoiFilterType::New();
  roiFilter->SetStartX(1000);
  roiFilter->SetStartY(1000);
  roiFilter->SetSizeX(100);
  roiFilter->SetSizeY(100);
  roiFilter->SetInput(filter->GetOutput());

  writer->SetInput(roiFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
