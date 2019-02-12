/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "itkMacro.h"

#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbMorphologicalPyramidAnalysisFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];
  const unsigned int numberOfLevels = atoi(argv[4]);
  const float        decimationRatio = atof(argv[5]);

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
  PyramidFilterType;
  typedef PyramidFilterType::OutputImageListType::Iterator ImageListIterator;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  PyramidFilterType::Pointer pyramid = PyramidFilterType::New();
  pyramid->SetNumberOfLevels(numberOfLevels);
  pyramid->SetDecimationRatio(decimationRatio);
  pyramid->SetInput(reader->GetOutput());
  pyramid->Update();

  // Retrieving iterators on the results images
  ImageListIterator itAnalyse = pyramid->GetOutput()->Begin();
  ImageListIterator itSupFilter = pyramid->GetSupFilter()->Begin();
  ImageListIterator itInfFilter = pyramid->GetInfFilter()->Begin();
  ImageListIterator itInfDeci = pyramid->GetInfDeci()->Begin();
  ImageListIterator itSupDeci =  pyramid->GetSupDeci()->Begin();

  WriterType::Pointer writer =  WriterType::New();

  int i = 1;
  //      std::stringstream oss;
  std::ostringstream oss;
  // Writing the results images
  while ((itAnalyse != pyramid->GetOutput()->End())
         && (itSupFilter != pyramid->GetSupFilter()->End())
         && (itInfFilter != pyramid->GetInfFilter()->End())
         && (itInfDeci != pyramid->GetInfDeci()->End())
         && (itSupDeci != pyramid->GetSupDeci()->End())
         )
    {
    oss << outputFilenamePrefix << "_an_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itAnalyse.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_sf_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itSupFilter.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_if_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itInfFilter.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_id_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itInfDeci.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    std::cout << "File " << oss.str() << " written" << std::endl;
    oss.str("");
    oss << outputFilenamePrefix << "_sd_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itSupDeci.Get());
    writer->SetFileName(oss.str());
    writer->Update();
    oss.str("");
    ++itAnalyse;
    ++itSupFilter;
    ++itInfFilter;
    ++itInfDeci;
    ++itSupDeci;
    ++i;
    }

  return EXIT_SUCCESS;
}
