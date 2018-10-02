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

#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkProcessObject.h"
#include <type_traits>
#include "otbFunctionTraits.h"

namespace otb
{

template<typename TOutput>
struct ImageFunctorBase
{
  void SetRadius(size_t r)
  {
    m_Radius = r;
  }
   
  unsigned int GetNumberOfOutputBands() const
  {
    return 1;
  }

protected:
  size_t m_Radius = 0;

};

template<typename TInput, typename TOutput>
struct ImageFunctor : ImageFunctorBase<TOutput> {
  using HasNeighborhood = std::false_type;
  using InputImageType = typename std::conditional<std::is_scalar<TInput>::value, 
                                                   otb::Image<TInput>,
                                                   otb::VectorImage<TInput>>::type;

};


template<typename TInput, typename TOutput>
struct ImageFunctor<typename itk::ConstNeighborhoodIterator<TInput>, 
                    TOutput> : ImageFunctorBase<TOutput> {
  using HasNeighborhood = std::true_type;
  using InputImageType = TInput;
};


template<typename T>
using FTraits = typename FunctionTraits::function_traits<T>;

template<typename T>
using FResultType = typename FTraits<T>::result_type;

template<typename T, size_t i>
using ArgType = typename FTraits<T>::template arg<i>::type;

template<typename T>
using InputArgumentType = ArgType<T, 0>;


template<class T, class R = void>  
struct enable_if_type { typedef R type; };

template<class T, class Enable = void>
struct IsNeighborhood : std::false_type {};

template<class T>
struct IsNeighborhood<T, typename enable_if_type<typename T::NeighborhoodType>::type> : std::true_type
{};

template<typename TFunction, bool B = true>
struct TII
{
  using type = typename InputArgumentType<TFunction>::ImageType;
};

template<typename TFunction>
struct TII<TFunction, !IsNeighborhood<InputArgumentType<TFunction>>::value>
{
  using InputArgument = InputArgumentType<TFunction>;
  using type = typename std::conditional<std::is_scalar<InputArgument>::value,
                                         typename otb::Image<InputArgument>,
                                         typename otb::VectorImage<InputArgument>
                                         >::type ;
};

template<typename TFunction>
using TInputImage = typename TII<TFunction>::type;

// OutputImageType: if the result of the 
// functor is a scalar -> otb::Image,
// otherwise, otb::VectorImage
         template<typename TFunction, typename ResultType = FResultType<TFunction>>
         using TOutputImage =   
         typename std::conditional< std::is_scalar<ResultType>::value, 
                                    typename otb::Image<ResultType>,
                                    typename otb::VectorImage<ResultType>
                                    >::type;


/** \class FunctorImageFilter
 * \brief Implements 
 *
 * This class is 
 *
 * \ingroup IntensityImageFilters   Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TFunction>
    class ITK_EXPORT FunctorImageFilter
  : public itk::ImageToImageFilter<TInputImage<TFunction>, TOutputImage<TFunction>>
{

public:
  using Self = FunctorImageFilter;
  using FunctorType = TFunction;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  using InputImageType = TInputImage<TFunction>;
  using InputImagePointer = typename InputImageType::ConstPointer;
  using InputImageRegionType = typename InputImageType::RegionType;
  using InputImagePixelType = typename InputImageType::PixelType;
  using InputImageSizeType = typename InputImageType::SizeType;
  using InputImageIndexType = typename InputImageType::IndexType;

  using OutputImageType = TOutputImage<TFunction>;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using OutputImagePixelType = typename OutputImageType::PixelType;
  using Superclass = itk::ImageToImageFilter<InputImageType, 
                                             OutputImageType>;
  using ProcessObjectType = itk::ProcessObject;

 /** Method for creation by passing the filter type. */


  static Pointer New(const FunctorType& f) ;

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

/** Set the functor object.  This replaces the current Functor with a
   * copy of the specified Functor. This allows the user to specify a
   * functor that has ivars set differently than the default functor.
   * This method requires an operator!=() be defined on the functor
   * (or the compiler's default implementation of operator!=() being
   * appropriate). */
void SetFunctor(const FunctorType& functor)
{
  m_Functor = functor;
  this->Modified();
}

private:
FunctorImageFilter();
FunctorImageFilter(const FunctorType& f) : m_Functor(f) {};
FunctorImageFilter(const Self &) ;
void operator =(const Self&) ;
~FunctorImageFilter() override {}

protected:

/** FunctorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
    void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  /**
   * Pad the input requested region by radius
   */
  void GenerateInputRequestedRegion(void) override;


  FunctorType m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFunctorImageFilter.hxx"
#endif

#endif
