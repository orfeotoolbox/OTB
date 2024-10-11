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

#ifndef otbUnaryFunctorWithIndexImageFilter_h
#define otbUnaryFunctorWithIndexImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkProcessObject.h"

namespace otb
{
/** \class UnaryFunctorWithIndexImageFilter
 * \brief Implements neighborhood-wise generic operation on image
 *
 * This class is parameterized over the input image type
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT UnaryFunctorWithIndexImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorWithIndexImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorWithIndexImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TFunction                             FunctorType;
  typedef typename Superclass::InputImageType   InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename InputImageType::IndexType    InputImageIndexType;
  typedef typename Superclass::OutputImageType  OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;

  typedef itk::ProcessObject ProcessObjectType;

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

  typedef itk::ImageRegionConstIteratorWithIndex<TInputImage> IteratorType;

protected:
  /**
   * Constructor
   */
  UnaryFunctorWithIndexImageFilter();
  /**
   * Destructor
   */
  ~UnaryFunctorWithIndexImageFilter() override
  {
  }

  /** UnaryFunctorWithIndexImageFilter can be implemented as a multithreaded filter.
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

private:
  UnaryFunctorWithIndexImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  FunctorType m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryFunctorWithIndexImageFilter.hxx"
#endif

#endif
