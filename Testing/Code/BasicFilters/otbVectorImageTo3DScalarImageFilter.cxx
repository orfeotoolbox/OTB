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
#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImageSliceConstIteratorWithIndex.h"

int otbVectorImageTo3DScalarImageFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int BiDimension  = 2;
  const unsigned int TriDimension  = 3;

  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef unsigned char PixelType;

  typedef otb::VectorImage<PixelType, BiDimension> VectorImageType;
  typedef otb::Image<PixelType, TriDimension>      ImageType;
  typedef otb::Image<PixelType, BiDimension>       OutImageType;

  typedef otb::ImageFileReader<VectorImageType>                             ReaderType;
  typedef otb::ImageFileWriter<OutImageType>                                WriterType;
  typedef otb::VectorImageTo3DScalarImageFilter<VectorImageType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  filter->Update();

  // Build the first slice
  OutImageType::Pointer outImage = OutImageType::New();
  outImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outImage->Allocate();
  outImage->FillBuffer(0);

  typedef itk::ImageRegionIterator<OutImageType>           OutIteratorType;
  typedef itk::ImageSliceConstIteratorWithIndex<ImageType> InIteratorType;

  OutIteratorType outIt(outImage, outImage->GetLargestPossibleRegion());
  InIteratorType  inIt(filter->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion());
  inIt.SetFirstDirection(0);
  inIt.SetSecondDirection(1);

  outIt.GoToBegin();

  while (!outIt.IsAtEnd() && !inIt.IsAtEndOfSlice())
    {
    outIt.Set(inIt.Get());
    ++inIt;
    ++outIt;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(outImage);
  writer->Update();

  return EXIT_SUCCESS;
}
