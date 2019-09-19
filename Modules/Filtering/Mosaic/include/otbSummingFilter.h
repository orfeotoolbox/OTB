/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2019 Institut de Recherche en Sciences et
 *  Technologies pour l'Environnement et l'Agriculture (IRSTEA)
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
#ifndef __SummingFilter_H
#define __SummingFilter_H

#include "itkImageToImageFilter.h"
#include "itkImageRegionIterator.h"
namespace otb
{
/** \class SummingFilter
 * \brief Computes the sum of the input images pixels
 *
 * Support streaming
 *
 * The pixels must support the operator ==, +, /, etc.
 * The "no data value" can be chose.
 *
 * \ingroup OTBMosaic
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT SummingFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:

  /** Standard Self typedef */
  typedef SummingFilter                                      Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(SummingFilter, ImageToImageFilter);

  /** Some additional typedefs.  */
  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::RegionType        InputImageRegionType;
  typedef typename InputImageType::PixelType         InputImagePixelType;
  typedef typename InputImageType::PointType         InputImagePointType;
  typedef typename InputImageType::IndexType         InputImageIndexType;
  typedef typename InputImageType::SizeType          InputImageSizeType;
  typedef typename InputImageType::SpacingType       InputImageSpacingType;
  typedef typename InputImageType::InternalPixelType InputImageInternalPixelType;

  /** Some additional typedefs.  */
  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointer;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::IndexType         OutputImageIndexType;
  typedef typename OutputImageType::PointType         OutputImagePointType;
  typedef typename OutputImageType::SizeType          OutputImageSizeType;
  typedef typename OutputImageType::SpacingType       OutputImageSpacingType;
  typedef typename OutputImageType::PixelType         OutputImagePixelType;
  typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;

  /** Iterators */
  typedef itk::ImageRegionIterator< OutputImageType >   OutputIteratorType;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;

protected:
  SummingFilter() {
  }

  virtual ~SummingFilter() {
  }

  /** SummingFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */

  /** Overrided methods */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  virtual void GenerateOutputInformation(void);

private:
  SummingFilter(const Self&);   //purposely not implemented
  void operator=(const Self&);  //purposely not implemented

}; // end of class

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSummingFilter.hxx"
#endif

#endif
