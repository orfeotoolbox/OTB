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
#include "otbImageToImageRCC8Calculator.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageFileReader.h"

int otbImageToImageRCC8Calculator(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;

  int    nbImages  = atoi(argv[1]);
  char * outfile   = argv[2];

  typedef unsigned char                              PixelType;
  typedef otb::Image<PixelType, Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageToImageRCC8Calculator<ImageType> CalculatorType;
  typedef otb::ImageList<ImageType>                  ImageListType;
  typedef ImageListType::Iterator                    IteratorType;
  // reference image list
  ImageListType::Pointer images = ImageListType::New();

  // Reading input images
  std::ofstream out;
  out.open(outfile, std::ios::out);
  out << "Test results from otbImageToImageRCC8calculator test." << std::endl;
  for (int k = 1; k <= nbImages; ++k)
    {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[2 + k]);
    reader->Update();
    images->PushBack(reader->GetOutput());
    }
  // Declaration
  CalculatorType::Pointer calc, calc1, calc2;
  // Computing relations for each images couple
  int i = 1;
  int j = 1;
  for (IteratorType it1 = images->Begin(); it1 != images->End(); ++it1)
    {
    for (IteratorType it2 = images->Begin(); it2 != images->End(); ++it2)
      {
      std::cout << "Test: computing relation " << i << "," << j << std::endl;
      calc = CalculatorType::New();
      calc->SetInput1(it1.Get());
      calc->SetInput2(it2.Get());
      calc->Update();
      out << calc->GetValue() << "\t";

      if (calc->GetValue() < 3
          || calc->GetValue() == 4
          || calc->GetValue() == 6)
        {
        calc1 = CalculatorType::New();
        calc1->SetInput1(it1.Get());
        calc1->SetInput2(it2.Get());
        calc1->SetLevel1APrioriKnowledge(true);
        calc1->Update();
        if (calc1->GetValue() != calc->GetValue())
          {
          std::cout << "Test failed: Result with level1AprioriKnowledge ";
          std::cout << "different from result without a priori knowledge" << std::endl;
          std::cout << calc->GetValue() << "!=" << calc1->GetValue() << std::endl;
          return EXIT_FAILURE;
          }
        }
      if (calc->GetValue() < 4)
        {
        calc2 = CalculatorType::New();
        calc2->SetInput1(it1.Get());
        calc2->SetInput2(it2.Get());
        calc2->SetLevel3APrioriKnowledge(true);
        calc2->Update();
        if (calc2->GetValue() != calc->GetValue())
          {
          std::cout << "Test failed: Result with level3AprioriKnowledge ";
          std::cout << "different from result without a priori knowledge" << std::endl;
          std::cout << calc->GetValue() << "!=" << calc1->GetValue() << std::endl;
          return EXIT_FAILURE;
          }
        }
      ++j;
      }
    j = 1;
    ++i;
    out << std::endl;
    }
  out.close();

  return EXIT_SUCCESS;
}
