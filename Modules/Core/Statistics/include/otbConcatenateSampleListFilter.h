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

#ifndef otbConcatenateSampleListFilter_h
#define otbConcatenateSampleListFilter_h

#include "otbListSampleToListSampleFilter.h"

namespace otb
{
namespace Statistics
{

/** \class ConcatenateSampleListFilter
 *  \brief This class concatenates several sample list into a single one.
 *
 * For the sake of genericity on the MeasurementVector type, no casting is applied so
 * this filter does not allow different input/output SampleList.
 *
 * \sa ListSampleToListSampleFilter
 *
 * \ingroup OTBStatistics
 */
template <class TSampleList>
class ITK_EXPORT ConcatenateSampleListFilter : public otb::Statistics::ListSampleToListSampleFilter<TSampleList, TSampleList>
{
public:
  /** Standard class typedefs */
  typedef ConcatenateSampleListFilter Self;
  typedef otb::Statistics::ListSampleToListSampleFilter<TSampleList, TSampleList> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConcatenateSampleListFilter, otb::Statistics::ListSampleToListSampleFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** SampleList typedefs */
  typedef TSampleList                                    SampleListType;
  typedef typename SampleListType::Pointer               SampleListPointer;
  typedef typename SampleListType::ConstPointer          SampleListConstPointer;
  typedef typename SampleListType::MeasurementVectorType MeasurementVectorType;
  typedef typename MeasurementVectorType::ValueType      ValueType;

  /** Method to set/get the input list sample */
  void AddInput(const SampleListType* inputPtr);
  using Superclass::AddInput;

protected:
  /** This method causes the filter to generate its output. */
  void GenerateData() override;

  ConcatenateSampleListFilter();
  ~ConcatenateSampleListFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ConcatenateSampleListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
}; // end of class ImageToListGenerator

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConcatenateSampleListFilter.hxx"
#endif

#endif
