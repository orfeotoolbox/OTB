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

#ifndef otbShiftScaleSampleListFilter_h
#define otbShiftScaleSampleListFilter_h

#include "otbListSampleToListSampleFilter.h"


namespace otb {
namespace Statistics {

/** \class ShiftScaleSampleListFilter
 *  \brief This class generate a shifted and scaled version of the input sample list
 *
 *  For each component of the sample, the following formula is applied :
 *
 *  \f[ output = \frac{input - shift}{scale} \f]
 *
 *  Beware that the behaviour differs from itk::ShiftScaleImageFilter
 *  (which add shift instead of subtracting it).
 *
 * Standard casting is applied between input and output type.
 *
 * Shifts and scales can be set via the SetShift() and SetScales() methods.
 *
 * \sa ListSampleToListSampleFilter
 *
 * \ingroup OTBStatistics
 */
template < class TInputSampleList, class TOutputSampleList = TInputSampleList >
class ITK_EXPORT ShiftScaleSampleListFilter :
  public otb::Statistics::ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
{
public:
  /** Standard class typedefs */
  typedef ShiftScaleSampleListFilter                 Self;
  typedef otb::Statistics::ListSampleToListSampleFilter
  <TInputSampleList, TOutputSampleList>               Superclass;
  typedef itk::SmartPointer< Self >                  Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShiftScaleSampleListFilter, otb::Statistics::ListSampleToListSampleFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputSampleList typedefs */
  typedef TInputSampleList                                    InputSampleListType;
  typedef typename InputSampleListType::Pointer               InputSampleListPointer;
  typedef typename InputSampleListType::ConstPointer          InputSampleListConstPointer;
  typedef typename InputSampleListType::MeasurementVectorType InputMeasurementVectorType;
  typedef typename InputMeasurementVectorType::ValueType      InputValueType;

  /** OutputSampleList typedefs */
  typedef TOutputSampleList                                    OutputSampleListType;
  typedef typename OutputSampleListType::Pointer               OutputSampleListPointer;
  typedef typename OutputSampleListType::ConstPointer          OutputSampleListConstPointer;
  typedef typename OutputSampleListType::MeasurementVectorType OutputMeasurementVectorType;
  typedef typename OutputMeasurementVectorType::ValueType      OutputValueType;

  /** Set/Get the Shifts for this sample list */
  itkSetMacro(Shifts, InputMeasurementVectorType);
  itkGetMacro(Shifts, InputMeasurementVectorType);

  /** Set/Get the Scales for this sample list */
  itkSetMacro(Scales, InputMeasurementVectorType);
  itkGetMacro(Scales, InputMeasurementVectorType);

protected:
  /** This method causes the filter to generate its output. */
   void GenerateData() override;

  ShiftScaleSampleListFilter();
  ~ShiftScaleSampleListFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ShiftScaleSampleListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The vector of Shifts */
  InputMeasurementVectorType m_Shifts;

  /** The vector of Scales */
  InputMeasurementVectorType m_Scales;

}; // end of class ShiftScaleSampleListFilter

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShiftScaleSampleListFilter.hxx"
#endif

#endif
