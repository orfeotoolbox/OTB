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

namespace otb
{

template<typename TOutput>
struct ImageFunctorBase
{
  using OutputImageType = typename std::conditional<std::is_scalar<TOutput>::value, 
                                                    otb::Image<TOutput>,
                                                    otb::VectorImage<TOutput>>::type;

  void SetRadius(size_t r)
  {
    m_Radius = r;
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
  : public itk::ImageToImageFilter<typename TFunction::InputImageType, 
                                   typename TFunction::OutputImageType>
{
public:
  using Self = FunctorImageFilter;
  using FunctorType = TFunction;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  using InputImageType = typename FunctorType::InputImageType;
  using InputImagePointer = typename InputImageType::ConstPointer;
  using InputImageRegionType = typename InputImageType::RegionType;
  using InputImagePixelType = typename InputImageType::PixelType;
  using InputImageSizeType = typename InputImageType::SizeType;
  using InputImageIndexType = typename InputImageType::IndexType;
  using OutputImageType = typename FunctorType::OutputImageType;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using OutputImagePixelType = typename OutputImageType::PixelType;
  using Superclass = itk::ImageToImageFilter<InputImageType, 
                                             OutputImageType>;
  using ProcessObjectType = itk::ProcessObject;

/** Method for creation by passing the filter type. */
  itkNewMacro(Self);


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
FunctorImageFilter(){} ;
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
