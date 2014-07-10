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


#include "otbConcatenateScalarValueImageFilter.h"

#include "otbImageFileReader.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include <complex>

typedef double PixelType;

typedef otb::VectorImage<PixelType> ImageType;
typedef otb::ConcatenateScalarValueImageFilter<ImageType, ImageType> FilterType;

int otbConcatenateScalarValueImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  FilterType::Pointer filter = FilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbConcatenateScalarValueImageFilterTest(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  ImageType::Pointer image = ImageType::New();

  const PixelType ScalarValue = 42;
  const unsigned int Size = 10;
  const unsigned int NbComponentIn = 4;
  ImageType::RegionType region;
  region.SetIndex(0, 0);
  region.SetIndex(1, 0);
  region.SetSize(0, Size);
  region.SetSize(1, Size);
  image->SetRegions(region);
  image->SetNumberOfComponentsPerPixel(NbComponentIn);
  image->Allocate();


  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetLargestPossibleRegion());
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    ImageType::IndexType idx = it.GetIndex();
    ImageType::PixelType value;
    value.SetSize(NbComponentIn);

    for (unsigned int k = 0; k < NbComponentIn; ++k)
      {
      value[k] = idx[0] + idx[1] * Size + k;
      }

    it.Set(value);
    }

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(image);
  filter->SetScalarValue(ScalarValue);
  ImageType::Pointer outimage = filter->GetOutput();

  filter->UpdateOutputInformation();
  if (outimage->GetNumberOfComponentsPerPixel() != NbComponentIn + 1)
    {
    std::cerr << "Number of components should be " << NbComponentIn + 1 << std::endl;
    return EXIT_FAILURE;
    }

  filter->Update();


  itk::ImageRegionIteratorWithIndex<ImageType> outIt(outimage, outimage->GetLargestPossibleRegion());
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    ImageType::IndexType idx = outIt.GetIndex();
    ImageType::PixelType value = outIt.Get();

    for (unsigned int k = 0; k < NbComponentIn; ++k)
      {
      if (value[k] != idx[0] + idx[1] * Size + k)
        {
        std::cerr << "Error at index " << idx << " channel " << k << " : expected " << idx[0] + idx[1] * Size + k << " got " << value[k] << std::endl;
        return EXIT_FAILURE;
        }
      }
    if (value[NbComponentIn] != ScalarValue)
      {
      std::cerr << "Error at index " << idx << " channel " << NbComponentIn << " : expected " << ScalarValue << " got " << value[NbComponentIn] << std::endl;
      return EXIT_FAILURE;
      }
    }

  return EXIT_SUCCESS;
}
