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

int otbImageToSIFTKeyPointSetFilterValid(int argc, char * argv[])
{
  const char* inputImageFileName = argv[1];
  const char* inputKeysFileName = argv[2];
  const char* outputImageFileName = argv[3];
  
  typedef otb::Image<float,2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageFileName);
  reader->Update();
  
  ImageType::OffsetType t = {{ 0, 1}};
  ImageType::OffsetType b = {{ 0,-1}};
  ImageType::OffsetType l = {{ 1, 0}};
  ImageType::OffsetType r = {{-1, 0}};
    
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> OutputImageType;
  
  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  
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
  
  OutputImageType::PixelType greenPixel;
  OutputImageType::SizeType size = outputImage->GetLargestPossibleRegion().GetSize();
  
  greenPixel.SetGreen(255);
  greenPixel.SetRed(0);
  greenPixel.SetBlue(0);
  
  std::ifstream desc(inputKeysFileName);
  while(!desc.eof())
    {
      OutputImageType::IndexType index;
      OutputImageType::PointType point;
      std::string line;
      desc >> point[0] >> point[1];
      std::getline(desc,line);
      
      outputImage->TransformPhysicalPointToIndex(point, index);

      outputImage->SetPixel(index, greenPixel);
      if (static_cast<unsigned int>(index[1]) < static_cast<unsigned int>(size[1]) )
	outputImage->SetPixel(index+t,greenPixel);
      if (index[1] > 0)
	outputImage->SetPixel(index+b,greenPixel);
      if (static_cast<unsigned int>(index[0]) < static_cast<unsigned int>(size[0]) )
	outputImage->SetPixel(index+l,greenPixel);
      if (index[0] > 0)
	outputImage->SetPixel(index+r,greenPixel);
      
    }
  desc.close();

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputImageFileName);
  writer->Update();
  
  return EXIT_SUCCESS;
}

int otbImageToSIFTKeyPointSetFilter(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];
  const char * outputImageFilename = argv[3];

  const unsigned int octaves = atoi(argv[4]);
  const unsigned int scales = atoi(argv[5]);
  float threshold = .0;
  float ratio = .0;
  
  if (argc > 6)
    {
      threshold = atof(argv[6]);
      ratio = atof(argv[7]);
    }
  
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
  
  if (argc>6)
    {
      filter->SetDoGThreshold(threshold);
      filter->SetEdgeThreshold(ratio);
    }
  
  filter->Update();
  
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
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  ImageType::PointType origin = reader->GetOutput()->GetOrigin();
  OutputImageType::SizeType size = outputImage->GetLargestPossibleRegion().GetSize();
  
  while( pIt!=filter->GetOutput()->GetPoints()->End() )
    {
      ImageType::IndexType index;
      
      index[0] = (unsigned int)
	(vcl_floor
	 ((double)((pIt.Value()[0]-origin[0])/spacing[0]+0.5)));
      
      index[1] = (unsigned int)
	(vcl_floor
	 ((double)((pIt.Value()[1]-origin[1])/spacing[1]+0.5)));
      
      OutputImageType::PixelType keyPixel;
      keyPixel.SetRed(0);
      keyPixel.SetGreen(255);
      keyPixel.SetBlue(0);
      
      outputImage->SetPixel(index,keyPixel);
      if (static_cast<unsigned int>(index[1]) < static_cast<unsigned int>(size[1]) )
	outputImage->SetPixel(index+t,keyPixel);
      if (index[1] > 0)
	outputImage->SetPixel(index+b,keyPixel);
      if (static_cast<unsigned int>(index[0]) < static_cast<unsigned int>(size[0]) )
	outputImage->SetPixel(index+l,keyPixel);
      if (index[0] > 0)
	outputImage->SetPixel(index+r,keyPixel);
      ++pIt;
    }
  
  std::ofstream outfile(outfname);
  outfile << filter;
  outfile.close();
  
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputImageFilename);
  writer->Update();
  
  return EXIT_SUCCESS;
}
