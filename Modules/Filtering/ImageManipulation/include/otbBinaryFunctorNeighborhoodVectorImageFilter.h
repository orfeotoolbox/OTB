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

#ifndef otbBinaryFunctorNeighborhoodVectorImageFilter_h
#define otbBinaryFunctorNeighborhoodVectorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{
/** \class BinaryFunctorNeighborhoodVectorImageFilter
 * \brief Implements neighborhood-wise generic operation of two images being vector images.
 *
 * This class is parameterized over the types of the two input images
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied.  A Functor style is used.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage1, class TInputImage2,
    class TOutputImage, class TFunction>
class ITK_EXPORT BinaryFunctorNeighborhoodVectorImageFilter
  : public itk::InPlaceImageFilter<TInputImage1, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryFunctorNeighborhoodVectorImageFilter          Self;
  typedef itk::InPlaceImageFilter<TInputImage1, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryFunctorNeighborhoodVectorImageFilter, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TFunction                              FunctorType;
  typedef TInputImage1                           Input1ImageType;
  typedef typename Input1ImageType::ConstPointer Input1ImagePointer;
  typedef typename Input1ImageType::RegionType   Input1ImageRegionType;
  typedef typename Input1ImageType::PixelType    Input1ImagePixelType;
  typedef TInputImage2                           Input2ImageType;
  typedef typename Input2ImageType::ConstPointer Input2ImagePointer;
  typedef typename Input2ImageType::RegionType   Input2ImageRegionType;
  typedef typename Input2ImageType::PixelType    Input2ImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  typedef itk::ProcessObject ProcessObjectType;

  /** Connect one of the operands for pixel-wise addition */
  void SetInput1(const TInputImage1 * image1);

  /** Connect one of the operands for pixel-wise addition */
  void SetInput2(const TInputImage2 * image2);

  /** Connect the set of radius. In fact, Self keeps the larger size
   * only to define neighborhood parameters, and gives the min and max
   * radius to the functor. */
  void SetRadius(const unsigned char& min, const unsigned char& max);

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

  typedef itk::ConstNeighborhoodIterator<TInputImage1>
  NeighborhoodIteratorType1;
  typedef itk::ConstNeighborhoodIterator<TInputImage2>
  NeighborhoodIteratorType2;

  typedef typename NeighborhoodIteratorType1::RadiusType RadiusType1;
  typedef typename NeighborhoodIteratorType2::RadiusType RadiusType2;

  typedef unsigned char RadiusSizeType;

  itkSetMacro(Radius, RadiusSizeType);

protected:
  BinaryFunctorNeighborhoodVectorImageFilter();
  ~BinaryFunctorNeighborhoodVectorImageFilter() override {}

  /** BinaryFunctorNeighborhoodVectorImageFilter can be implemented as a multithreaded filter.
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

  RadiusSizeType m_Radius;

private:
  BinaryFunctorNeighborhoodVectorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  FunctorType m_Functor;
};
} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryFunctorNeighborhoodVectorImageFilter.hxx"
#endif

#endif
