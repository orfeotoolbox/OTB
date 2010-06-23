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

#include "itkHistogram.h"
#include "itkMacro.h"

namespace otb {
  
/** \class GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator 
 *  \brief This class computes texture feature coefficients from a grey level
 * co-occurrence matrix.
 */
    
template < typename THistogram >
class ITK_EXPORT GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator : 
  public itk::Object
{
public:
  /** Standard typedefs */
  typedef GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator  Self;
  typedef itk::Object                                                   Superclass;
  typedef itk::SmartPointer<Self>                                       Pointer;
  typedef itk::SmartPointer<const Self>                                 ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator, itk::Object);
  
  /** standard New() method support */
  itkNewMacro(Self);
    
  typedef THistogram                                      HistogramType;
  typedef typename HistogramType::Pointer                 HistogramPointer;
  typedef typename HistogramType::ConstPointer            HistogramConstPointer;
  typedef typename HistogramType::MeasurementType         MeasurementType;
  typedef typename HistogramType::MeasurementVectorType   MeasurementVectorType;
  typedef typename HistogramType::IndexType               IndexType;
  typedef typename HistogramType::FrequencyType           FrequencyType;
    
  /** Connects the GLCM histogram over which the features are going to be computed */
  itkSetObjectMacro( Histogram, HistogramType );
  itkGetObjectMacro( Histogram, HistogramType );
  
  itkGetMacro(Variance, double);
  itkGetMacro(SumAverage, double);
  itkGetMacro(SumVariance, double);
  itkGetMacro(SumEntropy, double);
  itkGetMacro(DifferenceEntropy, double);
  itkGetMacro(DifferenceVariance, double);
  itkGetMacro(IC1, double);
  itkGetMacro(IC2, double);
  
  /** Triggers the Computation of the histogram */
  void Compute( void );
  
protected:
  GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator() {};
  virtual ~GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  
private:
  GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
   
  HistogramPointer m_Histogram;
  
  void NormalizeHistogram(void);
  void ComputeMean( double &pixelMean );
  double m_Variance, m_SumAverage, m_SumVariance,m_SumEntropy, m_DifferenceEntropy,
  m_DifferenceVariance, m_IC1, m_IC2;

  double ComputePS ( long unsigned int k );
  double ComputePD ( long unsigned int k );
};
} // end of namespace otb 

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.txx"
#endif

#endif
