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
#include <cstdlib>
#include <cmath>

#include "otbRGBAPixelConverter.h"

int otbRGBAPixelConverterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef unsigned char                                          PixelType0;
  typedef double                                                 PixelType1;
  typedef itk::RGBAPixel<unsigned char>                          PixelType2;
  typedef itk::RGBPixel<double>                                  PixelType3;

  typedef otb::RGBAPixelConverter<PixelType0, PixelType0>        ConverterType0;
  typedef otb::RGBAPixelConverter<PixelType1, PixelType1>        ConverterType1;
  typedef otb::RGBAPixelConverter<PixelType0, PixelType2>        ConverterType2;
  typedef otb::RGBAPixelConverter<PixelType0, PixelType3>        ConverterType3;

  // Instantiating object
  ConverterType0::Pointer converter0 = ConverterType0::New();
  ConverterType1::Pointer converter1 = ConverterType1::New();
  ConverterType2::Pointer converter2 = ConverterType2::New();
  ConverterType3::Pointer converter3 = ConverterType3::New();

  std::cout << converter0 << std::endl;
  std::cout << converter1 << std::endl;
  std::cout << converter2 << std::endl;
  std::cout << converter3 << std::endl;

  return EXIT_SUCCESS;
}

int otbRGBAPixelConverter(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef unsigned int                                           PixelType0;
  typedef double                                                 PixelType1;
  typedef itk::RGBAPixel<unsigned int>                           PixelType2;
  typedef itk::RGBPixel<double>                                  PixelType3;

  typedef otb::RGBAPixelConverter<PixelType0, PixelType0>        ConverterType0;
  typedef otb::RGBAPixelConverter<PixelType1, PixelType0>        ConverterType1;
  typedef otb::RGBAPixelConverter<PixelType0, PixelType2>        ConverterType2;
  typedef otb::RGBAPixelConverter<PixelType0, PixelType3>        ConverterType3;

  // Instantiating object
  ConverterType0::Pointer converter0 = ConverterType0::New();
  ConverterType1::Pointer converter1 = ConverterType1::New();
  ConverterType2::Pointer converter2 = ConverterType2::New();
  ConverterType3::Pointer converter3 = ConverterType3::New();

  PixelType2 pixel0;
  pixel0[0] = 125;
  pixel0[1] = 105;
  pixel0[2] = 145;
  pixel0[3] = 0;
  ConverterType0::OutputPixelType outputPixel0 = converter0->Convert(pixel0);
  std::cout << "outputPixel0: " << outputPixel0 << std::endl;
  if(outputPixel0 != 112)
    {
    itkGenericExceptionMacro(<< "RGBA<unsigned int> 2 unsigned int Failled");
    }

  PixelType2 pixel1;
  pixel1[0] = 125;
  pixel1[1] = 105;
  pixel1[2] = 145;
  pixel1[3] = 0.75;
  ConverterType1::OutputPixelType outputPixel1 = converter1->Convert(pixel1);
  std::cout << "outputPixel1: " << outputPixel1 << std::endl;
  if(outputPixel1 != 112)
    {
    itkGenericExceptionMacro(<< "RGBA<double> 2 unsigned int  Failled");
    }

  PixelType2 pixel2;
  pixel2[0] = 125;
  pixel2[1] = 105;
  pixel2[2] = 145;
  pixel2[3] = 0;
  ConverterType2::OutputPixelType outputPixel2 = converter2->Convert(pixel2);
  std::cout << "outputPixel2: " << outputPixel2 << std::endl;
  if(outputPixel2[0] != 125 || outputPixel2[1] != 105 || outputPixel2[2] != 145 || outputPixel2[3] != 0 )
    {
    itkGenericExceptionMacro(<< "RGBA<unsigned int> 2 RGBA<double>  Failled");
    }

  PixelType2 pixel3;
  pixel3[0] = 125;
  pixel3[1] = 105;
  pixel3[2] = 145;
  pixel3[3] = 0;
  ConverterType3::OutputPixelType outputPixel3 = converter3->Convert(pixel3);
  std::cout << "outputPixel3: " << outputPixel3 << std::endl;
  if(outputPixel3[0] != 125 || outputPixel3[1] != 105 || outputPixel3[2] != 145)
    {
    itkGenericExceptionMacro(<< "RGBA<unsigned int> 2 RGB<double>  Failled");
    }


  return EXIT_SUCCESS;
}
