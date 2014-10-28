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
#include "otbPolygonToPolygonRCC8Calculator.h"
#include "otbObjectList.h"
#include "otbImageFileReader.h"
#include "otbPolygon.h"
#include "otbImageToEdgePathFilter.h"
#include "otbSimplifyPathListFilter.h"

int otbPolygonToPolygonRCC8Calculator(int itkNotUsed(argc), char* argv[])
{
  const unsigned int Dimension = 2;

  int    nbImages  = atoi(argv[1]);
  char * outfile   = argv[2];

  typedef unsigned char                                      PixelType;
  typedef otb::Polygon<>                                     PolygonType;
  typedef otb::Image<PixelType, Dimension>                   ImageType;
  typedef otb::ImageToEdgePathFilter<ImageType, PolygonType> EdgeExtractionFilterType;
  typedef otb::SimplifyPathListFilter<PolygonType>           SimplifyPathFilterType;
  typedef SimplifyPathFilterType::InputListType              PathListType;
  typedef otb::ImageFileReader<ImageType>                    ReaderType;
  typedef otb::PolygonToPolygonRCC8Calculator<PolygonType>   CalculatorType;

  PolygonType::Pointer path = PolygonType::New();

  PolygonType::ContinuousIndexType a1, a2, a3, a4, a5, a6, a7;

  a1[0] = 0;
  a1[1] = 0;
  a2[0] = 10;
  a2[1] = 0;
  a3[0] = 10;
  a3[1] = 10;
  a4[0] = 10;
  a4[1] = 0;

  a5[0] = 5;
  a5[1] = 5;

  a6[0] = 5;
  a6[1] = 0;

  a7[0] = 100;
  a7[1] = 100;

  path->AddVertex(a1);
  path->AddVertex(a2);
  path->AddVertex(a3);
  path->AddVertex(a4);

  CalculatorType::Pointer calc = CalculatorType::New();

  typedef PathListType::Iterator IteratorType;

  // reference image list
  PathListType::Pointer regions = PathListType::New();

  // Reading input images
  std::ofstream out;
  out.open(outfile, std::ios::out);
  out << "Test results from otbPolygonToPolygonRCC8calculator test." << std::endl;
  for (int cpt = 1; cpt <= nbImages; ++cpt)
    {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(argv[2 + cpt]);
    EdgeExtractionFilterType::Pointer extraction = EdgeExtractionFilterType::New();
    extraction->SetInput(reader->GetOutput());
    extraction->SetForegroundValue(255);
    extraction->Update();
    regions->PushBack(extraction->GetOutput());
    }

  SimplifyPathFilterType::Pointer simplifier = SimplifyPathFilterType::New();
  simplifier->SetInput(regions);
  simplifier->GetFunctor().SetTolerance(0.1);
  simplifier->Update();

  // Declaration
  CalculatorType::Pointer calc1, calc2, calc3;
  // Computing relations for each images couple
  unsigned int i = 1;
  unsigned int j = 1;
  for (IteratorType it1 = simplifier->GetOutput()->Begin(); it1 != simplifier->GetOutput()->End(); ++it1)
    {
    for (IteratorType it2 = simplifier->GetOutput()->Begin(); it2 != simplifier->GetOutput()->End(); ++it2)
      {
      std::cout << "Test: computing relation " << i << "," << j << std::endl;
      calc1 = CalculatorType::New();
      calc1->SetPolygon1(it1.Get());
      calc1->SetPolygon2(it2.Get());
      calc1->Compute();
      out << calc1->GetValue() << "\t";
      std::cout << "Result without a priori knowledge " << calc1->GetValue() << std::endl;

      if (calc1->GetValue() < 3
          || calc1->GetValue() == 4
          || calc1->GetValue() == 6)
        {
        calc2 = CalculatorType::New();
        calc2->SetPolygon1(it1.Get());
        calc2->SetPolygon2(it2.Get());
        calc2->SetLevel1APrioriKnowledge(true);
        calc2->Compute();
        std::cout << "Result with level1 a priori knowledge " << calc2->GetValue() << std::endl;
        if (calc2->GetValue() != calc1->GetValue())
          {
          std::cout << "Test failed: Result with level1AprioriKnowledge ";
          std::cout << "different from result without a priori knowledge" << std::endl;
          std::cout << calc1->GetValue() << "!=" << calc2->GetValue() << std::endl;
          return EXIT_FAILURE;
          }
        }
      if (calc1->GetValue() < 4)
        {
        calc3 = CalculatorType::New();
        calc3->SetPolygon1(it1.Get());
        calc3->SetPolygon2(it2.Get());
        calc3->SetLevel3APrioriKnowledge(true);
        calc3->Compute();
        std::cout << "Result with level3 a priori knowledge " << calc3->GetValue() << std::endl;
        if (calc3->GetValue() != calc1->GetValue())
          {
          std::cout << "Test failed: Result with level3AprioriKnowledge ";
          std::cout << "different from result without a priori knowledge" << std::endl;
          std::cout << calc1->GetValue() << "!=" << calc3->GetValue() << std::endl;
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
