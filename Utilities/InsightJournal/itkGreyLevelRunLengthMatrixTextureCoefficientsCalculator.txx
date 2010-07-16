/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator.txx,v $
  Language:  C++
  Date:      $Date: 2008/10/18 00:22:58 $
  Version:   $Revision: 1.1.1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator_txx
#define _itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator_txx

#include "itkGreyLevelRunLengthMatrixTextureCoefficientsCalculator.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"

namespace itk {
  namespace Statistics {

    template< class THistogram >
    void
    GreyLevelRunLengthMatrixTextureCoefficientsCalculator< THistogram >::
    Compute( void )
      {
      typedef typename HistogramType::Iterator HistogramIterator;

      this->m_TotalNumberOfRuns = static_cast<unsigned long>
        ( this->m_Histogram->GetTotalFrequency() );

      // Finally compute the texture features. Another one pass.
      this->m_ShortRunEmphasis = this->m_LongRunEmphasis =
        this->m_GreyLevelNonuniformity = this->m_RunLengthNonuniformity =
        this->m_LowGreyLevelRunEmphasis = this->m_HighGreyLevelRunEmphasis =
        this->m_ShortRunLowGreyLevelEmphasis =
        this->m_ShortRunHighGreyLevelEmphasis =
        this->m_LongRunLowGreyLevelEmphasis =
        this->m_LongRunHighGreyLevelEmphasis = 0;

      vnl_vector<double> greyLevelNonuniformityVector(
        this->m_Histogram->GetSize()[0], 0.0 );
      vnl_vector<double> runLengthNonuniformityVector(
        this->m_Histogram->GetSize()[1], 0.0 );

      for ( HistogramIterator hit = this->m_Histogram->Begin();
              hit != this->m_Histogram->End(); ++hit )
        {
        MeasurementType frequency = hit.GetFrequency();
        if ( frequency == 0 )
          {
          continue;
          }
        MeasurementVectorType measurement = hit.GetMeasurementVector();
        IndexType index
          = this->m_Histogram->GetIndex( hit.GetInstanceIdentifier() );

        double i2 = static_cast<double>( ( index[0] + 1 ) * ( index[0] + 1 ) );
        double j2 = static_cast<double>( ( index[1] + 1 ) * ( index[1] + 1 ) );

        // Traditional measures
        this->m_ShortRunEmphasis += ( frequency / j2 );
        this->m_LongRunEmphasis += ( frequency * j2 );

        greyLevelNonuniformityVector[index[0]] += frequency;
        runLengthNonuniformityVector[index[1]] += frequency;

        // measures from Chu et al.
        this->m_LowGreyLevelRunEmphasis += ( frequency / i2 );
        this->m_HighGreyLevelRunEmphasis += ( frequency * i2 );

        // measures from Dasarathy and Holder
        this->m_ShortRunLowGreyLevelEmphasis += ( frequency / ( i2 * j2 ) );
        this->m_ShortRunHighGreyLevelEmphasis += ( frequency * i2 / j2 );
        this->m_LongRunLowGreyLevelEmphasis += ( frequency * j2 / i2 );
        this->m_LongRunHighGreyLevelEmphasis += ( frequency * i2 * j2 );
        }
      this->m_GreyLevelNonuniformity
        = greyLevelNonuniformityVector.squared_magnitude();
      this->m_RunLengthNonuniformity
        = runLengthNonuniformityVector.squared_magnitude();

      // Normalize all measures by the total number of runs
      this->m_ShortRunEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_LongRunEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_GreyLevelNonuniformity /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_RunLengthNonuniformity /=
        static_cast<double>( this->m_TotalNumberOfRuns );

      this->m_LowGreyLevelRunEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_HighGreyLevelRunEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );

      this->m_ShortRunLowGreyLevelEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_ShortRunHighGreyLevelEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_LongRunLowGreyLevelEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      this->m_LongRunHighGreyLevelEmphasis /=
        static_cast<double>( this->m_TotalNumberOfRuns );
      }


    template< class THistogram >
    void
    GreyLevelRunLengthMatrixTextureCoefficientsCalculator< THistogram >::
    PrintSelf(std::ostream& os, Indent indent) const
      {
      Superclass::PrintSelf(os,indent);
      }

  } // end of namespace Statistics
} // end of namespace itk


#endif
