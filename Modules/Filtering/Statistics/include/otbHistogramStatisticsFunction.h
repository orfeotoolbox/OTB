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


#ifndef otbHistogramStatisticsFunction_h
#define otbHistogramStatisticsFunction_h

#include "itkNumericTraits.h"
#include "itkHistogramAlgorithmBase.h"

namespace otb
{

/** \class HistogramStatisticsFunction
 * \brief Computes parameters for a histogram.
 *
 * You plug in the target histogram using SetInputHistogram method and
 * get the entropy value from the histogram by calling the method GetEntropy
 * or the GenerateData method.
 *
 * \ingroup Function
 *
 * \ingroup OTBStatistics
 */

template<class TInputHistogram, class TOutput>
class HistogramStatisticsFunction :
  public itk::HistogramAlgorithmBase<TInputHistogram>
{
public:
  /**Standard class typedefs. */
  typedef HistogramStatisticsFunction                  Self;
  typedef itk::HistogramAlgorithmBase<TInputHistogram> Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  typedef typename TInputHistogram::MeasurementType              MeasurementType;
  typedef typename TInputHistogram::AbsoluteFrequencyType        FrequencyType; //FIXME several possibilities in the new framework
  typedef typename itk::NumericTraits<MeasurementType>::RealType RealType;

  /**Standard Macros */
  itkTypeMacro(HistogramStatisticsFunction, HistogramAlgorithmsBase);
  itkNewMacro(Self);

  /** Typedef for the output type */
  typedef std::vector<TOutput> OutputType;

  /** Returns the entropy value */
  OutputType GetEntropy();

  /** Returns the mean value from histogram*/
  OutputType GetMean();

  /** Returns the covariance value from histogram*/
  OutputType GetCovariance();

  /** Stores the histogram pointer */
  void SetInputHistogram(const TInputHistogram * histogram)
  {
    if (m_InputHistogram != histogram)
      {
      m_InputHistogram = histogram;
      this->Modified();
      m_IsModified = true;
      }
  }

  /** Calculates the thresholds and save them */
  void Compute() override
  {
    this->GenerateData();
  }

protected:

  HistogramStatisticsFunction();
  ~HistogramStatisticsFunction() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Calculates the thresholds and save them */
  void GenerateData();

  /** Calculate the entropy value */
  void CalculateEntropy();

  /** Calculate mean value */
  void CalculateMean();

  /** Calculate covariance value */
  void CalculateCovariance();

  

private:

  OutputType m_entropy;
  OutputType m_mean;
  OutputType m_covariance;
  bool       m_IsModified;

  /** Target histogram data pointer */
  typename TInputHistogram::ConstPointer m_InputHistogram;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramStatisticsFunction.hxx"
#endif

#endif
