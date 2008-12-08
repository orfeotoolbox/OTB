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
#include "itkExceptionObject.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkRandomImageSource.h"
#include "otbChangeLabelImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

int otbChangeLabelImageFilterTest(int argc, char * argv[])
{
  const unsigned int ImageDimension = 3;

  typedef otb::Image<unsigned short, ImageDimension>                   InputImageType;
  typedef otb::VectorImage<unsigned char, ImageDimension>              OutputImageType;
  typedef InputImageType::PixelType                                    InputPixelType;
  typedef OutputImageType::PixelType                                   OutputPixelType;
  typedef itk::ImageRegionIteratorWithIndex<InputImageType>            InputIteratorType;
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType>           OutputIteratorType;
  typedef itk::RandomImageSource<InputImageType>                       SourceType;
  typedef otb::ChangeLabelImageFilter<InputImageType, OutputImageType> FilterType;

  SourceType::Pointer source = SourceType::New();
  FilterType::Pointer filter = FilterType::New();
  InputImageType::Pointer  vectImage  = InputImageType::New();

  unsigned long sizeArray[ImageDimension] = { 3,3,3 };

  InputPixelType upper = 10;
  source->SetMin( itk::NumericTraits<InputPixelType>::Zero );
  source->SetMax( upper );
  source->SetSize( sizeArray );
  source->SetNumberOfThreads(1);

  filter->SetNumberOfComponentsPerPixel(5);

  // Eliminate most labels
  OutputPixelType background;
  background.SetSize( filter->GetNumberOfComponentsPerPixel() );
  background.Fill(1);
  InputPixelType maxRemainingLabel = 2;
  for (InputPixelType i = maxRemainingLabel; i <= upper; i++)
    {
      filter->SetChange( i, background );
    }

  filter->Print( std::cout );
  filter->SetInput( source->GetOutput() );

  OutputImageType::Pointer outputImage = filter->GetOutput();

  // Execute the filter
  try
  {
    filter->Update();
    filter->SetFunctor(filter->GetFunctor());
  }
  catch(...)
  {
    std::cerr << "Caught an unexpected exception. " << std::endl;
    std::cerr << "Test failed. " << std::endl;
    return EXIT_FAILURE;
  }

  // Create an iterator for going through the image output
  InputIteratorType  it( source->GetOutput(), source->GetOutput()->GetRequestedRegion() );
  OutputIteratorType ot(outputImage, outputImage->GetRequestedRegion());

  bool pass = true;

  //  Check the content of the result image
  std::cout << "Verification of the output " << std::endl;

  ot.GoToBegin();
  it.GoToBegin();
  while( !ot.IsAtEnd() )
  {
    const InputPixelType  input  = it.Get();
    const OutputPixelType output = ot.Get();
    std::cout <<  (double) input<<": ";
    for(unsigned int j=0; j<filter->GetNumberOfComponentsPerPixel(); j++)
      {
	std::cout<< " " << (double)output[j];
	if( (double)output[j] > (double)maxRemainingLabel )
	  {
	    pass = false;
	  }
      }
    std::cout<<std::endl;

    if ( !pass )
      {
        std::cerr << "Error in otbChangeLabelImageFilterTest " << std::endl;
        std::cerr << " input = " << input;
        std::cerr << " output = " << output;
        std::cerr << std::endl;

        return EXIT_FAILURE;
      }

    ++ot;
    ++it;
  }

  return EXIT_SUCCESS;
}
