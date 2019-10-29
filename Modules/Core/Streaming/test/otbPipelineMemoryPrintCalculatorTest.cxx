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

#include "otbPipelineMemoryPrintCalculator.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbVectorImageToIntensityImageFilter.h"


int otbPipelineMemoryPrintCalculatorTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::VectorImage<double, 2> VectorImageType;
  typedef otb::Image<double, 2>       ImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::VectorImageToIntensityImageFilter<VectorImageType, ImageType> IntensityImageFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  IntensityImageFilterType::Pointer intensity = IntensityImageFilterType::New();
  intensity->SetInput(reader->GetOutput());

  otb::PipelineMemoryPrintCalculator::Pointer calculator = otb::PipelineMemoryPrintCalculator::New();
  calculator->SetDataToWrite(intensity->GetOutput());
  calculator->Compute();

  otb::PipelineMemoryPrintCalculator::MemoryPrintType availableMemoryInBytes = 104858;
  otb::PipelineMemoryPrintCalculator::MemoryPrintType memoryPrint            = calculator->GetMemoryPrint();

  unsigned int optimalNbDiv = otb::PipelineMemoryPrintCalculator::EstimateOptimalNumberOfStreamDivisions(memoryPrint, availableMemoryInBytes);

  std::ofstream ofs(argv[2]);
  ofs << "Memory print of whole pipeline:     " << calculator->GetMemoryPrint() * otb::PipelineMemoryPrintCalculator::ByteToMegabyte << " Mb" << std::endl;
  ofs << "Available memory:                   " << availableMemoryInBytes * otb::PipelineMemoryPrintCalculator::ByteToMegabyte << " Mb" << std::endl;
  ofs << "Optimal number of stream divisions: " << optimalNbDiv << std::endl;
  ofs << "Bias correction factor applied:     " << calculator->GetBiasCorrectionFactor() << std::endl;
  ofs.close();

  return EXIT_SUCCESS;
}
