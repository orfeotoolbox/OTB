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

#include "itkMacro.h"
#include "otbFunctorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVariadicAddFunctor.h"
#include "otbVariadicConcatenateFunctor.h"
#include "otbVariadicNamedInputsImageFilter.h"
#include <tuple>

#include <numeric>
#include <complex>
#include <math.h>

// static tests

// Tests of IsNeighborhood, PixelTypeDeduction and ImageTypeDeduction structs
using namespace otb;
template <typename T>
struct TypesCheck
{
  using ScalarType             = T;
  using ImageType              = otb::Image<T>;
  using VectorType             = itk::VariableLengthVector<ScalarType>;
  using FixedArrayType         = itk::FixedArray<ScalarType, 4>;
  using RGBPixelType           = itk::RGBPixel<ScalarType>;
  using RGBAPixelType          = itk::RGBAPixel<ScalarType>;
  using VectorImageType        = otb::VectorImage<ScalarType>;
  using NeighborhoodType       = itk::ConstNeighborhoodIterator<ImageType>;
  using VectorNeighborhoodType = itk::ConstNeighborhoodIterator<VectorImageType>;

  // Test IsNeighborhood struct
  template <typename U>
  struct CheckIsNeighborhood
  {
    static constexpr bool value = !otb::IsNeighborhood<U>::value && otb::IsNeighborhood<const itk::ConstNeighborhoodIterator<Image<U>>&>::value;
  };

  static_assert(CheckIsNeighborhood<T>::value, "");
  static_assert(CheckIsNeighborhood<itk::VariableLengthVector<T>>::value, "");
  static_assert(CheckIsNeighborhood<itk::FixedArray<T, 4>>::value, "");
  static_assert(CheckIsNeighborhood<itk::RGBPixel<T>>::value, "");
  static_assert(CheckIsNeighborhood<itk::RGBAPixel<T>>::value, "");

  // Test PixelTypeDeduction struct
  template <typename U>
  struct CheckPixelTypeDeduction
  {
    static constexpr bool value = std::is_same<typename PixelTypeDeduction<U>::PixelType, U>::value &&
                                  std::is_same<typename PixelTypeDeduction<itk::ConstNeighborhoodIterator<Image<U>>>::PixelType, U>::value;
  };

  static_assert(CheckPixelTypeDeduction<T>::value, "");
  static_assert(CheckPixelTypeDeduction<itk::VariableLengthVector<T>>::value, "");
  static_assert(CheckPixelTypeDeduction<itk::FixedArray<T, 4>>::value, "");
  static_assert(CheckPixelTypeDeduction<itk::RGBPixel<T>>::value, "");
  static_assert(CheckPixelTypeDeduction<itk::RGBAPixel<T>>::value, "");

  // Test ImageTypeDeduction struct
  static_assert(std::is_same<typename ImageTypeDeduction<ScalarType>::ImageType, ImageType>::value, "");
  static_assert(std::is_same<typename ImageTypeDeduction<VectorType>::ImageType, VectorImageType>::value, "");

  // Fake test operator
  template <typename TOut, typename TIn>
  struct TestOperator
  {
    auto operator()(const TIn&) const
    {
      TOut res(OutputSize());
      return res;
    }

    constexpr size_t OutputSize(...) const
    {
      return 1;
    }
  };

  template <typename TOut, typename TIn>
  struct TestOperatorVoidReturn
  {
    void operator()(TOut& out, const TIn&) const
    {
      out = TOut(OutputSize());
    }

    constexpr size_t OutputSize(...) const
    {
      return 1;
    }
  };

  // Fake test operator non const
  template <typename TOut, typename TIn>
  struct TestOperatorNonConst
  {
    auto operator()(const TIn&)
    {
      TOut res(OutputSize());
      return res;
    }

    constexpr size_t OutputSize(...) const
    {
      return 1;
    }
  };


  template <typename TOut, typename TIn>
  void TestFilter()
  {
    // Deduce types
    using InputPixelType = typename PixelTypeDeduction<TIn>::PixelType;
    using InputImageType = typename ImageTypeDeduction<InputPixelType>::ImageType;

    // Allocate fake input
    auto                              in   = InputImageType::New();
    typename InputImageType::SizeType size = {{10, 10}};
    in->SetRegions(size);
    in->SetNumberOfComponentsPerPixel(1);
    in->Allocate();
    InputPixelType defaultValue(1);
    in->FillBuffer(defaultValue);

    // Build and run filter
    auto functor = TestOperator<TOut, TIn>{};
    auto filter  = NewFunctorFilter(functor);

    using FilterType = typename decltype(filter)::ObjectType;
    static_assert(FilterType::NumberOfInputs == 1, "");
    static_assert(std::is_same<typename FilterType::template InputImageType<0>, InputImageType>::value, "");
    filter->SetInputs(in);
    filter->SetInput1(in);
    filter->template SetInput<0>(in); // template keyword to
                                      // avoid C++ parse
                                      // ambiguity
    auto res = filter->template GetInput<0>();

    filter->Update();

    // Test named input version
    struct tag
    {
    };
    using inputNames = std::tuple<tag>;
    auto filter1     = NewFunctorFilter<decltype(functor), inputNames>(functor);
    filter1->template SetInput<tag>(in);
    filter1->SetInput(tag{}, in);
    res = filter1->template GetInput<tag>();
    res = filter1->GetInput(tag{});
    filter1->Update();

    // Test static New() operator
    auto oldStyleNewFilter = FunctorImageFilter<decltype(functor)>::New();
    oldStyleNewFilter->SetInputs(in);
    oldStyleNewFilter->Update();

    // Hack to silent -Wunused-but-set-variable
    std::cout << res << std::endl;

    // Test with void return
    auto functorWithVoidReturn     = TestOperatorVoidReturn<TOut, TIn>{};
    auto filterWithVoidReturn      = NewFunctorFilter(functorWithVoidReturn);
    using FilterWithVoidReturnType = typename decltype(filter)::ObjectType;
    static_assert(FilterWithVoidReturnType::NumberOfInputs == 1, "");
    static_assert(std::is_same<typename FilterWithVoidReturnType::template InputImageType<0>, InputImageType>::value, "");

    // Test with non const operator
    auto functorNonConstOperator    = TestOperatorNonConst<TOut, TIn>{};
    auto filterWithNonConstOperator = NewFunctorFilter(functorNonConstOperator);
    filterWithNonConstOperator->SetInput1(in);
    filterWithNonConstOperator->Update();

    filterWithVoidReturn->SetInputs(in);
    filterWithVoidReturn->SetInput1(in);
    filterWithVoidReturn->template SetInput<0>(in); // template keyword to avoid C++ parse ambiguity
    filterWithVoidReturn->Update();

    // Test with simple lambda
    auto lambda = [](const TIn&) {
      TOut ret(1);
      return ret;
    };
    auto filterWithLambda = NewFunctorFilter(lambda, 1, {{0, 0}});
    filterWithLambda->SetInputs(in);
    filterWithLambda->Update();

    // Test with standard filter use
    using FullFilterType = otb::FunctorImageFilter<TestOperator<TOut, TIn>>;
    auto filter2         = FullFilterType::New();
  }

  TypesCheck()
  {
    // Test possible combinations
    TestFilter<ScalarType, ScalarType>();
    TestFilter<ScalarType, VectorType>();
    TestFilter<ScalarType, NeighborhoodType>();
    TestFilter<VectorType, ScalarType>();
  }
};

auto checksInt     = TypesCheck<int>{};
auto checksDouble  = TypesCheck<double>{};
auto checksComplex = TypesCheck<std::complex<double>>{};

// Example functors


// 1 VectorImage -> 1 VectorImage with a different size depending on a
// parameter of the functor
// This Functor
template <typename TOut, typename TIn>
struct BandExtraction
{
  BandExtraction(unsigned int indices...) : m_Indices({indices})
  {
  }

  // TODO define a constructor to initialize m_Indices
  auto operator()(const itk::VariableLengthVector<TIn>& in) const
  {
    itk::VariableLengthVector<TOut> out(m_Indices.size());

    size_t idx = 0;
    for (auto v : m_Indices)
    {
      out[idx] = static_cast<TOut>(in[v]);
      ++idx;
    }

    return out;
  }

  // This time OutputSize does not depend on input image size, hence
  // the ...
  size_t OutputSize(...) const
  {
    return m_Indices.size();
  }

  // set of band indices to extract
  std::set<unsigned int> m_Indices;
};

// 1 Image with neighborhood -> 1 Image
// This Functor computes the mean in neighborhood
template <typename TOut, typename TIn>
struct Mean
{
  auto operator()(const itk::ConstNeighborhoodIterator<otb::Image<TIn>>& in) const
  {
    TOut out(0);

    for (auto idx = 0u; idx < in.Size(); idx++)
    {
      out += static_cast<TOut>(in.GetPixel(idx));
    }

    out /= in.Size();

    return out;
  }
};

// 1 Image with neighborhood of VariableLengthVector -> 1 image with
// VariableLengthVector
// For each channel, returns the maximum value in neighborhood
template <typename T>
struct MaxInEachChannel
{
  auto operator()(const itk::ConstNeighborhoodIterator<otb::VectorImage<T>>& in) const
  {
    auto out = in.GetCenterPixel();

    for (auto idx = 0u; idx < in.Size(); idx++)
    {
      auto pixel = in.GetPixel(idx);
      for (auto band = 0u; band < out.Size(); ++band)
      {
        if (pixel[band] < out[band])
          out[band] = in.GetPixel(idx)[band];
      }
    }
    return out;
  }

  size_t OutputSize(const std::array<size_t, 1>& nbBands) const
  {
    return nbBands[0];
  }
};


template <typename T>
struct VectorModulus
{
  itk::VariableLengthVector<double> operator()(const itk::VariableLengthVector<std::complex<T>>& in) const
  {
    itk::VariableLengthVector<double> out(in.Size());

    for (auto band = 0u; band < out.Size(); ++band)
    {
      out[band] = std::abs(in[band]);
    }
    return out;
  }

  size_t OutputSize(const std::array<size_t, 1>& nbBands) const
  {
    return nbBands[0];
  }
};

int otbFunctorImageFilter(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  // test functions in functor_filter_details namespace
  using VectorImageType        = VectorImage<double>;
  using ImageType              = Image<double>;
  using ComplexVectorImageType = VectorImage<std::complex<double>>;
  using ComplexImageType       = Image<std::complex<double>>;
  using RegionType             = typename ImageType::RegionType;
  using SizeType               = typename RegionType::SizeType;

  auto vimage  = VectorImageType::New();
  auto image   = ImageType::New();
  auto cvimage = ComplexVectorImageType::New();
  auto cimage  = ComplexImageType::New();

  SizeType size = {{200, 200}};

  vimage->SetRegions(size);
  vimage->SetNumberOfComponentsPerPixel(2);
  vimage->Allocate();
  itk::VariableLengthVector<double> v(2);
  v.Fill(0);
  vimage->FillBuffer(v);

  cvimage->SetRegions(size);
  cvimage->SetNumberOfComponentsPerPixel(2);
  cvimage->Allocate();
  itk::VariableLengthVector<std::complex<double>> cv(2);
  cv.Fill(0);
  cvimage->FillBuffer(cv);

  image->SetRegions(size);
  image->Allocate();
  image->FillBuffer(0);

  cimage->SetRegions(size);
  cimage->Allocate();
  cimage->FillBuffer(0);

  // Test InputsImageFilter
  auto filter = otb::VariadicInputsImageFilter<VectorImageType, VectorImageType, ImageType>::New();
  filter->SetInput(vimage);
  filter->SetInput<0>(vimage);
  filter->SetInput<1>(image);

  filter->SetInput1(vimage);
  filter->SetInput2(image);

  filter->SetInputs(vimage, image);
  std::cout << filter->GetInput<0>() << filter->GetInput<1>() << std::endl;

  // Test VariadicNamedInputsImageFilter
  struct xs
  {
  };
  struct pan
  {
  };
  using Names          = std::tuple<xs, pan>;
  auto filterWithNames = otb::VariadicNamedInputsImageFilter<VectorImageType, Names, VectorImageType, ImageType>::New();
  filterWithNames->SetInput<xs>(vimage);

  // Test FunctorImageFilter with a built-in math function
  using CosType  = double(double);
  auto filterCos = NewFunctorFilter(static_cast<CosType*>(std::cos));
  filterCos->SetInputs(image);
  filterCos->Update();

  filterWithNames->SetInput<pan>(image);

  std::cout << filterWithNames->GetInput<xs>() << filterWithNames->GetInput<pan>() << std::endl;


  // test FunctorImageFilter with a lambda
  double scale                               = 10.;
  auto Lambda1                               = [scale](double p) { return scale * p; };
  auto                          filterLambda = NewFunctorFilter(Lambda1);
  filterLambda->SetInputs(image);
  filterLambda->Update();

  // test FunctorImageFilter with a lambda that returns a
  // VariableLengthVector
  // Converts a neighborhood to a VariableLengthVector
  auto Lambda2 = [](const itk::ConstNeighborhoodIterator<otb::Image<double>>& in) {
    itk::VariableLengthVector<double> out(in.Size());
    for (size_t idx = 0; idx < in.Size(); idx++)
    {
      out[idx] = in.GetPixel(idx);
    }
    return out;
  };

  // In this case, we use the helper function which allows specifying
  // the number of outputs
  auto filterLambda2 = NewFunctorFilter(Lambda2, vimage->GetNumberOfComponentsPerPixel(), {{3, 3}});
  filterLambda2->SetInputs(image);
  filterLambda2->Update();

  // Test FunctorImageFilter with the VariadicConcatenate operator
  using ConcatFunctorType = Functor::VariadicConcatenate<double, double, itk::VariableLengthVector<double>>;
  auto concatenate        = NewFunctorFilter(ConcatFunctorType{});
  concatenate->SetInputs(image, vimage);
  concatenate->Update();

  // Test FunctorImageFilter With VariadicAdd functor
  using AddFunctorType = Functor::VariadicAdd<double, double, double>;
  auto add             = NewFunctorFilter(AddFunctorType{});
  add->SetInput<0>(image);
  add->SetInput<1>(image);
  add->Update();

  // Test FunctorImageFilter with BandExtraction functor
  using ExtractFunctorType = BandExtraction<double, double>;
  ExtractFunctorType extractFunctor{1, 2};
  auto               extract = NewFunctorFilter(extractFunctor);
  extract->SetInputs(vimage);
  extract->Update();

  // Test FunctorImageFilter With Mean functor
  using MeanFunctorType = Mean<double, double>;
  auto median           = NewFunctorFilter(MeanFunctorType{}, {{2, 2}});
  median->SetInputs(image);
  median->Update();

  // Test FunctorImageFilter with MaxInEachChannel
  using MaxInEachChannelType = MaxInEachChannel<double>;
  auto maxInEachChannel      = NewFunctorFilter(MaxInEachChannelType{}, {{3, 3}});
  maxInEachChannel->SetInputs(vimage);
  maxInEachChannel->Update();

  // Test FunctorImageFilter with Module (complex=
  using ModulusType = VectorModulus<double>;
  auto modulus      = NewFunctorFilter(ModulusType{});
  modulus->SetInputs(cvimage);
  modulus->Update();

  auto LambdaComplex                                            = [](const std::complex<double>& in) { return std::arg(in); };
  auto                                                argFilter = NewFunctorFilter(LambdaComplex);
  argFilter->SetInputs(cimage);
  argFilter->Update();

  return EXIT_SUCCESS;
}
