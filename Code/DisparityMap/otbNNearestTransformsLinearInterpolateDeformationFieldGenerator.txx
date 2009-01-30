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
#ifndef __otbNNearestTransformsLinearInterpolateDeformationFieldGenerator_txx
#define __otbNNearestTransformsLinearInterpolateDeformationFieldGenerator_txx

#define EPSILON 1e-15

#include "otbNNearestTransformsLinearInterpolateDeformationFieldGenerator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/** Main computation method */
template <class TPointSet,class TDeformationField>
void
NNearestTransformsLinearInterpolateDeformationFieldGenerator<TPointSet, TDeformationField>
::GenerateData(void)
{
  DeformationFieldPointerType outputPtr = this->GetOutput();
  PixelType defaultValue(2);
  defaultValue.Fill(this->GetDefaultValue());
  outputPtr->Allocate();
  outputPtr->FillBuffer(defaultValue);

  typedef itk::ImageRegionIteratorWithIndex<DeformationFieldType> IteratorType;
  IteratorType it(outputPtr,outputPtr->GetRequestedRegion());

  for (it.GoToBegin();!it.IsAtEnd();++it)
  {
    IndexType index = it.GetIndex();
    IndexVectorType indexVector = this->GenerateNearestValidPointsPointSet(it.GetIndex(),m_NumberOfPoints);
    PixelType pixel(2);
    double xdeformation, ydeformation,normalization;
    xdeformation = 0;
    ydeformation = 0;
    normalization = 0;

    for (typename IndexVectorType::iterator indexIt=indexVector.begin();indexIt!=indexVector.end();++indexIt)
    {
      PointType point;
      point[0] = static_cast<double>(this->GetPointSet()->GetPoints()->GetElement(*indexIt)[0]);
      point[1] = static_cast<double>(this->GetPointSet()->GetPoints()->GetElement(*indexIt)[1]);
      double distance = this->EuclideanDistance(index,point);
      if (distance<EPSILON)
      {
        distance = EPSILON;
      }

      ParametersType params(this->GetTransform()->GetNumberOfParameters());
      for (unsigned int  i = 0; i<this->GetTransform()->GetNumberOfParameters();++i)
      {
        params[i] = this->GetPointSet()->GetPointData()->GetElement((*indexIt))[i+3];
      }
      this->GetTransform()->SetParameters(params);
      PointType sourcePoint,targetPoint;

      outputPtr->TransformIndexToPhysicalPoint(it.GetIndex(),sourcePoint);
      targetPoint = this->GetTransform()->TransformPoint(sourcePoint);
      xdeformation += (targetPoint[0]-sourcePoint[0])/distance;
      ydeformation += (targetPoint[1]-sourcePoint[1]) /distance;
      normalization+=1/distance;
    }

    if (normalization>0)
    {
      pixel[0] = static_cast<ValueType>(xdeformation/normalization);
      pixel[1] = static_cast<ValueType>(ydeformation/normalization);
    }
    else
    {
      pixel=defaultValue;
    }
    it.Set(pixel);
  }
}
/**
 * PrintSelf Method
 */
template <class TPointSet,class TDeformationField>
void
NNearestTransformsLinearInterpolateDeformationFieldGenerator<TPointSet, TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
