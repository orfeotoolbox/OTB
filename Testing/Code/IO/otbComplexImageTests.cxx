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
#include <fstream>
#include <string>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

int otbVectorImageComplexNew(int argc, char* argv[])
{
  typedef std::complex<float> PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  ImageType::Pointer image = ImageType::New();
  image->Initialize();

  return EXIT_SUCCESS;
}


template<class InternalType>
int otbVectorImageComplexGenericTest(int argc, char* argv[])
{
  typedef std::complex<InternalType> PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();
  std::cout << reader->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  itk::ImageIOBase::Pointer io = reader->GetImageIO();
  std::cout << io << std::endl;
  reader->Update();
  typename ImageType::IndexType index;
  index[0]=0;
  index[1]=0;

  typename ImageType::PixelType pixel = reader->GetOutput()->GetPixel(index);
  std::cout << pixel << std::endl;

  //Test value
  if ((pixel[0] != PixelType(0,1))
      || (pixel[1] != PixelType(20000, 20001)))
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

int otbVectorImageComplexFloatTest(int argc, char* argv[])
{
  return otbVectorImageComplexGenericTest<float>(argc, argv);
}

int otbVectorImageComplexDoubleTest(int argc, char* argv[])
{
  return otbVectorImageComplexGenericTest<double>(argc, argv);
}
