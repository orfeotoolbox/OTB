/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbListSampleToListSampleFilter_h
#define otbListSampleToListSampleFilter_h

#include "otbListSampleSource.h"

namespace otb
{
namespace Statistics
{

/** \class ListSampleToListSampleFilter
 *  \brief This class is a base class for filters using ListSample as
 *  input and returning ListSample.
 *
 *
 *  This filter provide pipeline support for itk::Statistics::ListSample via itk::DataObjectDecorator for
 *  both the input and the output sample list.
 *
 *
 * \ingroup OTBStatistics
 */
template <class TInputSampleList, class TOutputSampleList = TInputSampleList>
class ITK_EXPORT ListSampleToListSampleFilter : public ListSampleSource<TOutputSampleList>
{
public:
  /** Standard class typedefs */
  typedef ListSampleToListSampleFilter        Self;
  typedef ListSampleSource<TOutputSampleList> Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToListSampleFilter, ListSampleSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TInputSampleList                                    InputSampleListType;
  typedef typename InputSampleListType::Pointer               InputSampleListPointer;
  typedef typename InputSampleListType::ConstPointer          InputSampleListConstPointer;
  typedef typename InputSampleListType::MeasurementVectorType InputMeasurementVectorType;
  typedef typename InputMeasurementVectorType::ValueType      InputValueType;

  /** OutputSampleList typedefs */
  typedef typename Superclass::OutputSampleListType         OutputSampleListType;
  typedef typename Superclass::OutputSampleListPointer      OutputSampleListPointer;
  typedef typename Superclass::OutputSampleListConstPointer OutputSampleListConstPointer;
  typedef typename Superclass::OutputMeasurementVectorType  OutputMeasurementVectorType;
  typedef typename Superclass::OutputValueType              OutputValueType;

  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer DataObjectPointer;
  // typedef itk::DataObjectDecorator< InputSampleListType >      InputSampleListObjectType;
  // typedef typename Superclass::OutputSampleListObjectType      OutputSampleListObjectType;

  /** Method to set/get the input list sample */
  using Superclass::SetInput;
  void SetInput(const InputSampleListType* inputPtr);
  // void SetInput( const InputSampleListObjectType * inputPtr );

  /** Returns the input sample list */
  // const InputSampleListType * GetInputSampleList() const;

  /** Returns the input sample list as a data object */
  const InputSampleListType* GetInput() const;

protected:
  ListSampleToListSampleFilter();
  ~ListSampleToListSampleFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ListSampleToListSampleFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

}; // end of class ListSampleToListSampleFilter

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToListSampleFilter.hxx"
#endif

#endif
