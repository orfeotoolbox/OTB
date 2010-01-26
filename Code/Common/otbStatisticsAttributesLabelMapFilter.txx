/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStatisticsAttributesLabelMapFilter_txx
#define __otbStatisticsAttributesLabelMapFilter_txx

#include "otbStatisticsAttributesLabelMapFilter.h"

#include "itkMinimumMaximumImageCalculator.h"
#include "itkProgressReporter.h"
#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace otb
{

template <class TImage, class TFeatureImage>
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::StatisticsAttributesLabelMapFilter()
{
  m_FeatureName = "Default";
  m_ReducedAttributeSet = true;
  this->SetNumberOfRequiredInputs(2);
}

template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  Superclass::ThreadedProcessLabelObject( labelObject );

  ImageType * output = this->GetOutput();
  const FeatureImageType * featureImage = this->GetFeatureImage();

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  FeatureImagePixelType min = itk::NumericTraits< FeatureImagePixelType >::max();
  FeatureImagePixelType max = itk::NumericTraits< FeatureImagePixelType >::NonpositiveMin();
  double sum = 0;
  double sum2 = 0;
  double sum3 = 0;
  double sum4 = 0;
  unsigned int totalFreq = 0;
  IndexType minIdx;
  minIdx.Fill( 0 );
  IndexType maxIdx;
  maxIdx.Fill( 0 );
  PointType centerOfGravity;
  centerOfGravity.Fill( 0 );
  MatrixType centralMoments;
  centralMoments.Fill( 0 );
  MatrixType principalAxes;
  principalAxes.Fill( 0 );
  VectorType principalMoments;
  principalMoments.Fill( 0 );

  // iterate over all the lines
  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    const IndexType & firstIdx = lit->GetIndex();
    unsigned long length = lit->GetLength();

    long endIdx0 = firstIdx[0] + length;
    for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++)
      {
      const FeatureImagePixelType & v = featureImage->GetPixel( idx );
      ++totalFreq;

      // update min and max
      if( v <= min )
        {
        min = v;
        minIdx = idx;
        }
      if( v >= max )
        {
        max = v;
        maxIdx = idx;
        }

      //increase the sums
      sum += v;
      sum2 += vcl_pow( (double)v, 2 );
      sum3 += vcl_pow( (double)v, 3 );
      sum4 += vcl_pow( (double)v, 4 );

      // moments
      PointType physicalPosition;
      output->TransformIndexToPhysicalPoint(idx, physicalPosition);
      for(unsigned int i=0; i<ImageDimension; i++)
        {
        centerOfGravity[i] += physicalPosition[i] * v;
        centralMoments[i][i] += v * physicalPosition[i] * physicalPosition[i];
        for(unsigned int j=i+1; j<ImageDimension; j++)
          {
          double weight = v * physicalPosition[i] * physicalPosition[j];
          centralMoments[i][j] += weight;
          centralMoments[j][i] += weight;
          }
        }

      }
    }

  // final computations
  double mean = sum / totalFreq;
  double variance = ( sum2 - ( vcl_pow( sum, 2 ) / totalFreq ) ) / ( totalFreq - 1 );
  double sigma = vcl_sqrt( variance );
  double mean2 = mean * mean;
  double skewness = ( ( sum3 - 3.0 * mean * sum2) / totalFreq + 2.0 * mean * mean2 ) / ( variance * sigma );
  double kurtosis = ( ( sum4 - 4.0 * mean * sum3 + 6.0 * mean2 * sum2) / totalFreq - 3.0 * mean2 * mean2 ) / ( variance * variance ) - 3.0;

  double elongation = 0;
  if( sum != 0 )
    {
    // Normalize using the total mass
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centerOfGravity[i] /= sum;
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        centralMoments[i][j] /= sum;
        }
      }
  
    // Center the second order moments
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        centralMoments[i][j] -= centerOfGravity[i] * centerOfGravity[j];
        }
      }
  
    // Compute principal moments and axes
    vnl_symmetric_eigensystem<double> eigen( centralMoments.GetVnlMatrix() );
    vnl_diag_matrix<double> pm = eigen.D;
    for(unsigned int i=0; i<ImageDimension; i++)
      {
  //    principalMoments[i] = 4 * vcl_sqrt( pm(i,i) );
      principalMoments[i] = pm(i,i);
      }
    principalAxes = eigen.V.transpose();
  
    // Add a final reflection if needed for a proper rotation,
    // by multiplying the last row by the determinant
    vnl_real_eigensystem eigenrot( principalAxes.GetVnlMatrix() );
    vnl_diag_matrix< vcl_complex<double> > eigenval = eigenrot.D;
    vcl_complex<double> det( 1.0, 0.0 );
  
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      det *= eigenval( i, i );
      }
  
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      principalAxes[ ImageDimension-1 ][i] *= std::real( det );
      }
  
    if( principalMoments[0] != 0 )
      {
  //    elongation = principalMoments[ImageDimension-1] / principalMoments[0];
      elongation = vcl_sqrt(principalMoments[ImageDimension-1] / principalMoments[0]);
      }
    }
  else
    {
    // can't compute anything in that case - just set everything to a default value
    // Normalize using the total mass
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centerOfGravity[i] = 0;
      principalMoments[i] = 0;
      for(unsigned int j=0; j<ImageDimension; j++)
        {
        principalAxes[i][j] = 0;
        }
      }
    }
  itk::OStringStream oss;

  // finally put the values in the label object
  oss.str("");
  oss<<"STATS::"<<m_FeatureName<<"::Mean";
  labelObject->SetAttribute(oss.str().c_str(), mean );
  
  oss.str("");
  oss<<"STATS::"<<m_FeatureName<<"::Variance";
  labelObject->SetAttribute(oss.str().c_str(), variance );

  oss.str("");
  oss<<"STATS::"<<m_FeatureName<<"::Skewness";
  labelObject->SetAttribute(oss.str().c_str(), skewness );

  oss.str("");
  oss<<"STATS::"<<m_FeatureName<<"::Kurtosis";
  labelObject->SetAttribute(oss.str().c_str(),  kurtosis );

  // If we want all the features
  if(!m_ReducedAttributeSet)
    {
    oss.str("");
    oss<<"STATS::"<<m_FeatureName<<"::Minimum";
    labelObject->SetAttribute(oss.str().c_str(),(double)min );
    
    oss.str("");
    oss<<"STATS::"<<m_FeatureName<<"::Maximum";
    labelObject->SetAttribute(oss.str().c_str(), (double)max );
    
    oss.str("");
    oss<<"STATS::"<<m_FeatureName<<"::Sum";
    labelObject->SetAttribute(oss.str().c_str(), sum );
    
    oss.str("");
    oss<<"STATS::"<<m_FeatureName<<"::Sigma";
    labelObject->SetAttribute(oss.str().c_str(), sigma );
    
    for(unsigned int dim = 0;dim < ImageDimension;++dim)
      {
      oss.str("");
      oss<<"STATS::"<<m_FeatureName<<"::CenterOfGravity"<<dim;
      labelObject->SetAttribute(oss.str().c_str(), centerOfGravity[dim]);
      
      oss.str("");
      oss<<"STATS::"<<m_FeatureName<<"::PrincipalMoments"<<dim;
      labelObject->SetAttribute( oss.str().c_str(),principalMoments[dim]);
      
      oss.str("");
      oss<<"STATS::"<<m_FeatureName<<"::FirstMinimumIndex"<<dim;
      labelObject->SetAttribute(oss.str().c_str(), minIdx[dim] );
      
      oss.str("");
      oss<<"STATS::"<<m_FeatureName<<"::FirstMaximumIndex"<<dim;
      labelObject->SetAttribute(oss.str().c_str(), maxIdx[dim]);
      
      for(unsigned int dim2 = 0;dim2 < ImageDimension;++dim2)
       {
       oss.str("");
       oss<<"STATS::"<<m_FeatureName<<"::PrincipalAxis"<<dim<<dim2;
       labelObject->SetAttribute( oss.str().c_str(),principalAxes(dim,dim2));
       }
      }
    }
}


template <class TImage, class TFeatureImage>
void
StatisticsAttributesLabelMapFilter<TImage, TFeatureImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}


}// end namespace itk
#endif
