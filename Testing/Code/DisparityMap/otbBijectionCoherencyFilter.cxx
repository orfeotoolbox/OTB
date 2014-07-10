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

#include "otbBijectionCoherencyFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"

typedef otb::Image<float, 2>                    FloatImageType;

typedef otb::VectorImage<float, 2>              FloatVectorImageType;

typedef otb::BijectionCoherencyFilter
  <FloatImageType,
    FloatImageType>                             BijectionFilterType;


int otbBijectionCoherencyFilterNew(int argc, char* argv[])
{
  // Instantiation
  BijectionFilterType::Pointer bijectFilter = BijectionFilterType::New();

  return EXIT_SUCCESS;
}

int otbBijectionCoherencyFilter(int argc, char* argv[])
{

  typedef otb::ImageFileReader
    <FloatVectorImageType>                        ReaderVectorType;

  typedef otb::ImageFileWriter
    <FloatImageType>                              WriterScalarType;

  typedef otb::ImageList<FloatImageType>          ImageListType;

  typedef otb::VectorImageToImageListFilter
    <FloatVectorImageType,
     ImageListType>                               VectorToListFilterType;

  if (argc < 8)
    {
    std::cout << "Usage: "<<argv[0]<<" left2right right2left minHDisp maxHDisp minVDisp maxVDisp output" << std::endl;
    return EXIT_FAILURE;
    }

  ReaderVectorType::Pointer l2rReader = ReaderVectorType::New();
  l2rReader->SetFileName(argv[1]);

  ReaderVectorType::Pointer r2lReader = ReaderVectorType::New();
  r2lReader->SetFileName(argv[2]);

  VectorToListFilterType::Pointer vectorToListFilter1 = VectorToListFilterType::New();
  vectorToListFilter1->SetInput(l2rReader->GetOutput());
  vectorToListFilter1->UpdateOutputInformation();

  VectorToListFilterType::Pointer vectorToListFilter2 = VectorToListFilterType::New();
  vectorToListFilter2->SetInput(r2lReader->GetOutput());
  vectorToListFilter2->UpdateOutputInformation();

  BijectionFilterType::Pointer bijectFilter = BijectionFilterType::New();
  bijectFilter->SetDirectHorizontalDisparityMapInput(vectorToListFilter1->GetOutput()->GetNthElement(0));
  bijectFilter->SetReverseHorizontalDisparityMapInput(vectorToListFilter2->GetOutput()->GetNthElement(0));
  bijectFilter->SetMinHDisp(atoi(argv[3]));
  bijectFilter->SetMaxHDisp(atoi(argv[4]));
  bijectFilter->SetMinVDisp(atoi(argv[5]));
  bijectFilter->SetMaxVDisp(atoi(argv[6]));

  WriterScalarType::Pointer writer = WriterScalarType::New();
  writer->SetInput(bijectFilter->GetOutput());
  writer->SetFileName(argv[7]);
  writer->Update();

  return EXIT_SUCCESS;
}
