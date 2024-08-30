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


#ifndef otbPathListToHistogramGenerator_h
#define otbPathListToHistogramGenerator_h

#include "itkSampleToHistogramFilter.h"
#include "itkHistogram.h"
#include "itkListSample.h"
#include "itkVector.h"
#include "otbObjectList.h"

// TODO : write a HistogramSource when outputting Histogram

namespace otb
{

/** \class PathListToHistogramGenerator
 *  \brief This class generates an histogram from a list of path.
 *
 *  The concept of Histogram in ITK is quite generic. It has been designed to
 *  manage multiple components data. This class facilitates the computation of
 *  an histogram from a list of path. Internally it creates a List that is feed into
 *  the ListSampleToHistogramGenerator.
 *
 *
 * \ingroup OTBPath
 */

template <class TPath, class TFunction>
class PathListToHistogramGenerator : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef PathListToHistogramGenerator  Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PathListToHistogramGenerator, itk::ProcessObject);

  /** standard New() method support */
  itkNewMacro(Self);

  typedef TPath                      PathType;
  typedef typename PathType::Pointer PathPointer;
  typedef ObjectList<PathType>       PathListType;

  typedef TFunction FunctionType;

  typedef typename TFunction::OutputType RealType;

  typedef RealType MeasurementType;
  typedef itk::Vector<MeasurementType, 1> ListSampleVectorType;
  typedef itk::Statistics::ListSample<ListSampleVectorType> ListSampleType;
  typedef typename ListSampleType::Pointer                  ListSamplePointer;
  typedef typename ListSampleType::ConstPointer             ListSampleConstPointer;

  typedef itk::Statistics::DenseFrequencyContainer2 FrequencyContainerType;
  typedef itk::Statistics::Histogram<MeasurementType, FrequencyContainerType> HistogramType;

  typedef itk::Statistics::SampleToHistogramFilter<ListSampleType, HistogramType> GeneratorType;
  typedef typename GeneratorType::Pointer GeneratorPointer;

  typedef typename HistogramType::Pointer               HistogramPointer;
  typedef typename HistogramType::ConstPointer          HistogramConstPointer;
  typedef typename HistogramType::SizeType              SizeType;
  typedef typename HistogramType::MeasurementVectorType MeasurementVectorType;

  /** DataObject typedef*/
  typedef typename Superclass::DataObjectPointer             DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  void GraftNthOutput(unsigned int idx, itk::DataObject* graft);
  void GraftOutput(itk::DataObject* graft);

public:
  /** Connects the input PathList for which the histogram is going to be computed */
  using Superclass::SetInput;
  void SetInput(const PathListType* path);
  const PathListType* GetInput() const;

  // Return the output histogram.
  const HistogramType* GetOutput() const;

  /** Set number of histogram bins */
  void SetNumberOfBins(const SizeType& size);

  /** Set marginal scale value to be passed to the histogram generator */
  void SetMarginalScale(double marginalScale);
  void SetHistogramMin(const MeasurementVectorType& histogramMin);
  void SetHistogramMax(const MeasurementVectorType& histogramMax);
  void SetAutoMinMax(bool autoMinMax);

protected:
  PathListToHistogramGenerator();
  ~PathListToHistogramGenerator() override
  {
  }
  void              GenerateData() override;
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType) override;
  using Superclass::MakeOutput;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GeneratorPointer m_HistogramGenerator;
};

} // end of namespace OTB

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathListToHistogramGenerator.hxx"
#endif

#endif
