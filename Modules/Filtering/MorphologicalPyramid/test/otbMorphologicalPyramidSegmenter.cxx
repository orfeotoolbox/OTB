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
#include "otbMorphologicalPyramidSegmenter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidSegmenter(int itkNotUsed(argc), char * argv[])
{
  const char*        inputFilename = argv[1];
  const char*        originalFilename = argv[2];
  const char*        outputFilename1 = argv[3];
  const bool         segmentDark = atoi(argv[4]);
  const float        seedsQuantile = atof(argv[5]);
  const float        segmentationQuantile = atof(argv[6]);
  const unsigned int minObjectSize = atoi(argv[7]);

  const unsigned int Dimension = 2;
  typedef double         InputPixelType;
  typedef unsigned short OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef otb::MorphologicalPyramid::Segmenter<InputImageType, OutputImageType>
  SegmenterType;

  // Input images reading
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(originalFilename);

  // Instantiation
  SegmenterType::Pointer segmenter = SegmenterType::New();
  segmenter->SetDetailsImage(reader->GetOutput());
  segmenter->SetOriginalImage(reader2->GetOutput());
  segmenter->SetSegmentDarkDetailsBool(segmentDark);
  segmenter->SetSeedsQuantile(seedsQuantile);
  segmenter->SetConnectedThresholdQuantile(segmentationQuantile);
  segmenter->SetMinimumObjectSize(minObjectSize);

  // File writing
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(segmenter->GetOutput());
  writer->SetFileName(outputFilename1);
  writer->Update();

  return EXIT_SUCCESS;
}
