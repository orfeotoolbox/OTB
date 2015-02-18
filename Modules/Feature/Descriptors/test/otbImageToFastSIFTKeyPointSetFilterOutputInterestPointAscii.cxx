/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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

int otbImageToFastSIFTKeyPointSetFilterOutputInterestPointAscii(int itkNotUsed(argc), char * argv[])
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

  typedef std::vector<float>          siftDataVector;
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

  std::ofstream outfile(outfname);

  outfile << "Number of scales: " << scales << std::endl;
  outfile << "Number of SIFT key points: " << filter->GetOutput()->GetNumberOfPoints() << std::endl;

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
