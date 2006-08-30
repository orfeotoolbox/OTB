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


#ifndef __otbHistogramStatisticFunction_h
#define __otbHistogramStatisticFunction_h

#include "itkHistogramAlgorithmBase.h"

namespace otb
{

/** \class HistogramStatisticFunction
 * \brief Computes parameters for a histogram.
 * 
 * You plug in the target histogram using SetInputHistogram method and 
 * get the entropy value from the histogram by calling the method GetEntropy
 * or the GenerateData method. 
 *
 * \ingroup Function
 */

template< class TInputHistogram, class TOutput >
class HistogramStatisticFunction :
      public itk::HistogramAlgorithmBase< TInputHistogram >
{
public:
  /**Standard class typedefs. */
  typedef HistogramStatisticFunction                      Self;
  typedef itk::HistogramAlgorithmBase<TInputHistogram>    Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef typename TInputHistogram::MeasurementType  MeasurementType;
  typedef typename TInputHistogram::FrequencyType    FrequencyType;

  typedef typename itk::NumericTraits<MeasurementType>::RealType EntropyType;

  /**Standard Macros */
  itkTypeMacro(HistogramStatisticFunction, HistogramAlgorithmsBase);
  itkNewMacro(Self) ;
                                                                                                                                      
  /** Typedef for the output type */
  typedef TOutput OutputType;
                                                                                                                                      
  /** Returns the entropy value */
  OutputType GetEntropy();
                                             
protected:
  HistogramStatisticFunction() ;
  virtual ~HistogramStatisticFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
                                                                                                                                      
  /** Calculates the thresholds and save them */
  void GenerateData() ;
                         

private:

  OutputType m_entropy ;

} ; // end of class
                                                                                                                                      
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramStatisticFunction.txx"
#endif

#endif
