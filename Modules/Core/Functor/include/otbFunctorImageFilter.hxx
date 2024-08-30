/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFunctorImageFilter_hxx
#define otbFunctorImageFilter_hxx

#include "otbFunctorImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageScanlineIterator.h"
#include <array>

namespace otb
{
namespace functor_filter_details
{
// Variadic SetRequestedRegion

// This function sets the requested region for one image
template <class T>
int SetInputRequestedRegion(const T* img, const itk::ImageRegion<2>& region, const itk::Size<2>& radius, bool pad)
{
  assert(img && "Input image is a nullptr");

  auto currentRegion = region;

  // Hopefully this will be optimized out by compiler
  if (pad)
    currentRegion.PadByRadius(radius);

  // The ugly cast in all ITK filters
  T* nonConstImg = const_cast<T*>(img);

  if (currentRegion.GetNumberOfPixels()==0 || currentRegion.Crop(img->GetLargestPossibleRegion()))
  {
    nonConstImg->SetRequestedRegion(currentRegion);
    return 0;
  }
  else
  {
    nonConstImg->SetRequestedRegion(currentRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation("::SetInputRequestedRegion<>()");
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(nonConstImg);
    throw e;
  }
}

// Will be easier to write in c++17 with std::apply and fold expressions
template <typename HasNeighborhood, class Tuple, size_t... Is>
auto SetInputRequestedRegionsImpl(Tuple& t, const itk::ImageRegion<2>& region, std::index_sequence<Is...>, const itk::Size<2>& radius)
{
  return std::make_tuple(SetInputRequestedRegion(std::get<Is>(t), region, radius, typename std::tuple_element<Is, HasNeighborhood>::type::value_type())...);
}

// Will be easier to write in c++17 with std::apply and fold expressions
template <typename HasNeighborhood, typename... T>
auto SetInputRequestedRegions(std::tuple<T...>&& t, const itk::ImageRegion<2>& region, const itk::Size<2>& radius)
{
  return SetInputRequestedRegionsImpl<HasNeighborhood>(t, region, std::make_index_sequence<sizeof...(T)>{}, radius);
}

// Will be easier to write in c++17 with std::apply and fold expressions
template <class Tuple, size_t... Is>
auto GetNumberOfComponentsPerInputImpl(Tuple& t, std::index_sequence<Is...>)
{
  return std::array<size_t, sizeof...(Is)>{{std::get<Is>(t)->GetNumberOfComponentsPerPixel()...}};
}

// Will be easier to write in c++17 with std::apply and fold expressions
template <typename... T>
auto GetNumberOfComponentsPerInput(std::tuple<T...>& t)
{
  return GetNumberOfComponentsPerInputImpl(t, std::make_index_sequence<sizeof...(T)>{});
}

template <typename N>
struct MakeIterator
{
};

template <>
struct MakeIterator<std::false_type>
{
  template <class T>
  static auto Make(const T* img, const itk::ImageRegion<2>& region, const itk::Size<2>&)
  {
    itk::ImageRegionConstIterator<T> it(img, region);
    return it;
  }
};

template <>
struct MakeIterator<std::true_type>
{
  template <class T>
  static auto Make(const T* img, const itk::ImageRegion<2>& region, const itk::Size<2>& radius)
  {
    itk::ConstNeighborhoodIterator<T> it(radius, img, region);
    return it;
  }
};

// Will be easier to write in c++17 with std::apply and fold expressions
template <class TNeigh, class Tuple, size_t... Is>
auto MakeIteratorsImpl(const Tuple& t, const itk::ImageRegion<2>& region, const itk::Size<2>& radius, std::index_sequence<Is...>, TNeigh)
{
  return std::make_tuple(MakeIterator<typename std::tuple_element<Is, TNeigh>::type>::Make(std::get<Is>(t), region, radius)...);
}

// Will be easier to write in c++17 with std::apply and fold expressions
template <class TNeigh, typename... T>
auto MakeIterators(std::tuple<T...>&& t, const itk::ImageRegion<2>& region, const itk::Size<2>& radius, TNeigh n)
{
  return MakeIteratorsImpl(t, region, radius, std::make_index_sequence<sizeof...(T)>{}, n);
}

// Variadic call of operator from iterator tuple
template <typename T>
struct GetProxy
{
};


template <typename T>
struct GetProxy<itk::ImageRegionConstIterator<T>>
{
  static decltype(auto) Get(const itk::ImageRegionConstIterator<T>& t)
  {
    return t.Get();
  }
};

template <typename T>
struct GetProxy<itk::ConstNeighborhoodIterator<T>>
{
  static decltype(auto) Get(const itk::ConstNeighborhoodIterator<T>& t)
  {
    return t;
  }
};

/// Proxy for operator (dispatch between void operator()(Out &
/// out,...) and Out operator()(...)
template <class Oper>
struct OperProxy : public OperProxy<typename RetrieveOperator<Oper>::Type>
{
};

template <class Out, class... In>
struct OperProxy<Out (*)(In...)>
{
  template <class Oper>
  static void Compute(Oper& oper, Out& out, const In&... in)
  {
    out = oper(in...);
  }
};

template <class C, class Out, class... In>
struct OperProxy<Out (C::*)(In...)>
{
  template <class Oper>
  static void Compute(Oper& oper, Out& out, const In&... in)
  {
    out = oper(in...);
  }
};

template <class C, class Out, class... In>
struct OperProxy<Out (C::*)(In...) const>
{
  template <class Oper>
  static void Compute(Oper& oper, Out& out, const In&... in)
  {
    out = oper(in...);
  }
};

template <class Out, class... In>
struct OperProxy<void (*)(Out&, In...)>
{
  template <class Oper>
  static void Compute(Oper& oper, Out& out, const In&... in)
  {
    oper(out, in...);
  }
};

template <class C, class Out, class... In>
struct OperProxy<void (C::*)(Out&, In...)>
{
  template <class Oper>
  static void Compute(Oper& oper, Out& out, const In&... in)
  {
    oper(out, in...);
  }
};

template <class C, class Out, class... In>
struct OperProxy<void (C::*)(Out&, In...) const>
{
  template <class Oper>
  static void Compute(Oper& oper, Out& out, const In&... in)
  {
    oper(out, in...);
  }
};


// Will be easier to write in c++17 with std::apply and fold expressions
template <class Tuple, class Out, class Oper, size_t... Is>
auto CallOperatorImpl(Tuple& t, Out& out, Oper& oper, std::index_sequence<Is...>)
{
  OperProxy<Oper>::Compute(oper, out, GetProxy<typename std::remove_reference<decltype(std::get<Is>(t))>::type>::Get(std::get<Is>(t))...);
}

// Will be easier to write in c++17 with std::apply and fold expressions
template <class Out, class Oper, typename... Args>
auto CallOperator(Out& out, Oper& oper, std::tuple<Args...>& t)
{
  CallOperatorImpl(t, out, oper, std::make_index_sequence<sizeof...(Args)>{});
}

// Variadic move of iterators
// Will be easier to write in c++17 with std::apply and fold expressions
template <class Tuple, size_t... Is>
auto MoveIteratorsImpl(Tuple& t, std::index_sequence<Is...>)
{
  return std::make_tuple(++(std::get<Is>(t))...);
}

template <typename... Args>
void MoveIterators(std::tuple<Args...>& t)
{
  MoveIteratorsImpl(t, std::make_index_sequence<sizeof...(Args)>{});
}


// Default implementation does nothing
template <class F, class O, size_t N>
struct NumberOfOutputComponents
{
};

template <class F, class T, size_t N>
struct NumberOfOutputComponents<F, otb::Image<T>, N>
{
  // We can not be here if output type is VectorImage
  static void Set(const F&, otb::Image<T>*, std::array<size_t, N>)
  {
  }
};

// O is a VectorImage AND F has a fixed OutputSize static constexrp size_t;
template <class F, class T, size_t N>
struct NumberOfOutputComponents<F, otb::VectorImage<T>, N>
{
  static void Set(const F& f, otb::VectorImage<T>* outputImage, std::array<size_t, N> inNbBands)
  {
    outputImage->SetNumberOfComponentsPerPixel(f.OutputSize(inNbBands));
  }
};

} // end namespace functor_filter_details

template <class TFunction, class TNameMap>
void FunctorImageFilter<TFunction, TNameMap>::GenerateInputRequestedRegion()
{
  // Get requested region for output
  typename Superclass::OutputImagePointer outputPtr       = this->GetOutput();
  auto                                    requestedRegion = outputPtr->GetRequestedRegion();

  // Propagate to each variadic inputs, including possible radius
  // TODO: For now all inputs are padded with the radius, even if they
  // are not neighborhood based
  functor_filter_details::SetInputRequestedRegions<InputHasNeighborhood>(this->GetInputs(), requestedRegion, m_Radius);
}

template <class TFunction, class TNameMap>
void FunctorImageFilter<TFunction, TNameMap>::GenerateOutputInformation()
{
  // Call Superclass implementation
  Superclass::GenerateOutputInformation();

  // Get All variadic inputs
  auto inputs = this->GetInputs();

  // Retrieve an array of number of components per input
  auto inputNbComps = functor_filter_details::GetNumberOfComponentsPerInput(inputs);

  // Call the helper to set the number of components for the output image
  functor_filter_details::NumberOfOutputComponents<TFunction, OutputImageType, inputNbComps.size()>::Set(m_Functor, this->GetOutput(), inputNbComps);
}

/**
 * ThreadedGenerateData Performs the neighborhood-wise operation
 */
template <class TFunction, class TNameMap>
void FunctorImageFilter<TFunction, TNameMap>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  const auto& regionSize = outputRegionForThread.GetSize();

  if (regionSize[0] == 0)
  {
    return;
  }
  const auto            numberOfLinesToProcess = outputRegionForThread.GetNumberOfPixels() / regionSize[0];
  itk::ProgressReporter p(this, threadId, numberOfLinesToProcess);

  // Build output iterator
  itk::ImageScanlineIterator<OutputImageType> outIt(this->GetOutput(), outputRegionForThread);

  // This will build a tuple of iterators to be used
  auto inputIterators = functor_filter_details::MakeIterators(this->GetInputs(), outputRegionForThread, m_Radius, InputHasNeighborhood{});

  // Build a default value
  typename OutputImageType::PixelType outputValueHolder;
  itk::NumericTraits<typename OutputImageType::PixelType>::SetLength(outputValueHolder, this->GetOutput()->GetNumberOfComponentsPerPixel());

  while (!outIt.IsAtEnd())
  {
    // MoveIterartors will ++ all iterators in the tuple
    for (; !outIt.IsAtEndOfLine(); ++outIt, functor_filter_details::MoveIterators(inputIterators))
    {
      // This will call the operator with inputIterators Get() results
      // and fill outputValueHolder with the result.
      functor_filter_details::CallOperator(outputValueHolder, m_Functor, inputIterators);
      outIt.Set(outputValueHolder);
    }
    outIt.NextLine();
    p.CompletedPixel(); // may throw
  }
}

} // end namespace otb

#endif
