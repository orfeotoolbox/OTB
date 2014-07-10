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



#include <complex>
#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

#include "itkMacro.h"
#include "itkImageRegionIterator.h"


/***********
 * 1.
 * Write Image<scalar> with an ImageFileWriter<scalar>
 ***********/
template<class InternalType>
int otbImageScalarStreamingFileWriterTestWithoutInputGeneric(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * outputFilename = argv[1];

  typedef InternalType                                  PixelType;
  typedef otb::Image<PixelType, 2>                      ImageType;
  typedef typename ImageType::PixelType                 ImagePixelType;
  typedef typename ImageType::RegionType                RegionType;
  typedef typename ImageType::SizeType                  SizeType;
  typedef typename ImageType::IndexType                 IndexType;
  typedef itk::ImageRegionIterator<ImageType>           IteratorType;
  typedef otb::ImageFileWriter<ImageType>      WriterType;

  typename ImageType::Pointer  image = ImageType::New();
  typename WriterType::Pointer writer = WriterType::New();

  SizeType size;
  size.Fill(atoi(argv[2]));
  IndexType orig;
  orig.Fill(0);
  RegionType region;
  region.SetSize(size);
  region.SetIndex(orig);

  typedef typename ImageType::PointType        PointType;
  typedef typename ImageType::SpacingType      SpacingType;
  PointType origin;
  origin.Fill(0.5);
  SpacingType spacing;
  spacing.Fill(1.0);
  image->SetOrigin(origin);
  image->SetSpacing(spacing);

  image->SetRegions(region);
  image->Allocate();

  // Fill to zero values
  image->FillBuffer(static_cast<PixelType>(0));

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    it.Set(static_cast<PixelType> (size[0] * it.GetIndex()[1] + it.GetIndex()[0]));
    ++it;
    }

  writer->SetFileName(outputFilename);
  writer->SetNumberOfDivisionsStrippedStreaming(10);
  writer->SetInput(image);
  writer->Update();


  return EXIT_SUCCESS;
}

int otbImageScalarStreamingFileWriterTestWithoutInputShort(int argc, char * argv[])
{
  return otbImageScalarStreamingFileWriterTestWithoutInputGeneric<short>(argc, argv);
}
int otbImageScalarStreamingFileWriterTestWithoutInputInt(int argc, char * argv[])
{
  return otbImageScalarStreamingFileWriterTestWithoutInputGeneric<int>(argc, argv);
}
int otbImageScalarStreamingFileWriterTestWithoutInputFloat(int argc, char * argv[])
{
  return otbImageScalarStreamingFileWriterTestWithoutInputGeneric<float>(argc, argv);
}
int otbImageScalarStreamingFileWriterTestWithoutInputDouble(int argc, char * argv[])
{
  return otbImageScalarStreamingFileWriterTestWithoutInputGeneric<double>(argc, argv);
}

/***********
 * 1.
 * Write Image<scalar> with an ImageFileWriter<scalar>
 ***********/
template<class InternalType>
int otbImageComplexStreamingFileWriterTestWithoutInputGeneric(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * outputFilename = argv[1];

  typedef std::complex<InternalType>                    PixelType;
  typedef otb::Image<PixelType, 2>                      ImageType;
  typedef typename ImageType::PixelType                 ImagePixelType;
  typedef typename ImageType::RegionType                RegionType;
  typedef typename ImageType::SizeType                  SizeType;
  typedef typename ImageType::IndexType                 IndexType;
  typedef itk::ImageRegionIterator<ImageType>           IteratorType;
  typedef otb::ImageFileWriter<ImageType>      WriterType;

  typename ImageType::Pointer image = ImageType::New();
  typename WriterType::Pointer writer = WriterType::New();

  SizeType size;
  size.Fill(atoi(argv[2]));
  IndexType orig;
  orig.Fill(0);
  RegionType region;
  region.SetSize(size);
  region.SetIndex(orig);

  typedef typename ImageType::PointType        PointType;
  typedef typename ImageType::SpacingType      SpacingType;
  PointType origin;
  origin.Fill(0.5);
  SpacingType spacing;
  spacing.Fill(1.0);
  image->SetOrigin(origin);
  image->SetSpacing(spacing);

  image->SetRegions(region);
  image->Allocate();

  // Fill to zero values
  image->FillBuffer(static_cast<PixelType>(0));

  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();

  typedef typename itk::NumericTraits<PixelType>::ScalarRealType ScalarRealType;
  while (!it.IsAtEnd())
    {
    ScalarRealType realPart = 2*(size[0] * it.GetIndex()[1] + it.GetIndex()[0]);
    ScalarRealType imagPart = 2*(size[0] * it.GetIndex()[1] + it.GetIndex()[0]) +1;

    PixelType pixelVal  (realPart, imagPart);

    it.Set(pixelVal);
    ++it;
    }

  writer->SetFileName(outputFilename);
  writer->SetNumberOfDivisionsStrippedStreaming(10);
  writer->SetInput(image);
  writer->Update();


  return EXIT_SUCCESS;
}


int otbImageComplexStreamingFileWriterTestWithoutInputShort(int argc, char * argv[])
{
  return otbImageComplexStreamingFileWriterTestWithoutInputGeneric<short>(argc, argv);
}
int otbImageComplexStreamingFileWriterTestWithoutInputInt(int argc, char * argv[])
{
  return otbImageComplexStreamingFileWriterTestWithoutInputGeneric<int>(argc, argv);
}
int otbImageComplexStreamingFileWriterTestWithoutInputFloat(int argc, char * argv[])
{
  return otbImageComplexStreamingFileWriterTestWithoutInputGeneric<float>(argc, argv);
}
int otbImageComplexStreamingFileWriterTestWithoutInputDouble(int argc, char * argv[])
{
  return otbImageComplexStreamingFileWriterTestWithoutInputGeneric<double>(argc, argv);
}
