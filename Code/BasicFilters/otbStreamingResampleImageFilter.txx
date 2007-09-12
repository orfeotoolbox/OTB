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
#ifndef _otbStreamingResampleImageFilter_txx
#define _otbStreamingResampleImageFilter_txx

#include "otbStreamingResampleImageFilter.h"
#include "otbStreamingTraits.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkContinuousIndex.h"


namespace otb {

template<class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
StreamingResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::StreamingResampleImageFilter()
{
  
  // Default neighborhood interpolation radius is one pixel
  m_InterpolatorNeighborhoodRadius = 1 ;
//	m_RadiusIsDeterminedByUser = false;
}


template<class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void
StreamingResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::GenerateInputRequestedRegion()
{
  
	Superclass::GenerateInputRequestedRegion();
  
	if ( this->GetInput() )
  {
		otbDebugMacro(<< "-------------- GenerateInputRequestedRegion	---------------" << std::endl);
		
		InputImagePointer inputImage = const_cast< typename Superclass::InputImageType *>( this->GetInput() );
    OutputImagePointer outputImage = const_cast< typename Superclass::OutputImageType *>( this->GetOutput() );

    IndexType index = outputImage->GetRequestedRegion().GetIndex();
    SizeType size = outputImage->GetRequestedRegion().GetSize();
		
		// Obtain coordinates of upperleft, upperright, lowerleft and lowerright points in the image
		IndexType indexTmp ;
		std::vector<IndexType> vPoints;
		typename std::vector<IndexType>::iterator it;
		  		
		otbDebugMacro(<< "Size : " << size[0] << " " << size[1]);
		
		indexTmp[0]=index[0];
		indexTmp[1]=index[1]; 
		vPoints.push_back(indexTmp); 
		otbDebugMacro(<< "indexUL : (" << indexTmp[0] << "," << indexTmp[1] << ")");

		indexTmp[0]=index[0]+size[0];
		indexTmp[1]=index[1];  
		vPoints.push_back(indexTmp); 
		otbDebugMacro(<< "indexUR : (" << indexTmp[0] << "," << indexTmp[1] << ")");
		
		indexTmp[0]=index[0]+size[0];
		indexTmp[1]=index[1]+size[1]; 
		vPoints.push_back(indexTmp); 
		otbDebugMacro(<< "indexLR : (" << indexTmp[0] << "," << indexTmp[1] << ")");
		
		indexTmp[0]=index[0];
		indexTmp[1]=index[1]+size[1];  
		vPoints.push_back(indexTmp); 
		otbDebugMacro(<< "indexLL : (" << indexTmp[0] << "," << indexTmp[1] << ")");

		typedef itk::ContinuousIndex<TInterpolatorPrecisionType, 2> ContinuousIndexType;
		typename ContinuousIndexType::ValueType minX = itk::NumericTraits<typename ContinuousIndexType::ValueType>::max();
		typename ContinuousIndexType::ValueType maxX = 0;
		typename ContinuousIndexType::ValueType minY = itk::NumericTraits<typename ContinuousIndexType::ValueType>::max();
		typename ContinuousIndexType::ValueType maxY = 0;	

		// Coordinates of current output pixel
		PointType outputPoint;         
	  PointType inputPoint;
		
		// Transform each "corner" point
		for (it = vPoints.begin(); it != vPoints.end(); it++)
		{
			ContinuousIndexType indexTmpTr;
			
			// Calculate transformed points needed for previous filter in the pipeline
			outputImage->TransformIndexToPhysicalPoint( *it, outputPoint );

    	// Compute corresponding input pixel continuous index
    	inputPoint = this->GetTransform()->TransformPoint(outputPoint);
    	inputImage->TransformPhysicalPointToContinuousIndex(inputPoint, indexTmpTr);
			
			if (indexTmpTr[0]>maxX)
				maxX = indexTmpTr[0];
			else if (indexTmpTr[0]<minX)
				minX = indexTmpTr[0];
			
			if (indexTmpTr[1]>maxY)
				maxY = indexTmpTr[1];
			else if (indexTmpTr[1]<minY)
				minY = indexTmpTr[1];
	
			otbDebugMacro(<< "indexTr : (" << indexTmpTr[0] << "," << indexTmpTr[1] << ")");
		}
		
		otbDebugMacro(<< "MinX : " << minX << " MinY : " << minY << " MaxX : " << maxX << " MaxY " << maxY);
		
		// Create region needed in previous filter in the pipeline, which is the bounding box of previous transformed points
    InputImageRegionType region;
		index[0] = static_cast<long int>(minX);
		index[1] = static_cast<long int>(minY);
		size[0] = static_cast<long unsigned int>(maxX - minX);
		size[1] = static_cast<long unsigned int>(maxY - minY);
		
		otbDebugMacro(<< "Index : (" << index[0] << "," << index[1] << ") Size : (" << size[0] << "," << size[1] << ")");
					
    region.SetSize(size);
    region.SetIndex(index);
		
		// Grow region to be sure that interpolator can found needed point on image borders
		unsigned int neededRadius =	otb::StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(this->GetInterpolator());
		
		if (neededRadius == 0)
		{
			itkGenericOutputMacro(<< "If you haven't fixed interpolator radius,	default is 1");
			neededRadius = m_InterpolatorNeighborhoodRadius;
		}
		
		otbDebugMacro(<< "Interpolation needed radius : " << neededRadius);
		region.PadByRadius(neededRadius);
		
		otbDebugMacro(<< "Initial Region : Index(" << inputImage->GetLargestPossibleRegion().GetIndex()[0] << "," << inputImage->GetLargestPossibleRegion().GetIndex()[1] << ") Size(" <<	inputImage->GetLargestPossibleRegion().GetSize()[0] << "," << inputImage->GetLargestPossibleRegion().GetSize()[1] << ")");
		
		// To be sure that requested region in pipeline is not largest than real input image
		otbDebugMacro(<< "Final Region (Before Crop) : Index(" << region.GetIndex()[0] << "," << region.GetIndex()[1] << ") Size(" <<	region.GetSize()[0] << "," << region.GetSize()[1] << ")");

		// If requested region is not contained in input image, then result region is null
		if (!region.Crop(inputImage->GetLargestPossibleRegion()))
		{
			index[0]=0;
			index[1]=0;
			size[0]=0;
			size[1]=0;
			region.SetIndex(index);
			region.SetSize(size);
		}
		
		inputImage->SetRequestedRegion(region);
		
		otbDebugMacro(<< "Final Region (After  Crop) : Index(" << region.GetIndex()[0] << "," << region.GetIndex()[1] << ") Size(" <<	region.GetSize()[0] << "," << region.GetSize()[1] << ")");
  }
	
}

/*template<class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
StreamingResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::SetInterpolatorNeighborhoodRadius(unsigned int radius)
{
	m_InterpolatorNeighborhoodRadius = radius;
//	m_RadiusIsDeterminedByUser = true;	
}*/


template<class TInputImage, class TOutputImage, class TInterpolatorPrecisionType>
void 
StreamingResampleImageFilter<TInputImage,TOutputImage,TInterpolatorPrecisionType>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "StreamingResampleImageFilter " << std::endl;
}


}// end namespace otb
#endif
