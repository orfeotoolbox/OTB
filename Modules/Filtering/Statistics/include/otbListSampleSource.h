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

#ifndef otbListSampleSource_h
#define otbListSampleSource_h

#include "itkProcessObject.h"
#include "itkDataObjectDecorator.h"

namespace otb {
namespace Statistics {

/** \class ListSampleSource
 *  \brief This class is a base class for filters using ListSample as
 *  input and returning ListSample.
 *
 *
 *  This filter provides pipeline support since
 *  itk::Statistics::ListSample is a DataObject
 *  the output sample list.
 *
 *
 * \ingroup OTBStatistics
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ListSampleSource :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ListSampleSource               Self;
  typedef itk::ProcessObject                         Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleSource, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** OutputSampleList typedefs */
  typedef TOutputSampleList                                    OutputSampleListType;
  typedef typename OutputSampleListType::Pointer               OutputSampleListPointer;
  typedef typename OutputSampleListType::ConstPointer          OutputSampleListConstPointer;
  typedef typename OutputSampleListType::MeasurementVectorType OutputMeasurementVectorType;
  typedef typename OutputMeasurementVectorType::ValueType      OutputValueType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** ListSample is not a DataObject, we need to decorate it to push it down
   * a ProcessObject's pipeline */
  typedef itk::DataObject::Pointer                             DataObjectPointer;

  /** Returns the output sample list as a data object */
  OutputSampleListType * GetOutput();

protected:
  /** Standard itk::ProcessObject subclass method. */
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  ListSampleSource();
  ~ListSampleSource() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ListSampleSource(const Self&) = delete;
  void operator=(const Self&) = delete;

}; // end of class ListSampleSource

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleSource.hxx"
#endif

#endif
