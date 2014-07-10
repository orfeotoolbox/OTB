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
#include "itkRGBPixel.h"
#include "itkImageRegionIterator.h"

#include <iostream>
#include <fstream>

int otbImageToSIFTKeyPointSetFilterOutputImage(int itkNotUsed(argc), char * argv[])
{
  const char * infname = argv[1];
  const char * outputImageFilename = argv[2];

  const unsigned int octaves = atoi(argv[3]);
  const unsigned int scales = atoi(argv[4]);

  float threshold = atof(argv[5]);
  float ratio = atof(argv[6]);

  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef otb::Image<RealType, Dimension>                            ImageType;
  typedef itk::VariableLengthVector<RealType>                        RealVectorType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef itk::PointSet<RealVectorType, Dimension>                   PointSetType;
  typedef otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType> ImageToSIFTKeyPointSetFilterType;
  typedef PointSetType::PointsContainer PointsContainerType;
  typedef PointsContainerType::Iterator PointsIteratorType;

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

  ImageType::OffsetType t = {{ 0, 1}};
  ImageType::OffsetType b = {{ 0, -1}};
  ImageType::OffsetType l = {{ 1, 0}};
  ImageType::OffsetType r = {{-1, 0}};

  typedef unsigned char            PixelType;
  typedef itk::RGBPixel<PixelType>    RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> OutputImageType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  OutputImageType::Pointer    outputImage = OutputImageType::New();
  OutputImageType::RegionType region;

  OutputImageType::SizeType outputSize;
  outputSize[0] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
  outputSize[1] = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
  region.SetSize(outputSize);

  OutputImageType::IndexType indexStart;
  indexStart[0] = 0;
  indexStart[1] = 0;
  region.SetIndex(indexStart);

  outputImage->SetRegions(region);
  outputImage->Allocate();

  itk::ImageRegionIterator<OutputImageType> iterOutput(outputImage,
                                                       outputImage->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> iterInput(reader->GetOutput(),
                                                reader->GetOutput()->GetLargestPossibleRegion());

  for (iterOutput.GoToBegin(), iterInput.GoToBegin();
       !iterOutput.IsAtEnd();
       ++iterOutput, ++iterInput)
    {
    OutputImageType::PixelType rgbPixel;
    rgbPixel.SetRed(static_cast<PixelType>(iterInput.Get()));
    rgbPixel.SetGreen(static_cast<PixelType>(iterInput.Get()));
    rgbPixel.SetBlue(static_cast<PixelType>(iterInput.Get()));

    iterOutput.Set(rgbPixel);
    }

  WriterType::Pointer writerTmp = WriterType::New();
  writerTmp->SetFileName(outputImageFilename);
  writerTmp->SetInput(outputImage);
  writerTmp->Update();

  std::cout << "Copy Input image in Output image" << std::endl;

  PointsIteratorType        pIt = filter->GetOutput()->GetPoints()->Begin();
  ImageType::SpacingType    spacing = reader->GetOutput()->GetSpacing();
  ImageType::PointType      origin = reader->GetOutput()->GetOrigin();
  //OutputImageType::SizeType size = outputImage->GetLargestPossibleRegion().GetSize();

  while (pIt != filter->GetOutput()->GetPoints()->End())
    {
    ImageType::IndexType index;

    index[0] = (unsigned int)
               (vcl_floor
                  ((double) ((pIt.Value()[0] - origin[0]) / spacing[0] + 0.5)));

    index[1] = (unsigned int)
               (vcl_floor
                  ((double) ((pIt.Value()[1] - origin[1]) / spacing[1] + 0.5)));

    OutputImageType::PixelType keyPixel;
    keyPixel.SetRed(0);
    keyPixel.SetGreen(255);
    keyPixel.SetBlue(0);

    if (outputImage->GetLargestPossibleRegion().IsInside(index))
      {
      outputImage->SetPixel(index, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + t)) outputImage->SetPixel(index + t, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + b)) outputImage->SetPixel(index + b, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + l)) outputImage->SetPixel(index + l, keyPixel);

      if (outputImage->GetLargestPossibleRegion().IsInside(index + r)) outputImage->SetPixel(index + r, keyPixel);
      }
    ++pIt;
    }

  std::cout << "Copy sift key" << std::endl;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageFilename);
  writer->SetInput(outputImage);
  writer->Update();

  std::cout << "Write image" << std::endl;
  return EXIT_SUCCESS;
}
