/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGreyLevelCooccurrenceMatrixTextureCoefficientsCalculator.h,v $
  Language:  C++
  Date:      $Date: 2009-05-20 16:21:47 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator_h
#define __otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator_h

#include "otbMacro.h"
#include "itkGreyLevelCooccurrenceMatrixTextureCoefficientsCalculator.h"

namespace otb {
  
/** \class GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator 
 *  \brief This class computes texture feature coefficients from a grey level
 * co-occurrence matrix.
 */
    
template < typename THistogram >
class ITK_EXPORT GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator : 
  public itk::Statistics::GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator<THistogram>
{
public:
  /** Standard typedefs */
  typedef GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator  Self;
  typedef itk::Statistics::GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator<THistogram>                                                   Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator, itk::Statistics::GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator);
  
  /** standard New() method support */
  itkNewMacro(Self);
    
  typedef THistogram                                      HistogramType;
  typedef typename HistogramType::Pointer                 HistogramPointer;
  typedef typename HistogramType::ConstPointer            HistogramConstPointer;
  typedef typename HistogramType::MeasurementType         MeasurementType;
  typedef typename HistogramType::MeasurementVectorType   MeasurementVectorType;
  typedef typename HistogramType::IndexType               IndexType;
  typedef typename HistogramType::FrequencyType           FrequencyType;
    
  itkGetMacro(SumOfSquares, double);
  itkGetMacro(SumAverage, double);
  
  /** Triggers the Computation of the histogram */
  void ComputeAdvancedTextures( void );
  
protected:
  GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator() {};
  virtual ~GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  
private:
  GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  HistogramPointer m_Histogram;
//   double m_Energy, m_Entropy, m_Correlation, m_InverseDifferenceMoment,
//     m_Inertia, m_ClusterShade, m_ClusterProminence, m_HaralickCorrelation;
   
  void NormalizeHistogram(void);
  void ComputeMeansAndVariances( double &pixelMean, double &marginalMean, 
                                double &marginalDevSquared, double &pixelVariance );
  double m_SumOfSquares, m_SumAverage, m_SumVariance,m_SumEntropy, m_DifferenceEntropy,
  m_DifferenceVariance, m_IC1, m_IC2;

  double ComputePS ( unsigned int k );
  double ComputePD ( unsigned int k );
};
} // end of namespace otb 

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.txx"
#endif

#endif
