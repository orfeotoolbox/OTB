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
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>

int otbImageToSIFTKeyPointSetFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const char * outputImageFilename = argv[3];

  const unsigned int octaves = atoi(argv[4]);
  const unsigned int scales = atoi(argv[5]);
  
  typedef float RealType;
  const unsigned int Dimension =2;

  typedef otb::Image<RealType,Dimension> ImageType;
  typedef itk::VariableLengthVector<RealType> RealVectorType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType,PointSetType> ImageToSIFTKeyPointSetFilterType;

  // PointSet iterator types
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;
  typedef PointSetType::PointDataContainer PointDataContainerType;
  typedef PointDataContainerType::Iterator PointDataIteratorType;
  
  // Instantiating object
  ReaderType::Pointer reader = ReaderType::New();
  ImageToSIFTKeyPointSetFilterType::Pointer filter = ImageToSIFTKeyPointSetFilterType::New();
  
  reader->SetFileName(infname);
  filter->SetInput(0,reader->GetOutput());
  filter->SetOctavesNumber(octaves);
  filter->SetScalesNumber(scales);
  filter->Update();
  
  std::ofstream outfile(outfname);
  outfile<<"Number of SIFT key: "<<filter->GetOutput()->GetNumberOfPoints()<<std::endl;
  ImageType::OffsetType t = {{ 0, 1}};
  ImageType::OffsetType b = {{ 0,-1}};
  ImageType::OffsetType l = {{ 1, 0}};
  ImageType::OffsetType r = {{-1, 0}};
  
  typedef otb::Image<unsigned char,2> UCharImageType;
  
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> OutputImageType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  
  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  
  OutputImageType::SizeType lRadius;
  lRadius.Fill(1);
  

  itk::ImageRegionIterator<OutputImageType> iterOutput(outputImage,
						       outputImage->GetRequestedRegion());
  
  for (iterOutput.GoToBegin(); !iterOutput.IsAtEnd(); ++iterOutput)
    {
      ImageType::IndexType index = iterOutput.GetIndex();
      ImageType::PixelType grayPix = reader->GetOutput()->GetPixel(index);
      OutputImageType::PixelType rgbPixel;
      rgbPixel.SetRed( static_cast<unsigned char>(grayPix) );
      rgbPixel.SetGreen( static_cast<unsigned char>(grayPix) );
      rgbPixel.SetBlue( static_cast<unsigned char>(grayPix) );
      
      iterOutput.Set(rgbPixel);
    }
  
  PointsIteratorType pIt = filter->GetOutput()->GetPoints()->Begin();
  PointDataIteratorType pdIt = filter->GetOutput()->GetPointData()->Begin();
  while( pIt!=filter->GetOutput()->GetPoints()->End() &&
	 pdIt!=filter->GetOutput()->GetPointData()->End() )
    {
      ImageType::IndexType index;
      reader->GetOutput()->TransformPhysicalPointToIndex(pIt.Value(),index);
      outfile<<"Point: "<<pIt.Value()<<", Index: "<<index \
	     << " Octave: " << pdIt.Value()[0] \
	     << " Scale: " <<  pdIt.Value()[1] << std::endl;
      
      OutputImageType::PixelType keyPixel;
      keyPixel.SetRed(0);
      keyPixel.SetGreen(255);
      keyPixel.SetBlue(0);
      
      outputImage->SetPixel(index,keyPixel);
      outputImage->SetPixel(index+t,keyPixel);
      outputImage->SetPixel(index+b,keyPixel);
      outputImage->SetPixel(index+l,keyPixel);
      outputImage->SetPixel(index+r,keyPixel);
      ++pIt;
      ++pdIt;
    }
  outfile.close();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputImageFilename);
  writer->Update();
  
  return EXIT_SUCCESS;
}
