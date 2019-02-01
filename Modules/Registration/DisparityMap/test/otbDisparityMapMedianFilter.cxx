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

#include "otbDisparityMapMedianFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"

 const unsigned int Dimension = 2;
 typedef float                                                         PixelType;
 typedef otb::Image<PixelType, Dimension>                              FloatImageType;
 typedef otb::ImageFileReader<FloatImageType>                          ReaderType;
 typedef otb::ImageFileWriter<FloatImageType>                 FloatWriterType;

 typedef otb::DisparityMapMedianFilter<FloatImageType,FloatImageType,FloatImageType>   DisparityMapMedianFilterType;



int otbDisparityMapMedianFilter(int argc, char* argv[])
{

  if ((argc != 5) && (argc != 6))
    {
    std::cerr << "Usage: " << argv[0] << " hdispinput_fname output_fname radius incoherencethres (maskinput_fname) ";
    return EXIT_FAILURE;
    }

  // Instantiation
  DisparityMapMedianFilterType::Pointer filter = DisparityMapMedianFilterType::New();

  ReaderType::Pointer imgReader = ReaderType::New();
  imgReader->SetFileName(argv[1]);

  filter->SetInput(imgReader->GetOutput());
  filter->SetRadius(atoi(argv[3]));
  filter->SetIncoherenceThreshold(atof(argv[4]));

  ReaderType::Pointer maskReader = ReaderType::New();

  if (argc > 5)
    {
    maskReader->SetFileName(argv[5]);
    filter->SetMaskInput(maskReader->GetOutput());
    }

  FloatWriterType::Pointer writer = FloatWriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);

  otb::StandardWriterWatcher watcher1(writer, filter, "filtering");
  writer->Update();

  return EXIT_SUCCESS;
}
