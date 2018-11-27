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

#ifndef otbStackFunctorImageFilter_h
#define otbStackFunctorImageFilter_h

#include "otbFunctorTraits.h"
#include "itkImageToImageFilter.h"

#include <functional>
#include <vector>

namespace otb {

template <typename T> struct RemoveVectorAndReferenceWrapper {};

template <typename T> struct RemoveVectorAndReferenceWrapper<std::vector<std::reference_wrapper<T>>>
{
  using Type = T;
};


template <typename T> struct StackFunctorFilterSuperclassHelper : public StackFunctorFilterSuperclassHelper<typename RetrieveOperator<T>::Type> {};


template <typename Out, typename In> struct StackFunctorFilterSuperclassHelper<Out(*)(In)>
{
  using OutputImageType = typename ImageTypeDeduction<Out>::ImageType;
  using InputImageType  = typename ImageTypeDeduction<typename PixelTypeDeduction<typename RemoveVectorAndReferenceWrapper<In>::Type>::PixelType>::ImageType;
  using FilterType = itk::ImageToImageFilter<InputImageType,OutputImageType>;
  static constexpr bool InputHasNeighborhood = IsNeighborhood<In>::value;
};

template <typename Out, typename C, typename In> struct StackFunctorFilterSuperclassHelper<Out(C::*)(In)>
{
  using OutputImageType = typename ImageTypeDeduction<Out>::ImageType;
  using InputImageType  = typename ImageTypeDeduction<typename PixelTypeDeduction<typename RemoveVectorAndReferenceWrapper<In>::Type>::PixelType>::ImageType;
  using FilterType = itk::ImageToImageFilter<InputImageType,OutputImageType>;
  static constexpr bool InputHasNeighborhood = IsNeighborhood<In>::value;
};

template <typename Out, typename C, typename In> struct StackFunctorFilterSuperclassHelper<Out(C::*)(In) const>
{
  using OutputImageType = typename ImageTypeDeduction<Out>::ImageType;
  using InputImageType  = typename ImageTypeDeduction<typename PixelTypeDeduction<typename RemoveVectorAndReferenceWrapper<In>::Type>::PixelType>::ImageType;
  using FilterType = itk::ImageToImageFilter<InputImageType,OutputImageType>;
  static constexpr bool InputHasNeighborhood = IsNeighborhood<In>::value;
};


template <typename Out, typename In> struct StackFunctorFilterSuperclassHelper<void(*)(Out&, In)>
{
  using OutputImageType = typename ImageTypeDeduction<Out>::ImageType;
  using InputImageType  = typename ImageTypeDeduction<typename PixelTypeDeduction<typename RemoveVectorAndReferenceWrapper<In>::Type>::PixelType>::ImageType;
  using FilterType = itk::ImageToImageFilter<InputImageType,OutputImageType>;
  static constexpr bool InputHasNeighborhood = IsNeighborhood<In>::value;
};

template <typename Out, typename C, typename In> struct StackFunctorFilterSuperclassHelper<void(C::*)(Out&, In)>
{
  using OutputImageType = typename ImageTypeDeduction<Out>::ImageType;
  using InputImageType  = typename ImageTypeDeduction<typename PixelTypeDeduction<typename RemoveVectorAndReferenceWrapper<In>::Type>::PixelType>::ImageType;
  using FilterType = itk::ImageToImageFilter<InputImageType,OutputImageType>;
  static constexpr bool InputHasNeighborhood = IsNeighborhood<In>::value;
};

template <typename Out, typename C, typename In> struct StackFunctorFilterSuperclassHelper<void(C::*)(Out&,In) const>
{
  using OutputImageType = typename ImageTypeDeduction<Out>::ImageType;
  using InputImageType  = typename ImageTypeDeduction<typename PixelTypeDeduction<typename RemoveVectorAndReferenceWrapper<In>::Type>::PixelType>::ImageType;
  using FilterType = itk::ImageToImageFilter<InputImageType,OutputImageType>;
  static constexpr bool InputHasNeighborhood = IsNeighborhood<In>::value;
};

template <typename Functor> auto NewStackFunctorFilter(Functor f, itk::Size<2> radius = {{0,0}});


template <class TFunction> class StackFunctorImageFilter : public StackFunctorFilterSuperclassHelper<TFunction>::FilterType
{
public:
  // Standards typedefs
  using Self         = StackFunctorImageFilter;
  using Superclass   = typename StackFunctorFilterSuperclassHelper<TFunction>::FilterType;
  using FunctorType  = TFunction;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Run-time type information (and related methods). */
  itkTypeMacro(StackFunctorImageFilter, ImageToImageFilter);

  // Other typedefs
  using OutputImageType       = typename Superclass::OutputImageType;
  using InputImageType        = typename Superclass::InputImageType;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  
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
  StackFunctorImageFilter(const FunctorType& f, itk::Size<2> radius = {{0,0}}) : m_Functor(f), m_Radius(radius) {};
  StackFunctorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  ~StackFunctorImageFilter() = default;

private:
  /// Actual creation of the filter is handled by this free function
  friend auto NewStackFunctorFilter<TFunction>(TFunction f, itk::Size<2> radius);

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
template <typename Functor> auto NewStackFunctorFilter(Functor f, itk::Size<2> radius)
{
  using FilterType = StackFunctorImageFilter<Functor>;
  using PointerType = typename FilterType::Pointer;

  PointerType  p = new FilterType(f,radius);
  p->UnRegister();
  return p;
}

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStackFunctorImageFilter.hxx"
#endif

#endif
