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

#include "otbVariadicNamedInputsImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkFixedArray.h"
#include "itkDefaultConvertPixelTraits.h"
#include <type_traits>

namespace otb
{
/**  
 * \struct IsNeighborhood 
 * \brief Struct testing if T is a neighborhood
 * 
 * Provides:
 * - ValueType type set to false_type or true_type
 * - value set to true or false
 */
template <class T> struct IsNeighborhood : std::false_type {};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct IsNeighborhood<itk::Neighborhood<T>> : std::true_type {};


/// Partial specialisation for const itk::Neighborhood<T> &
template <class T> struct IsNeighborhood<const itk::Neighborhood<T>&> : std::true_type {};

/**
 * \struct IsSuitableType
 * \brief Helper struct to check if a type can be used as pixel type.
 *
 * ::value maps to true if type can be used and false otherwhise.
 */
template <class T> struct IsSuitableType : std::is_scalar<T>::type {};

/// Unwrap complex
template <class T> struct IsSuitableType<std::complex<T>> : IsSuitableType<T>::type {};

/// Unwrap VariableLengthVector
template <class T> struct IsSuitableType<itk::VariableLengthVector<T>> : IsSuitableType<T>::type {};

/// Unwrap FixedArray
template <class T, unsigned int N> struct IsSuitableType<itk::FixedArray<T,N>> : IsSuitableType<T>::type {};

/// Unwrap RGBPixel
template <class T> struct IsSuitableType<itk::RGBPixel<T>> : IsSuitableType<T>::type {};

/// Unwrap RGBAPixel
template <class T> struct IsSuitableType<itk::RGBAPixel<T>> : IsSuitableType<T>::type {};

/**
 * \struct PixelTypeDeduction
 * \brief Helper struct to derive PixelType from template parameter.
 * 
 * T                           -> PixelType = T
 * itk::Neighborhood<T>        -> PixelType = T
 * const itk::Neighborhood<T>& -> PixelType = T
*/
template <class T> struct PixelTypeDeduction
{
  static_assert(IsSuitableType<T>::value,
                "T can not be used as a template parameter for Image or VectorImage classes.");
  using PixelType = T;
};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct PixelTypeDeduction<itk::Neighborhood<T>>
{
  static_assert(IsSuitableType<T>::value,
                "T can not be used as a template parameter for Image or VectorImage classes.");
  using PixelType = T;
};

/** 
 * \struct ImageTypeDeduction
 * \brief Helper struct to derive ImageType from template parameter
 * 
 * T                            -> ImageType = otb::Image<T>
 * itk::VariableLengthVector<T> -> ImageType = otb::VectorImage<T>
 * const T &                    -> ImageType = ImageTypeDeduction<T>::ImageType
 */
template <class T> struct ImageTypeDeduction 
{
  using ImageType = otb::Image<T>;
};

/// Partial specialisation for itk::VariableLengthVector<T>
template <class T> struct ImageTypeDeduction<itk::VariableLengthVector<T>>
{
  using ImageType = otb::VectorImage<T>;
};

// Helper to remove const, volatite and Ref qualifier (until c++20
/// that has std::remove_cvref)
template <typename T> using RemoveCVRef = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

/**
* \struct RetrieveOperator
*
* \brief Struct to retrieve the operator type
* 
* \tparam T the type to retrieve operator() from
*
*/
template <typename T> struct RetrieveOperator
{
  static_assert(std::is_class<T>::value || std::is_function<T>::value, "T is not a class or function");
  using Type = decltype(&T::operator());
};

/**
* \struct FunctorFilterSuperclassHelper 
* \brief Struct allowing to derive the superclass prototype for the
*        FunctorImageFilter class
*
* Provides the following:
* - OutputImageType : type of the output image
* - FilterType : correct instanciation of VariadicInputsImageFilter from
* - the operator() prototype
* - InputHasNeighborhood a tuple of N false_type or true_type to denote
* - if Ith arg of operator() expects a neighborhood.
*/
template <typename T, typename TNameMap> struct FunctorFilterSuperclassHelper : public FunctorFilterSuperclassHelper<typename RetrieveOperator<T>::Type,TNameMap> {};

namespace functor_filter_details
{
template <typename R, typename TNameMap, typename...T> struct FunctorFilterSuperclassHelperImpl
{
  // OutputImageType is derived from return type R
  using OutputImageType = typename ImageTypeDeduction<R>::ImageType;
  // InputImageType is derived using pixel type deduction and image
  // type deduction
  template <typename V> using InputImageType = typename ImageTypeDeduction<typename PixelTypeDeduction<RemoveCVRef<V>>::PixelType>::ImageType;

  // Filter type is either VariadicInputsImageFilter or
  // VariadicNamedInputsImageFilter depending on if there is a
  // TNameMap or not
  using FilterType = typename std::conditional<std::is_void<TNameMap>::value,
                                               VariadicInputsImageFilter<OutputImageType,InputImageType<T>...>,
                                               VariadicNamedInputsImageFilter<OutputImageType,TNameMap,InputImageType<T>...>>::type;

  // InputHasNeighborhood is derived from IsNeighborhood
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::type...>;
};
} // End namespace functor_filter_details

/// Partial specialisation for R(*)(T...)
template <typename R, typename... T, typename TNameMap> struct FunctorFilterSuperclassHelper<R(*)(T...),TNameMap>
{
  using OutputImageType      = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::OutputImageType;
  using FilterType           = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::FilterType;
  using InputHasNeighborhood = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::InputHasNeighborhood;
};

/// Partial specialisation for R(C::*)(T...) const
template <typename C, typename R, typename... T, typename TNameMap> struct FunctorFilterSuperclassHelper<R(C::*)(T...) const, TNameMap>
{
  using OutputImageType      = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::OutputImageType;
  using FilterType           = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::FilterType;
  using InputHasNeighborhood = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::InputHasNeighborhood;

};

/// Partial specialisation for R(C::*)(T...)
template <typename C, typename R, typename... T, typename TNameMap> struct FunctorFilterSuperclassHelper<R(C::*)(T...), TNameMap>
{
  using OutputImageType      = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::OutputImageType;
  using FilterType           = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::FilterType;
  using InputHasNeighborhood = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::InputHasNeighborhood;

};

/// Partial specialisation for void(*)(R &,T...)
template <typename R, typename... T, typename TNameMap> struct FunctorFilterSuperclassHelper<void(*)(R&, T...), TNameMap>
{
  using OutputImageType      = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::OutputImageType;
  using FilterType           = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::FilterType;
  using InputHasNeighborhood = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::InputHasNeighborhood;

};

/// Partial specialisation for void(C::*)(R&,T...) const
template <typename C, typename R, typename... T, typename TNameMap> struct FunctorFilterSuperclassHelper<void(C::*)(R&,T...) const, TNameMap>
{
  using OutputImageType      = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::OutputImageType;
  using FilterType           = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::FilterType;
  using InputHasNeighborhood = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::InputHasNeighborhood;

};

/// Partial specialisation for void(C::*)(R&,T...)
template <typename C, typename R, typename... T, typename TNameMap> struct FunctorFilterSuperclassHelper<void(C::*)(R&,T...), TNameMap>
{ 
  using OutputImageType      = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::OutputImageType;
  using FilterType           = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::FilterType;
  using InputHasNeighborhood = typename functor_filter_details::FunctorFilterSuperclassHelperImpl<R,TNameMap,T...>::InputHasNeighborhood;
};



/**
 * \brief This helper method builds a fully functional FunctorImageFilter from a functor instance
 * 
 * \tparam Functor can be any operator() (const or non-const) that matches the following:
 * - Accepts any number of arguments of T,
 * (const) itk::VariableLengthVector<T> (&),(const)
 * itk::Neighborhood<T> (&), (const)
 * itk::Neighborhood<itk::VariableLengthVector<T>> (&) with T a scalar type
 * - returns T or itk::VariableLengthVector<T>, with T a scalar type
 * or returns void and has first parameter as output (i.e. T& or itk::VariableLengthVector<T>&)
 *
 * The returned filter is ready to use. Inputs can be set through the
 * SetVariadicInputs() method (see VariadicInputsImageFilter class for
 * details)
 * 
 * \param[in] the Functor to build the filter from
 * \param[in] radius The size of neighborhood to use, if there is any
 * itk::Neighborhood<T> in the operator() arguments.
 * \return A ready to use OTB filter, which accepts n input image of
 * type derived from the operator() arguments, and producing an image
 * correpsonding to the operator() return type.
 *
 * Note that this function also works with a lambda as Functor,
 * provided it returns a scalar type. If your lambda returns a
 * VariableLengthVector, see the other NewFunctorFilter implementation.
 */
template <typename Functor, typename TNameMap = void> auto NewFunctorFilter(Functor f, itk::Size<2> radius = {{0,0}});


/** \class FunctorImageFilter
 * \brief A generic functor filter templated by its functor
 * 
 * \tparam TFunction can be any operator() (const or non-const) that matches the following:
 * - Accepts any number of arguments of T,
 * (const) itk::VariableLengthVector<T> (&),(const)
 * itk::Neighborhood<T> (&), (const)
 * itk::Neighborhood<itk::VariableLengthVector<T>> (&) with T a scalar type
 * - returns T or itk::VariableLengthVector<T>, with T a scalar type
 * or returns void and has first parameter as output (i.e. T& or itk::VariableLengthVector<T>&)
 *
 * All image types will be deduced from the TFunction operator().
 *
 * \sa VariadicInputsImageFilter
 * \sa NewFunctorFilter
 * 
 * \ingroup IntensityImageFilters   Multithreaded Streamed
 *
 * \ingroup OTBFunctor
*/
template <class TFunction, class TNameMap = void>
    class ITK_EXPORT FunctorImageFilter
  : public FunctorFilterSuperclassHelper<TFunction, TNameMap>::FilterType
{

public:
  // Standard typedefs
  using Self = FunctorImageFilter;
  using FunctorType = TFunction;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  // Superclass through the helper struct
  using SuperclassHelper = FunctorFilterSuperclassHelper<TFunction,TNameMap>;
  using Superclass = typename SuperclassHelper::FilterType;
  using OutputImageType = typename Superclass::OutputImageType;
  using OutputImageRegionType = typename OutputImageType::RegionType;

  // A tuple of bool of the same size as the number of arguments in
  // the functor
  using InputHasNeighborhood = typename SuperclassHelper::InputHasNeighborhood;
  using InputTypesTupleType = typename Superclass::InputTypesTupleType;
  template<size_t I> using InputImageType = typename Superclass::template InputImageType<I>;
  using Superclass::NumberOfInputs;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FunctorImageFilter, VariadicInputsImageFilter);

  /** New() macro defined only if TFunction is default constructible */
  template <typename F = TFunction> static std::enable_if_t<std::is_default_constructible<F>::value, Pointer> New()
  {
    // Explicit default construct
    FunctorType f;

    // Create a filter out of it
    Pointer  p = new Self(f, {{0,0}});
    p->UnRegister();
    return p;
  }

    /** New() macro defined only if TFunction is NOT default constructible
     * This will yield an error message since New() can not be implemented in this case. */
  template <typename F = TFunction> static std::enable_if_t<!std::is_default_constructible<F>::value, Pointer> New()
  {
    static_assert(std::is_default_constructible<F>::value,"Cannot call New() "
      "function as the functor used for the filter creation is not default "
      "constructible");

    return nullptr;
  }
  
  /** Get the functor object.
   * 
   *  \return A non-const reference to the Functor in use. Note that
   *  this call will call the Modified() function of the filter, which
   *  will trigger a full output computation upon Update(). For
   *  inspection only, prefer using GetFunctor()
   */
  FunctorType& GetModifiableFunctor()
  {
    this->Modified();
    return m_Functor;
  }

  /** Get the functor object.  
   * 
   *  \return A const reference to the Functor in use. For a
   *  modifiable version of the functor, see GetModifiableFunctor().
   */
  const FunctorType& GetFunctor() const
  {
    return m_Functor;
  }

protected:
  /// Constructor of functor filter, will copy the functor
  FunctorImageFilter(const FunctorType& f, itk::Size<2> radius) : m_Functor(f), m_Radius(radius) {};
  FunctorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  ~FunctorImageFilter() = default;

private:
  /// Actual creation of the filter is handled by this free function
  friend auto NewFunctorFilter<TFunction,TNameMap>(TFunction f, itk::Size<2> radius);

  /** Overload of ThreadedGenerateData  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  /**
   * Pad the input requested region by radius
   */
  void GenerateInputRequestedRegion(void) override;

  /**
   * Will use the OutputSize() method of the functor if output is VectorImage
   */
  void GenerateOutputInformation() override;


  // The functor member
  FunctorType m_Functor;

  // Radius if needed
  itk::Size<2> m_Radius;
};

// Actual implementation of NewFunctorFilter free function
template <typename Functor, typename TNameMap> auto NewFunctorFilter(Functor f, itk::Size<2> radius)
{
  using FilterType = FunctorImageFilter<Functor,TNameMap>;
  using PointerType = typename FilterType::Pointer;

  PointerType  p = new FilterType(f,radius);
  p->UnRegister();
  return p;
}

/** 
 * \struct NumberOfOutputBandsDecorator
 * \brief This struct allows to forward the operator of template
 *        parameter, while adding number of ouptut components service.
 * 
 * Its purpose is to enable the use of lambda or functor witht
 * Outputsize() method with FunctorImageFilter.
 * 
 * It is used internally in NewFunctorFilter version with
 * numberOfOutputBands parameter.
 */ 
template <typename F> struct NumberOfOutputBandsDecorator : public F
{
public:
  constexpr NumberOfOutputBandsDecorator(F t, unsigned int nbComp) : F(t), m_NumberOfOutputBands(nbComp) {}

  constexpr size_t OutputSize(...) const
  {
    return m_NumberOfOutputBands;
  }

private:
  unsigned int m_NumberOfOutputBands;
};

/** 
 * brief This helper method builds a fully functional
 * FunctorImageFilter from a functor instance which does not provide
 * the OutputSize() service, or a lambda, returing a VariableLengthVector
 * 
 * \param[in] f the Functor to build the filter from
 * \param[in] numberOfOutputBands The number of output bands that
 * this filter will return
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

template <typename Functor, typename TNameMap = void> auto NewFunctorFilter(Functor f, unsigned int numberOfOutputBands, itk::Size<2> radius)
{
  using FunctorType = NumberOfOutputBandsDecorator<Functor>;
  FunctorType decoratedF(f,numberOfOutputBands);
  return  NewFunctorFilter<FunctorType,TNameMap>(decoratedF,radius);
}

}// namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFunctorImageFilter.hxx"
#endif

#endif
