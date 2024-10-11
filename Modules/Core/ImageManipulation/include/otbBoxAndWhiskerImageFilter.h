/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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

#ifndef otbBoxAndWhiskerImageFilter_h
#define otbBoxAndWhiskerImageFilter_h

#include "itkMacro.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{

/** \class BoxAndWhiskerImageFilter
 * \brief This class performs the detection of outlier with the Box and Whisker technique
 *
 * This is appropriated to perform simple outlier detection over vector data.
 * The input image has to contain vector pixel through a VectorImage type.
 * When an outlier is detected, it is treated to as an missing value through the function
 * \code otb::EuclideanDistanceMetricWithMissingValue::SetToMissingValue() \endcode.
 *
 * A component is considered to as a missing value when
 * \f$ \| x_i - \beta \left( x_{3/4} - x_{1/4} \right) \| > \| x_{1/2} \| \f$
 * where \f$ x_{1/4}, x_{1/2}, x_{3/4} \f$ stand for the first, second (median) and
 * third quantile values.
 *
 * \ingroup Streamed
 * \sa EuclideanDistanceMetricWithMissingValue
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage>
class ITK_EXPORT BoxAndWhiskerImageFilter : public itk::InPlaceImageFilter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BoxAndWhiskerImageFilter                      Self;
  typedef typename itk::InPlaceImageFilter<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BoxAndWhiskerImageFilter, InPlaceImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::OutputImageType       OutputImageType;
  typedef typename Superclass::OutputImagePointer    OutputImagePointer;
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::OutputImagePixelType  OutputImagePixelType;

  typedef typename Superclass::InputImageType         InputImageType;
  typedef typename Superclass::InputImagePointer      InputImagePointer;
  typedef typename Superclass::InputImageConstPointer InputImageConstPointer;
  typedef typename Superclass::InputImageRegionType   InputImageRegionType;
  typedef typename Superclass::InputImagePixelType    InputImagePixelType;

  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType ValueType;
  typedef typename InputImageType::SizeType          SizeType;
  typedef typename InputImageType::RegionType        RegionType;

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, OutputImageType::ImageDimension);

  /** Get the Radius (all to 1) */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Fix the whisker */
  itkGetConstMacro(Beta, double);
  itkSetMacro(Beta, double);
  itkGetConstMacro(NumberFound, unsigned int);

protected:
  BoxAndWhiskerImageFilter();
  ~BoxAndWhiskerImageFilter() override
  {
  }

  /** Main computation method implemented as a multithreaded filter */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;
  void GenerateOutputInformation() override;
  void AllocateOutputs() override;

  /** Perform the outlier detection */
  PixelType PerformBoxAndWhiskerDetection(const PixelType& pixel);

private:
  BoxAndWhiskerImageFilter(const Self&);
  void operator=(const Self&); // not implemented

  SizeType m_Radius;
  double   m_Beta;
  long int m_NumberFound;

}; // end of class BoxAndWhiskerImageFilter

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBoxAndWhiskerImageFilter.hxx"
#endif

#endif
