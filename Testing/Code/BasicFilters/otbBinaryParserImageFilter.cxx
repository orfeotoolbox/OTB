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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include <iostream>

#include "otbMath.h"
#include "otbImage.h"
#include "otbBinaryParserImageFilter.h"

int otbBinaryParserImageFilter( int argc, char* argv[])
{
  typedef double                                            PixelType;
  typedef otb::Image<PixelType, 2>                          ImageType;
  typedef otb::BinaryParserImageFilter<ImageType>           FilterType;
  
  unsigned int i;
  const unsigned int N = 100;

  ImageType::SizeType size;
  size.Fill(N);
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  ImageType::Pointer image1 = ImageType::New();
  ImageType::Pointer image2 = ImageType::New();

  image1->SetLargestPossibleRegion( region );
  image1->SetBufferedRegion( region );
  image1->SetRequestedRegion( region );
  image1->Allocate();

  image2->SetLargestPossibleRegion( region );
  image2->SetBufferedRegion( region );
  image2->SetRequestedRegion( region );
  image2->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
  IteratorType it1(image1, region);
  IteratorType it2(image2, region);

  for (it1.GoToBegin(), it2.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2)
  {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();

    it1.Set( i1[0] + i1[1] -50 );
    it2.Set( i2[0] * i2[1] );
  }


  FilterType::Pointer         filter       = FilterType::New();
  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;

  filter->SetInput1(image1);
  filter->SetInput2(image2);

  filter->SetExpression("cos(2 * pi * Image1)/(2 * pi * Image2 + 1E-3) + ndvi(Image1,Image2)*sqrt(2)");
  filter->Update();

  std::cout << "---  Standard Use"                                 << std::endl;
  std::cout << "Parsed Expression :   " << filter->GetExpression() << std::endl;
  std::cout << "About the Filter  :   " << filter                  << std::endl;
 
  ImageType::Pointer output = filter->GetOutput();
  IteratorType it(output, region);

  for (it1.GoToBegin(), it2.GoToBegin(), it.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it)
    {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    double px1 = ( i1[0] + i1[1] -50 );
    double px2 = ( i2[0] * i2[1] );
    double result = it.Get();
    double ndvi_expected;
    
    if ( vcl_abs( px1 + px2) < 1E-6 )
      ndvi_expected = 0.0;
    else 
      ndvi_expected = (px2-px1)/(px2+px1);
    
    double expected = vcl_cos( 2 * otb::CONST_PI * px1 ) / ( 2 * otb::CONST_PI * px2 + 1E-3 )
      + ndvi_expected * vcl_sqrt(2);
    
    /*
    std::cout << "Pixel_1 =  " << it1.Get() << "     Pixel_2 =  " << it2.Get() 
	      << "     Result =  " << it.Get() << "     Expected =  " << expected << std::endl;  
    */
    
    if ( vcl_abs( result - expected ) > 1E-10 )
      {
      itkGenericExceptionMacro(  << "Error > 1E-10 -> Test Failled" << std::endl 
				 << "Pixel_1 =  "     << it1.Get()  << "     Pixel_2 =  "  << it2.Get() 
				 << "     Result =  " << it.Get()   << "     Expected =  " << expected 
				 << std::endl );
      }
  }
  std::cout << "Process Completed" << std::endl;

  /** Edge Effect Handling */
  
  std::cout << "--- Edge Effect Handling" << std::endl;
  std::cout << "- +/-inf section"         << std::endl;
  filter->SetExpression("Image1 / Image2");
  filter->Update();

  std::cout << "- nan section" << std::endl;
  it1.GoToBegin(); it2.GoToBegin(); it.GoToBegin();
  for(i=1; i<=50; i++ , ++it1, ++it2, ++it){}
  if(isnan(it.Get()))
    std::cout << "Pixel_1 =  " << it1.Get() << "     Pixel_2 =  " << it2.Get() << "     Result =  " << it.Get() << "     Expected =  nan" << std::endl;
  else
    itkGenericExceptionMacro(  << "Error > Bad Edge Effect Handling -> Test Failled" << std::endl 
			       << "Pixel_1 =  "     << it1.Get()  << "     Pixel_2 =  "  << it2.Get() 
			       << "     Result =  " << it.Get()   << "     Expected =  nan" << std::endl );
 
  return EXIT_SUCCESS;
}
