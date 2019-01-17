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

#ifndef otbStreamingLineSegmentDetector_h
#define otbStreamingLineSegmentDetector_h

#include <vector>

#include "itkImageRegion.h"
#include "itkPreOrderTreeIterator.h"

#include "itkDataObjectDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbLineSegmentDetector.h"

#include "otbPersistentFilterStreamingDecorator.h"
#include "otbPersistentImageToVectorDataFilter.h"

namespace otb
{
/** \class PersistentStreamingLineSegmentDetector
 *  \brief Perform Line segment detector in a persistent way.
 *
 *  This filter is a generic PersistentImageFilter, which encapsulate
 *  the Line Segment detector filter.
 *
 * \sa PersistentImageToVectorDataFilter
 *
 *
 * \ingroup OTBEdge
 */
template <class TImageType>
class PersistentStreamingLineSegmentDetector
  : public otb::PersistentImageToVectorDataFilter<TImageType,
     typename otb::LineSegmentDetector<TImageType, double>::VectorDataType >
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingLineSegmentDetector                              Self;
  typedef PersistentImageToVectorDataFilter<TImageType,
      typename otb::LineSegmentDetector<TImageType, double>::VectorDataType > Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  typedef otb::LineSegmentDetector<TImageType, double>     LSDType;
  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointerType;

  typedef typename Superclass::OutputVectorDataType        OutputVectorDataType;
  typedef typename Superclass::OutputVectorDataPointerType OutputVectorDataPointerType;

  typedef typename Superclass::ExtractImageFilterType      ExtractImageFilterType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingLineSegmentDetector, PersistentImageToVectorDataFilter);

protected:
  PersistentStreamingLineSegmentDetector();

  ~PersistentStreamingLineSegmentDetector() override;

  void GenerateInputRequestedRegion() override;

private:
  PersistentStreamingLineSegmentDetector(const Self &) = delete;
  void operator =(const Self&) = delete;

  OutputVectorDataPointerType ProcessTile() override;
};

template <class TImageType>
class StreamingLineSegmentDetector
{
public:

  // define the PersistentStreamingLineSegmentDetector template
  typedef PersistentStreamingLineSegmentDetector<TImageType>
    PersistentStreamingLineSegmentDetectorType;

  typedef typename PersistentStreamingLineSegmentDetectorType::InputImageType
      InputImageType;
  typedef typename PersistentStreamingLineSegmentDetectorType::OutputVectorDataType
      OutputVectorDataType;

  // typedef for streaming capable filter
  typedef PersistentFilterStreamingDecorator<PersistentStreamingLineSegmentDetectorType>
    FilterType;

};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingLineSegmentDetector.hxx"
#endif

#endif
