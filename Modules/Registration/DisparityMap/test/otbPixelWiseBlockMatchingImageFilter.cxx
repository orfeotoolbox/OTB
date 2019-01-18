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


#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"

typedef otb::Image<unsigned short>                    ImageType;
typedef otb::Image<float>                             FloatImageType;
typedef otb::ImageFileReader<ImageType>               ReaderType;
typedef otb::ImageFileWriter<FloatImageType> FloatWriterType;

typedef otb::PixelWiseBlockMatchingImageFilter<ImageType,FloatImageType,FloatImageType,ImageType> PixelWiseBlockMatchingImageFilterType;

typedef otb::Functor::NCCBlockMatching<ImageType,FloatImageType> NCCBlockMatchingFunctorType;

typedef otb::PixelWiseBlockMatchingImageFilter<ImageType,FloatImageType,FloatImageType,ImageType, NCCBlockMatchingFunctorType> PixelWiseNCCBlockMatchingImageFilterType;


int otbPixelWiseBlockMatchingImageFilter(int argc, char * argv[])
{
  ReaderType::Pointer leftReader = ReaderType::New();
  leftReader->SetFileName(argv[1]);

  ReaderType::Pointer rightReader = ReaderType::New();
  rightReader->SetFileName(argv[2]);

  PixelWiseBlockMatchingImageFilterType::Pointer bmFilter = PixelWiseBlockMatchingImageFilterType::New();
  bmFilter->SetLeftInput(leftReader->GetOutput());
  bmFilter->SetRightInput(rightReader->GetOutput());
  bmFilter->SetRadius(atoi(argv[5]));
  bmFilter->SetMinimumHorizontalDisparity(atoi(argv[6]));
  bmFilter->SetMaximumHorizontalDisparity(atoi(argv[7]));

  ReaderType::Pointer maskReader = ReaderType::New();
  if(argc > 8)
    {
    maskReader->SetFileName(argv[8]);
    bmFilter->SetLeftMaskInput(maskReader->GetOutput());
    }

  FloatWriterType::Pointer dispWriter = FloatWriterType::New();
  dispWriter->SetInput(bmFilter->GetHorizontalDisparityOutput());
  dispWriter->SetFileName(argv[3]);

  otb::StandardWriterWatcher watcher1(dispWriter,bmFilter,"Computing disparity map");

  dispWriter->Update();

  FloatWriterType::Pointer metricWriter = FloatWriterType::New();
  metricWriter->SetInput(bmFilter->GetMetricOutput());
  metricWriter->SetFileName(argv[4]);

  otb::StandardWriterWatcher watcher2(metricWriter,bmFilter,"Computing metric map");

  metricWriter->Update();

  return EXIT_SUCCESS;
}

int otbPixelWiseBlockMatchingImageFilterNCC(int argc, char * argv[])
{
  ReaderType::Pointer leftReader = ReaderType::New();
  leftReader->SetFileName(argv[1]);

  ReaderType::Pointer rightReader = ReaderType::New();
  rightReader->SetFileName(argv[2]);

  PixelWiseNCCBlockMatchingImageFilterType::Pointer bmFilter = PixelWiseNCCBlockMatchingImageFilterType::New();
  bmFilter->SetLeftInput(leftReader->GetOutput());
  bmFilter->SetRightInput(rightReader->GetOutput());
  bmFilter->SetRadius(atoi(argv[5]));
  bmFilter->SetMinimumHorizontalDisparity(atoi(argv[6]));
  bmFilter->SetMaximumHorizontalDisparity(atoi(argv[7]));
  bmFilter->MinimizeOff();

  ReaderType::Pointer maskReader = ReaderType::New();
  if(argc > 8)
    {
    maskReader->SetFileName(argv[8]);
    bmFilter->SetLeftMaskInput(maskReader->GetOutput());
    }

  FloatWriterType::Pointer dispWriter = FloatWriterType::New();
  dispWriter->SetInput(bmFilter->GetHorizontalDisparityOutput());
  dispWriter->SetFileName(argv[3]);

  otb::StandardWriterWatcher watcher1(dispWriter,bmFilter,"Computing disparity map");

  dispWriter->Update();

  FloatWriterType::Pointer metricWriter = FloatWriterType::New();
  metricWriter->SetInput(bmFilter->GetMetricOutput());
  metricWriter->SetFileName(argv[4]);

  otb::StandardWriterWatcher watcher2(metricWriter,bmFilter,"Computing metric map");

  metricWriter->Update();


  return EXIT_SUCCESS;
}
