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

#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbMorphologicalPyramidSynthesisFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidSynthesisFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int numberOfLevels = atoi(argv[3]);
  const float        decimationRatio = atof(argv[4]);

  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningClosingFilterType;
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType, OutputImageType, OpeningClosingFilterType>
  PyramidAnalysisFilterType;
  typedef otb::MorphologicalPyramidSynthesisFilter<InputImageType, OutputImageType>
  PyramidSynthesisFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Analysis
  PyramidAnalysisFilterType::Pointer pyramidAnalysis = PyramidAnalysisFilterType::New();
  pyramidAnalysis->SetNumberOfLevels(numberOfLevels);
  pyramidAnalysis->SetDecimationRatio(decimationRatio);
  pyramidAnalysis->SetInput(reader->GetOutput());
  pyramidAnalysis->Update();

  // Synthesis
  PyramidSynthesisFilterType::Pointer pyramidSynthesis = PyramidSynthesisFilterType::New();
  pyramidSynthesis->SetInput(pyramidAnalysis->GetOutput()->Back());
  pyramidSynthesis->SetSupFilter(pyramidAnalysis->GetSupFilter());
  pyramidSynthesis->SetSupDeci(pyramidAnalysis->GetSupDeci());
  pyramidSynthesis->SetInfFilter(pyramidAnalysis->GetInfFilter());
  pyramidSynthesis->SetInfDeci(pyramidAnalysis->GetInfDeci());
  pyramidSynthesis->Update();

  // Writing the output image
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(pyramidSynthesis->GetOutput()->Back());
  writer->Update();

  return EXIT_SUCCESS;
}
