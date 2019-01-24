/*
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

#ifndef otbPointSetToDensityImageFilter_h
#define otbPointSetToDensityImageFilter_h

#include "itkProcessObject.h"
#include "itkPointSet.h"
#include "itkPointSetToImageFilter.h"
#include "otbPointSetDensityFunction.h"
#include "itkPoint.h"

namespace otb
{

/** \class PointSetToDensityImageFilter
 *  \brief Draw the density of a point set on an image
 *
 * \ingroup OTBDensity
 */

template <class TInputPointSet, class TOutputImage,
    class TDensityFunction = PointSetDensityFunction<TInputPointSet, typename TOutputImage::PixelType> >
class ITK_EXPORT PointSetToDensityImageFilter
  : public itk::PointSetToImageFilter<TInputPointSet, TOutputImage>
{

public:

  /** Standard class typedefs. */
  typedef PointSetToDensityImageFilter                             Self;
  typedef itk::PointSetToImageFilter<TInputPointSet, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetToDensityImageFilter, itk::PointSetToImageFilter);

  /**   typedefs parameters support */
  typedef TInputPointSet PointSetType;

  typedef TOutputImage                          OutputImageType;
  typedef typename  OutputImageType::PixelType  PixelType;
  typedef typename  OutputImageType::IndexType  IndexType;
  typedef typename  OutputImageType::RegionType OutputImageRegionType;

  /**   typedef filter support*/
  typedef TDensityFunction                                PointSetDensityFunctionType;
  typedef typename PointSetDensityFunctionType::InputType InputType;
  typedef typename PointSetDensityFunctionType::Pointer   PointSetDensityFunctionPointerType;

  /** Set/Get Radius*/
  itkGetMacro(Radius, unsigned int);
  itkSetMacro(Radius, unsigned int);

protected:

  /**
   * Constructor.
   */
  PointSetToDensityImageFilter();
  /**
   * Destructor.
   */
  ~PointSetToDensityImageFilter() override {}
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /**
   * Call the ImageSource::GenerateData which handle multithreading
   */
  void GenerateData() override;

  /**
   * Main computation method.
   */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) override;

  /**
  * Main computation method.
  */
  void  GenerateOutputInformation() override;

private:

  PointSetToDensityImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  unsigned int m_Radius;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetToDensityImageFilter.hxx"
#endif

#endif
