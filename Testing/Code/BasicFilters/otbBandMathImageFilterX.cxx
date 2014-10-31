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
#include <iostream>
#include <complex>  //only for the isnan() test line 148

#include "otbMath.h"
#include "otbVectorImage.h"
#include "otbBandMathImageFilterX.h"
#include "otbImageFileWriter.h"

int otbBandMathImageFilterXNew( int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef double                                            PixelType;
  typedef otb::VectorImage<PixelType, 2>                     ImageType;
  typedef otb::BandMathImageFilterX<ImageType>               FilterType;

   FilterType::Pointer         filter       = FilterType::New();

   return EXIT_SUCCESS;
}


int otbBandMathImageFilterX( int itkNotUsed(argc), char* itkNotUsed(argv) [])
{

  typedef otb::VectorImage<double, 2>              ImageType;
  typedef ImageType::PixelType                      PixelType;
  typedef otb::BandMathImageFilterX<ImageType>      FilterType;

  unsigned int i;
  const unsigned int N = 100, D1=3, D2=1, D3=1;
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
  image1->SetNumberOfComponentsPerPixel(D1);
  image1->Allocate();

  image2->SetLargestPossibleRegion( region );
  image2->SetBufferedRegion( region );
  image2->SetRequestedRegion( region );
  image2->SetNumberOfComponentsPerPixel(D2);
  image2->Allocate();

  image3->SetLargestPossibleRegion( region );
  image3->SetBufferedRegion( region );
  image3->SetRequestedRegion( region );
  image3->SetNumberOfComponentsPerPixel(D3);
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

    it1.Get()[0] = i1[0] + i1[1] -50; it1.Get()[1] = i1[0] * i1[1] -50; it1.Get()[2] = i1[0] / (i1[1]+1)+5;
    it2.Get()[0] = i2[0] * i2[1];
    it3.Get()[0] = i3[0] + i3[1] * i3[1];

  }


  FilterType::Pointer         filter       = FilterType::New();
  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;


  filter->SetNthInput(0, image1);
  filter->SetNthInput(1, image2);
  filter->SetNthInput(2, image3, "canal3");

  filter->SetExpression("vcos(2 * pi * im1) div (2 * pi * bands(im2,{1,1,1})) mult vsin(pi * bands(canal3,{1,1,1}))"); //Sub-test 1
  filter->SetExpression("im1b1 / im2b1"); //Sub-test 2 (Edge Effect Handling)
  filter->Update();

  //if (filter->GetNumberOfOutputs() != 2)

  ImageType::Pointer output1 = filter->GetOutput(0);
  ImageType::Pointer output2 = filter->GetOutput(1);

  std::cout << "\n---  Standard Use\n";
  std::cout << "Parsed Expression :   " << filter->GetExpression(0) << std::endl;



  //Sub-test 1
  IteratorType itoutput1(output1, region);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(), itoutput1.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3, ++itoutput1)
    {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();
    PixelType px1(D1),px2(D2),px3(D3);

    px1[0] = ( i1[0] + i1[1] -50 ); px1[1] = ( i1[0] * i1[1] -50 ); px1[2] = ( i1[0] / (i1[1]+1)+5 );
    px2[0] = ( i2[0] * i2[1] );
    px3[0] = ( i3[0] + i3[1] * i3[1] );

    double result1 = itoutput1.Get()[0], result2 = itoutput1.Get()[1], result3 = itoutput1.Get()[2];
    double error1,error2,error3;


    double expected1 = vcl_cos( 2 * otb::CONST_PI * px1[0] ) / ( 2 * otb::CONST_PI * px2[0] ) * vcl_sin( otb::CONST_PI * px3[0] );
    double expected2 = vcl_cos( 2 * otb::CONST_PI * px1[1] ) / ( 2 * otb::CONST_PI * px2[0] ) * vcl_sin( otb::CONST_PI * px3[0] );
    double expected3 = vcl_cos( 2 * otb::CONST_PI * px1[2] ) / ( 2 * otb::CONST_PI * px2[0] ) * vcl_sin( otb::CONST_PI * px3[0] );    

    /*std::cout << "Pixel_1 =  " << it1.Get()[0] << "     Pixel_2 =  " << it2.Get()[0] << "     Pixel_3 =  " << it3.Get()[0]
        << "     Result =  " << itoutput1.Get()[0] << "     Expected =  " << expected1 << std::endl;*/
    

    error1 = (result1 - expected1) * (result1 - expected1) / (result1 + expected1);
    error2 = (result2 - expected2) * (result2 - expected2) / (result2 + expected2);
    error3 = (result3 - expected3) * (result3 - expected3) / (result3 + expected3);

    if ( ( error1 > 1E-9 ) || ( error2 > 1E-9 ) || ( error3 > 1E-9 ))
      {
      itkGenericExceptionMacro(  <<std::endl
<< "Error = " << error1 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "Pixel_1 =  "       << it1.Get()[0]
         << "     Pixel_2 =  "  << it2.Get()[0]
         << "     Pixel_3 =  "  << it3.Get()[0]
         << "     Result =  "   << result1
         << "     Expected =  " << expected1     << std::endl 
<< "Error = " << error2 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "Pixel_1 =  "       << it1.Get()[1]
         << "     Pixel_2 =  "  << it2.Get()[0]
         << "     Pixel_3 =  "  << it3.Get()[0]
         << "     Result =  "   << result2
         << "     Expected =  " << expected2     << std::endl
<< "Error = " << error3 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "Pixel_1 =  "       << it1.Get()[2]
         << "     Pixel_2 =  "  << it2.Get()[0]
         << "     Pixel_3 =  "  << it3.Get()[0]
         << "     Result =  "   << result3
         << "     Expected =  " << expected3     << std::endl);
      }
  }

  //Sub-test 2
  /** Edge Effect Handling */

  IteratorType itoutput2(output2, region);
  std::cout << "\n--- Edge Effect Handling\n";
  std::cout << "- +/-inf section\n";

  std::cout << "- nan section\n";
  it1.GoToBegin(); it2.GoToBegin(); itoutput2.GoToBegin();
  for(i=1; i<=50; ++i , ++it1, ++it2, ++itoutput2){}
  if(vnl_math_isnan(itoutput2.Get()[0]))
    std::cout << "Pixel_1 =  " << it1.Get() << "     Pixel_2 =  " << it2.Get() << "     Result =  " << itoutput2.Get() << "     Expected =  nan\n";
  else
    itkGenericExceptionMacro(
             << "\nError > Bad Edge Effect Handling -> Test Failled\n"
             << "Pixel_1 =  "     << it1.Get()  << "     Pixel_2 =  "  << it2.Get()
             << "     Result =  " << itoutput2.Get()   << "     Expected =  nan\n" );
  std::cout << std::endl; 


  return EXIT_SUCCESS;
}


int otbBandMathImageFilterXConv( int itkNotUsed(argc), char* itkNotUsed(argv) [])
{

  typedef otb::VectorImage<double, 2>              ImageType;
  typedef ImageType::PixelType                      PixelType;
  typedef otb::BandMathImageFilterX<ImageType>      FilterType;

  unsigned int i;
  const unsigned int N = 100, D1=3, D2=1, D3=1;
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
  image1->SetNumberOfComponentsPerPixel(D1);
  image1->Allocate();

  image2->SetLargestPossibleRegion( region );
  image2->SetBufferedRegion( region );
  image2->SetRequestedRegion( region );
  image2->SetNumberOfComponentsPerPixel(D2);
  image2->Allocate();

  image3->SetLargestPossibleRegion( region );
  image3->SetBufferedRegion( region );
  image3->SetRequestedRegion( region );
  image3->SetNumberOfComponentsPerPixel(D3);
  image3->Allocate();

  typedef itk::ConstNeighborhoodIterator<ImageType> IteratorType;
  IteratorType::RadiusType radius;
  radius[0]=1; // Size x direction
  radius[1]=2; // Size y direction

  IteratorType it1(radius, image1, region);
  IteratorType it2(radius, image2, region);
  IteratorType it3(radius, image3, region);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3)
  {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();

    it1.GetCenterPixel()[0] = i1[0] + i1[1] -50; it1.GetCenterPixel()[1] = i1[0] * i1[1] -50; it1.GetCenterPixel()[2] = i1[0] / (i1[1]+1)+5;
    it2.GetCenterPixel()[0] = i2[0] * i2[1];
    it3.GetCenterPixel()[0] = i3[0] + i3[1] * i3[1];

  }


  FilterType::Pointer         filter       = FilterType::New();
  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;


  filter->SetNthInput(0, image1,"imageA");
  filter->SetNthInput(1, image2);
  filter->SetNthInput(2, image3, "canal3");
  filter->SetMatrix("kernel1","{ 0.1 , 0.2 , 0.3 ; 0.4 , 0.5 , 0.6 ; 0.7 , 0.8 , 0.9 ; 1.0 , 1.1 , 1.2 ; 1.3 , 1.4 , 1.5 }"); 
  filter->SetExpression("conv(kernel1,imageAb1N3x5,imageAb2N3x5) | im2b1^1.1 | vcos(canal3)"); 
  filter->Update();

  if (filter->GetNumberOfOutputs() != 1)
    itkGenericExceptionMacro(<< "Wrong number of outputs.");
  

  ImageType::Pointer output1 = filter->GetOutput(0);

  if (output1->GetNumberOfComponentsPerPixel() != 4)
    itkGenericExceptionMacro(<< "Wrong number of components per pixel.");

  std::cout << "\n---  Standard Use\n";
  std::cout << "Parsed Expression :   " << filter->GetExpression(0) << std::endl;



  //Sub-test 1
  IteratorType itoutput1(radius, output1, region);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(), itoutput1.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3, ++itoutput1)
    {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();
    PixelType px1(output1->GetNumberOfComponentsPerPixel());

    float coefs[15] = { 0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.0 , 1.1 , 1.2 , 1.3 , 1.4 , 1.5};

    px1[0]=0; 
    for(int i=0; i<it1.Size(); ++i)
        px1[0] += coefs[i]*it1.GetPixel(i)[0]; 
      

    px1[1]=0; 
    for(int i=0; i<it1.Size(); ++i)
        px1[1] += coefs[i]*it1.GetPixel(i)[1]; 

    px1[2]= vcl_pow(it2.GetCenterPixel()[0],1.1);  

    px1[3]= vcl_cos(it3.GetCenterPixel()[0]); 


    double result1 = itoutput1.GetCenterPixel()[0], result2 = itoutput1.GetCenterPixel()[1], result3 = itoutput1.GetCenterPixel()[2], result4 = itoutput1.GetCenterPixel()[3];
    double error1,error2,error3,error4; 

    double expected1 = px1[0], expected2 = px1[1], expected3 = px1[2], expected4 = px1[3];

    error1 = (result1 - expected1) * (result1 - expected1) / (result1 + expected1);
    error2 = (result2 - expected2) * (result2 - expected2) / (result2 + expected2);
    error3 = (result3 - expected3) * (result3 - expected3) / (result3 + expected3);
    error4 = (result4 - expected4) * (result4 - expected4) / (result4 + expected4);

    if ( ( error1 > 1E-9 ) || ( error2 > 1E-9 ) || ( error3 > 1E-9 ) || ( error4 > 1E-9 ))
      {      
      itkGenericExceptionMacro(  
         << "Error1 = " << error1 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "     Result1 =  "   << result1
         << "     Expected1 =  " << expected1     << std::endl
         << "Error2 = " << error2 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "     Result2 =  "   << result2
         << "     Expected2 =  " << expected2     << std::endl
         << "Error3 = " << error3 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "     Result3 =  "   << result3
         << "     Expected3 =  " << expected3     << std::endl
         << "Error4 = " << error4 << "  > 1E-9     -> TEST FAILLED" << std::endl
         << "     Result4 =  "   << result4
         << "     Expected4 =  " << expected4     << std::endl); 
      }
  }

  return EXIT_SUCCESS;
}


int otbBandMathImageFilterXWithIdx( int itkNotUsed(argc), char* argv[])
{
  const char * outfname1       = argv[1];
  const char * outfname2       = argv[2];

  
  typedef otb::VectorImage<double, 2>                          ImageType;
  typedef ImageType::PixelType                            PixelType;
  typedef otb::BandMathImageFilterX<ImageType>            FilterType;
  typedef otb::ImageFileWriter<ImageType>                 WriterType;

  const unsigned int N = 100, D1=1, D2=1, D3=1;

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
  image1->SetNumberOfComponentsPerPixel(D1);
  image1->Allocate();

  image2->SetLargestPossibleRegion( region );
  image2->SetBufferedRegion( region );
  image2->SetRequestedRegion( region );
  image2->SetNumberOfComponentsPerPixel(D2);
  image2->Allocate();

  image3->SetLargestPossibleRegion( region );
  image3->SetBufferedRegion( region );
  image3->SetRequestedRegion( region );
  image3->SetNumberOfComponentsPerPixel(D3);
  image3->Allocate();

  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
  IteratorType it1(image1, region);
  IteratorType it2(image2, region);
  IteratorType it3(image3, region);

  image1->SetOrigin(origin);
  image1->SetSpacing(spacing);
  image2->SetOrigin(origin);
  image2->SetSpacing(spacing);
  image3->SetOrigin(origin);
  image3->SetSpacing(spacing);

  for (it1.GoToBegin(), it2.GoToBegin(), it3.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2, ++it3)
  {
    ImageType::IndexType i1 = it1.GetIndex();
    ImageType::IndexType i2 = it2.GetIndex();
    ImageType::IndexType i3 = it3.GetIndex();

    it1.Get()[0] = i1[0] + i1[1] -50;
    it2.Get()[0] = i2[0] * i2[1];
    it3.Get()[0] = i3[0] + i3[1] * i3[1];

  }


  FilterType::Pointer         filter       = FilterType::New();
  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;


  filter->SetNthInput(0, image1);
  filter->SetNthInput(1, image2);
  filter->SetNthInput(2, image3);

  filter->SetExpression("(sqrt(idxX*idxX+idxY*idxY) < 50) ? im1b1 : im2b1");
  filter->SetExpression("(sqrt(im2PhyX*im2PhyX+im2PhyY*im2PhyY) < 25) ? im2b1 : im3b1");

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput(0));
  writer->SetFileName(outfname1);
  writer->Update();
  
  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput(filter->GetOutput(1));
  writer2->SetFileName(outfname2);
  writer2->Update();

  return EXIT_SUCCESS;
}
