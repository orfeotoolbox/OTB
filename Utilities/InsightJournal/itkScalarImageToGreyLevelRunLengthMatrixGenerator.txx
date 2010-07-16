/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarImageToGreyLevelRunLengthMatrixGenerator.txx,v $
  Language:  C++
  Date:      $Date: 2009/04/10 16:16:26 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkScalarImageToGreyLevelRunLengthMatrixGenerator_txx
#define _itkScalarImageToGreyLevelRunLengthMatrixGenerator_txx

#include "itkScalarImageToGreyLevelRunLengthMatrixGenerator.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhood.h"
#include "vnl/vnl_math.h"


namespace itk {
  namespace Statistics {

    template< class TImageType, class THistogramFrequencyContainer >
    ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    ScalarImageToGreyLevelRunLengthMatrixGenerator() :
    m_NumberOfBinsPerAxis(itkGetStaticConstMacro(DefaultBinsPerAxis))
      {
      this->m_Min = NumericTraits<PixelType>::NonpositiveMin();
      this->m_Max = NumericTraits<PixelType>::max();
      this->m_MinDistance = NumericTraits<RealType>::Zero;
      this->m_MaxDistance = NumericTraits<RealType>::max();
      
      // Get a set of default offset values.
      typedef Neighborhood<PixelType, ImageDimension> NeighborhoodType;
      NeighborhoodType neighborhood;
      typename NeighborhoodType::RadiusType radius;
      radius.Fill( 1 );
      neighborhood.SetRadius( radius );

      this->m_Offsets = OffsetVector::New();  
      unsigned int numberOfOffsets = static_cast<unsigned int>( 
        0.5 * neighborhood.Size() );
      for( unsigned int n = 0; n < numberOfOffsets; n++ )
        {
        this->AddOffset( neighborhood.GetOffset( n ) );
        }  
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    Compute( void )
      {
      // First, create an appropriate histogram with the right number of bins
      // and mins and maxes correct for the image type.
      this->m_Output = HistogramType::New();
      typename HistogramType::SizeType size;
      size.Fill(this->m_NumberOfBinsPerAxis);
      this->m_LowerBound[0] = this->m_Min;
      this->m_LowerBound[1] = this->m_MinDistance;
      this->m_UpperBound[0] = this->m_Max;
      this->m_UpperBound[1] = this->m_MaxDistance;

      this->m_Output->Initialize(size, this->m_LowerBound, this->m_UpperBound);

      // Now fill in the histogram
      this->FillHistogram();
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    FillHistogram()
      {

      // Iterate over all of those pixels and offsets, adding each
      // co-occurrence pair to the histogram

      typedef ConstNeighborhoodIterator<ImageType> NeighborhoodIteratorType;
      typename NeighborhoodIteratorType::RadiusType radius;
      radius.Fill( 1 );
      NeighborhoodIteratorType neighborIt( radius,
        this->m_Input, this->m_Input->GetRequestedRegion() );
        
      typename OffsetVector::ConstIterator offsets;
      for( offsets = this->GetOffsets()->Begin(); 
        offsets != this->GetOffsets()->End(); offsets++ )
        {
        typedef Image<bool, ImageDimension> BoolImageType;
        typename BoolImageType::Pointer alreadyVisitedImage = BoolImageType::New();
        alreadyVisitedImage->SetRegions( this->m_Input->GetRequestedRegion() );
        alreadyVisitedImage->SetOrigin( this->m_Input->GetOrigin() );
        alreadyVisitedImage->SetSpacing( this->m_Input->GetSpacing() );
        alreadyVisitedImage->Allocate();
        alreadyVisitedImage->FillBuffer( false );

        neighborIt.GoToBegin();
        OffsetType offset = offsets.Value();

        for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
          {
          const PixelType centerPixelIntensity = neighborIt.GetCenterPixel();
          IndexType centerIndex = neighborIt.GetIndex();
          if ( centerPixelIntensity < this->m_Min ||
               centerPixelIntensity > this->m_Max ||
               alreadyVisitedImage->GetPixel( centerIndex ) )
            {
            continue; // don't put a pixel in the histogram if the value
                      // is out-of-bounds.
            }

          MeasurementType centerBinMin = this->GetOutput()->
            GetBinMinFromValue( 0, centerPixelIntensity );
          MeasurementType centerBinMax = this->GetOutput()->
            GetBinMaxFromValue( 0, centerPixelIntensity );

          IndexType index = centerIndex;
          PixelType pixelIntensity = this->GetInput()->GetPixel( index );
          while ( pixelIntensity >= centerBinMin &&
                  pixelIntensity <= centerBinMax &&
                  !alreadyVisitedImage->GetPixel( index ) )
            {
            alreadyVisitedImage->SetPixel( index, true );
            index += offset;
            if ( this->GetInput()->GetRequestedRegion().IsInside( index ) )
              {
              pixelIntensity = this->GetInput()->GetPixel( index );
              }
            else
              {
              break;
              }
            }

          PointType centerPoint;
          this->m_Input->TransformIndexToPhysicalPoint( centerIndex, centerPoint );
          PointType point;
          this->m_Input->TransformIndexToPhysicalPoint( index, point );

          MeasurementVectorType run;
          run[0] = centerPixelIntensity;
          run[1] = centerPoint.EuclideanDistanceTo( point );

          if ( run[1] >= this->m_MinDistance &&
               run[1] <= this->m_MaxDistance )
            {
            this->m_Output->IncreaseFrequency( run, 1 );
            }
          }
        }
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    SetPixelValueMinMax( PixelType min, PixelType max )
      {
      itkDebugMacro("setting Min to " << min << "and Max to " << max);
      this->m_Min = min;
      this->m_Max = max;
      this->Modified();
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    SetDistanceValueMinMax( RealType min, RealType max )
      {
      itkDebugMacro( "setting MinDistance to " << min <<
                     "and MaxDistance to " << max );
      this->m_MinDistance = min;
      this->m_MaxDistance = max;
      this->Modified();
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    PrintSelf(std::ostream& os, Indent indent) const
      {
      Superclass::PrintSelf(os,indent);
      }

  } // end of namespace Statistics
} // end of namespace itk


#endif
