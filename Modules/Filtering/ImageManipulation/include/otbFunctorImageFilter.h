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

#ifndef otbFunctorImageFilter_h
#define otbFunctorImageFilter_h

#include "otbVariadicInputsImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#include <type_traits>
#include <utility>

namespace otb
{

/**  
 * \struct IsNeighborhood 
 * Struct testing if T is a neighborhood
 * Provides:
 * - ValueType type set to false_type or true_type
 * - value set to true or false
 * - PixelType type to the underlying pixel type
 */
template <class T, class Enable = void> struct IsNeighborhood{};

/// Partial specialisation for scalar types
template <class T> struct IsNeighborhood<T,typename std::enable_if<std::is_scalar<typename std::remove_reference<typename std::remove_cv<T>::type>::type>::value >::type>
{
  using ValueType = std::false_type;
  static constexpr bool value = false;
  using PixelType = T;
};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct IsNeighborhood<itk::Neighborhood<T>>
{
  using ValueType = std::true_type;
  static constexpr bool value = true;
  using PixelType = T;
};

/// Partial specialisation for const itk::Neighborhood<T> &
template <class T> struct IsNeighborhood<const itk::Neighborhood<T>&>
{
  using ValueType = std::true_type;
  static constexpr bool value = true;
  using PixelType = T;
};

/// Partial specialisation for itk::VariableLengthVector<T>
template <class T> struct IsNeighborhood<itk::VariableLengthVector<T>>
{
  using ValueType = std::false_type;
  static constexpr bool value = false;
  using PixelType = itk::VariableLengthVector<T>;
};

/// Partial specialisation for const itk::VariableLengthVector<T> &
template <class T> struct IsNeighborhood<const itk::VariableLengthVector<T>&>
{
  using ValueType = std::false_type;
  static constexpr bool value = false;
  using PixelType = itk::VariableLengthVector<T>;
};

/**
 * \struct InputImageTraits
 * Struct allowing to derive input image types from operator()
 * arguments
 * Defines:
 * - PixelType type to the underlying pixel type
 * - ScalarType type to the underlying scalar type
 * - ImageType type to the mocked up image type
 *
 * ImageType = Image<T> for T or itk::Neighborhood<T>
 * ImageType = VectorImage<T> for itk::VariableLengthVector<T> or itk::Neighborhood<itk::VariableLengthVector<T>>
 */
template<typename T>
struct InputImageTraits
{
  using ArgumentType = T;
  using PixelType = typename IsNeighborhood<typename std::remove_cv<typename std::remove_reference< ArgumentType>::type >::type>::PixelType;
  using ScalarType = typename itk::DefaultConvertPixelTraits<PixelType>::ComponentType;
  using ImageType = typename std::conditional<std::is_scalar<PixelType>::value,
                                     otb::Image< ScalarType >,
                                     otb::VectorImage< ScalarType > >::type;
};

/**
 * \struct OutputImageTraits
 * Struct allowing to derive output image type from operator()
 * Defines:
 * - ScalarType type to the underlying scalar type
 * - ImageType type to the mocked up image type
 * - ImageType = Image<T> for T
 * - ImageType = VectorImage<T> for itk::VariableLengthVector<T>
 */
template<typename T>
struct OutputImageTraits
{
  using ResultType = T;
  using ScalarType = typename itk::DefaultConvertPixelTraits<ResultType>::ComponentType;
  using ImageType =  typename std::conditional<std::is_scalar<ResultType>::value,
                                     typename otb::Image<ScalarType>,
                                     typename otb::VectorImage<ScalarType> >::type;
};


/**
* \struct FunctorFilterSuperclassHelper 
* Struct allowing to derive the superclass prototype for the
* FunctorImageFilter class
* Provides the following:
* - OutputImageType : type of the output image
* - FilterType : correct instanciation of VariadicInputsImageFilter from
* - the operator() prototype
* - InputHasNeighborhood a tuple of N false_type or true_type to denote
* - if Ith arg of operator() expects a neighborhood.
*/
template <typename T> struct FunctorFilterSuperclassHelper : public FunctorFilterSuperclassHelper<decltype(&T::operator())> {};

/// Partial specialisation for R(*)(T...)
template <typename R, typename... T> struct FunctorFilterSuperclassHelper<R(*)(T...)>
{
  using OutputImageType = typename OutputImageTraits<R>::ImageType;
  using FilterType = VariadicInputsImageFilter<OutputImageType,typename InputImageTraits<T>::ImageType...>;
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::ValueType...>;
};

// Partial specialisation for R(C::*)(T...) const
template <typename C, typename R, typename... T> struct FunctorFilterSuperclassHelper<R(C::*)(T...) const>
{
  using OutputImageType = typename OutputImageTraits<R>::ImageType;
  using FilterType = VariadicInputsImageFilter<OutputImageType,typename InputImageTraits<T>::ImageType...>;
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::ValueType...>;
};

// Partial specialisation for R(C::*)(T...)
template <typename C, typename R, typename... T> struct FunctorFilterSuperclassHelper<R(C::*)(T...)>
{
  using OutputImageType = typename OutputImageTraits<R>::ImageType;
  using FilterType = VariadicInputsImageFilter<OutputImageType,typename InputImageTraits<T>::ImageType...>;
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::ValueType...>;
};

/**
 * This helper method builds a fully functional FunctorImageFilter from a functor instance
 * 
 * Functor can be any operator() that matches the following:
 * - Accepts any number of arguments of T,
 * (const) itk::VariableLengthVector<T> (&),(const)
 * itk::Neighborhood<T> (&), (const)
 * itk::Neighborhood<itk::VariableLengthVector<T>> (&) with T a scalar type
 * - returns T or itk::VariableLengthVector<T>, with T a scalar type
 *
 * The returned filter is ready to use. Inputs can be set through the
 * SetVInputs() method (see VariadicInputsImageFilter class for
 * details)
 * 
 * \param f the Functor to build the filter from
 * \param radius The size of neighborhood to use, if there is any
 * itk::Neighborhood<T> in the operator() arguments.
 * \return A ready to use OTB filter, which accepts n input image of
 * type derived from the operator() arguments, and producing an image
 * correpsonding to the operator() return type.
 *
 * Note that this function also works with a lambda as Functor,
 * provided it returns a scalar type. If your lambda returns a
 * VariableLengthVector, see the other NewFunctorFilter implementation.
 */
template <typename Functor> auto NewFunctorFilter(const Functor& f, itk::Size<2> radius = {{0,0}});


/** \class FunctorImageFilter
 * \brief TODO
 *
 * 
 *
 * \ingroup IntensityImageFilters   Multithreaded Streamed
 *
 * \ingroup OTBImageManipulation
*/
template <class TFunction>
    class ITK_EXPORT FunctorImageFilter
  : public FunctorFilterSuperclassHelper<TFunction>::FilterType
{

public:
  using Self = FunctorImageFilter;
  using FunctorType = TFunction;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  using Superclass = typename FunctorFilterSuperclassHelper<TFunction>::FilterType;
  using OutputImageType = typename Superclass::OutputImageType;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  
  using ProcessObjectType = itk::ProcessObject;

  using InputHasNeighborhood = typename FunctorFilterSuperclassHelper<TFunction>::InputHasNeighborhood;
 
/** Run-time type information (and related methods). */
  itkTypeMacro(FunctorImageFilter, ImageToImageFilter);
  
  
/** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  FunctorType& GetFunctor()
  {
    this->Modified();
    return m_Functor;
  }

/** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer.) */
  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  }

private:
  /// Actual creation of the filter is handled by this free function
  friend auto NewFunctorFilter<TFunction>(const TFunction& f, itk::Size<2> radius);

  /// Constructor of functor filter, will copy the functor
  FunctorImageFilter(const FunctorType& f, itk::Size<2> radius) : m_Functor(f), m_Radius(radius) {};
  FunctorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  ~FunctorImageFilter() = default;

/** Overload of ThreadedGenerateData  */
virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  /**
   * Pad the input requested region by radius
   */
  virtual void GenerateInputRequestedRegion(void) override;

  /**
   * Will use the OutputSize() method if
   */
  virtual void GenerateOutputInformation() override;


  // The functor member
  FunctorType m_Functor;

  // Radius if needed
  itk::Size<2> m_Radius;
};

template <typename Functor> auto NewFunctorFilter(const Functor& f, itk::Size<2> radius)
{
  using FilterType = FunctorImageFilter<Functor>;
  using PointerType = typename FilterType::Pointer;

  PointerType  p = new FilterType(f,radius);
  return p;
}


template <typename F> struct NumberOfOutputBandsDecorator : F
{
public:
  NumberOfOutputBandsDecorator(const F t, unsigned int nbComp) : F(t), m_NumberOfOutputComponents(nbComp) {}
  using F::operator();

  constexpr size_t OutputSize(...) const
  {
    return m_NumberOfOutputComponents;
  }

private:
  unsigned int m_NumberOfOutputComponents;
};


template <typename Functor> auto NewFunctorFilter(const Functor& f, unsigned int numberOfOutputBands, itk::Size<2> radius)
{
  using FunctorType = NumberOfOutputBandsDecorator<Functor>;
  FunctorType decoratedF(f,numberOfOutputBands);
  return  NewFunctorFilter(decoratedF,radius);
}


}// namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFunctorImageFilter.hxx"
#endif

#endif
