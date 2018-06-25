/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbQuaternaryFunctorImageFilter_h
#define otbQuaternaryFunctorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{

/** \class QuaternaryFunctorImageFilter
 * \brief Implements pixel-wise generic operation of four images.
 *
 * This class is parameterized over the types of the four input images
 * and the type of the output image.  It is also parameterized by the
 * operation to be applied, using a Functor style.
 *
 * \sa TernaryFunctorImageFilter, BinaryFunctorImageFilter UnaryFunctorImageFilter
 *
 * \ingroup IntensityImageFilters Multithreaded
 *
 * \ingroup OTBCommon
 */
template <class TInputImage1, class TInputImage2,
    class TInputImage3, class TInputImage4,
    class TOutputImage, class TFunction>
class ITK_EXPORT QuaternaryFunctorImageFilter :
  public itk::InPlaceImageFilter<TInputImage1, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef QuaternaryFunctorImageFilter                        Self;
  typedef itk::InPlaceImageFilter<TInputImage1, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(QuaternaryFunctorImageFilter, InPlaceImageFilter);

  /** Some typedefs. */
  typedef TFunction                              FunctorType;
  typedef TInputImage1                           Input1ImageType;
  typedef typename Input1ImageType::ConstPointer Input1ImagePointer;
  typedef typename Input1ImageType::RegionType   Input1ImageRegionType;
  typedef typename Input1ImageType::PixelType    Input1ImagePixelType;
  typedef TInputImage2                           Input2ImageType;
  typedef typename Input2ImageType::ConstPointer Input2ImagePointer;
  typedef typename Input2ImageType::RegionType   Input2ImageRegionType;
  typedef typename Input2ImageType::PixelType    Input2ImagePixelType;
  typedef TInputImage3                           Input3ImageType;
  typedef typename Input3ImageType::ConstPointer Input3ImagePointer;
  typedef typename Input3ImageType::RegionType   Input3ImageRegionType;
  typedef typename Input3ImageType::PixelType    Input3ImagePixelType;
  typedef TInputImage4                           Input4ImageType;
  typedef typename Input4ImageType::ConstPointer Input4ImagePointer;
  typedef typename Input4ImageType::RegionType   Input4ImageRegionType;
  typedef typename Input4ImageType::PixelType    Input4ImagePixelType;
  typedef TOutputImage                           OutputImageType;
  typedef typename OutputImageType::Pointer      OutputImagePointer;
  typedef typename OutputImageType::RegionType   OutputImageRegionType;
  typedef typename OutputImageType::PixelType    OutputImagePixelType;

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput1(const TInputImage1 *image1);

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput2(const TInputImage2 *image2);

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput3(const TInputImage3 *image3);

  /** Connect one of the operands for pixel-wise addition. */
  void SetInput4(const TInputImage4 *image4);

  /** Get the functor object.  The functor is returned by reference.
   * (Functors do not have to derive from itk::LightObject, so they do
   * not necessarily have a reference count. So we cannot return a
   * SmartPointer). */
  FunctorType& GetFunctor(void)
  {
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
   * This method requires an operator==() be defined on the functor
   * (or the compiler's default implementation of operator==() being
   * appropriate). */
  void SetFunctor(const FunctorType& functor)
  {
    if (!(functor == m_Functor))
      {
      m_Functor = functor;
      this->Modified();
      }
  }

  /** Image dimensions */
  itkStaticConstMacro(Input1ImageDimension, unsigned int,
                      TInputImage1::ImageDimension);
  itkStaticConstMacro(Input2ImageDimension, unsigned int,
                      TInputImage2::ImageDimension);
  itkStaticConstMacro(Input3ImageDimension, unsigned int,
                      TInputImage3::ImageDimension);
  itkStaticConstMacro(Input4ImageDimension, unsigned int,
                      TInputImage4::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

protected:
  QuaternaryFunctorImageFilter();
  ~QuaternaryFunctorImageFilter() override {}

  /** Validate the presence of all three inputs. If one or more inputs
   * are missing, throw an exception. */
  void BeforeThreadedGenerateData() override;

  /** QuaternaryFunctorImageFilter can be implemented as a multithreaded filter.
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

private:
  QuaternaryFunctorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  FunctorType m_Functor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbQuaternaryFunctorImageFilter.hxx"
#endif

#endif
