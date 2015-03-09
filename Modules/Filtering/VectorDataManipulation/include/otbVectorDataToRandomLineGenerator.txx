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
#ifndef __otbVectorDataToRandomLineGenerator_txx
#define __otbVectorDataToRandomLineGenerator_txx

#include "otbVectorDataToRandomLineGenerator.h"

#include "otbMath.h"

namespace otb
{
template<class TVectorData>
VectorDataToRandomLineGenerator<TVectorData>
::VectorDataToRandomLineGenerator() :
  m_NumberOfOutputLine(100),
  m_MinLineSize(2),
  m_MaxLineSize(10),
  m_CurrentID(0)
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_RandomGenerator = RandomGeneratorType::GetInstance();
  m_RandomSizeGenerator = RandomGeneratorType::GetInstance();
}

template <class TVectorData>
void
VectorDataToRandomLineGenerator<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Required Number of Output Line: " << m_NumberOfOutputLine << std::endl;
}

template <class TVectorData>
void
VectorDataToRandomLineGenerator<TVectorData>
::SetInput(const VectorDataType * vectorData)
{
  this->Superclass::SetNthInput(0, const_cast<VectorDataType *>(vectorData));
}

template <class TVectorData>
const TVectorData *
VectorDataToRandomLineGenerator<TVectorData>
::GetInput() const
{
  return static_cast<const VectorDataType *>(this->Superclass::GetInput(0));
}

template <class TVectorData>
typename VectorDataToRandomLineGenerator<TVectorData>
::PointVectorType
VectorDataToRandomLineGenerator<TVectorData>
::RandomPointsGenerator(DataNodeType * node)
{
  // Output
  PointVectorType vPoint;

  // Gathering Information
  RegionType generatorRegion = node->GetPolygonExteriorRing()->GetBoundingRegion();
  typename RegionType::SizeType generatorRegionSize = generatorRegion.GetSize();
  typename RegionType::IndexType generatorRegionIndex = generatorRegion.GetIndex();
  //typename RegionType::IndexType generatorRegionOrigin = generatorRegion.GetOrigin();

  // Generation
  PointType rangeMin, rangeMax;

  for(unsigned int dim = 0; dim < 2; ++dim)
    {
    rangeMin[dim] = generatorRegionIndex[dim];
    rangeMax[dim] = generatorRegionIndex[dim] + generatorRegionSize[dim];
    }

  unsigned int nbPoint = this->m_RandomSizeGenerator->GetUniformVariate(this->GetMinLineSize(), this->GetMaxLineSize());

  while(nbPoint > 0)
    {
    VertexType candidate;
    for(unsigned int dim = 0; dim < 2; ++dim)
      {
      candidate[dim] = this->m_RandomGenerator->GetUniformVariate(rangeMin[dim], rangeMax[dim]);
      }

    if(node->GetPolygonExteriorRing()->IsInside(candidate))
      {
      PointType point;
      point[0] = candidate[0];
      point[1] = candidate[1];
      vPoint.push_back(point);
      nbPoint --;
      }
    }
  return vPoint;
}

template <class TVectorData>
void
VectorDataToRandomLineGenerator<TVectorData>
::GenerateData()
{
  this->GetOutput()->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());

  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput()->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);

   // Iterates through the polygon features and generates random Lines inside the polygon
  typename VectorDataType::ConstPointer vectorData = static_cast<const VectorDataType *>(this->GetInput());

  TreeIteratorType itVector(vectorData->GetDataTree());
  itVector.GoToBegin();
  while (!itVector.IsAtEnd())
    {
    if (itVector.Get()->IsPolygonFeature())
      {

      for(unsigned int i=0; i<this->GetNumberOfOutputLine(); ++i)
        {
        typename DataNodeType::Pointer currentGeometry = DataNodeType::New();
        currentGeometry->SetNodeId(this->GetNextID());
        currentGeometry->SetNodeType(otb::FEATURE_LINE);
        typename LineType::Pointer line = LineType::New();
        currentGeometry->SetLine(line);
        PointVectorType vPoints = RandomPointsGenerator(itVector.Get());
        for (typename PointVectorType::const_iterator it = vPoints.begin(); it != vPoints.end(); ++it)
          {
          VertexType vertex;
          vertex[0] = (*it)[0];
          vertex[1] = (*it)[1];
          currentGeometry->GetLine()->AddVertex(vertex);
          }
        this->GetOutput(0)->GetDataTree()->Add(currentGeometry, document);
        }
      }
    ++itVector;
    }
}

} // end namespace otb

#endif


