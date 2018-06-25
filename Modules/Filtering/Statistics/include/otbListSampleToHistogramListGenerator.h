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

#ifndef otbListSampleToHistogramListGenerator_h
#define otbListSampleToHistogramListGenerator_h

#include "itkNumericTraits.h"
#include "itkProcessObject.h"
#include "itkHistogram.h"
#include "itkStatisticsAlgorithm.h"
#include "otbObjectList.h"

namespace otb {

/** \class ListSampleToHistogramListGenerator
 *  \brief Generates a list of 1D  Histograms from a ListSample
 *  This class is similar to
 *  ListSampleToVariableDimensionHistogramGenerator, except from the
 *  fact that it computes one histogram per channel instead of a
 *  single joint-histogram. The output is a Histogram list (\sa
 *  ObjectList).
 * \sa ListSampleToVariableDimensionHistogram
 *
 * \ingroup OTBStatistics
 */
template<class TListSample,
    class THistogramMeasurement,
         class TFrequencyContainer = itk::Statistics::DenseFrequencyContainer2>
class ITK_EXPORT ListSampleToHistogramListGenerator :
  public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef ListSampleToHistogramListGenerator Self;
  typedef itk::ProcessObject                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToHistogramListGenerator, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** typedef the input ListSample type */
  typedef  TListSample                                ListSampleType;

  /** Type needed for defining the limits of the histogram bins */
  typedef typename itk::NumericTraits
  <THistogramMeasurement>::RealType                   HistogramMeasurementRealType;
  typedef itk::Statistics::Histogram<HistogramMeasurementRealType,
      TFrequencyContainer>                            HistogramType;
  typedef typename HistogramType::SizeType            HistogramSizeType;
  typedef typename TListSample::MeasurementVectorType MeasurementVectorType;
  typedef ObjectList<HistogramType>                   HistogramListType;
  typedef typename HistogramListType::Pointer         HistogramListPointerType;

  typedef typename TListSample::ConstPointer ListSampleConstPointerType;

  /** DataObject typedef*/
  typedef typename Superclass::DataObjectPointer        DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  // Set/Get the input list sample
  void SetListSample(const ListSampleType* inputlist);
  const ListSampleType* GetListSample() const;

  // Get the output histogram
  const HistogramListType*  GetOutput();


//   /** plug in the ListSample object */
//   void SetListSample(const TListSample* list)
//   {
//     m_List = list;
//   }

  /** Set the no data value. These value are ignored in histogram
   *  computation if NoDataFlag is On
   */
  itkSetMacro(NoDataValue, THistogramMeasurement);

  /** Set the no data value. These value are ignored in histogram
   *  computation if NoDataFlag is On
   */
  itkGetConstReferenceMacro(NoDataValue, THistogramMeasurement);

  /** Set the NoDataFlag. If set to true, samples with values equal to
   *  m_NoDataValue are ignored.
   */
  itkSetMacro(NoDataFlag, bool);

  /** Get the NoDataFlag. If set to true, samples with values equal to
   *  m_NoDataValue are ignored.
   */
  itkGetMacro(NoDataFlag, bool);

  /** Toggle the NoDataFlag. If set to true, samples with values equal to
   *  m_NoDataValue are ignored.
   */
  itkBooleanMacro(NoDataFlag);


  void SetMarginalScale(float scale)
  { m_MarginalScale = scale; }

  void SetNumberOfBins(unsigned int size)
  {
    m_Size[0] = size;
  }

//   HistogramListPointerType GetOutput() const
//   { return m_HistogramList; }

  itkSetMacro(AutoMinMax, bool);
  itkGetConstReferenceMacro(AutoMinMax, bool);

  void SetHistogramMin(const MeasurementVectorType& histogramMin)
  {
    m_HistogramMin = histogramMin;
    m_AutoMinMax = false;
  }

  void SetHistogramMax(const MeasurementVectorType& histogramMax)
  {
    m_HistogramMax = histogramMax;
    m_AutoMinMax = false;
  }

protected:
  ListSampleToHistogramListGenerator();
  ~ListSampleToHistogramListGenerator() override {}

  void GenerateData() override;

  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  //ListSampleConstPointerType m_List;
  HistogramSizeType          m_Size;
  float                      m_MarginalScale;
  MeasurementVectorType      m_HistogramMin;
  MeasurementVectorType      m_HistogramMax;
  bool                       m_AutoMinMax;
  //HistogramListPointerType   m_HistogramList;
  bool                       m_NoDataFlag;
  THistogramMeasurement      m_NoDataValue;

}; // end of class

} // end of namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToHistogramListGenerator.hxx"
#endif

#endif
