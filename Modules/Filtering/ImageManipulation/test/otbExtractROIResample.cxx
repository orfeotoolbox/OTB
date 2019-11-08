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


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "itkIdentityTransform.h"
#include "otbStreamingResampleImageFilter.h"

// Pipeline reader -> Resampler (with Identity transform) -> ExtractROI
// -> Writer
// the resample is optionally removed with the command line parameter.
//

int otbExtractROIResample(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cout << argv[0] << " <input filename> <output filename> <use resample>" << std::endl;
    return EXIT_FAILURE;
  }

  bool useResample = true;
  if (atoi(argv[3]) == 0)
  {
    useResample = false;
  }

  unsigned int startX = 10;
  unsigned int startY = 10;
  unsigned int sizeX  = 100;
  unsigned int sizeY  = 100;

  typedef double PixelType;
  typedef otb::Image<PixelType, 2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  typedef itk::IdentityTransform<double, 2> IdentityTransformType;
  IdentityTransformType::Pointer transform = IdentityTransformType::New();

  typedef otb::StreamingResampleImageFilter<ImageType, ImageType, double> ResampleType;
  ResampleType::Pointer filterResample = ResampleType::New();
  filterResample->SetInput(reader->GetOutput());
  filterResample->SetTransform(transform);
  filterResample->SetOutputSize(reader->GetOutput()->GetLargestPossibleRegion().GetSize());

  typedef otb::ExtractROI<PixelType, PixelType> ExtractROIType;
  ExtractROIType::Pointer filterResampleRoi = ExtractROIType::New();

  filterResampleRoi->SetStartX(startX);
  filterResampleRoi->SetStartY(startY);
  filterResampleRoi->SetSizeX(sizeX);
  filterResampleRoi->SetSizeY(sizeY);

  if (useResample)
  {
    filterResampleRoi->SetInput(filterResample->GetOutput());
  }
  else
  {
    filterResampleRoi->SetInput(reader->GetOutput());
  }

  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer                     streamingWriter = WriterType::New();

  streamingWriter->SetFileName(argv[2]);
  streamingWriter->SetInput(filterResampleRoi->GetOutput());
  streamingWriter->Update();

  return EXIT_SUCCESS;
}
