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
#include "otbSiftFastImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbRationalQuotientResampleImageFilter.h"
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>

int otbImageToFastSIFTKeyPointSetFilterOutputAscii(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  const unsigned int scales = atoi(argv[3]);

  typedef float RealType;
  const unsigned int Dimension =2;

  typedef otb::Image<RealType,Dimension> ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;
  typedef otb::SiftFastImageFilter<ImageType,PointSetType> ImageToFastSIFTKeyPointSetFilterType;
  
  // PointSet iterator types
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;

  // Instantiating object
  ReaderType::Pointer reader = ReaderType::New();
  ImageToFastSIFTKeyPointSetFilterType::Pointer filter = ImageToFastSIFTKeyPointSetFilterType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfScales(scales);
  filter->Update();

  PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
  PointDataIteratorType pDataIt = filter->GetOutput()->GetPointData()->Begin();

  std::ofstream outfile(outfname);

  outfile << "Number of scales: "<<scales << std::endl;
  outfile << "Number of SIFT key points: " << filter->GetOutput()->GetNumberOfPoints() << std::endl;
  while( pIt!=filter->GetOutput()->GetPoints()->End() )
    {
      outfile << "[" << std::fixed << std::setprecision(2) << pIt.Value()[0] << ", " << std::setprecision(2) << pIt.Value()[1] << "][";

      unsigned int lIterDesc=0;
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
