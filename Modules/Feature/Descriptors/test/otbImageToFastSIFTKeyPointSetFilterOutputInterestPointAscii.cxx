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
#include <algorithm>

#include "otbSiftFastImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

bool CMP(std::vector<float>  a, std::vector<float>  b)
{
  return lexicographical_compare(a.begin(), a.begin() + 2, b.begin(), b.begin() + 2);
}

int
otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii( int itkNotUsed( argc ), char * argv[] )
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int scales = atoi(argv[3]);
  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef otb::Image<RealType, Dimension>          ImageType;
  typedef itk::VariableLengthVector<RealType>      RealVectorType;
  typedef otb::ImageFileReader<ImageType>          ReaderType;
  typedef itk::PointSet<RealVectorType, Dimension> PointSetType;

  typedef otb::SiftFastImageFilter<ImageType, PointSetType> ImageToFastSIFTKeyPointSetFilterType;

  // PointSet iterator types
  typedef PointSetType::PointsContainer    PointsContainerType;
  typedef PointsContainerType::Iterator    PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;

  typedef std::vector< RealType > siftDataVector;
  typedef std::vector<siftDataVector> ImageDataType;   //Kind of PointSet with vectors

  // Instantiating object
  ReaderType::Pointer                           reader = ReaderType::New();
  ImageToFastSIFTKeyPointSetFilterType::Pointer filter = ImageToFastSIFTKeyPointSetFilterType::New();

  //Instantiation of std::vector for lexicographiacal sorting
  ImageDataType imageData;

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetScalesNumber(scales);
  filter->Update();
  PointsIteratorType    pIt = filter->GetOutput()->GetPoints()->Begin();
  PointDataIteratorType pDataIt = filter->GetOutput()->GetPointData()->Begin();

  assert(
    filter->GetOutput()->GetPoints()->Size() ==
    filter->GetOutput()->GetPointData()->Size() );

  std::ofstream outfile(outfname);

  outfile << "Number of scales: " << scales << std::endl;

  outfile << "Number of SIFT key points: "
	  << filter->GetOutput()->GetNumberOfPoints()
	  << std::endl;

  outfile << "Number of points: "
	  << filter->GetOutput()->GetPoints()->Size()
	  << std::endl;

  outfile << "Number of points data: "
	  << filter->GetOutput()->GetPointData()->Size()
	  << std::endl;

  if( filter->GetOutput()->GetPoints()->Size() !=
      filter->GetOutput()->GetPointData()->Size() )
    return EXIT_FAILURE;

  // Copy the PointSet to std::vector< std::vector >
  while (pIt != filter->GetOutput()->GetPoints()->End())
    {
    siftDataVector siftData;

    siftData.push_back(pIt.Value()[0]);
    siftData.push_back(pIt.Value()[1]);

    unsigned int lIterDesc = 0;
    while (lIterDesc < pDataIt.Value().Size())
      {
      siftData.push_back(pDataIt.Value()[lIterDesc]);
      lIterDesc++;
      }

    imageData.push_back(siftData);
    ++pIt;
    ++pDataIt;
    }

  //Sorting the vectors
  ImageDataType::iterator itData;
  sort(imageData.begin(), imageData.end(), CMP);

  itData = imageData.begin();

  while (itData != imageData.end())
    {
    outfile << "[" << std::fixed << std::setprecision(1) << (*itData)[0] << ", " << std::setprecision(1) <<
      (*itData)[1] << "]" << std::endl;

    ++itData;
    }
  outfile.close();

  return EXIT_SUCCESS;
}
