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

#ifndef otbUnaryFunctorNeighborhoodVectorImageFilter_h
#define otbUnaryFunctorNeighborhoodVectorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{
/** \class UnaryFunctorNeighborhoodVectorImageFilter
 * \brief Implements neighborhood-wise generic operation of one vector image.
 *
 * This class is parameterized over the types of the input image
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 *
 * \ingroup OTBCommon
 */
template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT UnaryFunctorNeighborhoodVectorImageFilter
  : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef UnaryFunctorNeighborhoodVectorImageFilter           Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UnaryFunctorNeighborhoodVectorImageFilter, InPlaceImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Some convenient typedefs. */
  typedef TFunction                             FunctorType;
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef TOutputImage                          OutputImageType;
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

  typedef itk::ConstNeighborhoodIterator<TInputImage>
    NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::RadiusType
    RadiusType;

  /** Set/Get Radius
   * The main difference from itkUnaryFunctiorNeighborhoodImageFilter
   * and otbBinaryFunctorNeighborhoodVectorImageFilter is that the
   * radius may not be square...
   */
  itkSetMacro(Radius, RadiusType);
  itkGetMacro(Radius, RadiusType);

protected:
  UnaryFunctorNeighborhoodVectorImageFilter();
  ~UnaryFunctorNeighborhoodVectorImageFilter() override { }

  /** UnaryFunctorNeighborhoodVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                    itk::ThreadIdType threadId) override;

  /**
   * Since the number of components per pixel depends on the radius range, one must reimplement
   * this method to set the proper number of component on the filter output.
   */
  void GenerateOutputInformation(void) override;

  RadiusType m_Radius;

private:
  UnaryFunctorNeighborhoodVectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  FunctorType m_Functor;
}; // end of class

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryFunctorNeighborhoodVectorImageFilter.hxx"
#endif

#endif


