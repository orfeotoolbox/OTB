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
#include "otbGreyLevelCooccurrenceIndexedList.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"

int otbGreyLevelCooccurrenceIndexedList(int, char* [] )
{

  //Data definitions
  const unsigned int  IMGWIDTH         =  4;
  const unsigned int  IMGHEIGHT        =  4;
  const unsigned int  NDIMENSION       =  2;

  //------------------------------------------------------
  //Create a simple test images
  //------------------------------------------------------
  typedef unsigned char InputPixelType;
  typedef itk::Image<InputPixelType, NDIMENSION> InputImageType;


  typedef itk::ImageRegionIterator< InputImageType > InputImageIterator;
  typedef InputImageType::RegionType InputRegionType;

  InputImageType::Pointer image = InputImageType::New();

  InputImageType::SizeType inputImageSize = {{ IMGWIDTH, IMGHEIGHT }};

  InputImageType::IndexType index;
  index.Fill(0);
  InputRegionType region;

  region.SetSize( inputImageSize );
  region.SetIndex( index );

  image->SetRegions( region );
  image->Allocate();

  //--------------------------------------------------------------------------
  // Set up the image first. It looks like:
  //  1 2 1 2 1
  //  1 2 1 2 1
  //  1 2 1 2 1
  //  1 2 1 2 1
  //  1 2 1 2 1
  //--------------------------------------------------------------------------

  // setup the iterator
  InputImageIterator imageIt( image, image->GetBufferedRegion() );

  imageIt.GoToBegin();

  for(unsigned int i = 0; i < IMGWIDTH; i++)
    {
    for(unsigned int j = 0; j < IMGHEIGHT; j++, ++imageIt)
      {
      imageIt.Set(j % 2 + 1);
      }
    }

    typedef otb::GreyLevelCooccurrenceIndexedList< InputPixelType >   CooccurrenceIndexedListType;
    typedef CooccurrenceIndexedListType::FrequencyType  FrequencyType;
    typedef CooccurrenceIndexedListType::TotalFrequencyType  TotalFrequencyType;
    CooccurrenceIndexedListType::Pointer cooccurrenceObj1 = CooccurrenceIndexedListType::New();
    std::cout << "Class name = ";
    std::cout << cooccurrenceObj1->GetNameOfClass() << std::endl;

    bool passed = true;
    try
      {
      // Check Intialize method.
      cooccurrenceObj1->Initialize(4, 0, 8);
      bool symmetry = cooccurrenceObj1->GetSymmetry();
      float totalFrequency = cooccurrenceObj1->GetTotalFrequency();

      if( !symmetry || totalFrequency != 0)
        {
        std::cerr << "Error" << std::endl;
        std::cerr << "Expected symmetry = 1, totalFrequency = 0, got symmetry = "
                  << symmetry << " totalFrequency = " << totalFrequency <<  std::endl;
        passed = false;
        }
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "ExceptionObject caught !" << std::endl;
      std::cerr << err << std::endl;
      std::cerr << "Test failed" << std::endl;
      return EXIT_FAILURE;
      }

    static const FrequencyType out1[] = {12,6,12,12,12,12,12,6,12,6,12,12,12,12,12,6,12,6,12,12,12,12,12,6,8,4,8,8,8,8,8,4};
    std::vector<FrequencyType> avector (out1, out1 + sizeof(out1) / sizeof(out1[0]) );

    static const FrequencyType out2[] = {18,24,24,18,18,24,24,18,18,24,24,18,12,16,16,12};
    std::vector< TotalFrequencyType> tavector(out2, out2 + sizeof(out2) / sizeof(out2[0]) );;

    InputImageType::OffsetType offset = {{0, 1}};

    //vector to save results
    std::vector< FrequencyType> fvector;
    std::vector< TotalFrequencyType> tfvector;

    InputImageType::SizeType neighborhoodRadius;

    unsigned int minRadius = 0;
    for ( unsigned int i = 0; i < offset.GetOffsetDimension(); i++ )
      {
      unsigned int distance = vcl_abs(offset[i]);
      if ( distance > minRadius )
        {
        minRadius = distance;
        }
      }
    neighborhoodRadius.Fill(minRadius);

    InputImageType::SizeType radius = {{ 2, 2 }};

    // Iterate over input image and create co-occurrence list
    itk::ImageRegionIteratorWithIndex<InputImageType> imageItWithIndex(image, region);
    imageItWithIndex.GoToBegin();
    while (!imageItWithIndex.IsAtEnd())
      {
      InputRegionType::IndexType inputIndex;
      InputRegionType::SizeType inputSize;
      for (unsigned int dim = 0; dim < InputImageType::ImageDimension; ++dim)
        {
        inputIndex[dim] = imageItWithIndex.GetIndex()[dim] - radius[dim];
        inputSize[dim] = 2 * radius[dim] + 1;
        }

      // Build the input  region
      InputRegionType inputRegion;
      inputRegion.SetIndex(inputIndex);
      inputRegion.SetSize(inputSize);
      inputRegion.Crop(image->GetRequestedRegion());

      CooccurrenceIndexedListType::Pointer cooccurrenceObj2 = CooccurrenceIndexedListType::New();
      cooccurrenceObj2->Initialize(8, 0, 3);

      typedef itk::ConstNeighborhoodIterator< InputImageType > NeighborhoodIteratorType;
      NeighborhoodIteratorType neighborIt;
      neighborIt = NeighborhoodIteratorType(neighborhoodRadius, image, inputRegion);
      for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
        {
        const InputPixelType centerPixelIntensity = neighborIt.GetCenterPixel();
        bool pixelInBounds;
        const InputPixelType pixelIntensity =  neighborIt.GetPixel(offset, pixelInBounds);
        if ( !pixelInBounds )
          {
          continue; // don't put a pixel in the co-occurrence list if it's out-of-bounds.
          }
        cooccurrenceObj2->AddPixelPair(centerPixelIntensity, pixelIntensity);
        }

      typedef CooccurrenceIndexedListType::VectorType     VectorType;
      VectorType vector;
      //get indexed list and total frequency
      vector = cooccurrenceObj2->GetVector();
      //save total frequency
      tfvector.push_back(cooccurrenceObj2->GetTotalFrequency());
      //save each vector to fvector so as to compare results later
      VectorType::const_iterator    it;
      for (it = vector.begin(); it != vector.end(); ++it)
        {
        fvector.push_back( (*it).second );
        }
      ++imageItWithIndex;
      }

    //Check size of fvector against the actual results
    if( fvector.size() != avector.size())
      {
      std::cerr << "Size of computed co-occurrence indexed list for entire image is: " << fvector.size()
                << " expected size is: " << avector.size() << std::endl;
      passed = false;
      }
    else
      {
      //check contents individually
      std::vector<FrequencyType>::const_iterator fit, ait;
      ait = avector.begin();
      fit = fvector.begin();
      while(ait != avector.end())
        {
        if ((*fit) != (*ait))
          {
          std::cerr << "Expected value: " << (*ait) << " got :" << (*fit) << std::endl;
          passed = false;
          break;
          }
        ++ait;        ++fit;
        }
      std::cerr << "Co-occurrence list frequencies are correct" << std::endl;
      }

    //Check size of tfvector against the tavector
    if( tfvector.size() != tavector.size())
      {
      std::cerr << "Size of computed co-occurrence indexed list for entire image is: " << tfvector.size()
                << " expected size is: " << tavector.size() << std::endl;
      passed = false;
      }
    else
      {
      //check contents individually
      std::vector<FrequencyType>::const_iterator fit, ait;
      ait = tavector.begin();
      fit = tfvector.begin();
      while(ait != tavector.end())
        {
        if ((*fit) != (*ait))
          {
          std::cerr << "Expected value: " << (*ait) << " got :" << (*fit) << std::endl;
          passed = false;
          break;
          }
        ++ait;        ++fit;
        }
      std::cerr << "Co-occurrence list total frequencies are correct" << std::endl;
      }

    if (!passed)
      {
      std::cerr << "Test failed" << std::endl;
      return EXIT_FAILURE;
      }
    else
      {
      std::cerr << "Test succeeded" << std::endl;
      return EXIT_SUCCESS;
      }
}
