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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbInnerProductPCAImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

int otbInnerProductPCAImageFilter( int argc, char* argv[] )
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  const char * inputFileName = argv[1];
  const char * outputFilename = argv[2];
  const bool generateMeanComponent(atoi(argv[3]));
  const bool centerdata = atoi(argv[4]);
  const unsigned int numberOfPrincipalComponentsRequired(atoi(argv[5]));

  typedef otb::Image<PixelType,Dimension> MonoImageType;
  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::ImageFileReader< ImageType >                     ReaderType;
  typedef otb::StreamingImageFileWriter< ImageType >                     WriterType;
  typedef otb::StreamingImageFileWriter< MonoImageType >                     WriterType2;
  typedef otb::InnerProductPCAImageFilter<ImageType,ImageType> PCAFilterType;

  ReaderType::Pointer     reader     = ReaderType::New();
  reader->SetFileName(inputFileName);
  WriterType::Pointer     writer     = WriterType::New();
  writer->SetFileName(outputFilename);

  PCAFilterType::Pointer     pcafilter     = PCAFilterType::New();

  pcafilter->SetNumberOfPrincipalComponentsRequired(numberOfPrincipalComponentsRequired);
  pcafilter->SetGenerateMeanComponent(generateMeanComponent);
  pcafilter->SetCenterData(centerdata);
  pcafilter->SetInput(reader->GetOutput());
  writer->SetInput(pcafilter->GetOutput());
  writer->Update();

  typedef otb::MultiToMonoChannelExtractROI< PixelType, PixelType >  ExtractROIFilterType;
  unsigned int nbComponents = numberOfPrincipalComponentsRequired;
  if (generateMeanComponent == true) nbComponents += 1;

  for(unsigned int cpt=0 ; cpt < nbComponents ; cpt++)
  {
    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    WriterType2::Pointer     writer2     = WriterType2::New();
    extractROIFilter->SetInput(pcafilter->GetOutput());
    extractROIFilter->SetChannel(cpt+1);
    writer2->SetInput(extractROIFilter->GetOutput());

    writer2->SetFileName(argv[cpt+6]);
    writer2->Update();
  }

  return EXIT_SUCCESS;
}
