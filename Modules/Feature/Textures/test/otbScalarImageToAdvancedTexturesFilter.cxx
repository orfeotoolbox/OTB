/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbScalarImageToAdvancedTexturesFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbScalarImageToAdvancedTexturesFilter(int argc, char* argv[])
{
  if (argc != 7)
  {
    std::cerr << "Usage: " << argv[0] << " infname outprefix nbBins radius offsetx offsety" << std::endl;
    return EXIT_FAILURE;
  }
  const char*        infname   = argv[1];
  const char*        outprefix = argv[2];
  const unsigned int nbBins    = atoi(argv[3]);
  const unsigned int radius    = atoi(argv[4]);
  const int          offsetx   = atoi(argv[5]);
  const int          offsety   = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef float      PixelType;
  typedef otb::Image<PixelType, Dimension>                               ImageType;
  typedef otb::ScalarImageToAdvancedTexturesFilter<ImageType, ImageType> TexturesFilterType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer         reader = ReaderType::New();
  TexturesFilterType::Pointer filter = TexturesFilterType::New();
  WriterType::Pointer         writer = WriterType::New();

  // Read image
  reader->SetFileName(infname);

  // Build radius
  TexturesFilterType::SizeType sradius;
  sradius.Fill(radius);

  // Build offset
  TexturesFilterType::OffsetType offset;
  offset[0] = offsetx;
  offset[1] = offsety;

  filter->SetInput(reader->GetOutput());
  filter->SetRadius(sradius);
  filter->SetOffset(offset);

  otb::StandardFilterWatcher watcher(filter, "Textures filter");

  filter->SetNumberOfBinsPerAxis(nbBins);
  filter->SetInputImageMinimum(0);
  filter->SetInputImageMaximum(255);

  // Write outputs
  std::ostringstream oss;

  writer->SetNumberOfDivisionsStrippedStreaming(2);

  oss.str("");
  oss << outprefix << "Variance.tif";
  writer->SetInput(filter->GetVarianceOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "Mean.tif";
  writer->SetInput(filter->GetMeanOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "Dissimilarity.tif";
  writer->SetInput(filter->GetDissimilarityOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "SumAverage.tif";
  writer->SetInput(filter->GetSumAverageOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "SumVariance.tif";
  writer->SetInput(filter->GetSumVarianceOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "SumEntropy.tif";
  writer->SetInput(filter->GetSumEntropyOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "DifferenceEntropy.tif";
  writer->SetInput(filter->GetDifferenceEntropyOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "DifferenceVariance.tif";
  writer->SetInput(filter->GetDifferenceVarianceOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "IC1.tif";
  writer->SetInput(filter->GetIC1Output());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "IC2.tif";
  writer->SetInput(filter->GetIC2Output());
  writer->SetFileName(oss.str());
  writer->Update();

  return EXIT_SUCCESS;
}
