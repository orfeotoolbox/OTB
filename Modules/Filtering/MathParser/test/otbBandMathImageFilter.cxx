/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include "itkMacro.h"
#include <iostream>
#include <complex>  //only for the isnan() test line 148

#include "otbMath.h"
#include "otbImage.h"
#include "otbBandMathImageFilter.h"
#include "otbImageFileWriter.h"


int otbBandMathImageFilter( int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef double                                            PixelType;
  //typedef float                                             PixelType;
  typedef otb::Image<PixelType, 2>                          ImageType;
  typedef otb::BandMathImageFilter<ImageType>               FilterType;

  unsigned int i;
  const unsigned int N = 100;
  unsigned int FAIL_FLAG = 0;

  ImageType::SizeType size;
  size.Fill(N);
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  ImageType::Pointer image1 = ImageType::New();
  ImageType::Pointer image2 = ImageType::New();
  ImageType::Pointer image3 = ImageType::New();

  image1->SetLargestPossibleRegion( region );
  image1->SetBufferedRegion( region );
  image1->SetRequestedRegion( region );
  image1->Allocate();

  image2->SetLargestPossibleRegion( region );
  image2->SetBufferedRegion( region );
  image2->SetRequestedRegion( region );
  image2->Allocate();

  image3->SetLargestPossibleRegion( region );
  image3->SetBufferedRegion( region );
  image3->SetRequestedRegion( region );
  image3->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
  IteratorType it1(image1, region);
  IteratorType it2(image2, region);
  IteratorType it3(image3, region);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3)
  {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();

    it1.Set( i1[0] + i1[1] -50 );
    it2.Set( i2[0] * i2[1] );
    it3.Set( i3[0] + i3[1] * i3[1] );
  }


  FilterType::Pointer         filter       = FilterType::New();
  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;


  filter->SetNthInput(0, image1);
  filter->SetNthInput(1, image2);
  filter->SetNthInput(2, image3, "canal3");

  filter->SetExpression("cos(2 * pi * b1)/(2 * pi * b2 + 1E-3)*sin(pi * canal3) + ndvi(b1, b2) * sqrt(2) * canal3");
  filter->Update();

  std::cout << "\n---  Standard Use\n";
  std::cout << "Parsed Expression :   " << filter->GetExpression() << std::endl;

  ImageType::Pointer output = filter->GetOutput();
  IteratorType it(output, region);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(), it.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3, ++it)
    {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();
    PixelType px1 = ( i1[0] + i1[1] -50 );
    PixelType px2 = ( i2[0] * i2[1] );
    PixelType px3 = ( i3[0] + i3[1] * i3[1] );
    PixelType result = it.Get();
    PixelType ndvi_expected;
    PixelType error;

    if ( std::abs( px1 + px2) < 1E-6 )
      ndvi_expected = 0.0;
    else
      ndvi_expected = (px2-px1)/(px2+px1);

    PixelType expected = std::cos( 2 * otb::CONST_PI * px1 ) / ( 2 * otb::CONST_PI * px2 + 1E-3 ) * std::sin( otb::CONST_PI * px3 )
      + ndvi_expected * std::sqrt(PixelType(2)) * px3;

    /*
    std::cout << "Pixel_1 =  " << it1.Get() << "     Pixel_2 =  " << it2.Get() << "     Pixel_3 =  " << it3.Get()
        << "     Result =  " << it.Get() << "     Expected =  " << expected << std::endl;
    */

    error = (result - expected) * (result - expected) / (result + expected);
    if ( error > 1E-9 )
      {
      std::cout
         << "Error = " << error << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "Pixel_1 =  "       << it1.Get()
         << "     Pixel_2 =  "  << it2.Get()
         << "     Pixel_3 =  "  << it3.Get()
         << "     Result =  "   << it.Get()
         << "     Expected =  " << expected << std::endl;
      FAIL_FLAG++;
      break;
      }
    }


  /** Edge Effect Handling */
  std::cout << "\n--- Edge Effect Handling\n";
  std::cout << "- +/-inf section\n";
  filter->SetExpression("b1 / b2");
  filter->Update();

  std::cout << "- nan section\n";
  it1.GoToBegin(); it2.GoToBegin(); it.GoToBegin();
  for(i=1; i<=50; ++i , ++it1, ++it2, ++it){}
  if(vnl_math_isnan(it.Get()))
    std::cout << "Pixel_1 =  " << it1.Get() << "     Pixel_2 =  " << it2.Get() << "     Result =  " << it.Get() << "     Expected =  nan\n" << std::endl;
  else
    {
    std::cout
             << "\nError > Bad Edge Effect Handling -> Test Failled\n"
             << "Pixel_1 =  "     << it1.Get()  << "     Pixel_2 =  "  << it2.Get()
             << "     Result =  " << it.Get()   << "     Expected =  nan\n" << std::endl;
    FAIL_FLAG++;
    }

  if (FAIL_FLAG)
    {
    std::cout << "[FAILLED]" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "[PASSED]" << std::endl;
  return EXIT_SUCCESS;
}


int otbBandMathImageFilterWithIdx( int itkNotUsed(argc), char* argv[])
{
  const char * outfname1       = argv[1];
  const char * outfname2       = argv[2];

  typedef double                                            PixelType;
  //typedef float                                             PixelType;
  typedef otb::Image<PixelType, 2>                          ImageType;
  typedef otb::BandMathImageFilter<ImageType>               FilterType;
  typedef otb::ImageFileWriter<ImageType>          WriterType;

  const unsigned int N = 100;

  ImageType::SizeType size;
  size.Fill(N);
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);
  ImageType::PointType origin;
  origin[0] = -25;
  origin[1] = -25;
  ImageType::SpacingType spacing;
  spacing[0] = 0.5;
  spacing[1] = 0.5;

  ImageType::Pointer image1 = ImageType::New();
  ImageType::Pointer image2 = ImageType::New();
  ImageType::Pointer image3 = ImageType::New();

  image1->SetLargestPossibleRegion( region );
  image1->SetBufferedRegion( region );
  image1->SetRequestedRegion( region );
  image1->Allocate();

  image2->SetLargestPossibleRegion( region );
  image2->SetBufferedRegion( region );
  image2->SetRequestedRegion( region );
  image2->Allocate();

  image3->SetLargestPossibleRegion( region );
  image3->SetBufferedRegion( region );
  image3->SetRequestedRegion( region );
  image3->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
  IteratorType it1(image1, region);
  IteratorType it2(image2, region);
  IteratorType it3(image3, region);

  image1->SetOrigin(origin);
  image1->SetSignedSpacing(spacing);
  image2->SetOrigin(origin);
  image2->SetSignedSpacing(spacing);
  image3->SetOrigin(origin);
  image3->SetSignedSpacing(spacing);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3)
  {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();

    it1.Set( i1[0] + i1[1] -50 );
    it2.Set( i2[0] * i2[1] );
    it3.Set( i3[0] + i3[1] * i3[1] );
  }


  FilterType::Pointer         filter       = FilterType::New();
  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;


  filter->SetNthInput(0, image1);
  filter->SetNthInput(1, image2);
  filter->SetNthInput(2, image3);

  #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  filter->SetExpression("(sqrt(idxX*idxX+idxY*idxY) < 50) ? b2 : b3");
  #else
  filter->SetExpression("if(sqrt(idxX*idxX+idxY*idxY) < 50, b2, b3)");
  #endif
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname1);
  writer->Update();

  #ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  filter->SetExpression("(sqrt(idxPhyX*idxPhyX+idxPhyY*idxPhyY) < 25) ? b2 : b3");
  #else
  filter->SetExpression("if(sqrt(idxPhyX*idxPhyX+idxPhyY*idxPhyY) < 25, b2, b3)");
  #endif
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(filter->GetOutput());
  writer2->SetFileName(outfname2);
  writer2->Update();

  return EXIT_SUCCESS;
}
