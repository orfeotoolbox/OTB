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
#ifndef __otbFineCorrelationImageFilter_txx
#define __otbFineCorrelationImageFilter_txx

#include "otbFineCorrelationImageFilter.h"

#include "itkMatrix.h"
#include "itkVector.h"
#include "itkProgressReporter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkImageRegionIteratorWithIndex.h"
#include <algorithm>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
FineCorrelationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::FineCorrelationImageFilter()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfOutputs(2);
  this->SetNthOutput(1,TOutputDeformationField::New());
  m_Radius.Fill(2);
  m_SearchRadius.Fill(4);
  m_RefinementMode = SUBPIXEL;
  m_SubPixelPrecision = 100;

  // Default interpolator
  m_Interpolator = itk::LinearInterpolateImageFunction<TInputImage,double>::New();
}

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
void
FineCorrelationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::SetFixedInput( const TInputImage * image )
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
void
FineCorrelationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::SetMovingInput( const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
const TInputImage *
FineCorrelationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::GetFixedInput()
{
  if (this->GetNumberOfInputs()<1)
  {
    return 0;
  }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
const TInputImage *
FineCorrelationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::GetMovingInput()
{
  if (this->GetNumberOfInputs()<2)
  {
    return 0;
  }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class T0utputCorrelation, class TOutputDeformationField>
TOutputDeformationField *
FineCorrelationImageFilter<TInputImage,T0utputCorrelation,TOutputDeformationField>
::GetOutputDeformationField()
{
  if (this->GetNumberOfOutputs()<2)
  {
    return 0;
  }
  return static_cast<TOutputDeformationField *>(this->itk::ProcessObject::GetOutput(1));
}


template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
void
FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  TInputImage * fixedPtr =  const_cast< TInputImage * >( this->GetFixedInput());
  TInputImage * movingPtr = const_cast< TInputImage * >( this->GetMovingInput());

  TOutputCorrelation * outputPtr = this->GetOutput();

  if ( !fixedPtr || !movingPtr || !outputPtr )
  {
    return;
  }

  // get a copy of the fixed requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType fixedRequestedRegion;
  fixedRequestedRegion = fixedPtr->GetRequestedRegion();
  // pad the input requested region by the operator radius
  fixedRequestedRegion.PadByRadius( m_Radius );

  // get a copy of the moving requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType movingRequestedRegion;
  movingRequestedRegion = movingPtr->GetRequestedRegion();
  // pad the input requested region by the operator radius
  movingRequestedRegion.PadByRadius( m_SearchRadius + m_Radius );

  // crop the fixed region at the fixed's largest possible region
  if ( fixedRequestedRegion.Crop(fixedPtr->GetLargestPossibleRegion()))
  {
    fixedPtr->SetRequestedRegion( fixedRequestedRegion );
  }
  else
  {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    fixedPtr->SetRequestedRegion( fixedRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
    << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(fixedPtr);
    throw e;
  }

  // crop the moving region at the moving's largest possible region
  if ( movingRequestedRegion.Crop(movingPtr->GetLargestPossibleRegion()))
  {
    movingPtr->SetRequestedRegion( movingRequestedRegion );
  }
  else
  {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    movingPtr->SetRequestedRegion( movingRequestedRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    itk::OStringStream msg;
    msg << this->GetNameOfClass()
    << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of image 1.");
    e.SetDataObject(movingPtr);
    throw e;
  }

  return;
}

template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
inline double
FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::Correlation(const NeighborhoodType & fixed, const NeighborhoodType & moving, const OffsetType & offset, unsigned int scale) const
{
  double crossProductSum = 0;
  double fSquareSum = 0;
  double mSquareSum = 0;

  OffsetType currentOffset, currentMovingOffset;

  for(int i=-(int)m_Radius[0];i<=(int)m_Radius[0];++i)
      {
      for(int j=-(int)m_Radius[1]; j<=(int)m_Radius[1];++j)
	{
	currentOffset[0]=i;
	currentOffset[1]=j;
	currentMovingOffset[0] = i * (int)scale;
	currentMovingOffset[1] = j * (int)scale;

	double vfixed = fixed[currentOffset];
	double vmoving = moving[currentMovingOffset+offset];

	crossProductSum+=vfixed*vmoving;
	fSquareSum += vfixed*vfixed;
	mSquareSum += vmoving*vmoving;
	}
      }

  double res = 0;
  if( fSquareSum == 0. || mSquareSum == 0. )
    {
      res = itk::NumericTraits<double>::max();
    }
  else
    res = crossProductSum/vcl_sqrt(fSquareSum*mSquareSum);

  return res;
}

template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
const typename FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::NeighborhoodType
FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::ComputeSubPixelNeighborhood(const IndexType & location, unsigned int scale) const
{
  // First, create the neighborhood to return
  NeighborhoodType subPixelNeighborhood;

  // Create the appropriate radius
  RadiusType subPixelRadius;
  subPixelRadius[0] = (m_Radius[0]+1) * scale;
  subPixelRadius[1] = (m_Radius[1]+1) * scale;
  
  // Initialize the neighborhood
  subPixelNeighborhood.SetRadius(subPixelRadius);

  // A continuous index;
  ContinuousIndexType interpolatedPos;
  OffsetType          neighborhoodPos;

  // Inverted scale (computed once)
  double invertedScale = 1./(double)scale;

  // Fill the neighborhood
  for(int i = -(int)subPixelRadius[0]; i<=(int)subPixelRadius[0]; ++i)
    {
    for(int j = -(int)subPixelRadius[0]; j<=(int)subPixelRadius[0];++j)
      {
      // Update the interpolated position
      interpolatedPos[0] = location[0] + i * invertedScale;
      interpolatedPos[1] = location[1] + j * invertedScale;

      // Update the neighborhood position
      neighborhoodPos[0] = i;
      neighborhoodPos[1] = j;

      subPixelNeighborhood[neighborhoodPos] = m_Interpolator->EvaluateAtContinuousIndex(interpolatedPos);
      }
    }
  // Finally, return the neighborhood
  return subPixelNeighborhood;
}


template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
inline double
FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::RefineLocation(const NeighborhoodType& correlMap, const OffsetType & maxPos, DeformationValueType & value) const
{
  // 1ST: Find the 6 highest correlation values
  PairVectorType pairVector;

  // Offset
  OffsetType offset;

  // For each location
  for(int i =(int)maxPos[0]-1; i<=(int)maxPos[0]+1;++i)
    {
    for(int j=(int)maxPos[1]-1; j<=(int)maxPos[1]+1;++j)
      {
      // Avoid being out of bound
      if( (i>=-(int)m_SearchRadius[0] && i<=(int)m_SearchRadius[0])
	  && (j>=-(int)m_SearchRadius[1] && i<=(int)m_SearchRadius[1]))
	{
	
	// Update offset
	offset[0]=i;
	offset[1]=j;
	
	PairType p(correlMap[offset],offset);
	pairVector.push_back(p);
	}
      }
    }

    // Check if we have enough values for quadric fitting
    if(pairVector.size()<6)
      {
      value[0]=maxPos[0];
      value[1]=maxPos[1];
      return correlMap[maxPos];
      }


    // Sort the map by highest correlation values
    std::sort(pairVector.begin(),pairVector.end(),&CompPairs);


    // 2ND: Fill the linear system: f(x,y) = Ax²+ By² + Cx + Dy + Exy + F
    
    itk::Matrix<double,6,6> mat,invert;
    itk::Vector<double,6>   correlValues,coefs;

    for(unsigned int i = 0;i<6;++i)
      {
      // Fill the matrix
      offset = pairVector[i].second;
      mat(i,0) = (double)(offset[0]*offset[0]);
      mat(i,1) = (double)(offset[1]*offset[1]);
      mat(i,2) = (double)(offset[0]);
      mat(i,3) = (double)(offset[1]);
      mat(i,4) = (double)(offset[0]*offset[1]);
      mat(i,5) = 1.;

      // Fill the correl values vector
      correlValues[i] = pairVector[i].first;      
      }

    // 3RD: Solve the linear system

    // We try to invert the matrix
    try
      {
      invert = mat.GetInverse();
      }
    catch( itk::ExceptionObject & )
      {
      // Matrix is not invertible
      value[0]=maxPos[0];
      value[1]=maxPos[1];

      return correlMap[maxPos];
      }

    // If are here, the inversion did succeed
    coefs = invert * correlValues;
    // 4TH: Look for a maximum

    // Check if there is a maximum (sign of 4 AB - E²)
    double det = 4*coefs[0]*coefs[1] - coefs[4]*coefs[4];

    if(det<=1e-6)//0)
      {
      // Quadric has no maximum values
      value[0]=maxPos[0];
      value[1]=maxPos[1];

      return correlMap[maxPos];
      }
    
    // Xmax = - (2BC - DE)/det
    value[0] = - (2*coefs[1]*coefs[2] - coefs[3]*coefs[4])/det;

    // Ymax = - (2AD - CE)/det
    value[1] = - (2*coefs[0]*coefs[3] - coefs[2]*coefs[4])/det;

    // Apply f to get the maximum correlation value

    double maxCorrel = coefs[0] * value[0]*value[0] + coefs[1] * value[1]*value[1]
      + coefs[2] * value[0] + coefs[3] * value[1] + coefs[4] * value[0] * value[1] + coefs[5];

    // Check for a consistent maxima. If not, do not use the values
    // TODO: Check this
    //if(maxCorrel > 1 || maxCorrel < 0 || vnl_math_isnan(maxCorrel))
    if( ((maxCorrel-1.) > 1e-6) || maxCorrel < 1e-6 || vnl_math_isnan(maxCorrel))
      {
      value[0]=maxPos[0];
      value[1]=maxPos[1];

      return correlMap[maxPos];
      }

    return maxCorrel;
}

template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
void
FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::BeforeThreadedGenerateData()
{
  m_Interpolator->SetInputImage(this->GetMovingInput());
}

template <class TInputImage, class TOutputCorrelation, class TOutputDeformationField>
void
FineCorrelationImageFilter<TInputImage,TOutputCorrelation,TOutputDeformationField>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
  // Get the image pointers
  const TInputImage * fixedPtr = this->GetFixedInput();
  const TInputImage * movingPtr = this->GetMovingInput();
  TOutputCorrelation * outputPtr = this->GetOutput();
  TOutputDeformationField * outputDfPtr = this->GetOutputDeformationField();

  // Iterators
  NeighborhoodIteratorType fixedIt(m_Radius,fixedPtr,outputRegionForThread);
  NeighborhoodIteratorType movingIt(m_SearchRadius + m_Radius,movingPtr,outputRegionForThread);
  itk::ImageRegionIteratorWithIndex<TOutputCorrelation> outputIt(outputPtr,outputRegionForThread);
  itk::ImageRegionIterator<TOutputDeformationField> outputDfIt(outputDfPtr,outputRegionForThread);
  
  // Boundary conditions
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> fixedNbc;
  itk::ZeroFluxNeumannBoundaryCondition<TInputImage> movingNbc;
  fixedIt.OverrideBoundaryCondition(&fixedNbc);
  movingIt.OverrideBoundaryCondition(&movingNbc);
 
  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // GoToBegin
  fixedIt.GoToBegin();
  movingIt.GoToBegin();
  outputIt.GoToBegin();
  outputDfIt.GoToBegin();

  // Corellation neighborhood
  NeighborhoodType correlationMap;
  correlationMap.SetRadius(m_SearchRadius);

  // Offset
  OffsetType offset;
  
  // Correl, max correl, maxPosition
  double correl, maxCorrel;
  OffsetType maxOffset;

// Walk the images
    while (!fixedIt.IsAtEnd() && !movingIt.IsAtEnd() && !outputIt.IsAtEnd() && !outputDfIt.IsAtEnd() )
    {
    // Initialize
    correl = 0;
    maxCorrel = 0;
    maxOffset.Fill(0);
    DeformationValueType deformationValue;

    // Get the neighborhood
    NeighborhoodType fixedN = fixedIt.GetNeighborhood();
    NeighborhoodType movingN = movingIt.GetNeighborhood();

    // Compute the correlation at each location
    for(int i =-(int)m_SearchRadius[0]; i<=(int)m_SearchRadius[0];++i)
      {
      for(int j=-(int)m_SearchRadius[1]; j<=(int)m_SearchRadius[1];++j)
	{
	// Update offset
	offset[0]=i;
	offset[1]=j;

	// compute correlation
	double correl = this->Correlation(fixedN,movingN,offset,1);

        // Store in the map (for further refinenement)
	correlationMap[offset]=correl;

	// Check for maximum
	if(correl > maxCorrel)
	  {
	  maxCorrel = correl;
	  maxOffset = offset;
	  }
	}
      }
  
    // Perform LSQR QUADFIT refinement
    if(m_RefinementMode == LSQR_QUADFIT)
      {
      maxCorrel = this->RefineLocation(correlationMap,maxOffset,deformationValue);
      }
    // Perform SUBPIXEL refinement
    else if(m_RefinementMode == SUBPIXEL)
      {
      // Compute the interpolated fine grid
      NeighborhoodType subPixelN = this->ComputeSubPixelNeighborhood(fixedIt.GetIndex()+maxOffset,m_SubPixelPrecision);

      // The fine offset
      OffsetType fineOffset, maxFineOffset;
      maxFineOffset.Fill(0);

      // Compute the correlation at each fine location
      for(int i =-(int)m_SubPixelPrecision+1; i<(int)m_SubPixelPrecision;++i)
	{
	for(int j=-(int)m_SubPixelPrecision+1; j<(int)m_SubPixelPrecision;++j)
	  {
	  // Update the fine offset
	  fineOffset[0] = i;
	  fineOffset[1] = j;

	  // Compute the correlation
	  double correl = this->Correlation(fixedN,subPixelN,fineOffset,m_SubPixelPrecision);
	  
	  // If correlation is better
	  if(correl > maxCorrel)
	    {
	    // Update values
	    maxCorrel = correl;
	    maxFineOffset = fineOffset;
	    }
	  }
	}
 
      // Finally, update deformation values
      deformationValue[0]=maxOffset[0] + (double)maxFineOffset[0]/(double)m_SubPixelPrecision;
      deformationValue[1]=maxOffset[1] + (double)maxFineOffset[1]/(double)m_SubPixelPrecision;;

      }
    // Default and COARSE case
    else
      {
      deformationValue[0]=maxOffset[0];
      deformationValue[1]=maxOffset[1];
      }

    // Store the offset and the correlation value
    outputIt.Set(maxCorrel);
    outputDfIt.Set(deformationValue);
    // Update iterators
    ++fixedIt;
    ++movingIt;
    ++outputIt;
    ++outputDfIt;
   
    // Update progress
    progress.CompletedPixel();
    }
}

} // end namespace otb

#endif
