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
#ifndef __otbLabeledSampleLocalizationGenerator_txx
#define __otbLabeledSampleLocalizationGenerator_txx

#include "otbLabeledSampleLocalizationGenerator.h"

#include "otbMath.h"

namespace otb
{
template<class TVectorData>
LabeledSampleLocalizationGenerator<TVectorData>
::LabeledSampleLocalizationGenerator() :
  m_ClassKey("Class"),
  m_NoClassIdentifier(0),
  m_RandomLocalizationDensity(.005),
  m_InhibitionRadius(5.0),
  m_NbMaxIteration(10000)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
 
  m_RandomGenerator = RandomGeneratorType::New();
}

template <class TVectorData>
void
LabeledSampleLocalizationGenerator<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Class Key: " << m_ClassKey << std::endl;
  os << indent << "Negative Class Identifier: " << m_NoClassIdentifier << std::endl;
  os << indent << "Sampling Density: " << m_RandomLocalizationDensity << std::endl;
  os << indent << "Inhibition Radius: " << m_InhibitionRadius << std::endl;
}


template <class TVectorData>
void
LabeledSampleLocalizationGenerator<TVectorData>
::PushBackInput(const VectorDataType * vectorData)
{
  this->Superclass::SetNthInput(this->GetNumberOfInputs(), const_cast<VectorDataType *>(vectorData));
}

template <class TVectorData>
const TVectorData *
LabeledSampleLocalizationGenerator<TVectorData>
::GetInput(unsigned int idx) const
{
  if (this->GetNumberOfInputs() < idx)
    {
    return 0;
    }

  return static_cast<const VectorDataType *>(this->Superclass::GetInput(idx));
}

template <class TVectorData>
typename LabeledSampleLocalizationGenerator<TVectorData>
::PointVectorType
LabeledSampleLocalizationGenerator<TVectorData>
::RandomPointsGenerator(DataNodeType * node)
{
  // Output
  PointVectorType vPoint;

  // Euclidean distance
  typename EuclideanDistanceType::Pointer euclideanDistance = EuclideanDistanceType::New();

  // Gathering Information
  RegionType generatorRegion = node->GetPolygonExteriorRing()->GetBoundingRegion();
  typename RegionType::SizeType generatorRegionSize = generatorRegion.GetSize();
  typename RegionType::IndexType generatorRegionIndex = generatorRegion.GetIndex();
  typename RegionType::IndexType generatorRegionOrigin = generatorRegion.GetOrigin();
  
  // Identify inside known points
  std::vector<PointType> insiders;
  typename VectorDataType::ConstPointer vectorData = static_cast<const VectorDataType *>(this->GetOutput(0));
  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPointFeature() && itVector.Get()->GetFieldAsInt(m_ClassKey)!=m_NoClassIdentifier)
      {
      VertexType vertex;
      vertex[0] = itVector.Get()->GetPoint()[0];
      vertex[1] = itVector.Get()->GetPoint()[1];

      if(node->GetPolygonExteriorRing()->IsInside(vertex))
        {
        insiders.push_back(itVector.Get()->GetPoint());
        }
      }
    ++itVector;
    }

  //std::cout << "insiders: " << insiders.size() << std::endl;

  // Search parametrization 
  //unsigned int nbMaxIter = (unsigned int)(node->GetPolygonExteriorRing()->GetArea());
  //                                         - insiders.size() * CONST_PI * vcl_pow(this->GetInhibitionRadius(), 2))
  //                                         / (CONST_PI * vcl_pow(this->GetInhibitionRadius(), 2)));
  
  unsigned int nbMaxPosition = (unsigned int)(node->GetPolygonExteriorRing()->GetArea() * this->GetRandomLocalizationDensity());
  
  //std::cout << "nbMaxIter: " << this->GetNbMaxIteration() << std::endl;
  //std::cout << "nbMaxPosition: " << nbMaxPosition << std::endl;
  
  // Generation
  unsigned long int nbIter =  this->GetNbMaxIteration();
  unsigned int nbPosition = nbMaxPosition;

  PointType rangeMin,rangeMax;
  
  for(unsigned int dim = 0; dim < 2; ++dim)
    {
    rangeMin[dim] = generatorRegionIndex[dim];
    rangeMax[dim] = generatorRegionIndex[dim] + generatorRegionSize[dim];
    }
  
  while(nbIter > 0 && nbPosition > 0)
    {
    VertexType candidate;
    for(unsigned int dim = 0; dim < 2; ++dim)
      {
      candidate[dim] = this->m_RandomGenerator->GetUniformVariate(rangeMin[dim],rangeMax[dim]);
      }
    if(node->GetPolygonExteriorRing()->IsInside(candidate))
      {
      typename PointVectorType::const_iterator pit = insiders.begin();
      bool valid = true;

      while(valid && pit!=insiders.end())
        {
        valid = (euclideanDistance->Evaluate(candidate,*pit) > this->GetInhibitionRadius());
        ++pit;
        }
      
      if(valid)
        {
        PointType point;
        point[0] = candidate[0];
        point[1] = candidate[1];
        
        vPoint.push_back(point);
//        insiders.push_back(point);
        
        nbPosition --;
        }
      }
    nbIter --;
    }
  
  
  return vPoint;
}

template <class TVectorData>
void
LabeledSampleLocalizationGenerator<TVectorData>
::Update()
{
  this->GenerateData();
}

template <class TVectorData>
void
LabeledSampleLocalizationGenerator<TVectorData>
::GenerateData()
{
  unsigned int nbInputs = this->GetNumberOfInputs();

  this->GetOutput(0)->SetMetaDataDictionary(this->GetInput(0)->GetMetaDataDictionary());
  
  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput(0)->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);

  // Copy all point feature in output VectorData
  for (unsigned int i=0; i<nbInputs; i++)
    {
    typename VectorDataType::ConstPointer vectorData = static_cast<const VectorDataType *>(this->GetInput(i));
    
    TreeIteratorType itVector(vectorData->GetDataTree());
    itVector.GoToBegin();
    while (!itVector.IsAtEnd())
      {
      if (itVector.Get()->IsPointFeature())
        {
        this->GetOutput(0)->GetDataTree()->Add(itVector.Get(), document);
        }
       ++itVector;
      }   
    }

  // Iterates through the polygon features and generates random point inside the polygon with
  // the "NoClass" identifier
  for (unsigned int i=0; i<nbInputs; i++)
    {
    typename VectorDataType::ConstPointer vectorData = static_cast<const VectorDataType *>(this->GetInput(i));
    
    TreeIteratorType itVector(vectorData->GetDataTree());
    itVector.GoToBegin();
    while (!itVector.IsAtEnd())
      {
      if (itVector.Get()->IsPolygonFeature())
        {
        PointVectorType vPoint = RandomPointsGenerator(itVector.Get());
        
        for (typename PointVectorType::const_iterator it = vPoint.begin(); it != vPoint.end(); ++it)
          {
          typename DataNodeType::Pointer CurrentGeometry = DataNodeType::New();
          CurrentGeometry->SetNodeId("FEATURE_POINT");
          CurrentGeometry->SetNodeType(otb::FEATURE_POINT);
          CurrentGeometry->SetPoint(*it);
          CurrentGeometry->SetFieldAsInt(this->GetClassKey(), this->GetNoClassIdentifier());
          this->GetOutput(0)->GetDataTree()->Add(CurrentGeometry, document);
          }
        }
      ++itVector;
      }   
    }
}

} // end namespace otb

#endif
