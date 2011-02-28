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
#include "otbStreamingImageFileWriter.h"

#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"

#include "otbStandardWriterWatcher.h"

const unsigned int Dimension = 2;
typedef float PixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::MultiChannelExtractROI<PixelType,PixelType> MultiChannelExtractROIType;
typedef otb::VectorRescaleIntensityImageFilter<ImageType,ImageType> RescaleImageFilterType;
typedef otb::StreamingImageFileWriter<ImageType> WriterType;

int main(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  MultiChannelExtractROIType::Pointer extractROI = MultiChannelExtractROIType::New();
  extractROI->SetInput(reader->GetOutput());

  unsigned int i = 0;
  for (i = 5; i < 107; i++)
    {
    extractROI->SetChannel(i);
    }
  for (i = 122; i < 152; i++)
    {
    extractROI->SetChannel(i);
    }
  for (i = 182; i < 202; i++)
    {
    extractROI->SetChannel(i);
    }

  RescaleImageFilterType::Pointer scale = RescaleImageFilterType::New();
  scale->SetInput(extractROI->GetOutput());

  ImageType::PixelType inputMin;
  ImageType::PixelType inputMax;
  ImageType::PixelType outputMin;
  ImageType::PixelType outputMax;

  //const unsigned int nbBands = extractROI->GetOutput()->GetNumberOfComponentsPerPixel();
  const unsigned int nbBands = 152;

  scale->SetAutomaticInputMinMaxComputation(false);

  inputMin.SetSize(nbBands);
  inputMin.Fill(0.0);
  scale->SetInputMinimum(inputMin);

  inputMax.SetSize(nbBands);
  inputMax.Fill(2500.0);
  scale->SetInputMaximum(inputMax);

  outputMin.SetSize(nbBands);
  outputMin.Fill(0.0);
  scale->SetOutputMinimum(outputMin);

  outputMax.SetSize(nbBands);
  outputMax.Fill(1.0);
  scale->SetOutputMaximum(outputMax);



  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(scale->GetOutput());

  otb::StandardWriterWatcher w4(writer,scale,"Band extraction + rescaling");
  writer->Update();

  return EXIT_SUCCESS;
}
