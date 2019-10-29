/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbUnaryImageFunctorWithVectorImageFilter_h
#define otbUnaryImageFunctorWithVectorImageFilter_h

#include "otbVectorImage.h"
#include "itkVariableLengthVector.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/** \class UnaryImageFunctorWithVectorImageFilter
 *  \brief Applies a functor to a VectorImage
 *
 *  The aim of the class is to work with vector images but with a functor
 *  that uses as input a component of the pixel.
 *
 *  For N components pixel, an array of functor is defined and each is applied to
 *  the corresponding component of the output pixel.
 *
 * \ingroup Functor
 * \ingroup VectorImage
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT UnaryImageFunctorWithVectorImageFilter : public itk::InPlaceImageFilter<TInputImage, TOutputImage>

{
public:
  /** Standard class typedefs. */
  typedef UnaryImageFunctorWithVectorImageFilter Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TFunction    FunctorType;

  /** Supported images definition. */
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  /** Image size "typedef" definition. */
  typedef typename InputImageType::SizeType SizeType;

  typedef std::vector<FunctorType> FunctorVectorType;

  /** return class name. */
  itkTypeMacro(UnaryImageFunctorWithVectorImageFilter, itk::InPlaceImageFilter);

  /** Get the functor list */
  FunctorVectorType& GetFunctorVector(void)
  {
    return m_FunctorVector;
  }

protected:
  UnaryImageFunctorWithVectorImageFilter();
  ~UnaryImageFunctorWithVectorImageFilter() override
  {
  }

  /** UnaryImageFunctorWithVectorImageFilter can produce an image which is a different
   * resolution than its input image.  As such, UnaryImageFunctorWithVectorImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  void GenerateOutputInformation() override;

  /** UnaryImageFunctorWithVectorImageFilter can be implemented as a multithreaded filter.
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

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** Ponderation declaration*/
  FunctorVectorType m_FunctorVector;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryImageFunctorWithVectorImageFilter.hxx"
#endif

#endif
