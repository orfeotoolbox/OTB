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
#ifndef __otbNormalizeVectorImageFilter_txx
#define __otbNormalizeVectorImageFilter_txx
#include "otbNormalizeVectorImageFilter.h"

#include "otbStreamingStatisticsVectorImageFilter2.h"

namespace otb 
{

template < class TInputImage, class TOutputImage >
NormalizeVectorImageFilter< TInputImage, TOutputImage >
::NormalizeVectorImageFilter ()
{
  m_IsGivenMean = false;
  m_IsGivenStdDev = false;

  m_UseMean = true;
  m_UseStdDev = true;
}

template < class TInputImage, class TOutputImage >
void
NormalizeVectorImageFilter< TInputImage, TOutputImage >
::BeforeThreadedGenerateData ()
{
  if ( !m_UseMean )
  {
    typename TInputImage::PixelType vector ( this->GetInput()->GetNumberOfComponentsPerPixel() );
    vector.Fill( itk::NumericTraits< typename TInputImage::PixelType::ValueType >::Zero );
    this->GetFunctor().SetMean( vector );
  }

  if ( !m_UseStdDev )
  {
    typename TInputImage::PixelType vector ( this->GetInput()->GetNumberOfComponentsPerPixel() );
    vector.Fill( itk::NumericTraits< typename TInputImage::PixelType::ValueType >::One );
    this->GetFunctor().SetStdDev( vector );
  }

  if ( !m_IsGivenMean )
  {
    typedef StreamingStatisticsVectorImageFilter2< InputImageType > CovarianceEstimatorFilterType;
    typename CovarianceEstimatorFilterType::Pointer estimator = CovarianceEstimatorFilterType::New();
    estimator->SetInput( const_cast<InputImageType*>( this->GetInput() ) );
    estimator->Update();

    this->GetFunctor().SetMean( estimator->GetMean() );

    if ( !m_IsGivenStdDev && m_UseStdDev )
    {
      typename StreamingStatisticsVectorImageFilter2< InputImageType >::RealPixelType sigma
        ( this->GetInput()->GetNumberOfComponentsPerPixel() );
      for ( unsigned int i = 0; i < sigma.Size(); i++ )
        sigma[i] = vcl_sqrt( estimator->GetCovariance()(i,i) );

      this->GetFunctor().SetStdDev( sigma );
    }
  }
}

} // end of namespace otb

#endif // __otbNormalizeVectorImageFilter_txx
