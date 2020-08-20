/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbListSampleToVariableDimensionHistogramGenerator_h
#define otbListSampleToVariableDimensionHistogramGenerator_h

#include "itkProcessObject.h"
#include "itkHistogram.h"
#include "itkStatisticsAlgorithm.h"

namespace otb
{

/** \class ListSampleToVariableDimensionHistogramGenerator
 *  \brief Generates a VariableDimensionHistogram from a ListSample
 *
 *  This class is a fork from itk::ListSampleToHistogramGenerator
 *  adapted to produce VariableDimensionHistogram, whose dimension can
 *  be chosen at runtime.
 *
 *  Since the MeasurementVectorSize template parameter is no longer
 *  needed, it has been removed.
 *
 * For more information, please refer to the
 * ListSampleToHistogramGenerator class documentation.
 *
 * \sa VariableDimensionHistogram, ListSample, ListSampleToHistogramGenerator
 *
 * \ingroup OTBStatistics
 */
template <class TListSample, class THistogramMeasurement, class TFrequencyContainer = itk::Statistics::DenseFrequencyContainer2>
class ITK_EXPORT ListSampleToVariableDimensionHistogramGenerator : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef ListSampleToVariableDimensionHistogramGenerator Self;
  typedef itk::ProcessObject                              Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleToVariableDimensionHistogramGenerator, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** typedef the ListSampleType */
  typedef TListSample ListSampleType;

  /** Type needed for defining the limits of the histogram bins */
  typedef typename itk::NumericTraits<THistogramMeasurement>::RealType HistogramMeasurementRealType;

  typedef itk::Statistics::Histogram<HistogramMeasurementRealType, TFrequencyContainer> HistogramType;

  typedef typename HistogramType::SizeType              HistogramSizeType;
  typedef typename HistogramType::MeasurementVectorType MeasurementVectorType;

  /** DataObject typedef*/
  typedef typename Superclass::DataObjectPointer             DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  // Set/Get the input list sample
  void SetListSample(const ListSampleType* inputlist);
  const ListSampleType* GetListSample() const;

  // Get the output histogram
  const HistogramType* GetOutput();

  void SetMarginalScale(float scale)
  {
    m_MarginalScale = scale;
  }

  void SetNumberOfBins(HistogramSizeType sizes)
  {
    m_Sizes = sizes;
  }

  itkSetMacro(AutoMinMax, bool);
  itkGetConstReferenceMacro(AutoMinMax, bool);

  void SetHistogramMin(const MeasurementVectorType& histogramMin)
  {
    m_HistogramMin = histogramMin;
    m_AutoMinMax   = false;
  }

  void SetHistogramMax(const MeasurementVectorType& histogramMax)
  {
    m_HistogramMax = histogramMax;
    m_AutoMinMax   = false;
  }

protected:
  ListSampleToVariableDimensionHistogramGenerator();
  ~ListSampleToVariableDimensionHistogramGenerator() override
  {
  }
  void GenerateData() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

private:
  HistogramSizeType     m_Sizes{0,0};
  float                 m_MarginalScale;
  MeasurementVectorType m_HistogramMin;
  MeasurementVectorType m_HistogramMax;
  bool                  m_AutoMinMax;

}; // end of class

} // end of namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleToVariableDimensionHistogramGenerator.hxx"
#endif

#endif
