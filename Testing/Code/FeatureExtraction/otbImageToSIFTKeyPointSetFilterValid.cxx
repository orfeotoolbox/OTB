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

/** This function takes an input image name, an input keys file name (from sift code)
 *  and produce an output image where key points found by sift code are put on
 *  image
 *
 *  This function is only present to compare the OTB implementation
 *  and the sift code implementation
 *
 *  Sift code is not under svn base otb project, but it can be found following the link
 *  http://vision.ucla.edu/~vedaldi/code/siftpp/siftpp.html
 */
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
