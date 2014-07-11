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
#include "otbDisparityTranslateFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"

typedef otb::Image<float, 2>                    FloatImageType;

typedef otb::VectorImage<float, 2>              FloatVectorImageType;

typedef otb::ImageFileReader<FloatImageType>    ReaderType;

typedef otb::ImageFileReader
  <FloatVectorImageType>                        ReaderVectorType;

typedef otb::ImageFileWriter
  <FloatVectorImageType>                        WriterType;

typedef otb::ImageFileWriter
  <FloatImageType>                              WriterScalarType;

typedef otb::ImageList<FloatImageType>          ImageListType;

typedef otb::VectorImageToImageListFilter
  <FloatVectorImageType,
    ImageListType>                               VectorToListFilterType;

typedef otb::ImageListToVectorImageFilter
  <ImageListType,
    FloatVectorImageType>                        ListToVectorFilterType;

typedef otb::DisparityTranslateFilter
  <FloatImageType,
    FloatVectorImageType,
    FloatImageType,
    FloatImageType>                              TranslateFilter;


int otbDisparityTranslateFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // Instantiation
  TranslateFilter::Pointer filter = TranslateFilter::New();
  return EXIT_SUCCESS;
}

int otbDisparityTranslateFilter(int argc, char* argv[])
{
  if (argc < 6)
    {
    std::cout << "Usage: "<<argv[0]<<" dispMap_epi leftInvGrid rightDirectGrid left_sensor dispMap_sensor" << std::endl;
    return EXIT_FAILURE;
    }

  ReaderVectorType::Pointer dispReader = ReaderVectorType::New();
  dispReader->SetFileName(argv[1]);

  ReaderVectorType::Pointer leftGridReader = ReaderVectorType::New();
  leftGridReader->SetFileName(argv[2]);

  ReaderVectorType::Pointer rightGridReader = ReaderVectorType::New();
  rightGridReader->SetFileName(argv[3]);

  ReaderType::Pointer leftSensor = ReaderType::New();
  leftSensor->SetFileName(argv[4]);

  VectorToListFilterType::Pointer vectorToListFilter = VectorToListFilterType::New();
  vectorToListFilter->SetInput(dispReader->GetOutput());
  vectorToListFilter->UpdateOutputInformation();

  TranslateFilter::Pointer translator = TranslateFilter::New();
  translator->SetHorizontalDisparityMapInput(vectorToListFilter->GetOutput()->GetNthElement(0));
  translator->SetVerticalDisparityMapInput(vectorToListFilter->GetOutput()->GetNthElement(1));
  translator->SetInverseEpipolarLeftGrid(leftGridReader->GetOutput());
  translator->SetDirectEpipolarRightGrid(rightGridReader->GetOutput());
  translator->SetLeftSensorImageInput(leftSensor->GetOutput());

  ImageListType::Pointer outputList = ImageListType::New();
  outputList->PushBack(translator->GetHorizontalDisparityMapOutput());
  outputList->PushBack(translator->GetVerticalDisparityMapOutput());

  ListToVectorFilterType::Pointer listToVectorFilter = ListToVectorFilterType::New();
  listToVectorFilter->SetInput(outputList);

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(listToVectorFilter->GetOutput());
  writer->SetFileName(argv[5]);
  writer->Update();

  return EXIT_SUCCESS;
}
