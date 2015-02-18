/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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

int otbImageToSURFKeyPointSetFilterOutputInterestPointAscii(int argc, char * argv[])
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

  // Instantiating object
  ReaderType::Pointer                       reader = ReaderType::New();
  ImageToSURFKeyPointSetFilterType::Pointer filter = ImageToSURFKeyPointSetFilterType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);
  filter->Update();

  PointsIteratorType    pIt = filter->GetOutput()->GetPoints()->Begin();
  filter->GetOutput()->GetPointData()->Begin();

  std::ofstream outfile(outfname);

  outfile << "Number of octaves: " << octaves << std::endl;
  outfile << "Number of scales: " << scales << std::endl;
  outfile << "Number of SURF key points: " << filter->GetNumberOfPoints() << std::endl;

  while (pIt != filter->GetOutput()->GetPoints()->End())
    {
    outfile << "[" << std::fixed << std::setprecision(2) << pIt.Value()[0] << ", " << std::setprecision(2) <<
    pIt.Value()[1] << "]" << std::endl;
    ++pIt;
    }

  outfile.close();

  return EXIT_SUCCESS;
}
