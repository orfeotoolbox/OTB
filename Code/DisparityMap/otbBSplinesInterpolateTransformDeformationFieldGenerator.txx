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
#ifndef _otbBSplinesInterpolateTransformDeformationFieldGenerator_txx
#define _otbBSplinesInterpolateTransformDeformationFieldGenerator_txx

#include "otbBSplinesInterpolateTransformDeformationFieldGenerator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkDeformationFieldSource.h"
#include "otbObjectList.h"
#include "otbImage.h"
#include "otbMacro.h"
#include "otbMath.h"
#include "otbImageFileWriter.h"


namespace otb
{
template <class TPointSet,class TDeformationField>
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::BSplinesInterpolateTransformDeformationFieldGenerator()
{
  m_SplineOrder = 3;
  m_NumberOfControlPoints = 4;
  m_NumberOfLevels = 10;
}

template <class TPointSet,class TDeformationField>
void
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetNumberOfComponentsPerPixel(2);
}


/** Main computation method */
template <class TPointSet,class TDeformationField>
void
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::GenerateData(void)
{
  DeformationFieldPointerType outputPtr = this->GetOutput();

  // Default deformation field
  PixelType defaultValue(2);
  defaultValue.Fill(this->GetDefaultValue());
  outputPtr->Allocate();
  outputPtr->FillBuffer(defaultValue);

  typedef otb::ObjectList<SPlineInterpolateFilterType> SPlineInterpolatorListType;
  typedef typename PointSetType::PointsContainer::ConstIterator PointSetIteratorType;
  typedef typename PointSetType::PointsContainer PointsContainerType;

  // Initialisation of the splines interpolator
  typename SPlineInterpolatorListType::Pointer splineIntList = SPlineInterpolatorListType::New();

  for(unsigned int paramIndex = 0;paramIndex<this->GetTransform()->GetNumberOfParameters();++paramIndex)
    {
      // Create a new interpolator
      splineIntList->PushBack(SPlineInterpolateFilterType::New());
      // Create a new point set
      typename InternalPointSetType::Pointer tmpPointSet = InternalPointSetType::New();
      tmpPointSet->Initialize();
      unsigned int pointDataCounter = 0;
      for(PointSetIteratorType it = this->GetPointSet()->GetPoints()->Begin();it!=this->GetPointSet()->GetPoints()->End();++it)
	{
	  if(vcl_abs(this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[0])>=this->GetMetricThreshold())
	    {
	      typename InternalPointSetType::PixelType V(0.0);
	      
	       if(!IsAngular(paramIndex))
		 {
		   V[0] = this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex+3];
		   V[1] = this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex+3];
		 }
	       else
		 {
		   V[0] = static_cast<ValueType>(vcl_cos(this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex+3]));
		   V[1] = static_cast<ValueType>(vcl_sin( this->GetPointSet()->GetPointData()->GetElement(pointDataCounter)[paramIndex+3]));
		 }
	       unsigned long nbPoints = tmpPointSet->GetNumberOfPoints();
	       tmpPointSet->SetPoint( nbPoints, it.Value());  
	       tmpPointSet->SetPointData( nbPoints, V );
	    }
	  ++pointDataCounter;
	} 

      // Set the interpolator parameters
      splineIntList->Back()->SetInput(tmpPointSet);
      splineIntList->Back()->SetSplineOrder(m_SplineOrder);  
      typename SPlineInterpolateFilterType::ArrayType ncps;  
      ncps.Fill(m_NumberOfControlPoints);  
      splineIntList->Back()->SetNumberOfControlPoints( ncps );
      splineIntList->Back()->SetNumberOfLevels(m_NumberOfLevels);
      // splineIntList->Back()->SetGenerateOutputImage(false);
  
      // Define the parametric domain. 
      splineIntList->Back()->SetOrigin(this->GetOutput()->GetOrigin());
      splineIntList->Back()->SetSpacing(this->GetOutput()->GetSpacing());
      splineIntList->Back()->SetSize(this->GetOutput()->GetLargestPossibleRegion().GetSize());
      splineIntList->Back()->Update();
    }

  // Interpolation
  typedef itk::ImageRegionIteratorWithIndex<DeformationFieldType> IteratorType;
  IteratorType outIt(outputPtr,outputPtr->GetRequestedRegion());
  

  // main loop
  for(outIt.GoToBegin();!outIt.IsAtEnd();++outIt)
    {
      // Index of the pixel
      IndexType index = outIt.GetIndex();
      // parameters vector
      ParametersType params(this->GetTransform()->GetNumberOfParameters());
      // we iterate over the number of parameters of the transform
      for(unsigned int paramIndex = 0;paramIndex<this->GetTransform()->GetNumberOfParameters();++paramIndex)
	{
	  typename InternalPointSetType::PointType p;
	  this->GetOutput()->TransformIndexToPhysicalPoint(index,p);
	  typename InternalPointSetType::PixelType V(0.0);
	  // For each parameter we use the associated spline interpolator
	  if(!IsAngular(paramIndex))
	    {
	      splineIntList->GetNthElement(paramIndex)->EvaluateAtPoint(p,V);
	      params[paramIndex]=V[0];
	    }
	  else
	    {
	      splineIntList->GetNthElement(paramIndex)->EvaluateAtPoint(p,V);
	      params[paramIndex]=vcl_atan2(V[1],V[0]);
	    }
	  // We then compute the target point using the transform
	  this->GetTransform()->SetParameters(params);
	  PointType sourcePoint,targetPoint;
	  outputPtr->TransformIndexToPhysicalPoint(index,sourcePoint);
	  targetPoint = this->GetTransform()->TransformPoint(sourcePoint);
	  // And the deformation
	  PixelType pixel;
	  pixel.SetSize(2);
	  pixel[0] = static_cast<ValueType>(targetPoint[0]-sourcePoint[0]);
 	  pixel[1] = static_cast<ValueType>(targetPoint[1]-sourcePoint[1]);
	  outIt.Set(pixel);
	}
    }
}
template <class TPointSet,class TDeformationField>
void
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::AddAngularParameter(unsigned int index)
{
  m_AngularParameters.push_back(index);
}
template <class TPointSet,class TDeformationField>
void
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::RemoveAngularParameter(unsigned int index)
{
  m_AngularParameters.erase(index);
}
template <class TPointSet,class TDeformationField>
bool
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::IsAngular(unsigned int index)
{
  bool resp = std::binary_search(m_AngularParameters.begin(),m_AngularParameters.end(),index);
  return resp;
}
/**
 * PrintSelf Method
 */
template <class TPointSet,class TDeformationField>
void
BSplinesInterpolateTransformDeformationFieldGenerator<TPointSet, TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
