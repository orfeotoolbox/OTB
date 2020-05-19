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

#ifndef otbGaussianAdditiveNoiseSampleListFilter_h
#define otbGaussianAdditiveNoiseSampleListFilter_h

#include "otbListSampleToListSampleFilter.h"

namespace otb
{
namespace Statistics
{

/** \class GaussianAdditiveNoiseSampleListFilter
 *  \brief This class generate a noised version of the input sample list
 *
 * For each component of the samples, a white gaussian random is added.
 *
 * It use the MersenneTwisterRandomVariateGenerator to generate a
 * sequence of numbers following the normal law considered as White
 * Gaussian Noise.
 * Mean and Variance are set via the methods SetMean() and SetVariance().
 * The input SampleList can be noised m_NumberOfIteration times via the
 * method SetNumberOfIteration()
 *
 * \sa ListSampleToListSampleFilter
 *
 * \ingroup OTBStatistics
 */
template <class TInputSampleList, class TOutputSampleList = TInputSampleList>
class ITK_EXPORT GaussianAdditiveNoiseSampleListFilter : public otb::Statistics::ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
{
public:
  /** Standard class typedefs */
  typedef GaussianAdditiveNoiseSampleListFilter Self;
  typedef otb::Statistics::ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(GaussianAdditiveNoiseSampleListFilter, otb::Statistics::ListSampleToListSampleFilter);

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
  itkSetMacro(Mean, double);
  itkGetMacro(Mean, double);

  /** Set/Get the Scales for this sample list */
  itkSetMacro(Variance, double);
  itkGetMacro(Variance, double);

  /** Set how many times the ListSample must be noised */
  itkSetMacro(NumberOfIteration, unsigned int);
  itkGetMacro(NumberOfIteration, unsigned int);

protected:
  /** This method causes the filter to generate its output. */
  void GenerateData() override;

  /** Generate a white gaussian noise with mean m_Mean and variance
    * m_Variance
    */
  void GenerateRandomSequence();

  GaussianAdditiveNoiseSampleListFilter();
  ~GaussianAdditiveNoiseSampleListFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  GaussianAdditiveNoiseSampleListFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Mean and variance for the white gaussian noise to generate
  double              m_Mean;
  double              m_Variance;
  std::vector<double> m_WhiteGaussianNoiseCoefficients;
  unsigned int        m_NumberOfIteration;

}; // end of class ImageToListGenerator

} // end of namespace Statistics
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGaussianAdditiveNoiseSampleListFilter.hxx"
#endif

#endif
