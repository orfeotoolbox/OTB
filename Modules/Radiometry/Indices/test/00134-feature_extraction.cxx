/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "itkMeanImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiChannelRAndNIRIndexImageFilter.h"

int main(int itkNotUsed(argc), char* argv[])
{
  const char *       inputName   = argv[1];
  const char *       outputName  = argv[2];
  const unsigned int radius      = atoi(argv[3]);
  const unsigned int id          = atoi(argv[4]);
  const unsigned int size        = atoi(argv[5]);

  typedef double                                PixelType;
  typedef otb::Image<PixelType, 2>              ImageType;
  typedef otb::VectorImage<PixelType, 2>        VectorImageType;
  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  typedef otb::ImageList<ImageType>                                                             ImageListType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType>                     ListToImageFilterType;
  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType>                               ExtractorFilterType;
  typedef otb::MultiChannelExtractROI<PixelType, PixelType>                                     MultiChannelExtractorFilterType;
  typedef itk::MeanImageFilter<ImageType, ImageType>                                            MeanFilterType;
  typedef otb::Functor::NDVI<PixelType, PixelType, PixelType>                                   NDVIFunctorType;
  typedef otb::MultiChannelRAndNIRIndexImageFilter<VectorImageType, ImageType, NDVIFunctorType> NDVIFilterType;

  ReaderType::Pointer                      reader    = ReaderType::New();
  WriterType::Pointer                      writer    = WriterType::New();
  ImageListType::Pointer                   imListOut = ImageListType::New();
  ExtractorFilterType::Pointer             extract   = ExtractorFilterType::New();
  MultiChannelExtractorFilterType::Pointer MCExtract = MultiChannelExtractorFilterType::New();
  MeanFilterType::Pointer                  meanner   = MeanFilterType::New();
  NDVIFilterType::Pointer                  ndvi      = NDVIFilterType::New();
  ListToImageFilterType::Pointer           caster    = ListToImageFilterType::New();

  reader->SetFileName(inputName);
  reader->GenerateOutputInformation();

  ImageType::RegionType region;
  ImageType::SizeType   extSize;
  ImageType::IndexType  extId;
  extSize.Fill(size);
  extId.Fill(id);
  region.SetSize(extSize);
  region.SetIndex(extId);
  MCExtract->SetInput(reader->GetOutput());
  MCExtract->SetExtractionRegion(region);
  MCExtract->UpdateOutputInformation();

  extract->SetInput(MCExtract->GetOutput());
  extract->SetChannel(1);
  extract->UpdateOutputInformation();

  ImageType::SizeType rad;
  rad[0] = radius;
  rad[1] = radius;
  meanner->SetInput(extract->GetOutput());
  meanner->SetRadius(rad);

  ndvi->SetInput(MCExtract->GetOutput());

  imListOut->PushBack(meanner->GetOutput());
  imListOut->PushBack(ndvi->GetOutput());

  caster->SetInput(imListOut);
  writer->SetInput(caster->GetOutput());
  writer->SetFileName(outputName);
  writer->Update();

  /*** With 2 writers : OK */
  /*
  typedef otb::ImageFileWriter<ImageType>                             WriterMonoType;
  WriterMonoType::Pointer w1 = WriterMonoType::New();
  WriterMonoType::Pointer w2 = WriterMonoType::New();
  w1->SetInput( imListOut->GetNthElement(0) );
  w2->SetInput( imListOut->GetNthElement(1) );
  w1->SetFileName("meaner.tif");
  w2->SetFileName("ndvi.tif");
  w1->Update();
  w2->Update();
  */

  return EXIT_SUCCESS;
}
