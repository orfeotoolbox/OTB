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

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "itkImageRegionIterator.h"


int main(int itkNotUsed(argc), char* argv[])
{
  typedef char PixelType;

  const char * out = argv[1];
  const PixelType value = static_cast<PixelType>(atof(argv[2] ));

  typedef otb::Image<PixelType, 2>              ImageType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  typedef itk::ImageRegionIterator<ImageType>   IterType;

  ImageType::IndexType id;
  ImageType::SizeType size;
  ImageType::RegionType region;

  id.Fill(0);
  size.Fill(10);

  region.SetSize( size );
  region.SetIndex( id );

  ImageType::Pointer image = ImageType::New();
  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(value);

  IterType it1( image, region );
  it1.GoToBegin();

  it1.GoToBegin();
  while( !it1.IsAtEnd() )
    {
    if( value != it1.Get() )
      {
      std::cout<<"Allocated image pixel value "<<it1.Get()<<" differs from the asked one "<<(double)value<<"."<<std::endl;
      return EXIT_FAILURE;
      }
    ++it1;
    }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image);
  writer->SetFileName(out);
  writer->Update();


  typedef otb::Image<double, 2>                     ReaderImageType;
  typedef otb::ImageFileReader<ReaderImageType>     ReaderType;
  typedef itk::ImageRegionIterator<ReaderImageType> ReaderIterType;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(out);
  reader->Update();

  ReaderIterType it2( reader->GetOutput(), region );
  it2.GoToBegin();
  while( !it2.IsAtEnd() )
    {
    if( value != it2.Get() )
      {
      std::cout<<"Read image pixel value "<<it2.Get()<<" differs from the asked one "<<(double)value<<"."<<std::endl;
      return EXIT_FAILURE;
      }
    ++it2;
    }


  return EXIT_SUCCESS;
}
