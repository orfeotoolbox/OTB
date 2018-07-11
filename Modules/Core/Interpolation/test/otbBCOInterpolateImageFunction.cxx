/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "itkMacro.h"

#include "otbBCOInterpolateImageFunction.h"
#include "otbImageFileReader.h"
#include <fstream>
#include "otbImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"


int otbBCOInterpolateImageFunctionNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::Image<double, 2>                                           ImageType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double>             InterpolatorType;

  // Instantiating object
  InterpolatorType::Pointer interp = InterpolatorType::New();

  std::cout << interp << std::endl;

  return EXIT_SUCCESS;
}

int otbBCOInterpolateImageFunction(int argc, char * argv[])
{
  const char * infname      = argv[1];
  const char * outfname     = argv[2];
  const unsigned int radius = atoi(argv[3]);
  const double alpha        = atof(argv[4]);

  typedef otb::Image<double, 2>                               ImageType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType               ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                     ReaderType;

  int i = 5;

  std::vector<ContinuousIndexType> indicesList;

  while (i < argc && (i + 1) < argc)
    {
    ContinuousIndexType idx;

    idx[0] = atof(argv[i]);
    idx[1] = atof(argv[i + 1]);

    indicesList.push_back(idx);

    i += 2;
    }

  // Instantiating object
  InterpolatorType::Pointer filter = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update(); //TODO check if necessary

  std::cout << "Alpha Checking : " << std::endl << filter->GetAlpha() << std::endl;
  filter->SetAlpha(-1.0);
  std::cout << filter->GetAlpha() << std::endl;
  filter->SetAlpha(alpha);
  std::cout << filter->GetAlpha() << std::endl;
  std::cout << "Radius Checking : " << std::endl << filter->GetRadius() << std::endl;
  filter->SetRadius(radius);
  std::cout << filter->GetRadius() << std::endl;

  filter->SetInputImage(reader->GetOutput());

  std::ofstream file;
  file.open(outfname);

  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
    file << (*it) << " -> " << filter->EvaluateAtContinuousIndex((*it)) << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}



int otbBCOInterpolateImageFunction2(int argc, char * argv[])
{

  typedef otb::Image<double, 2>              ImageType;
  ImageType::Pointer inputImage = ImageType::New();

  const unsigned int N = 100;

  ImageType::SizeType size;
  size.Fill(N);
  ImageType::IndexType index;
  index.Fill(0);
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  inputImage->SetLargestPossibleRegion( region );
  inputImage->SetBufferedRegion( region );
  inputImage->SetRequestedRegion( region );
  inputImage->SetNumberOfComponentsPerPixel(1);
  inputImage->Allocate();

  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  IteratorType it1(inputImage, region);

  for (it1.GoToBegin(); !it1.IsAtEnd(); ++it1)
    it1.Set(1.0);



  const unsigned int radius = atoi(argv[1]);
  const double alpha        = atof(argv[2]);

  typedef otb::BCOInterpolateImageFunction<ImageType, double> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType               ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                     ReaderType;


  std::vector<ContinuousIndexType> indicesList;

  for (int i=3; i+1 < argc; i=i+2)
    {
    ContinuousIndexType idx;

    idx[0] = atof(argv[i]);
    idx[1] = atof(argv[i + 1]);
    std::cout << idx[0] << " " << idx[1] << std::endl;
    indicesList.push_back(idx);

    }

  // Instantiating object
  InterpolatorType::Pointer filter = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();

  std::cout << "Alpha Checking : " << std::endl << filter->GetAlpha() << std::endl;
  filter->SetAlpha(-1.0);
  std::cout << filter->GetAlpha() << std::endl;
  filter->SetAlpha(alpha);
  std::cout << filter->GetAlpha() << std::endl;
  std::cout << "Radius Checking : " << std::endl << filter->GetRadius() << std::endl;
  filter->SetRadius(radius);
  std::cout << filter->GetRadius() << std::endl;

  filter->SetInputImage(inputImage);



  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
      std::cout << (*it) << " -> " << filter->EvaluateAtContinuousIndex((*it)) << std::endl;
      if (std::abs(filter->EvaluateAtContinuousIndex((*it))-1.0)>1e-6)
        return EXIT_FAILURE;
    }


  return EXIT_SUCCESS;
}



int otbBCOInterpolateImageFunctionOverVectorImageNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef otb::VectorImage<double, 2>                                     ImageType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double>             InterpolatorType;

  // Instantiating object
  InterpolatorType::Pointer filter = InterpolatorType::New();

  return EXIT_SUCCESS;
}


int otbBCOInterpolateImageFunctionOverVectorImage(int argc, char * argv[])
{
  const char * infname      = argv[1];
  const char * outfname     = argv[2];
  const unsigned int radius = atoi(argv[3]);
  const double alpha        = atof(argv[4]);

  typedef otb::VectorImage<double, 2>                         ImageType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType               ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                     ReaderType;

  int i = 5;

  std::vector<ContinuousIndexType> indicesList;

  while (i < argc && (i + 1) < argc)
    {
    ContinuousIndexType idx;

    idx[0] = atof(argv[i]);
    idx[1] = atof(argv[i + 1]);

    indicesList.push_back(idx);

    i += 2;
    }

  // Instantiating object
  InterpolatorType::Pointer filter = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update(); //TODO check if necessary

  std::cout << "Alpha Checking : " << std::endl << filter->GetAlpha() << std::endl;
  filter->SetAlpha(-1.0);
  std::cout << filter->GetAlpha() << std::endl;
  filter->SetAlpha(alpha);
  std::cout << filter->GetAlpha() << std::endl;
  std::cout << "Radius Checking : " << std::endl << filter->GetRadius() << std::endl;
  filter->SetRadius(radius);
  std::cout << filter->GetRadius() << std::endl;

  filter->SetInputImage(reader->GetOutput());

  std::ofstream file;
  file.open(outfname);

  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
    file << (*it) << " -> " << filter->EvaluateAtContinuousIndex((*it)) << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}


int otbBCOInterpolateImageFunctionTest(int itkNotUsed(argc), char * argv[])
{

  const char * infname      = argv[1];
  const char * outfname     = argv[2];
  const unsigned int radius = atoi(argv[3]);
  const double alpha        = atof(argv[4]);

  typedef otb::Image<double, 2>                                           ImageType;
  typedef otb::ImageFileReader<ImageType>                                 ReaderType;
  typedef otb::StreamingResampleImageFilter<ImageType, ImageType, double>   StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double>             InterpolatorType;
  typedef otb::ImageFileWriter<ImageType>                        WriterType;

  // Instantiating objects
  ReaderType::Pointer                         reader = ReaderType::New();
  WriterType::Pointer                         writer = WriterType::New();
  StreamingResampleImageFilterType::Pointer   resampler = StreamingResampleImageFilterType::New();
  InterpolatorType::Pointer                   interpolator = InterpolatorType::New();

  reader->SetFileName(infname);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());
  interpolator->SetRadius(radius);
  interpolator->SetAlpha(alpha);

  resampler->SetInput(reader->GetOutput());
  resampler->SetInterpolator(interpolator);
  StreamingResampleImageFilterType::SizeType size;
  size[0] = 256;
  size[1] = 256;
  resampler->SetOutputSize(size);
  resampler->SetOutputSpacing(2);
  resampler->Update();
  // Result of resampler is written
  writer->SetInput(resampler->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}

int otbBCOInterpolateImageFunctionVectorImageTest(int itkNotUsed(argc), char * argv[])
{

  const char * infname      = argv[1];
  const char * outfname     = argv[2];
  const unsigned int radius = atoi(argv[3]);
  const double alpha        = atof(argv[4]);

  typedef otb::VectorImage<double, 2>                                     ImageType;
  typedef otb::ImageFileReader<ImageType>                                 ReaderType;
  typedef otb::StreamingResampleImageFilter<ImageType, ImageType, double>   StreamingResampleImageFilterType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double>             InterpolatorType;
  typedef otb::ImageFileWriter<ImageType>                        WriterType;

  // Instantiating objects
  ReaderType::Pointer                         reader = ReaderType::New();
  WriterType::Pointer                         writer = WriterType::New();
  StreamingResampleImageFilterType::Pointer   resampler = StreamingResampleImageFilterType::New();
  InterpolatorType::Pointer                   interpolator = InterpolatorType::New();

  reader->SetFileName(infname);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());
  interpolator->SetRadius(radius);
  interpolator->SetAlpha(alpha);

  resampler->SetInput(reader->GetOutput());
  resampler->SetInterpolator(interpolator);
  StreamingResampleImageFilterType::SizeType size;
  size[0] = 256;
  size[1] = 256;
  resampler->SetOutputSize(size);
  resampler->SetOutputSpacing(2);
  resampler->Update();
  // Result of resampler is written
  writer->SetInput(resampler->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}
