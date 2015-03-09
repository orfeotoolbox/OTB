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

#include "otbImageListToImageListApplyFilter.h"
#include "otbImageList.h"
#include "otbImage.h"
#include "itkMeanImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageListToImageListApplyFilter(int itkNotUsed(argc), char * argv[])
{
  char * infname1  = argv[1];
  char * infname2  = argv[2];
  char * infname3  = argv[3];
  char * outfname1 = argv[4];
  char * outfname2 = argv[5];
  char * outfname3 = argv[6];

  const unsigned int Dimension = 2;
  typedef unsigned char                              PixelType;
  typedef otb::Image<PixelType, Dimension>           ImageType;
  typedef otb::ImageList<ImageType>                  ImageListType;
  typedef itk::MeanImageFilter<ImageType, ImageType> MeanFilterType;

  // IO
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  typedef otb::ImageListToImageListApplyFilter<ImageListType, ImageListType, MeanFilterType>
  ImageListToImageListApplyFilterType;

  // Instantiating object
  ImageListToImageListApplyFilterType::Pointer filter = ImageListToImageListApplyFilterType::New();

  ReaderType::Pointer reader1, reader2, reader3;

  reader1 = ReaderType::New();
  reader1->SetFileName(infname1);

  reader2 = ReaderType::New();
  reader2->SetFileName(infname2);

  reader3 = ReaderType::New();
  reader3->SetFileName(infname3);

  ImageListType::Pointer imageList = ImageListType::New();
  imageList->PushBack(reader1->GetOutput());
  imageList->PushBack(reader2->GetOutput());
  imageList->PushBack(reader3->GetOutput());

  MeanFilterType::Pointer meanFilter = MeanFilterType::New();

  filter->SetInput(imageList);
  filter->SetFilter(meanFilter);
  filter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outfname1);
  writer->SetInput(filter->GetOutput()->GetNthElement(0));
  writer->Update();

  writer = WriterType::New();
  writer->SetFileName(outfname2);
  writer->SetInput(filter->GetOutput()->GetNthElement(0));
  writer->Update();

  writer = WriterType::New();
  writer->SetFileName(outfname3);
  writer->SetInput(filter->GetOutput()->GetNthElement(0));
  writer->Update();

  return EXIT_SUCCESS;
}
