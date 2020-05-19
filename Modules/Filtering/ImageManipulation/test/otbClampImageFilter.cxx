/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbClampImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <limits>

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkImageRegionConstIterator.h"

/** Pixel typedefs */
typedef double       InputPixelType;
typedef unsigned int OutputPixelType;

/** Image typedefs */
const unsigned int Dimension = 2;
typedef otb::Image<InputPixelType, Dimension>  InputImageType;
typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

typedef otb::ClampImageFilter<InputImageType, OutputImageType> FilterType;


int otbClampImageFilterTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  /** instantiating the filter */
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  filter->SetInput(reader->GetOutput());
  filter->SetLower(100);
  filter->SetUpper(400);

  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}

template <class InImageType, class OutImageType>
typename OutImageType::Pointer Cross(std::string const& inputFileName)
{
  typedef otb::ImageFileReader<InImageType> ReaderType;
  typedef otb::ClampImageFilter<InImageType, OutImageType> ClampFilter;
  typename ReaderType::Pointer reader(ReaderType::New());
  reader->SetFileName(inputFileName);
  typename ClampFilter::Pointer clamp(ClampFilter::New());
  clamp->SetInput(reader->GetOutput());
  clamp->Update();
  return clamp->GetOutput();
}

template <class OutImageType>
typename OutImageType::Pointer Cross(otb::VectorImage<std::complex<float>>::Pointer input)
{
  typedef otb::ClampImageFilter<otb::VectorImage<std::complex<float>>, OutImageType> ClampFilter;
  typename ClampFilter::Pointer clamp(ClampFilter::New());
  clamp->SetInput(input);
  clamp->Update();
  return clamp->GetOutput();
}

template <class OutImageType>
typename OutImageType::Pointer Cross(otb::Image<itk::FixedArray<std::complex<float>, 2>>::Pointer input)
{
  typedef otb::ClampImageFilter<otb::Image<itk::FixedArray<std::complex<float>, 2>>, OutImageType> ClampFilter;
  typename ClampFilter::Pointer clamp(ClampFilter::New());
  clamp->SetInput(input);
  clamp->Update();
  return clamp->GetOutput();
}

typedef otb::VectorImage<double> ImageRefType;

template <class ImageType>
bool CompareImageReal(const ImageRefType::Pointer imRef, const ImageType* im)
{
  typedef typename ImageType::PixelType RealPixelType;

  RealPixelType min   = std::numeric_limits<RealPixelType>::lowest();
  RealPixelType max   = std::numeric_limits<RealPixelType>::max();
  auto          itRef = itk::ImageRegionConstIterator<ImageRefType>(imRef, imRef->GetLargestPossibleRegion());
  auto          it    = itk::ImageRegionConstIterator<ImageType>(im, im->GetLargestPossibleRegion());
  itRef.GoToBegin();
  it.GoToBegin();
  RealPixelType val;
  double        ref;
  while (!it.IsAtEnd())
  {
    val = it.Get();
    ref = itRef.Get()[0];
    if (ref > static_cast<double>(max) && val != max)
    {
      return false;
    }
    else if (ref < static_cast<double>(min) && val != min)
    {
      return false;
    }
    else if (static_cast<RealPixelType>(ref) != val)
    {
      return false;
    }
    ++it;
    ++itRef;
  }
  return true;
}

template <class ImageType>
bool CompareVectorReal(const ImageRefType::Pointer imRef, const ImageType* im)
{
  typedef typename ImageType::InternalPixelType RealPixelType;
  RealPixelType                                 min   = std::numeric_limits<RealPixelType>::lowest();
  RealPixelType                                 max   = std::numeric_limits<RealPixelType>::max();
  auto                                          itRef = itk::ImageRegionConstIterator<ImageRefType>(imRef, imRef->GetLargestPossibleRegion());
  auto                                          it    = itk::ImageRegionConstIterator<ImageType>(im, im->GetLargestPossibleRegion());
  itRef.GoToBegin();
  it.GoToBegin();
  unsigned int nbChanel = im->GetNumberOfComponentsPerPixel();
  // unsigned int nbChanelRef = imRef->GetNumberOfComponentsPerPixel ();
  RealPixelType val;
  double        ref;
  while (!it.IsAtEnd())
  {
    // std::cout<<it.Get()<<std::endl;
    // std::cout<<itRef.Get()<<std::endl;
    for (unsigned int i = 0; i < nbChanel; i++)
    {
      val = it.Get()[i];
      ref = itRef.Get()[i];

      if (ref > static_cast<double>(max) && val != max)
      {
        std::cout << "ref : " << static_cast<RealPixelType>(ref) << std::endl;
        std::cout << "val : " << val << std::endl;
        return false;
      }
      else if (ref < static_cast<double>(min) && val != min)
      {
        std::cout << "ref : " << static_cast<RealPixelType>(ref) << std::endl;
        std::cout << "val : " << val << std::endl;
        return false;
      }
      else if (static_cast<RealPixelType>(ref) != val)
      {
        std::cout << "ref : " << static_cast<RealPixelType>(ref) << std::endl;
        std::cout << "val : " << val << std::endl;
        return false;
      }
    }
    ++it;
    ++itRef;
  }
  return true;
}

template <class ImageType>
bool CompareImageComplex(const ImageRefType::Pointer imageRef, const ImageType* im)
{
  typedef typename ImageType::PixelType    ComplexType;
  typedef typename ComplexType::value_type RealType;

  RealType min   = std::numeric_limits<RealType>::lowest();
  RealType max   = std::numeric_limits<RealType>::max();
  auto     itRef = itk::ImageRegionConstIterator<ImageRefType>(imageRef, imageRef->GetLargestPossibleRegion());
  auto     it    = itk::ImageRegionConstIterator<ImageType>(im, im->GetLargestPossibleRegion());
  itRef.GoToBegin();
  it.GoToBegin();
  ComplexType val;
  double      reRef, imRef;
  while (!it.IsAtEnd())
  {
    val   = it.Get();
    reRef = itRef.Get()[0];
    imRef = itRef.Get()[1];
    if ((reRef > static_cast<double>(max) && val.real() != max) || (imRef > static_cast<double>(max) && val.imag() != max))
    {
      return false;
    }
    else if ((reRef < static_cast<double>(min) && val.real() != min) || (imRef < static_cast<double>(min) && val.imag() != min))
    {
      return false;
    }
    else if (static_cast<RealType>(reRef) != val.real() || static_cast<RealType>(imRef) != val.imag())
    {
      return false;
    }
    ++it;
    ++itRef;
  }
  return true;
}

template <class ImageType>
bool CompareVectorComplex(const ImageRefType::Pointer imageRef, const ImageType* im)
{
  typedef typename ImageType::InternalPixelType ComplexType;
  typedef typename ComplexType::value_type      RealType;

  RealType min   = std::numeric_limits<RealType>::lowest();
  RealType max   = std::numeric_limits<RealType>::max();
  auto     itRef = itk::ImageRegionConstIterator<ImageRefType>(imageRef, imageRef->GetLargestPossibleRegion());
  auto     it    = itk::ImageRegionConstIterator<ImageType>(im, im->GetLargestPossibleRegion());
  itRef.GoToBegin();
  it.GoToBegin();
  unsigned int nbChanel = im->GetNumberOfComponentsPerPixel();
  ComplexType  val;
  float        reRef, imRef;
  while (!it.IsAtEnd())
  {
    for (unsigned int i = 0; i < nbChanel; i++)
    {
      val   = it.Get()[i];
      reRef = itRef.Get()[2 * i];
      imRef = itRef.Get()[2 * i + 1];
      if ((reRef > static_cast<double>(max) && val.real() != max) || (imRef > static_cast<double>(max) && val.imag() != max))
      {
        return false;
      }
      else if ((reRef < static_cast<double>(min) && val.real() != min) || (imRef < static_cast<double>(min) && val.imag() != min))
      {
        return false;
      }
      else if (static_cast<RealType>(reRef) != val.real() || static_cast<RealType>(imRef) != val.imag())
      {
        return false;
      }
    }
    ++it;
    ++itRef;
  }
  return true;
}

template <class ImageType>
bool CompareArrayComplex(const ImageRefType::Pointer imageRef, const ImageType* im)
{
  typedef typename ImageType::PixelType    ArrayType;
  typedef typename ArrayType::ValueType    ComplexType;
  typedef typename ComplexType::value_type RealType;

  RealType min   = std::numeric_limits<RealType>::lowest();
  RealType max   = std::numeric_limits<RealType>::max();
  auto     itRef = itk::ImageRegionConstIterator<ImageRefType>(imageRef, imageRef->GetLargestPossibleRegion());
  auto     it    = itk::ImageRegionConstIterator<ImageType>(im, im->GetLargestPossibleRegion());
  itRef.GoToBegin();
  it.GoToBegin();
  unsigned int nbChanel = im->GetNumberOfComponentsPerPixel();
  ComplexType  val;
  float        reRef, imRef;
  while (!it.IsAtEnd())
  {
    for (unsigned int i = 0; i < nbChanel; i++)
    {
      val   = it.Get()[i];
      reRef = itRef.Get()[2 * i];
      imRef = itRef.Get()[2 * i + 1];
      if ((reRef > static_cast<double>(max) && val.real() != max) || (imRef > static_cast<double>(max) && val.imag() != max))
      {
        return false;
      }
      else if ((reRef < static_cast<double>(min) && val.real() != min) || (imRef < static_cast<double>(min) && val.imag() != min))
      {
        return false;
      }
      else if (static_cast<RealType>(reRef) != val.real() || static_cast<RealType>(imRef) != val.imag())
      {
        return false;
      }
    }
    ++it;
    ++itRef;
  }
  return true;
}

int otbClampImageFilterConversionTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ImageFileReader<ImageRefType> ReaderType;
  ReaderType::Pointer                        reader(ReaderType::New());
  reader->SetFileName(argv[1]);
  reader->Update();
  ImageRefType::Pointer imageRef = reader->GetOutput();

  // vect<real> --> vect<real>
  otb::VectorImage<short>::Pointer image0 = Cross<otb::VectorImage<double>, otb::VectorImage<short>>(argv[1]);
  bool                             test0  = CompareVectorReal<otb::VectorImage<short>>(imageRef, image0);
  std::cout << "Test 0 : " << test0 << std::endl;
  image0 = nullptr;

  // vect<real> --> vect<complex>
  otb::VectorImage<std::complex<unsigned short>>::Pointer image1 = Cross<otb::VectorImage<float>, otb::VectorImage<std::complex<unsigned short>>>(argv[1]);
  bool                                                    test1  = CompareVectorComplex<otb::VectorImage<std::complex<unsigned short>>>(imageRef, image1);
  std::cout << "Test 1 : " << test1 << std::endl;
  image1 = nullptr;

  // vect<real> --> image<real>
  otb::Image<int>::Pointer image2 = Cross<otb::VectorImage<float>, otb::Image<int>>(argv[1]);
  bool                     test2  = CompareImageReal<otb::Image<int>>(imageRef, image2);
  std::cout << "Test 2 : " << test2 << std::endl;
  image2 = nullptr;

  // vect<real> --> image<complex>
  otb::Image<std::complex<float>>::Pointer image3 = Cross<otb::VectorImage<float>, otb::Image<std::complex<float>>>(argv[1]);
  bool                                     test3  = CompareImageComplex<otb::Image<std::complex<float>>>(imageRef, image3);
  std::cout << "Test 3 : " << test3 << std::endl;
  image3 = nullptr;

  // image<real> --> image<real>
  otb::Image<unsigned short>::Pointer image4 = Cross<otb::Image<itk::FixedArray<double, 4>>, otb::Image<unsigned short>>(argv[1]);
  bool                                test4  = CompareImageReal<otb::Image<unsigned short>>(imageRef, image4);
  std::cout << "Test 4 : " << test4 << std::endl;
  image4 = nullptr;

  // image<real> --> image<complex>
  otb::Image<std::complex<int>>::Pointer image5 = Cross<otb::Image<itk::FixedArray<double, 4>>, otb::Image<std::complex<int>>>(argv[1]);
  bool                                   test5  = CompareImageComplex<otb::Image<std::complex<int>>>(imageRef, image5);
  std::cout << "Test 5 : " << test5 << std::endl;
  image5 = nullptr;

  // image<real> --> vector<real>
  otb::VectorImage<float>::Pointer image6 = Cross<otb::Image<itk::FixedArray<double, 4>>, otb::VectorImage<float>>(argv[1]);
  bool                             test6  = CompareVectorReal<otb::VectorImage<float>>(imageRef, image6);
  std::cout << "Test 6 : " << test6 << std::endl;
  image6 = nullptr;

  // image<real> --> vector<complex>
  otb::VectorImage<std::complex<float>>::Pointer image7 = Cross<otb::Image<itk::FixedArray<double, 4>>, otb::VectorImage<std::complex<float>>>(argv[1]);
  bool                                           test7  = CompareVectorComplex<otb::VectorImage<std::complex<float>>>(imageRef, image7);
  std::cout << "Test 7 : " << test7 << std::endl;

  // vector<complex> --> vector<real>
  otb::VectorImage<int>::Pointer image8 = Cross<otb::VectorImage<int>>(image7);
  bool                           test8  = CompareVectorReal<otb::VectorImage<int>>(imageRef, image8);
  std::cout << "Test 8 : " << test8 << std::endl;
  image8 = nullptr;

  // vector<complex> --> vector<complex>
  otb::VectorImage<std::complex<int>>::Pointer image9 = Cross<otb::VectorImage<std::complex<int>>>(image7);
  bool                                         test9  = CompareVectorComplex<otb::VectorImage<std::complex<int>>>(imageRef, image9);
  std::cout << "Test 9 : " << test9 << std::endl;
  image9 = nullptr;

  // vector<complex> --> image<real>
  otb::Image<int>::Pointer image10 = Cross<otb::Image<int>>(image7);
  bool                     test10  = CompareImageReal<otb::Image<int>>(imageRef, image10);
  std::cout << "Test 10 : " << test10 << std::endl;
  image10 = nullptr;

  // vector<complex> --> image<complex>
  otb::Image<std::complex<unsigned short>>::Pointer image11 = Cross<otb::Image<std::complex<unsigned short>>>(image7);
  bool                                              test11  = CompareImageComplex<otb::Image<std::complex<unsigned short>>>(imageRef, image11);
  std::cout << "Test 11 : " << test11 << std::endl;
  image11 = nullptr;

  // image<complex> --> vector<complex>
  otb::VectorImage<std::complex<float>>::Pointer image12 = Cross<otb::Image<std::complex<float>>, otb::VectorImage<std::complex<float>>>(argv[1]);
  bool                                           test12  = CompareVectorComplex<otb::VectorImage<std::complex<float>>>(imageRef, image12);
  std::cout << "Test 12 : " << test12 << std::endl;
  image12 = nullptr;

  // image<complex> --> image<complex>
  otb::Image<std::complex<short>>::Pointer image13 = Cross<otb::Image<std::complex<float>>, otb::Image<std::complex<short>>>(argv[1]);
  bool                                     test13  = CompareImageComplex<otb::Image<std::complex<short>>>(imageRef, image13);
  std::cout << "Test 13 : " << test13 << std::endl;
  image13 = nullptr;

  // image<complex> --> image<real>
  otb::Image<int>::Pointer image14 = Cross<otb::Image<std::complex<float>>, otb::Image<int>>(argv[1]);
  bool                     test14  = CompareImageReal<otb::Image<int>>(imageRef, image14);
  std::cout << "Test 14 : " << test14 << std::endl;
  image14 = nullptr;

  // image<complex> --> vector<real>
  otb::VectorImage<unsigned short>::Pointer image15 = Cross<otb::Image<std::complex<float>>, otb::VectorImage<unsigned short>>(argv[1]);
  bool                                      test15  = CompareVectorReal<otb::VectorImage<unsigned short>>(imageRef, image15);
  std::cout << "Test 15 : " << test15 << std::endl;
  image15 = nullptr;

  // image<fixedarray<real>> --> image<fixedarray<complex>>
  otb::Image<itk::FixedArray<std::complex<float>, 2>>::Pointer image16 =
      Cross<otb::Image<itk::FixedArray<double, 4>>, otb::Image<itk::FixedArray<std::complex<float>, 2>>>(argv[1]);
  bool test16 = CompareArrayComplex<otb::Image<itk::FixedArray<std::complex<float>, 2>>>(imageRef, image16);
  std::cout << "Test 16 : " << test16 << std::endl;

  // image<fixedarray<complex>> --> vectorimage<real>
  otb::VectorImage<int>::Pointer image17 = Cross<otb::VectorImage<int>>(image16);
  bool                           test17  = CompareVectorReal<otb::VectorImage<int>>(imageRef, image17);
  std::cout << "Test 17 : " << test17 << std::endl;
  image17 = nullptr;

  // vector<real> --> image<fixedarray<complex>>
  otb::Image<itk::FixedArray<std::complex<float>, 2>>::Pointer image18 =
      Cross<otb::VectorImage<int>, otb::Image<itk::FixedArray<std::complex<float>, 2>>>(argv[1]);
  bool test18 = CompareArrayComplex<otb::Image<itk::FixedArray<std::complex<float>, 2>>>(imageRef, image18);
  image18     = nullptr;
  std::cout << "Test 18 : " << test18 << std::endl;

  if (test1 && test2 && test3 && test4 && test5 && test6 && test7 && test8 && test9 && test10 && test11 && test12 && test13 && test14 && test15 && test16 &&
      test17 && test18)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
