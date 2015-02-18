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

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageAndVectorImageOperationFilter.h"

int otbImageAndVectorImageOperationFilterTest(int itkNotUsed(argc), char * argv[])
{
  const char * infname       = argv[1];
  const char * invectfname   = argv[2];
  const char * outfnameAdd   = argv[3];
  const char * outfnameSub   = argv[4];
  const char * outfnameMul   = argv[5];
  const char * outfnameDiv   = argv[6];

  typedef double                                PixelType;
  typedef otb::Image<PixelType, 2>              ScalarImageType;
  typedef otb::VectorImage<PixelType, 2>        VectorImageType;
  typedef otb::ImageFileReader<ScalarImageType> ReaderType;
  typedef otb::ImageFileReader<VectorImageType> VectorReaderType;
  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  typedef otb::ImageAndVectorImageOperationFilter<ScalarImageType, VectorImageType, VectorImageType> FilterType;

  ReaderType::Pointer       reader       = ReaderType::New();
  VectorReaderType::Pointer vectorReader = VectorReaderType::New();
  FilterType::Pointer       filter       = FilterType::New();
  WriterType::Pointer       writerAdd    = WriterType::New();
  WriterType::Pointer       writerSub    = WriterType::New();
  WriterType::Pointer       writerMul    = WriterType::New();
  WriterType::Pointer       writerDiv    = WriterType::New();

  reader->SetFileName(infname);
  vectorReader->SetFileName(invectfname);

  filter->SetInput(reader->GetOutput());
  filter->SetVectorInput(vectorReader->GetOutput());

  filter->UseAddition();
  writerAdd->SetInput(filter->GetOutput());
  writerAdd->SetFileName(outfnameAdd);
  writerAdd->Update();

  filter->UseSubstraction();
  writerSub->SetInput(filter->GetOutput());
  writerSub->SetFileName(outfnameSub);
  writerSub->Update();

  filter->UseMultiplication();
  writerMul->SetInput(filter->GetOutput());
  writerMul->SetFileName(outfnameMul);
  writerMul->Update();

  filter->UseDivision();
  writerDiv->SetInput(filter->GetOutput());
  writerDiv->SetFileName(outfnameDiv);
  writerDiv->Update();

  return EXIT_SUCCESS;
}
