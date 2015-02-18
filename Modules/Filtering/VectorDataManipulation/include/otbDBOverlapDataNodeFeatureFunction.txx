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
#ifndef __otbDBOverlapDataNodeFeatureFunction_txx
#define __otbDBOverlapDataNodeFeatureFunction_txx

#include "otbDBOverlapDataNodeFeatureFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TCoordRep, class TPrecision>
DBOverlapDataNodeFeatureFunction<TCoordRep, TPrecision>
::DBOverlapDataNodeFeatureFunction()
 :m_DistanceThreshold(50)//in physical coordinates
{
}

/**
 * Standard "PrintSelf" method
 */
template <class TCoordRep, class TPrecision>
void
DBOverlapDataNodeFeatureFunction<TCoordRep, TPrecision>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Distance Threshold: " << m_DistanceThreshold << std::endl;
}

/**
 * Method to compute the distance of a point to a segment
 */
template <class TCoordRep, class TPrecision>
double
DBOverlapDataNodeFeatureFunction<TCoordRep, TPrecision>
::ComputeEuclideanDistanceMetricToSegment(VertexType q1, VertexType q2, VertexType p) const
{
  // Length of the segment
  double l2 = q1.SquaredEuclideanDistanceTo(q2);

  // Is the projection of p on the segment inside (0<u<1) or
  // inside the segment bounds
  double u = ((p[0] - q1[0]) *(q2[0] - q1[0] ) +
    (p[1] - q1[1]) *(q2[1] - q1[1])) / l2;

  if( u < 1e-10 ) u = 0.;
  if( u -1. > 1e-10 ) u = 1.;
  double x = q1[0] + u *(q2[0] - q1[0] );
  double y = q1[1] + u *(q2[1] - q1[1] );
  double dx = x - p[0];
  double dy = y - p[1];

  return vcl_sqrt(dx*dx + dy*dy);
 }

template <class TCoordRep, class TPrecision>
typename DBOverlapDataNodeFeatureFunction<TCoordRep, TPrecision>
::OutputType
DBOverlapDataNodeFeatureFunction<TCoordRep, TPrecision>
::Evaluate( const DataNodeType& node ) const
{
  OutputType output;

  //build the tmp DataTree containing polygon within the radius
  typename VectorDataType::Pointer tmpDataTree = VectorDataType::New();
  typename DataNodeType::Pointer root = tmpDataTree->GetDataTree()->GetRoot()->Get();
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  tmpDataTree->GetDataTree()->Add(document, root);

  TreeIteratorType itVector(this->GetInputVectorData()->GetDataTree());
  itVector.GoToBegin();
    while (!itVector.IsAtEnd())
      {
      if (itVector.Get()->IsPolygonFeature())
        {
        typename DataNodeType::Pointer currentGeometry = itVector.Get();
        unsigned int i=0;
        while (i<currentGeometry->GetPolygonExteriorRing()->GetVertexList()->Size())
          {
          unsigned int j=0;
          while (j<node.GetLine()->GetVertexList()->Size()-1)
            {
            double dist;
            dist = this->ComputeEuclideanDistanceMetricToSegment(node.GetLine()->GetVertexList()->GetElement(j),
                                                     node.GetLine()->GetVertexList()->GetElement(j+1),
                                                     currentGeometry->GetPolygonExteriorRing()->GetVertexList()->GetElement(i));
            //std::cout << "dist: " << dist << std::endl;
            //std::cout << "m_DistanceThreshold: " << m_DistanceThreshold << std::endl;
            if (dist <= m_DistanceThreshold)
              {
              //Add the current polygon to the tmp DataTree
              //jump to the next one
              tmpDataTree->GetDataTree()->Add(currentGeometry, document);
              j = node.GetLine()->GetVertexList()->Size();
              i = currentGeometry->GetPolygonExteriorRing()->GetVertexList()->Size();
              }
            ++j;
            }
          ++i;
          }
        }
      ++itVector;
      }

  /*
  std::cout << this->GetInputVectorData()->GetDataTree()->Count() << std::endl;
  std::cout << tmpDataTree->GetDataTree()->Count() << std::endl;
  */
  unsigned int crossAcc = 0;
  unsigned int nbBuildings = 0;

  TreeIteratorType it(tmpDataTree->GetDataTree());
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    if (it.Get()->IsPolygonFeature())
      {
      typename DataNodeType::Pointer currentGeometry = it.Get();
      nbBuildings ++;
      for (unsigned int i=0; i<node.GetLine()->GetVertexList()->Size()-1; ++i)
        {

        if(currentGeometry->GetPolygonExteriorRing()->NbCrossing(node.GetLine()->GetVertexList()->GetElement(i),
                                                                 node.GetLine()->GetVertexList()->GetElement(i+1)))
          {
          crossAcc ++;
          break;
          }
        }
      }
    ++it;
    }

  if(nbBuildings == 0)
    {
    output.push_back(static_cast<PrecisionType>(0.));
    }
  else
    {
    output.push_back(static_cast<PrecisionType>((double)(crossAcc)/(double)(nbBuildings)));
    }

  output.push_back(static_cast<PrecisionType>(crossAcc));
  output.push_back(static_cast<PrecisionType>(nbBuildings));

  return output;
}


} // end namespace otb

#endif
