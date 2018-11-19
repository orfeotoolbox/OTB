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
template <class T> struct IsNeighborhood
{
  using ValueType = std::false_type;
  static constexpr bool value = false;
};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct IsNeighborhood<itk::Neighborhood<T>>
{
  using ValueType = std::true_type;
  static constexpr bool value = true;
};


/// Partial specialisation for const itk::Neighborhood<T> &
template <class T> struct IsNeighborhood<const itk::Neighborhood<T>&>
{
  using ValueType = std::true_type;
  static constexpr bool value = true;
};

/**
 * \struct IsSuitableType
 * \brief Helper struct to check if a type can be used as pixel type.
 *
 * ::value maps to true if type can be used and false otherwhise.
 */
template <class T> struct IsSuitableType
{
  static constexpr bool value = std::is_scalar<T>::value;
};

/// Unwrap complex
template <class T> struct IsSuitableType<std::complex<T>> {
  static constexpr bool value = IsSuitableType<T>::value;
};

/// Unwrap VariableLengthVector
template <class T> struct IsSuitableType<itk::VariableLengthVector<T>> {
  static constexpr bool value = IsSuitableType<T>::value;
};

/// Unwrap FixedArray
template <class T, size_t N> struct IsSuitableType<itk::FixedArray<T,N>> {
  static constexpr bool value = IsSuitableType<T>::value;
};

/// Unwrap RGBPixel
template <class T> struct IsSuitableType<itk::RGBPixel<T>> {
  static constexpr bool value = IsSuitableType<T>::value;
};

/// Unwrap RGBAPixel
template <class T> struct IsSuitableType<itk::RGBAPixel<T>> {
  static constexpr bool value = IsSuitableType<T>::value;
};

/// Discard const qualifier
template <class T> struct IsSuitableType<const T>{
  static constexpr bool value = IsSuitableType<T>::value;
};

/// Discard reference
template <class T> struct IsSuitableType<T &>{
  static constexpr bool value = IsSuitableType<T>::value;
};


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
  static_assert(IsSuitableType<T>::value,"T can not be used as a template parameter for Image or VectorImage classes.");
  using PixelType = T;
};

/// Partial specialisation for itk::Neighborhood<T>
template <class T> struct PixelTypeDeduction<itk::Neighborhood<T>>
{
  static_assert(IsSuitableType<T>::value,"T can not be used as a template parameter for Image or VectorImage classes.");
  using PixelType = T;
};

/// Discard const qualifier
template <class T> struct PixelTypeDeduction<const  T>
{
  using PixelType = typename PixelTypeDeduction<T>::PixelType;
};

/// Discard reference
template <class T> struct PixelTypeDeduction<T &>
{
  using PixelType = typename PixelTypeDeduction<T>::PixelType;
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
template <typename T> struct FunctorFilterSuperclassHelper : public FunctorFilterSuperclassHelper<decltype(&T::operator())> {};

/// Partial specialisation for R(*)(T...)
template <typename R, typename... T> struct FunctorFilterSuperclassHelper<R(*)(T...)>
{
  using OutputImageType = typename ImageTypeDeduction<R>::ImageType;
  using FilterType = VariadicInputsImageFilter<OutputImageType,typename ImageTypeDeduction<typename PixelTypeDeduction<T>::PixelType>::ImageType...>;
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::ValueType...>;
};

/// Partial specialisation for R(C::*)(T...) const
template <typename C, typename R, typename... T> struct FunctorFilterSuperclassHelper<R(C::*)(T...) const>
{
  using OutputImageType = typename ImageTypeDeduction<R>::ImageType;
  using FilterType = VariadicInputsImageFilter<OutputImageType,typename ImageTypeDeduction<typename PixelTypeDeduction<T>::PixelType>::ImageType...>;
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::ValueType...>;
};

/// Partial specialisation for R(C::*)(T...)
template <typename C, typename R, typename... T> struct FunctorFilterSuperclassHelper<R(C::*)(T...)>
{ 
  using OutputImageType = typename ImageTypeDeduction<R>::ImageType;
  using FilterType = VariadicInputsImageFilter<OutputImageType,typename ImageTypeDeduction<typename PixelTypeDeduction<T>::PixelType>::ImageType...>;
  using InputHasNeighborhood = std::tuple<typename IsNeighborhood<T>::ValueType...>;
};

/**
 * \brief This helper method builds a fully functional FunctorImageFilter from a functor instance
 * 
 * Functor can be any operator() that matches the following:
 * - Accepts any number of arguments of T,
 * (const) itk::VariableLengthVector<T> (&),(const)
 * itk::Neighborhood<T> (&), (const)
 * itk::Neighborhood<itk::VariableLengthVector<T>> (&) with T a scalar type
 * - returns T or itk::VariableLengthVector<T>, with T a scalar type
 *
 * The returned filter is ready to use. Inputs can be set through the
 * SetVariadicInputs() method (see VariadicInputsImageFilter class for
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
 * \brief A generic functor filter templated by its functor
 * 
 * \sa VariadicInputsImageFilter
 * \sa NewFunctorFilter
 * 
 * \ingroup IntensityImageFilters   Multithreaded Streamed
 *
 * \ingroup OTBFunctor
*/
template <class TFunction>
    class ITK_EXPORT FunctorImageFilter
  : public FunctorFilterSuperclassHelper<TFunction>::FilterType
{

public:
  // Standard typedefs
  using Self = FunctorImageFilter;
  using FunctorType = TFunction;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  // Superclass through the helper struct
  using Superclass = typename FunctorFilterSuperclassHelper<TFunction>::FilterType;
  using OutputImageType = typename Superclass::OutputImageType;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  
  using ProcessObjectType = itk::ProcessObject;

  // A tuple of bool of the same size as the number of arguments in
  // the functor
  using InputHasNeighborhood = typename FunctorFilterSuperclassHelper<TFunction>::InputHasNeighborhood;
  using InputTypesTupleType = typename Superclass::InputTypesTupleType;
  template<size_t I> using InputImageType = typename Superclass::template InputImageType<I>;
  using Superclass::NumberOfInputs;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(FunctorImageFilter, ImageToImageFilter);
  
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
  friend auto NewFunctorFilter<TFunction>(const TFunction& f, itk::Size<2> radius);

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
template <typename Functor> auto NewFunctorFilter(const Functor& f, itk::Size<2> radius)
{
  using FilterType = FunctorImageFilter<Functor>;
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

template <typename F> struct NumberOfOutputBandsDecorator : F
{
public:
  constexpr NumberOfOutputBandsDecorator(const F t, unsigned int nbComp) : F(t), m_NumberOfOutputBands(nbComp) {}
  using F::operator();

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
 * \param f the Functor to build the filter from
 * \param numberOfOutputBands The number of output bands that
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
