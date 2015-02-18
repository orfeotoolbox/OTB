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


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"

#include "otbPrintableImageFilter.h"

int otbPrintableImageFilterWithMask(int itkNotUsed(argc), char * argv[])
{
  const char * inputFilename  = argv[1];
  const char * masktFilename  = argv[2];
  const char * outputFilename = argv[3];

  typedef double InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::PrintableImageFilter<InputImageType>    FilterType;
  typedef FilterType::OutputImageType                  OutputImageType;
  typedef OutputImageType::PixelType                   OutputPixelType;
  typedef FilterType::MaskImageType                    MaskImageType;
  typedef FilterType::MaskPixelType                    MaskPixelType;

  typedef otb::ImageFileReader<InputImageType>           InputReaderType;
  typedef otb::ImageFileReader<MaskImageType>            MaskReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  FilterType::Pointer      printableImageFilter = FilterType::New();
  InputReaderType::Pointer inputReader          = InputReaderType::New();
  MaskReaderType::Pointer  maskReader           = MaskReaderType::New();
  WriterType::Pointer      writer               = WriterType::New();

  inputReader->SetFileName(inputFilename);
  maskReader->SetFileName(masktFilename);
  maskReader->GenerateOutputInformation();
  writer->SetFileName(outputFilename);

  printableImageFilter->SetInput(inputReader->GetOutput());
  printableImageFilter->SetInputMask(maskReader->GetOutput());

  FilterType::ChannelsType chList;
  chList.push_back(3);
  chList.push_back(2);
  chList.push_back(1);
  printableImageFilter->SetChannelList(chList);

  OutputPixelType objectColor;
  objectColor.SetSize(3);
  objectColor[0] = 0;
  objectColor[1] = 255;
  objectColor[2] = 0;
  printableImageFilter->SetObjectColor(objectColor);
  printableImageFilter->SetBackgroundMaskValue(static_cast<MaskPixelType>(atof(argv[4])));

  writer->SetInput(printableImageFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
