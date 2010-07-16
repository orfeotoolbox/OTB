/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator.h,v $
  Language:  C++
  Date:      $Date: 2008/10/18 00:22:58 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator_h
#define __itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator_h

#include "itkHistogram.h"
#include "itkMacro.h"

namespace itk {
  namespace Statistics {

/** \class GreyLevelRunLengthMatrixTextureCoefficientsCalculator 
*  \brief This class computes texture feature coefficients from a grey level
* run-length matrix.
*/
    
template< class THistogram >
class GreyLevelRunLengthMatrixTextureCoefficientsCalculator : public Object
  {
  public:
    /** Standard typedefs */
    typedef GreyLevelRunLengthMatrixTextureCoefficientsCalculator Self;
    typedef Object Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(GreyLevelRunLengthMatrixTextureCoefficientsCalculator, Object);
    
    /** standard New() method support */
    itkNewMacro(Self) ;
    
    typedef THistogram                                      HistogramType;
    typedef typename HistogramType::Pointer                 HistogramPointer;
    typedef typename HistogramType::ConstPointer            HistogramConstPointer;
    typedef typename HistogramType::MeasurementType         MeasurementType;
    typedef typename HistogramType::MeasurementVectorType   MeasurementVectorType;
    typedef typename HistogramType::IndexType               IndexType;
    typedef typename HistogramType::FrequencyType           FrequencyType;
    
    /** Triggers the Computation of the histogram */
    void Compute( void );
    
    /** Connects the GLCM histogram over which the features are going to be computed */
    itkSetObjectMacro( Histogram, HistogramType );
    itkGetObjectMacro( Histogram, HistogramType );

    /** Methods to return the feature values.
      \warning These outputs are only valid after the 
       Compute() method has been invoked 
      \sa Compute */
    
    itkGetMacro( ShortRunEmphasis, double );
    itkGetMacro( LongRunEmphasis, double );
    itkGetMacro( GreyLevelNonuniformity, double );
    itkGetMacro( RunLengthNonuniformity, double );
    itkGetMacro( LowGreyLevelRunEmphasis, double );
    itkGetMacro( HighGreyLevelRunEmphasis, double );
    itkGetMacro( ShortRunLowGreyLevelEmphasis, double );
    itkGetMacro( ShortRunHighGreyLevelEmphasis, double );
    itkGetMacro( LongRunLowGreyLevelEmphasis, double );
    itkGetMacro( LongRunHighGreyLevelEmphasis, double );

    itkGetMacro( TotalNumberOfRuns, unsigned long );
    
  protected:
    GreyLevelRunLengthMatrixTextureCoefficientsCalculator() {};
    virtual ~GreyLevelRunLengthMatrixTextureCoefficientsCalculator() {};
    void PrintSelf(std::ostream& os, Indent indent) const;    

   private:
    HistogramPointer m_Histogram;
    double m_ShortRunEmphasis, m_LongRunEmphasis, m_GreyLevelNonuniformity,
      m_RunLengthNonuniformity, m_LowGreyLevelRunEmphasis, 
      m_HighGreyLevelRunEmphasis, m_ShortRunLowGreyLevelEmphasis, 
      m_ShortRunHighGreyLevelEmphasis, m_LongRunLowGreyLevelEmphasis,
      m_LongRunHighGreyLevelEmphasis;

    unsigned long m_TotalNumberOfRuns; 
  };
    
    
  } // end of namespace Statistics 
} // end of namespace itk 

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator.txx"
#endif

#endif
