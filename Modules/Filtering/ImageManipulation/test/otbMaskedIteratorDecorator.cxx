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
#include <set>
#include "otbImage.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionReverseIterator.h"
#include "itkImageRandomIteratorWithIndex.h"
#include "itkImageScanlineIterator.h"
#include "itkImageRandomNonRepeatingIteratorWithIndex.h"
#include "otbSubsampledImageRegionIterator.h"

#include "otbMaskedIteratorDecorator.h"

// Generate a test image of specified size and value
template <typename ImageType>
typename ImageType::Pointer GetTestImage(itk::SizeValueType fillSize, const typename ImageType::PixelType& value)
{
  typename ImageType::Pointer image = ImageType::New();
  typename ImageType::SizeType size;
  size.Fill(fillSize);

  typename ImageType::RegionType region;
  region.SetSize(size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(value);
  return image;
}

// Fill half of the pixels with a value
// Used for generating a test mask
template <typename ImageType>
void FillHalf(typename ImageType::Pointer image, const typename ImageType::RegionType& region, const typename ImageType::PixelType& value)
{
  itk::ImageRegionIterator<ImageType> it(image, region);
  unsigned int count = 0;
  for(it.GoToBegin(); !it.IsAtEnd(); ++it, ++count)
  {
    if (count % 2 == 0)
    {
      it.Set(value);
    }
  }
}

// Test template instanciation
int otbMaskedIteratorDecoratorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2> ImageType;
  ImageType::Pointer image = GetTestImage<ImageType>(10, 10);
  ImageType::Pointer mask = GetTestImage<ImageType>(10, 0);
  ImageType::RegionType region(image->GetLargestPossibleRegion());

  otb::MaskedIteratorDecorator<itk::ImageRegionIterator<ImageType> > it(mask, image, region);
  return EXIT_SUCCESS;
}

// Function to test the forward iteration interface
template <typename IteratorType>
int ForwardTest(typename IteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  otb::MaskedIteratorDecorator<IteratorType> it(mask, image, region);

  it.GoToBegin();
  if (!it.IsAtBegin()) {return EXIT_FAILURE;}

  unsigned int loopCount = 0;
  for(; !it.IsAtEnd(); ++it)
  {
    if (loopCount != 0 && it.IsAtBegin()) {return EXIT_FAILURE;}
    if (it.IsAtEnd()) {return EXIT_FAILURE;}

    //it.Set(it.Value() * 0.42);

    loopCount += 1;
  }

  if(!it.IsAtEnd()) {return EXIT_FAILURE;}
  return EXIT_SUCCESS;
}

// Test reverse iteration interface
template <typename IteratorType>
int ReverseTest(typename IteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  otb::MaskedIteratorDecorator<IteratorType> it(mask, image, region);

  it.GoToEnd();
  if (!it.IsAtEnd()) {return EXIT_FAILURE;}

  bool beginReached = false;
  do
  {
    --it;

    if (it.IsAtEnd()) {return EXIT_FAILURE;}
    if (it.IsAtBegin())
    {
      if (beginReached)
      {
        return EXIT_FAILURE;
      }
      else {
        beginReached = true;
      }
    }

    //it.Set(it.Value() * 0.42);
  } while (!it.IsAtBegin());

  if(!it.IsAtBegin()) {return EXIT_FAILURE;}
  return EXIT_SUCCESS;
}

// Check bijection between iterated and non masked
// i.e all locations where mask value != 0 are in the iteration (injection)
// and mask value != 0 at all iteration locations (surjection)
// Templated to test decoration of different iterator types
template <typename IteratorType>
int BijectiveTest(typename IteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  otb::MaskedIteratorDecorator<IteratorType> itDecorated(mask, image, region);
  IteratorType it(image, region);

  it.GoToBegin();
  itDecorated.GoToBegin();
  while (!it.IsAtEnd() && itDecorated.IsAtEnd())
  {
    // Iteration locations are the same
    if (it.GetIndex() != itDecorated.GetIndex()) {return EXIT_FAILURE;}

    ++itDecorated;
    do
    {
      ++it;
    } while (mask->GetPixel(it.GetIndex()) == 0 && !it.IsAtEnd());
  }
  return EXIT_SUCCESS;
}

// Multiplex to forward, reverse and bijection test
template <typename IteratorType>
int TripleTest(typename IteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  return ForwardTest<IteratorType>(mask, image, region)
      || ReverseTest<IteratorType>(mask, image, region)
      || BijectiveTest<IteratorType>(mask, image, region);
}

// Nominal case
int otbMaskedIteratorDecoratorNominal(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2> ImageType;
  ImageType::Pointer image = GetTestImage<ImageType>(10, 10);
  ImageType::Pointer mask = GetTestImage<ImageType>(10, 0);
  ImageType::RegionType region(image->GetLargestPossibleRegion());
  FillHalf<ImageType>(mask, region, 1);
  
  std::cout << "EXIT_SUCCESS : "<< int(EXIT_SUCCESS) << std::endl;
  std::cout << "EXIT_FAILURE : "<< int(EXIT_FAILURE) << std::endl;

  return TripleTest<itk::ImageRegionIterator<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageRegionConstIterator<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageRandomConstIteratorWithIndex<ImageType> >(image, mask, region)
      || TripleTest< otb::SubsampledImageRegionIterator<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageRandomIteratorWithIndex<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageScanlineIterator<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageScanlineConstIterator<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageRandomNonRepeatingConstIteratorWithIndex<ImageType> >(image, mask, region)
      || TripleTest< itk::ImageRandomNonRepeatingIteratorWithIndex<ImageType> >(image, mask, region)
      ;
}

/* Other iterators potentially compatible:

Different constructor arguments than (image, region)
itk::PathIterator
itk::LineIterator
itk::SliceIterator
itk::NeighborhoodIterator

GoToEnd is not implemented
itk::ImageLinearIteratorWithIndex
itk::ReflectiveImageRegionIterator
itk::ImageRegionExclusionConstIteratorWithIndex
itk::ImageRegionIteratorWithIndex

Other problem:
itk::ImageRegionReverseIterator>() // IsAtEnd not a const method
otb::PolyLineImageIterator>() // header not found
itk::ImageRandomConstIteratorWithOnlyIndex>() // no Value method
*/
