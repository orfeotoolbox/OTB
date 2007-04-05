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
#ifndef _otbPointSetToDeformationFieldGenerator_txx
#define _otbPointSetToDeformationFieldGenerator_txx

#include "otbPointSetToDeformationFieldGenerator.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPointSet, class TDeformationField>
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::PointSetToDeformationFieldGenerator()
{
  this->SetNumberOfRequiredInputs(1);
  m_MetricThreshold = 0.;
  m_OutputSize.Fill(100);
  m_OutputSpacing.Fill(1.);
  m_OutputOrigin.Fill(0.);
  m_DefaultValue = 0;
}
/**
 * Set the pointset containing the disparity.
 * \param pointset The pointset containing the disparity.
 */
template <class TPointSet, class TDeformationField>
void
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::SetPointSet(const TPointSet * pointset)
{
  this->itk::ProcessObject::SetNthInput(1,const_cast<PointSetType *>(pointset));
}
/**
 * Get the pointset containing the disparity.
 * \return The pointset containing the disparity.
 */
template <class TPointSet, class TDeformationField>
const TPointSet *
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::GetPointSet(void)
{
  return static_cast<const PointSetType *>(this->itk::ProcessObject::GetInput(1));
}
/** Generate output information */
template <class TPointSet, class TDeformationField>
void
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::GenerateOutputInformation(void)
{
  DeformationFieldPointerType outputPtr = this->GetOutput();
  typename DeformationFieldType::RegionType largest;
  largest.SetSize(m_OutputSize);
  IndexType index;
  index.Fill(0);
  largest.SetIndex(index);
  outputPtr->SetLargestPossibleRegion(largest);
  outputPtr->SetSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);
  // Force the deformation field to have vector pixel of size 2.
  outputPtr->SetNumberOfComponentsPerPixel(2);
}
/**
 * Get the n nearest point in point set
 *  \param index The index of the pixel to compute.
 *  \param n The number of nearest point to seek.
 *  \return The pointset containing the n nearest point to this pixel, ordered from nearest to most far.
 */
template <class TPointSet, class TDeformationField>
TPointSet * 
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::GetNNearestPointInPointSet(IndexType index, unsigned int n)
{
  typedef Functor::DistanceComparisonFunctor ComparisonFunctorType;
  DistanceVectorType distanceVector;
  IndexVectorType indexVector;
  IndexVectorType sortVector;
  unsigned int i =  0;
  unsigned int j = 0;
  
  typedef typename PointSetType::PointsContainer::ConstIterator PointSetIteratorType;
  typedef typename PointSetType::PointsContainer PointsContainerType;
  PointSetIteratorType it = this->GetPointSet()->GetPoints()->Begin();
  for(;it!=this->GetPointSet()->GetPoints()->End();++it)
    {
      PointType p;
      p[0]=it.Value()[0];
      p[1]=it.Value()[1];
      if(vcl_abs(this->GetPointSet()->GetPointData()->GetElement(i)[1])>=m_MetricThreshold)
	{
	  distanceVector.push_back(EuclideanDistance(index,p));
	  sortVector.push_back(i);
	  ++i;
	  indexVector.push_back(j);	   
	}
      ++j;
    }
  
  ComparisonFunctorType comp;
  comp.SetDistanceVector(distanceVector);
  sort(indexVector.begin(),indexVector.end(),comp);

  PointSetPointerType output = PointSetType::New();
  unsigned int nbElements = (n<indexVector.size() ? n : indexVector.size());
  for(i=0;i<nbElements;i++)
    {
      output->SetPoint(i,this->GetPointSet()->GetPoints()->GetElement(indexVector[sortVector[i]]));
      output->SetPointData(i,this->GetPointSet()->GetPointData()->GetElement(sortVector[indexVector[i]]));
    }
  return output;
}

template <class TPointSet, class TDeformationField>
double 
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::EuclideanDistance(IndexType index, PointType p)
{
  PointType pprime;
  this->GetOutput()->TransformIndexToPhysicalPoint(index,pprime);
  return vcl_sqrt(vcl_pow(pprime[0]-p[0],2)+vcl_pow(pprime[1]-p[1],2));
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDeformationField>
void
PointSetToDeformationFieldGenerator<TPointSet, TDeformationField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
