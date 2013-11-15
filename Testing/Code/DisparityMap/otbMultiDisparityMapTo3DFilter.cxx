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

#include "otbMultiDisparityMapTo3DFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImageToImageListFilter.h"

typedef otb::Image<float, 2>                    FloatImageType;

typedef otb::VectorImage<float, 2>              FloatVectorImageType;

typedef otb::MultiDisparityMapTo3DFilter
  <FloatImageType,
    FloatVectorImageType,
    FloatImageType>                              Multi3DFilterType;


int otbMultiDisparityMapTo3DFilterNew(int argc, char* argv[])
{
  // Instantiation
  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();

  return EXIT_SUCCESS;
}

int otbMultiDisparityMapTo3DFilter(int argc, char* argv[])
{
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

  if (argc < 10)
    {
    std::cout << "Usage: "<<argv[0]<<" masterImage slaveImage1 slaveImage2 dispMap1 dispMap2 mask1 mask2 output residue" << std::endl;
    return EXIT_FAILURE;
    }

  ReaderType::Pointer masterReader = ReaderType::New();
  masterReader->SetFileName(argv[1]);
  masterReader->UpdateOutputInformation();

  ReaderType::Pointer slave1Reader = ReaderType::New();
  slave1Reader->SetFileName(argv[2]);
  slave1Reader->UpdateOutputInformation();

  ReaderType::Pointer slave2Reader = ReaderType::New();
  slave2Reader->SetFileName(argv[3]);
  slave2Reader->UpdateOutputInformation();

  ReaderVectorType::Pointer disp1Reader = ReaderVectorType::New();
  disp1Reader->SetFileName(argv[4]);

  ReaderVectorType::Pointer disp2Reader = ReaderVectorType::New();
  disp2Reader->SetFileName(argv[5]);

  ReaderType::Pointer mask1Reader = ReaderType::New();
  mask1Reader->SetFileName(argv[6]);

  ReaderType::Pointer mask2Reader = ReaderType::New();
  mask2Reader->SetFileName(argv[7]);

  VectorToListFilterType::Pointer vectorToListFilter1 = VectorToListFilterType::New();
  vectorToListFilter1->SetInput(disp1Reader->GetOutput());
  vectorToListFilter1->UpdateOutputInformation();

  VectorToListFilterType::Pointer vectorToListFilter2 = VectorToListFilterType::New();
  vectorToListFilter2->SetInput(disp2Reader->GetOutput());
  vectorToListFilter2->UpdateOutputInformation();

  Multi3DFilterType::Pointer multiFilter = Multi3DFilterType::New();
  multiFilter->SetReferenceKeywordList(masterReader->GetOutput()->GetImageKeywordlist());

  multiFilter->SetNumberOfMovingImages(2);

  multiFilter->SetHorizontalDisparityMapInput(0,vectorToListFilter1->GetOutput()->GetNthElement(0));
  multiFilter->SetVerticalDisparityMapInput(0,vectorToListFilter1->GetOutput()->GetNthElement(1));
  multiFilter->SetDisparityMaskInput(0,mask1Reader->GetOutput());
  multiFilter->SetMovingKeywordList(0,slave1Reader->GetOutput()->GetImageKeywordlist());

  multiFilter->SetHorizontalDisparityMapInput(1,vectorToListFilter2->GetOutput()->GetNthElement(0));
  multiFilter->SetVerticalDisparityMapInput(1,vectorToListFilter2->GetOutput()->GetNthElement(1));
  multiFilter->SetDisparityMaskInput(1,mask2Reader->GetOutput());
  multiFilter->SetMovingKeywordList(1,slave2Reader->GetOutput()->GetImageKeywordlist());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(multiFilter->GetOutput());
  writer->SetFileName(argv[8]);
  writer->Update();

  WriterScalarType::Pointer writer2 = WriterScalarType::New();
  writer2->SetInput(multiFilter->GetResidueOutput());
  writer2->SetFileName(argv[9]);
  writer2->Update();

  return EXIT_SUCCESS;
}
