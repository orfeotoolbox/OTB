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

#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

int otbImageToSIFTKeyPointSetFilterOutputDescriptorAscii(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int octaves = atoi(argv[3]);
  const unsigned int scales = atoi(argv[4]);
  const float        threshold = atof(argv[5]);
  const float        ratio = atof(argv[6]);

  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef otb::Image<RealType, Dimension>                            ImageType;
  typedef itk::VariableLengthVector<RealType>                        RealVectorType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef itk::PointSet<RealVectorType, Dimension>                   PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType> ImageToSIFTKeyPointSetFilterType;

  // PointSet terator type
  typedef PointSetType::PointsContainer    PointsContainerType;
  typedef PointsContainerType::Iterator    PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;

  // Instantiating object
  ReaderType::Pointer                       reader = ReaderType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter = ImageToSIFTKeyPointSetFilterType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);
  filter->SetDoGThreshold(threshold);
  filter->SetEdgeThreshold(ratio);
  filter->Update();

  PointsIteratorType    pIt = filter->GetOutput()->GetPoints()->Begin();
  PointDataIteratorType pDataIt = filter->GetOutput()->GetPointData()->Begin();

  std::ofstream outfile(outfname);

  outfile << "Number of octaves: " << octaves << std::endl;
  outfile << "Number of scales: " << scales << std::endl;
  outfile << "Number of SIFT key points: " << filter->GetOutput()->GetNumberOfPoints() << std::endl;
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
