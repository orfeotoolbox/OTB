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
#ifndef _otbPointSetBasedResamplingFilter_txx
#define _otbPointSetBasedResamplingFilter_txx

#include "otbPointSetBasedResamplingFilter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TPointSet, class TOutputImage>
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::PointSetBasedResamplingFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_MetricThreshold = 0.0;
  m_OutputSize.Fill(100);
  m_OutputSpacing.Fill(1.0);
  m_OutputOrigin.Fill(0.0);
  m_DefaultValue = 0;
  m_Interpolator = 0; // has to be provided by the user
  m_Transform = 0;    // has to be provided by the user
}
/**
 * Set the pointset containing the disparity.
 * \param pointset The pointset containing the disparity.
 */
template <class TInputImage, class TPointSet, class TOutputImage>
void
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::SetPointSet(const TPointSet * pointset)
{
  this->itk::ProcessObject::SetNthInput(1,const_cast<PointSetType *>(pointset));
}
/**
 * Get the pointset containing the disparity.
 * \return The pointset containing the disparity.
 */
template <class TInputImage, class TPointSet, class TOutputImage>
const TPointSet *
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::GetPointSet(void)
{
  return static_cast<const PointSetType *>(this->itk::ProcessObject::GetInput(1));
}
/** Generate output information */
template <class TInputImage, class TPointSet, class TOutputImage>
void
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::GenerateOutputInformation(void)
{
  OutputImagePointerType outputPtr = this->GetOutput();
  typename OutputImageType::RegionType largest;
  largest.SetSize(m_OutputSize);
  IndexType index;
  index.Fill(0);
  largest.SetIndex(index);
  outputPtr->SetLargestPossibleRegion(largest);
  outputPtr->SetSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);
}
/** Main computation method */
template <class TInputImage, class TPointSet, class TOutputImage>
void
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::GenerateData(void)
{
  this->AllocateOutputs();
  OutputImagePointerType outputPtr = this->GetOutput();
  InputImageConstPointerType inputPtr = this->GetInput();
  outputPtr->FillBuffer(0);
  m_Interpolator->SetInputImage(inputPtr);
  
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType> IteratorType;
  IteratorType outputIt(outputPtr,outputPtr->GetRequestedRegion());
  
  for(outputIt.GoToBegin();!outputIt.IsAtEnd();++outputIt)
    {
      PointType outputPoint,inputPoint;
      IndexType index;
      typename TransformType::Pointer inverseTransform;

      index = outputIt.GetIndex();
      m_Transform->SetParameters(this->GetNearestPointTransformParameters(index));
      m_Transform->GetInverse(inverseTransform);
      outputPtr->TransformIndexToPhysicalPoint(index,outputPoint);
      inputPoint = inverseTransform->TransformPoint(outputPoint);
      otbMsgDevMacro(<<"back point: "<<inputPoint<<"(inverse trasform param: "<<inverseTransform->GetParameters()<<")");

      if(m_Interpolator->IsInsideBuffer(inputPoint))
	{
	  outputIt.Set(m_Interpolator->Evaluate(inputPoint));
	}
      else
	{
	  outputIt.Set(m_DefaultValue);
	}
    }
}
/** 
 * \return The parameters of the transform associated with the nearest suitable point in pointset.
 */
template <class TInputImage, class TPointSet, class TOutputImage>
typename PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::ParametersType
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::GetNearestPointTransformParameters(IndexType &index)
{
  typedef typename PointSetType::PointsContainer::ConstIterator PointSetIteratorType;
  typedef typename PointSetType::PointsContainer PointsContainerType;
  typedef typename PointSetType::PointDataContainer::ConstIterator PointDataIteratorType;
  typedef typename PointSetType::PointDataContainer PointDataContainerType;

  PointSetIteratorType it = this->GetPointSet()->GetPoints()->Begin();
  PointDataIteratorType itData = this->GetPointSet()->GetPointData()->Begin();
  OutputImagePointerType outputPtr = this->GetOutput();

  double distance = -1.;
  ParametersType parameters(m_Transform->GetNumberOfParameters());
  PointType indexPoint;
  outputPtr->TransformIndexToPhysicalPoint(index,indexPoint);

  for(;it!=this->GetPointSet()->GetPoints()->End()&&itData!=this->GetPointSet()->GetPointData()->End();++it,++itData)
    {
      // If the point has a sufficient score
      if(vcl_abs(itData.Value()[0])>m_MetricThreshold)
	{
	  ParametersType tmpParameters(m_Transform->GetNumberOfParameters());
	  PointType inputPoint, outputPoint;

	  for(unsigned int i = 0; i<m_Transform->GetNumberOfParameters();++i)
	    {
	      tmpParameters[i]=itData.Value()[i+1];
	    }
	  inputPoint[0] = it.Value()[0];
	  inputPoint[1] = it.Value()[1];
	  
	  m_Transform->SetParameters(tmpParameters);
	  outputPoint = m_Transform->TransformPoint(inputPoint);
	  // compute the distance to current point
	  double d = vcl_pow(outputPoint[0]-indexPoint[0],2) + vcl_pow(outputPoint[1]-indexPoint[1],2);

	  if(distance<0||distance>d)
	    {
	      distance = d;
	      parameters = tmpParameters;
	    }
	}
    }
  return parameters;
}
    
/**
 * PrintSelf Method
 */
template <class TInputImage, class TPointSet, class TOutputImage>
void
PointSetBasedResamplingFilter<TInputImage, TPointSet, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
