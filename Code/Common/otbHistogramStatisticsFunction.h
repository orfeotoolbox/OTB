/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbHistogramStatisticsFunction_h
#define __otbHistogramStatisticsFunction_h

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
  void Compute() 
  {
    this->GenerateData();
  }

protected:

  HistogramStatisticsFunction();
  virtual ~HistogramStatisticsFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

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
#include "otbHistogramStatisticsFunction.txx"
#endif

#endif
