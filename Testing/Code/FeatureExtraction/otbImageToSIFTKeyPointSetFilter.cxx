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
#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"
#include "otbRationalQuotientResampleImageFilter.h"

#include <iostream>
#include <fstream>

int otbImageToSIFTKeyPointSetFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const unsigned int octaves = atoi(argv[3]);
  const unsigned int scales = atoi(argv[4]);
  
  typedef float RealType;
  const unsigned int Dimension =2;

  typedef otb::Image<RealType,Dimension> ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType,PointSetType> ImageToSIFTKeyPointSetFilterType;
  
  // Instantiating object
  ReaderType::Pointer reader = ReaderType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter = ImageToSIFTKeyPointSetFilterType::New();

  reader->SetFileName(infname);
  // flou !!
  filter->SetInput(0,reader->GetOutput());
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);
  filter->Update();
  
//   typedef otb::Image<unsigned char,2> UCharImageType;
//   typedef otb::ImageFileWriter<UCharImageType> WriterType;
    
//   UCharImageType::Pointer outputImage = UCharImageType::New();
//   outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
//   outputImage->Allocate();
//   outputImage->FillBuffer(0);
  
//   std::ofstream outfile(outfname);
//   //std::ios::write);
//   ImageType::IndexType index;
  
//   outfile<<"Number of SIFT key: "<<filter->GetOutput()->GetNumberOfPoints()<<std::endl;
  
//   if(filter->GetOutput()->GetNumberOfPoints()>0)
//     {

//       PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
//       PointDataIteratorType pdIt = filter->GetOutput()->GetPointData()->Begin();
      
//       while(pIt!=filter->GetOutput()->GetPoints()->End()
// 	    && pdIt!=filter->GetOutput()->GetPointData()->End())
// 	{
// 	  reader->GetOutput()->TransformPhysicalPointToIndex(pIt.Value(),index);
	  
// 	  outputImage->SetPixel(index,255);
	  
// 	  outfile<<"Point "<<pIt.Value()<<", Index: "<<index<<", associated data: "<<pdIt.Value()<<std::endl;
// 	  ++pIt;
// 	  ++pdIt;
// 	}
//     }
//   outfile.close();
  
//   WriterType::Pointer writer = WriterType::New();
//   writer->SetInput(outputImage);
//   writer->SetFileName("test.png");
//   writer->Update();
  
  return EXIT_SUCCESS;
}
