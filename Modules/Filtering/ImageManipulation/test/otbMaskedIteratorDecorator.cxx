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
#include "otbVectorImage.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionReverseIterator.h"
#include "itkImageRandomIteratorWithIndex.h"
#include "itkImageScanlineIterator.h"
#include "itkImageRandomNonRepeatingIteratorWithIndex.h"
#include "otbSubsampledImageRegionIterator.h"

#include "otbMaskedIteratorDecorator.h"

// Generate a test image of specified size and value
template <typename ImageType>
typename ImageType::Pointer GetTestImage(itk::SizeValueType fillSize, const typename ImageType::PixelType& value, unsigned int nbBand=1)
{
  typename ImageType::Pointer image = ImageType::New();
  typename ImageType::SizeType size;
  size.Fill(fillSize);

  typename ImageType::RegionType region;
  region.SetSize(size);

  image->SetNumberOfComponentsPerPixel(nbBand);
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

// ---------------------- Initialization code ----------------------------------
template <typename IteratorType>
void
InitializeIterator(IteratorType&, typename IteratorType::RegionType)
{
  // by default : nothing to do
}

// specialization for otb::SubsampledImageRegionIterator<otb::Image<double, 2> >
template <>
void
InitializeIterator< otb::SubsampledImageRegionIterator< otb::Image<double, 2> > >(
  otb::SubsampledImageRegionIterator< otb::Image<double, 2> >& it,
  typename otb::SubsampledImageRegionIterator<otb::Image<double, 2> >::RegionType)
{
  it.SetSubsampleFactor(2);
}


// -------------------------- ForwardTest --------------------------------------
// Function to test the forward iteration interface
template <typename IteratorType, typename MaskIteratorType>
int ForwardTest(typename MaskIteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  otb::MaskedIteratorDecorator<IteratorType,MaskIteratorType> it(mask, image, region);
  
  // specific initialization code
  InitializeIterator<IteratorType>(it.GetImageIterator(),region);
  if (it.HasMask())
    {
    InitializeIterator<MaskIteratorType>(it.GetMaskIterator(),region);
    }

  it.GoToBegin();
  if (!it.IsAtBegin()) {return 1;}

  unsigned int loopCount = 0;
  for(; !it.IsAtEnd(); ++it)
  {
    if (loopCount != 0 && it.IsAtBegin()) {return 2;}
    if (it.IsAtEnd()) {return 3;}

    //it.Set(it.Value() * 0.42);

    loopCount += 1;
  }

  if(!it.IsAtEnd()) {return 4;}
  return 0;
}

// -------------------------- ReverseTest --------------------------------------
// Test reverse iteration interface
template <typename IteratorType, typename MaskIteratorType>
int ReverseTest(typename MaskIteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  otb::MaskedIteratorDecorator<IteratorType,MaskIteratorType> it(mask, image, region);
  
  // specific initialization code
  InitializeIterator<IteratorType>(it.GetImageIterator(),region);
  if (it.HasMask())
    {
    InitializeIterator<MaskIteratorType>(it.GetMaskIterator(),region);
    }

  it.GoToEnd();
  if (!it.IsAtEnd()) {return 1;}

  bool beginReached = false;
  do
  {
    --it;

    if (it.IsAtEnd()) {return 2;}
    if (it.IsAtBegin())
    {
      if (beginReached)
      {
        return 3;
      }
      else {
        beginReached = true;
      }
    }

    //it.Set(it.Value() * 0.42);
  } while (!it.IsAtBegin());

  if(!it.IsAtBegin()) {return 4;}
  return 0;
}

// -------------------------- BijectiveTest --------------------------------------
// Check bijection between iterated and non masked
// i.e all locations where mask value != 0 are in the iteration (injection)
// and mask value != 0 at all iteration locations (surjection)
// Templated to test decoration of different iterator types
template <typename IteratorType, typename MaskIteratorType>
int BijectiveTest(typename MaskIteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::Pointer image, typename IteratorType::ImageType::RegionType region)
{
  otb::MaskedIteratorDecorator<IteratorType,MaskIteratorType> itDecorated(mask, image, region);
  IteratorType it(image, region);
  
  // specific initialization code
  InitializeIterator<IteratorType>(itDecorated.GetImageIterator(),region);
  InitializeIterator<IteratorType>(it,region);
  if (itDecorated.HasMask())
    {
    InitializeIterator<MaskIteratorType>(itDecorated.GetMaskIterator(),region);
    }
  

  it.GoToBegin();
  itDecorated.GoToBegin();

  // Find the non maked begin for the image iterator
  if (mask)
    {
    while (mask->GetPixel(it.GetIndex()) == 0 && !it.IsAtEnd())
      {
      ++it;
      }
    }

  // Begins are the same
  if (!(it.GetIndex() == itDecorated.GetIndex()
     && it.GetIndex() == itDecorated.GetImageIterator().GetIndex()))
  {
    return 1;
  }
  
  if (itDecorated.HasMask() && it.GetIndex() != itDecorated.GetMaskIterator().GetIndex())
    {
    return 1;
    }

  // Advance both and check
  while (!it.IsAtEnd() && !itDecorated.IsAtEnd())
  {
    // Iteration locations are the same
    if (!(it.GetIndex() == itDecorated.GetIndex()
       && it.GetIndex() == itDecorated.GetImageIterator().GetIndex()))
      {
      return 2;
      }
    if (itDecorated.HasMask() && it.GetIndex() != itDecorated.GetMaskIterator().GetIndex())
      {
      return 2;
      }

    ++itDecorated;
    if (mask)
      {
      do
        {
        ++it;
        } while (mask->GetPixel(it.GetIndex()) == 0 && !it.IsAtEnd());
      }
    else
      {
      ++it;
      }
  }

  // Check IsAtEnd
  if (!(it.IsAtEnd() && itDecorated.IsAtEnd()))
  {
    return 3;
  }

  return 0;
}

// Multiplex to forward, reverse and bijection test
template <typename IteratorType, typename MaskIteratorType>
int TripleTest(typename IteratorType::ImageType::Pointer image,typename MaskIteratorType::ImageType::Pointer mask, typename IteratorType::ImageType::RegionType region)
{
  int ret;
  int retGlobal = EXIT_SUCCESS;

  ret = ForwardTest<IteratorType,MaskIteratorType>(mask, image, region);
  if (ret>0)
    {
    std::cout << "Forward(FAILED:"<<ret<<") ";
    retGlobal = EXIT_FAILURE;
    }

  ret = ReverseTest<IteratorType,MaskIteratorType>(mask, image, region);
  if (ret>0)
    {
    std::cout << "Reverse(FAILED:"<<ret<<") ";
    retGlobal = EXIT_FAILURE;
    }

  ret = BijectiveTest<IteratorType,MaskIteratorType>(mask, image, region);
  if (ret>0)
    {
    std::cout << "Bijective(FAILED:"<<ret<<") ";
    retGlobal = EXIT_FAILURE;
    }

  if (retGlobal == EXIT_SUCCESS)
    {
    std::cout << "PASSED";
    }
  return retGlobal;
}

// -------------------------- Nominal case -------------------------------------
int otbMaskedIteratorDecoratorNominal(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2> ImageType;
  ImageType::Pointer image = GetTestImage<ImageType>(10, 10);
  ImageType::Pointer mask = GetTestImage<ImageType>(10, 0);
  ImageType::RegionType region(image->GetLargestPossibleRegion());
  FillHalf<ImageType>(mask, region, 1);
  int ret;
  int retGlobal = EXIT_SUCCESS;

  std::cout << std::endl << "itk::ImageRegionIterator : ";
  ret = TripleTest< itk::ImageRegionIterator<ImageType>,
                    itk::ImageRegionIterator<ImageType> >(image, mask, region);
  retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);

  std::cout << std::endl << "itk::ImageRegionConstIterator : ";
  ret = TripleTest< itk::ImageRegionConstIterator<ImageType>,
                    itk::ImageRegionConstIterator<ImageType> >(image, mask, region);
  retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);
  
  // std::cout << std::endl << "otb::SubsampledImageRegionIterator : ";
  // ret = TripleTest< otb::SubsampledImageRegionIterator<ImageType>,
  //                   otb::SubsampledImageRegionIterator<ImageType> >(image, mask, region);
  // retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);

  return retGlobal;
}

// ------------------------ Degenerate cases -----------------------------------
int otbMaskedIteratorDecoratorDegenerate(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Fully masked (0 everywhere) and image is smaller than mask
  typedef otb::Image<double, 2> ImageType;
  ImageType::Pointer image = GetTestImage<ImageType>(9, 10);
  ImageType::Pointer mask = GetTestImage<ImageType>(100, 0);
  ImageType::RegionType region(image->GetLargestPossibleRegion());

  return TripleTest<itk::ImageRegionIterator<ImageType>,
                    itk::ImageRegionIterator<ImageType> >(image, mask, region);
}

// --------------------------- Extended cases ----------------------------------
int otbMaskedIteratorDecoratorExtended(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::VectorImage<double, 2> ImageType;
  typedef otb::Image<unsigned char, 2> MaskType;
  otb::VectorImage<double, 2>::PixelType pixel(3);
  pixel.Fill(12);
  ImageType::Pointer image = GetTestImage<ImageType>(10,pixel ,3);
  MaskType::Pointer mask = GetTestImage<MaskType>(10, 0);
  ImageType::RegionType region(image->GetLargestPossibleRegion());
  FillHalf<MaskType>(mask, region, 1);
  int ret;
  int retGlobal = EXIT_SUCCESS;

  std::cout << std::endl << "itk::ImageRegionIterator : ";
  ret = TripleTest< itk::ImageRegionIterator<ImageType>,
                    itk::ImageRegionIterator<MaskType> >(image, mask, region);
  retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);

  std::cout << std::endl << "itk::ImageRegionConstIterator : ";
  ret = TripleTest< itk::ImageRegionConstIterator<ImageType>,
                    itk::ImageRegionConstIterator<MaskType> >(image, mask, region);
  retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);

  std::cout << std::endl << "itk::ImageRegionIterator without mask: ";
  ret = TripleTest< itk::ImageRegionIterator<ImageType>,
                    itk::ImageRegionIterator<MaskType> >(image, NULL, region);
  retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);

  std::cout << std::endl << "itk::ImageRegionConstIterator without mask: ";
  ret = TripleTest< itk::ImageRegionConstIterator<ImageType>,
                    itk::ImageRegionConstIterator<MaskType> >(image, NULL, region);
  retGlobal = (ret == EXIT_FAILURE ? EXIT_FAILURE : retGlobal);

  return retGlobal;
}



/* Other iterators potentially compatible:

Different constructor arguments than (image, region)
itk::PathIterator
itk::LineIterator
itk::SliceIterator
itk::NeighborhoodIterator

Needs initialization code:
itk::ImageRandomConstIteratorWithIndex
itk::ImageRandomIteratorWithIndex
itk::ImageRandomNonRepeatingConstIteratorWithIndex
itk::ImageRandomNonRepeatingIteratorWithIndex

otb::SubsampledImageRegionIterator  : GoToEnd is buggy !

Different iteration interface than normal iterators:
itk::ImageScanlineIterator
itk::ImageScanlineConstIterator

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
