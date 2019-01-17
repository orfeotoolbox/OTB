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

#include <iomanip>
#include <iostream>
#include <fstream>

#include "otbImageToSURFKeyPointSetFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

int otbImageToSURFKeyPointSetFilterOutputDescriptorAscii(int argc, char * argv[])
{

  if (argc < 5)
    {
    std::cout << " Usage : otbSURFTest imageName FileOutName Octave[int] Level[int]" << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int octaves = atoi(argv[3]);
  const unsigned int scales = atoi(argv[4]);

  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef otb::Image<RealType, Dimension>                            ImageType;
  typedef itk::VariableLengthVector<RealType>                        RealVectorType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef itk::PointSet<RealVectorType, Dimension>                   PointSetType;
  typedef otb::ImageToSURFKeyPointSetFilter<ImageType, PointSetType> ImageToSURFKeyPointSetFilterType;
  typedef PointSetType::PointsContainer    PointsContainerType;
  typedef PointsContainerType::Iterator    PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;

  // Instantiating object
  ReaderType::Pointer                       reader = ReaderType::New();
  ImageToSURFKeyPointSetFilterType::Pointer filter = ImageToSURFKeyPointSetFilterType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);
  filter->Update();

  PointsIteratorType    pIt = filter->GetOutput()->GetPoints()->Begin();
  PointDataIteratorType pDataIt = filter->GetOutput()->GetPointData()->Begin();

  std::ofstream outfile(outfname);

  outfile << "Number of octaves: " << octaves << std::endl;
  outfile << "Number of scales: " << scales << std::endl;
  outfile << "Number of SURF key points: " << filter->GetNumberOfPoints() << std::endl;

  while (pIt != filter->GetOutput()->GetPoints()->End())
    {
    outfile << "[";
    unsigned int lIterDesc = 0;
    while (lIterDesc < pDataIt.Value().Size())
      {
      outfile << std::setprecision(3) << pDataIt.Value()[lIterDesc] << " ";
      lIterDesc++;
      }
    outfile << "]" << std::endl;
    ++pIt;
    ++pDataIt;
    }

  outfile.close();

  return EXIT_SUCCESS;
}
