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

#include "otbVectorImage.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"

int otbListSampleToHistogramListGenerator(int itkNotUsed(argc), char* argv[])
{
  typedef double                                       PixelType;
  typedef otb::VectorImage<PixelType>                  VectorImageType;
  typedef VectorImageType::PixelType                   VectorPixelType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef otb::ListSampleToHistogramListGenerator<ListSampleType, PixelType> HistogramGeneratorType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;

  // Instantiation
  ReaderType::Pointer             reader    = ReaderType::New();
  HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
  ListSampleType::Pointer         ls        = ListSampleType::New();

  reader->SetFileName(argv[1]);
  reader->Update();

  // Set the size of the ListSample Measurement Vector
  ls->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());

  itk::ImageRegionConstIterator<VectorImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {
    ls->PushBack(it.Get());
  }

  float        mscale = atof(argv[4]);
  unsigned int nbBins = atoi(argv[3]);

  generator->SetListSample(ls);
  generator->SetNumberOfBins(nbBins);
  generator->SetMarginalScale(mscale);
  generator->Update();

  std::ofstream ofs;
  ofs.open(argv[2]);
  for (unsigned int comp = 0; comp < reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++comp)
  {
    ofs << "Channel: " << comp << " histogram: " << std::endl;
    for (unsigned int bin = 0; bin < nbBins; ++bin)
    {
      ofs << generator->GetOutput()->GetNthElement(comp)->GetFrequency(bin) << "\t";
    }
    ofs << std::endl;
  }

  ofs.close();

  return EXIT_SUCCESS;
}
