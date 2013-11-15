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
#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"
#include <vector>
#include <algorithm>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "itkImageRegionConstIterator.h"
#include <sstream>

int computeRules(double TM1, double TM2, double TM3, double TM4, double TM5, double TM61, double TM62, double TM7)
{
  typedef double PrecisionType;
  typedef itk::FixedArray< PrecisionType, 8 >     InputPixelType;
  typedef unsigned char OutputPixelType;

  InputPixelType pixel;
  pixel[0] = TM1;
  pixel[1] = TM2;
  pixel[2] = TM3;
  pixel[3] = TM4;
  pixel[4] = TM5;
  pixel[5] = TM61;
  pixel[6] = TM62;
  pixel[7] = TM7;

  std::vector< PrecisionType > v13;
  v13.push_back(TM1);
  v13.push_back(TM3);

  PrecisionType max13 = *(max_element ( v13.begin(), v13.end() ));

  std::vector< PrecisionType > v123;
  v123.push_back(TM1);
  v123.push_back(TM2);
  v123.push_back(TM3);

  PrecisionType max123 = *(max_element ( v123.begin(), v123.end() ));
  PrecisionType min123 = *(min_element ( v123.begin(), v123.end() ));

  std::vector< PrecisionType > v12347;
  v12347.push_back(TM1);
  v12347.push_back(TM2);
  v12347.push_back(TM3);
  v12347.push_back(TM4);
  v12347.push_back(TM7);

  PrecisionType max12347 = *(max_element ( v12347.begin(), v12347.end() ));
  PrecisionType min12347 = *(min_element ( v12347.begin(), v12347.end() ));

  std::vector< PrecisionType > v234;
  v234.push_back(TM2);
  v234.push_back(TM3);
  v234.push_back(TM4);

  PrecisionType max234 = *(max_element ( v234.begin(), v234.end() ));


  std::vector< PrecisionType > v45;
  v45.push_back(TM4);
  v45.push_back(TM5);

  PrecisionType max45 = *(max_element ( v45.begin(), v45.end() ));

  PrecisionType TV1 = 0.7;
  PrecisionType TV2 = 0.5;

  typedef otb::Functor::LandsatTM::ThickCloudsSpectralRule<InputPixelType, OutputPixelType> R1FunctorType;
  R1FunctorType r1Funct = R1FunctorType();
  OutputPixelType result = r1Funct(pixel);
  OutputPixelType goodResult = static_cast<OutputPixelType>(
    ( ((min123 >= (TV1 * max123))
      && (max123 <= TV1 * TM4))
     || ((TM2 >= TV1 * max13)
         && (max123 <= TM4)))
    && (TM5 <= TV1 * TM4)
    && (TM5 >= TV1 * max123)
    && (TM7 <= TV1 * TM4));

  if( result!=goodResult )
    {
    std::cerr << "Rule 1 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::ThinCloudsSpectralRule<InputPixelType, OutputPixelType> R2FunctorType;
  R2FunctorType r2Funct = R2FunctorType();
  result = r2Funct(pixel);
  goodResult = static_cast<OutputPixelType>((min123 >= (TV1 * max123))
                                            && (TM4 >= max123)
                                            && !(TM1<=TM2 && TM2<=TM3 && TM3<=TM4 && TM3 >= TV1*TM4)
                                            && (TM4 >= TV1*TM5) && (TM5 >= TV1*TM4)
                                            && (TM5 >= TV1*max123) && (TM5 >= TV1*TM7));


  if( result!=goodResult )
    {
      std::cerr << "Rule 2 " << goodResult << " " << result << std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::SnowOrIceSpectralRule<InputPixelType, OutputPixelType> R3FunctorType;
  R3FunctorType r3Funct = R3FunctorType();
  result = r3Funct(pixel);
  goodResult = static_cast<OutputPixelType>((min123 >= (TV1 * max123))
                                            && (TM4 >= TV1 * max123)
                                            && (TM5 <= TV2 * TM4)
                                            && (TM5 <= TV1* min123)
                                            && (TM7 <= TV2 * TM4)
                                            && (TM7 <= TV1*min123));


  if( result!=goodResult )
    {
      std::cerr << "Rule 3 " << goodResult << " " << result << std::endl;
      return EXIT_FAILURE;
    }


  typedef otb::Functor::LandsatTM::WaterOrShadowSpectralRule<InputPixelType, OutputPixelType> R4FunctorType;
  R4FunctorType r4Funct = R4FunctorType();
  result = r4Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM1 >= TM2) && (TM2 >= TM3) && (TM3 >= TM4) && (TM4 >= TM5) && (TM4 >= TM7));


  if( result!=goodResult )
    {
    std::cerr << "Rule 4 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }


  typedef otb::Functor::LandsatTM::PitbogOrGreenhouseSpectralRule<InputPixelType, OutputPixelType> R5FunctorType;
  R5FunctorType r5Funct = R5FunctorType();
  result = r5Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM3 >= TV1 * TM1)
                                            && (TM1 >= TV1 * TM3)
                                            && (max123 <= TV1 * TM4)
                                            && (TM5 <= TV1 * TM4)
                                            && (TM3 >= TV2 * TM5)
                                            && (min123 >= TV1 * TM7));


  if( result!=goodResult )
    {
      std::cerr << "Rule 5 " << goodResult << " " << result << std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::DominantBlueSpectralRule<InputPixelType, OutputPixelType> R6FunctorType;
  R6FunctorType r6Funct = R6FunctorType();
  result = r6Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM1 >= TV1 * TM2)
                                            && (TM1 >= TV1 * TM3)
                                            && (TM1 >= TV1 * TM4)
                                            && (TM1 >= TV1 * TM5)
                                            && (TM1 >= TV1 * TM7));


  if( result!=goodResult )
    {
    std::cerr << "Rule 6 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::VegetationSpectralRule<InputPixelType, OutputPixelType> R7FunctorType;
  R7FunctorType r7Funct = R7FunctorType();
  result = r7Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM2 >= TV2 * TM1)
                                            && (TM2 >= TV1 * TM3)
                                            && (TM3 < TV1 * TM4)
                                            && (TM4 > max123)
                                            && (TM5 < TV1 * TM4)
                                            && (TM5 >= TV1 * TM3)
                                            && (TM7 < TV1 * TM5));


  if( result!=goodResult )
    {
      std::cerr << "Rule 7 " << goodResult << " " << result << std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::RangelandSpectralRule<InputPixelType, OutputPixelType> R8FunctorType;
  R8FunctorType r8Funct = R8FunctorType();
  result = r8Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM2 >= TV2 * TM1)
                                            && (TM2 >= TV1 * TM3)
                                            && (TM4 > max123)
                                            && (TM3 < TV1 * TM4)
                                            && (TM4 >= TV1 * TM5)
                                            && (TM5 >= TV1 * TM4)
                                            && (TM5 > max123)
                                            && (TM7 < TV1 * max45)
                                            && (TM5 >= TM7));


  if( result!=goodResult )
    {
    std::cerr << "Rule 8 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::BarrenLandOrBuiltUpOrCloudsSpectralRule<InputPixelType, OutputPixelType> R9FunctorType;
  R9FunctorType r9Funct = R9FunctorType();
  result = r9Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM3 >= TV2 * TM1)
                                            && (TM3 >= TV1 * TM2)
                                            && (TM4 >= TV1 * max123)
                                            && (TM5 >= max123)
                                            && (TM5 >= TV1 * TM4)
                                            && (TM5 >= TV1 * TM7)
                                            && (TM7 >= TV2 * max45));


  if( result!=goodResult )
    {
      std::cerr << "Rule 9 " << goodResult << " " << result << std::endl;
      std::cerr << TM1 << " " << TM2 << " " << TM3 << " " << TM4 << " " << TM5 << " " << TM7 << std::endl;
      std::cerr << max123 << " " << min123 << " " << max45 << " " << max234 << " " << min12347 << " " << max12347 << std::endl;
      return EXIT_FAILURE;
    }


  typedef otb::Functor::LandsatTM::FlatResponseBarrenLandOrBuiltUpSpectralRule<InputPixelType, OutputPixelType> R10FunctorType;
  R10FunctorType r10Funct = R10FunctorType();
  result = r10Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM5 >= TV1 * max12347)
                                            && (min12347 >= TV2 * TM5));


  if( result!=goodResult )
    {
    std::cerr << "Rule 10 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }


  typedef otb::Functor::LandsatTM::ShadowWithBarrenLandSpectralRule<InputPixelType, OutputPixelType> R11FunctorType;
  R11FunctorType r11Funct = R11FunctorType();
  result = r11Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM1 >= TM2 )
                                            && (TM2 >= TM3)
                                            && (TM3 >= TV1 * TM4)
                                            && (TM1 >= TM5)
                                            && (TM5 >= TV1 * TM4)
                                            && (TM5 >= TV2 * TM7));


  if( result!=goodResult )
    {
      std::cerr << "Rule 11 " << goodResult << " " << result << std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::ShadowWithVegetationSpectralRule<InputPixelType, OutputPixelType> R12FunctorType;
  R12FunctorType r12Funct = R12FunctorType();
  result = r12Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM1 >= TM2)
                                            && (TM2 >= TM3)
                                            && (TM1 >= TV2 * TM4)
                                            && (TM3 < TV1 * TM4)
                                            && (TM5 < TV1 * TM4)
                                            && (TM3 >= TV2 * TM5)
                                            && (TM7 < TV1 * TM4));


  if( result!=goodResult )
    {
    std::cerr << "Rule 12 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::ShadowCloudOrSnowSpectralRule<InputPixelType, OutputPixelType> R13FunctorType;
  R13FunctorType r13Funct = R13FunctorType();
  result = r13Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM1 >= TV1 * max234)
                                            && (max234 >= TV1 * TM1)
                                            && (TM5 < TM1)
                                            && (TM7 < TV1 * TM1));


  if( result!=goodResult )
    {
      std::cerr << "Rule 13 " << goodResult << " " << result << std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::Functor::LandsatTM::WetlandSpectralRule<InputPixelType, OutputPixelType> R14FunctorType;
  R14FunctorType r14Funct = R14FunctorType();
  result = r14Funct(pixel);
  goodResult = static_cast<OutputPixelType>((TM1 >= TM2)
                                            && (TM2 >= TM3)
                                            && (TM1 >= TV1 * TM4)
                                            && (TM3 < TM4)
                                            && (TM4 >= TV1 * TM5)
                                            && (TM5 >= TV1 * TM4)
                                            && (TM3 >= TV2 * TM5)
                                            && (TM5 >= TM7));


  if( result!=goodResult )
    {
    std::cerr << "Rule 14 " << goodResult << " " << result << std::endl;
    return EXIT_FAILURE;
    }




  return EXIT_SUCCESS;

}

int otbLandsatTMKernelSpectralRules(int argc, char * argv[])
{
  double TM1 = (::atof(argv[1]));
  double TM2 = (::atof(argv[2]));
  double TM3 = (::atof(argv[3]));
  double TM4 = (::atof(argv[4]));
  double TM5 = (::atof(argv[5]));
  double TM61 = (::atof(argv[6]));
  double TM62 = (::atof(argv[7]));
  double TM7 = (::atof(argv[8]));

  return computeRules(TM1, TM2, TM3, TM4, TM5, TM61, TM62, TM7);

}


int otbLandsatTMKernelSpectralRulesWithImage(int argc, char * argv[])
{

  typedef double InputPixelType;

  typedef otb::VectorImage< InputPixelType, 2 > InputImageType;

  typedef otb::ImageFileReader< InputImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::ImageRegionConstIterator< InputImageType > IteratorType;

  IteratorType it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  it.GoToBegin();


  if( it.Get().Size() != 7 && it.Get().Size() != 8)
    {
    std::cerr << " Image must have either 7 or 8 bands " << std::endl;
    return EXIT_FAILURE;
    }

  while(! it.IsAtEnd() )
    {

    InputImageType::PixelType pix = it.Get();

    int returnCode = EXIT_FAILURE;

    if( pix.Size() == 7 )
      {
      returnCode = computeRules(pix[0], pix[1], pix[2], pix[3], pix[4], pix[5], pix[5], pix[6]);
      }
    if( pix.Size() == 8 )
      {
      returnCode = computeRules(pix[0], pix[1], pix[2], pix[3], pix[4], pix[5], pix[6], pix[7]);
      }

    if( returnCode == EXIT_FAILURE )
      {
      return EXIT_FAILURE;
      }

    ++it;
    }



  return EXIT_SUCCESS;
}



