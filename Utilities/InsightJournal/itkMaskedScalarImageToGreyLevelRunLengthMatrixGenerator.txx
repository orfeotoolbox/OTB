/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator.txx,v $
  Language:  C++
  Date:      $Date: 2009/03/23 18:13:26 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator_txx
#define _itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator_txx

#include "itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator.h"

#include "itkConstNeighborhoodIterator.h"
#include "vnl/vnl_math.h"


namespace itk {
  namespace Statistics {

    template< class TImageType, class THistogramFrequencyContainer >
    MaskedScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    MaskedScalarImageToGreyLevelRunLengthMatrixGenerator() :
    m_ImageMask( NULL ), m_InsidePixelValue( NumericTraits<PixelType>::One )
      {
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    MaskedScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    FillHistogram( RadiusType radius, RegionType region )
      {
      if ( this->m_ImageMask.IsNull() )
        {
        // If there's no mask set, just use the (faster) superclass method
        Superclass::FillHistogram( radius, region );
        return;
        }

      // Iterate over all of those pixels and offsets, adding each
      // co-occurrence pair to the histogram
/*
      typedef ConstNeighborhoodIterator<ImageType> NeighborhoodIteratorType;
      typename NeighborhoodIteratorType::RadiusType radius;
      radius.Fill( 1 );
      NeighborhoodIteratorType neighborIt( radius,
        this->GetInput(), this->GetInput()->GetRequestedRegion() );
*/
      typedef ConstNeighborhoodIterator<ImageType> NeighborhoodIteratorType;
      NeighborhoodIteratorType neighborIt, maskNeighborIt;
      neighborIt = NeighborhoodIteratorType( radius, this->m_Input, region );
      maskNeighborIt = NeighborhoodIteratorType( radius, this->m_ImageMask, region );

      typename OffsetVector::ConstIterator offsets;
      for( offsets = this->GetOffsets()->Begin(); 
        offsets != this->GetOffsets()->End(); offsets++ )
        {
         
        typedef Image<bool, ImageDimension> BoolImageType;
        typename BoolImageType::Pointer alreadyVisitedImage = BoolImageType::New();
        alreadyVisitedImage->SetRegions( this->GetInput()->GetRequestedRegion() );
        alreadyVisitedImage->SetOrigin( this->GetInput()->GetOrigin() );
        alreadyVisitedImage->SetSpacing( this->GetInput()->GetSpacing() );
        alreadyVisitedImage->Allocate();
        alreadyVisitedImage->FillBuffer( false );

        neighborIt.GoToBegin();
        OffsetType offset = offsets.Value();

        for ( neighborIt.GoToBegin(); !neighborIt.IsAtEnd(); ++neighborIt )
          {
          if ( maskNeighborIt.GetCenterPixel() != this->m_InsidePixelValue )
            {
            continue; // Go to the next loop if we're not in the mask
            }

          const PixelType centerPixelIntensity = neighborIt.GetCenterPixel();
          IndexType centerIndex = neighborIt.GetIndex();

          if ( centerPixelIntensity < this->GetMin() ||
               centerPixelIntensity > this->GetMax() ||
               alreadyVisitedImage->GetPixel( centerIndex ) ||
               this->m_ImageMask->GetPixel( centerIndex )
                  != this->m_InsidePixelValue )
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
                  !alreadyVisitedImage->GetPixel( index ) &&
                  this->m_ImageMask->GetPixel( index )
                    == this->m_InsidePixelValue )
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
          this->GetInput()->TransformIndexToPhysicalPoint( centerIndex, centerPoint );
          PointType point;
          this->GetInput()->TransformIndexToPhysicalPoint( index, point );

          MeasurementVectorType run;
          run[0] = centerPixelIntensity;
          run[1] = centerPoint.EuclideanDistanceTo( point );

          if ( run[1] >= this->GetMinDistance() &&
               run[1] <= this->GetMaxDistance() )
            {
            this->GetOutput()->IncreaseFrequency( run, 1 );
            }
          }
        }
      }

    template< class TImageType, class THistogramFrequencyContainer >
    void
    MaskedScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType,
    THistogramFrequencyContainer >::
    PrintSelf(std::ostream& os, Indent indent) const
      {
      Superclass::PrintSelf(os,indent);
      }

  } // end of namespace Statistics
} // end of namespace itk


#endif
