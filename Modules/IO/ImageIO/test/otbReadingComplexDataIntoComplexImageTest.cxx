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
#include <complex>

#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbMacro.h"

int otbReadingComplexDataIntoComplexImageTest(int itkNotUsed(argc), char * argv[])
{
  typedef double                                RealType;
  typedef std::complex<RealType>                PixelType;
  typedef otb::Image<PixelType, 2>              ImageType;
  typedef otb::VectorImage<RealType, 2>         VectorImageType;
  typedef otb::ImageFileReader<ImageType>       Reader1Type;
  typedef otb::ImageFileReader<VectorImageType> Reader2Type;

  ImageType::IndexType index;
  index[0] = atoi(argv[2]);
  index[1] = atoi(argv[3]);

  ImageType::SizeType size;
  size[0] = 1;
  size[1] = 1;

  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  Reader1Type::Pointer reader1 = Reader1Type::New();
  reader1->SetFileName(argv[1]);

  reader1->GetOutput()->UpdateOutputInformation();
  reader1->GetOutput()->SetRequestedRegion(region);
  reader1->GetOutput()->PropagateRequestedRegion();
  reader1->GetOutput()->UpdateOutputData();

  PixelType complexValue = reader1->GetOutput()->GetPixel(index);

  Reader2Type::Pointer reader2 = Reader2Type::New();
  reader2->SetFileName(argv[1]);

  reader2->GetOutput()->UpdateOutputInformation();
  reader2->GetOutput()->SetRequestedRegion(region);
  reader2->GetOutput()->PropagateRequestedRegion();
  reader2->GetOutput()->UpdateOutputData();

  VectorImageType::PixelType vectorValue = reader2->GetOutput()->GetPixel(index);

  std::cout << "Image value (complex): " << complexValue << std::endl;
  std::cout << "Image value (vector ): " << vectorValue << std::endl;

  otbControlConditionTestMacro(vectorValue.Size() < 2, "VectorImage has less than 2 components. No input complex data.");
  otbControlConditionTestMacro(
    complexValue.real() != vectorValue[0],
    "The real part is not read properly. Have a look at the file itk::ConvertPixelBuffer, the problem might be there.");
  otbControlConditionTestMacro(
    complexValue.imag() != vectorValue[1],
    "The imaginary part is not read properly. Have a look at the file itk::ConvertPixelBuffer, the problem might be there.");

  return EXIT_SUCCESS;
}
