/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbSarConcatenateBurstsImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"

typedef otb::VectorImage<unsigned short>                    ImageType;
typedef otb::ImageFileReader<ImageType>               ReaderType;
typedef otb::ImageFileWriter<ImageType>               WriterType;
typedef otb::SarConcatenateBurstsImageFilter<ImageType> SarConcatenateBurstsImageFilterType;
typedef otb::MultiChannelExtractROI<ImageType::InternalPixelType, ImageType::InternalPixelType> ExtractROIFilterType;

int otbSarConcatenateBurstsImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  // This test concatenates 2 bursts
  unsigned int nbBursts = 2;
  unsigned int burstIndex = std::stoi(argv[4]);

  ReaderType::Pointer readerBurst1 = ReaderType::New();
  readerBurst1->SetFileName(argv[1]);
  readerBurst1->UpdateOutputInformation();

  ReaderType::Pointer readerBurst2 = ReaderType::New();
  readerBurst2->SetFileName(argv[2]);
  readerBurst2->UpdateOutputInformation();

  ReaderType::Pointer readerMetadata = ReaderType::New();
  readerMetadata->SetFileName(argv[3]);
  readerMetadata->UpdateOutputInformation();

  auto fusionFilter = SarConcatenateBurstsImageFilterType::New();
  fusionFilter->SetSLCImageMetadata(readerMetadata->GetOutput()->GetImageMetadata());

  SarConcatenateBurstsImageFilterType::SizeType layout;
  layout[0] = 1;
  layout[1] = nbBursts;
  fusionFilter->SetLayout(layout);

  std::vector<std::pair<unsigned long, unsigned long>> lines;
  std::vector<std::pair<unsigned long, unsigned long>> samples;

  auto const & imd1 = readerBurst1->GetOutput()->GetImageMetadata();
  const bool inputWithInvalidPixels = imd1.Has("invalid_pixels") && imd1["invalid_pixels"] == "yes";

  fusionFilter->getDeburstLinesAndSamples(lines, samples, burstIndex, inputWithInvalidPixels);

  // Extract first burst
  auto extractROIFilter1 = ExtractROIFilterType::New();

  const auto & line1 = lines[0];
  const auto & sample1 = samples[0];

  unsigned long minSamples = std::min(sample1.second, static_cast<unsigned long>(readerBurst1->GetOutput()->GetLargestPossibleRegion().GetSize()[0] - 1));
  unsigned long minLines   = std::min(line1.second, static_cast<unsigned long>(readerBurst1->GetOutput()->GetLargestPossibleRegion().GetSize()[1] - 1));

  unsigned long originOffset_samples = static_cast<long>(readerBurst1->GetOutput()->GetOrigin()[0] - 0.5);
  unsigned long originOffset_lines   = static_cast<long>(readerBurst1->GetOutput()->GetOrigin()[1] - 0.5);

  unsigned long startL = line1.first - originOffset_lines;
  unsigned long sizeL  = minLines - line1.first + 1;
  unsigned long startS = sample1.first - originOffset_samples;
  unsigned long sizeS  = minSamples - sample1.first + 1;
  // Readjust if origin is superior to the first selected line/sample for the current burst
  if (line1.first < originOffset_lines)
  {
    startL = 0;
    sizeL  = minLines - line1.first - originOffset_lines + 1;
  }
  if (sample1.first < originOffset_samples)
  {
    startS = 0;
    sizeS  = minSamples - sample1.first - originOffset_samples + 1;
  }

  extractROIFilter1->SetInput(readerBurst1->GetOutput());
  extractROIFilter1->SetStartX(startS);
  extractROIFilter1->SetStartY(startL);
  extractROIFilter1->SetSizeX(sizeS);
  extractROIFilter1->SetSizeY(sizeL);
  extractROIFilter1->UpdateOutputInformation();

  // Extract second burst
  auto extractROIFilter2 = ExtractROIFilterType::New();

  const auto & line2 = lines[1];
  const auto & sample2 = samples[1];

  unsigned long minSamples2 = std::min(sample2.second, static_cast<unsigned long>(readerBurst2->GetOutput()->GetLargestPossibleRegion().GetSize()[0] - 1));
  unsigned long minLines2   = std::min(line2.second, static_cast<unsigned long>(readerBurst2->GetOutput()->GetLargestPossibleRegion().GetSize()[1] - 1));

  unsigned long originOffset_samples2 = static_cast<long>(readerBurst2->GetOutput()->GetOrigin()[0] - 0.5);
  unsigned long originOffset_lines2   = static_cast<long>(readerBurst2->GetOutput()->GetOrigin()[1] - 0.5);

  unsigned long startL2 = line2.first - originOffset_lines2;
  unsigned long sizeL2  = minLines2 - line2.first + 1;
  unsigned long startS2 = sample1.first - originOffset_samples2;
  unsigned long sizeS2  = minSamples2 - sample2.first + 1;
  // Readjust if origin is superior to the first selected line/sample for the current burst
  if (line2.first < originOffset_lines2)
  {
    startL2 = 0;
    sizeL2  = minLines2 - line2.first - originOffset_lines2 + 1;
  }
  if (sample2.first < originOffset_samples2)
  {
    startS2 = 0;
    sizeS2  = minSamples2 - sample2.first - originOffset_samples2 + 1;
  }

  extractROIFilter2->SetInput(readerBurst2->GetOutput());
  extractROIFilter2->SetStartX(startS2);
  extractROIFilter2->SetStartY(startL2);
  extractROIFilter2->SetSizeX(sizeS2);
  extractROIFilter2->SetSizeY(sizeL2);
  extractROIFilter2->UpdateOutputInformation();

  fusionFilter->SetInput(0, extractROIFilter1->GetOutput());
  fusionFilter->SetInput(1, extractROIFilter2->GetOutput());

  auto writer = WriterType::New();
  writer->SetInput(fusionFilter->GetOutput());
  writer->SetFileName(argv[5]);
  writer->Update();

  return EXIT_SUCCESS;
}
