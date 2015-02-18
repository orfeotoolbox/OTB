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
#ifndef __otbVectorDataToRightAngleVectorDataFilter_txx
#define __otbVectorDataToRightAngleVectorDataFilter_txx

#include "otbVectorDataToRightAngleVectorDataFilter.h"
#include "otbVectorData.h"

namespace otb
{
template <class TVectorData>
VectorDataToRightAngleVectorDataFilter<TVectorData>
::VectorDataToRightAngleVectorDataFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_DistanceThreshold = 20.;
  m_AngleThreshold = CONST_PI / 30.;  //we want a threshold at 6 degrees
}

template <class TVectorData>
void
VectorDataToRightAngleVectorDataFilter<TVectorData>
::GenerateData()
{
  // Get the input segments
  typename VectorDataType::Pointer vData = const_cast<VectorDataType *>(this->GetInput());

  // Output
  this->GetOutput(0)->SetMetaDataDictionary(vData->GetMetaDataDictionary());

  // Retrieving root node
  typename DataNodeType::Pointer root = this->GetOutput(0)->GetDataTree()->GetRoot()->Get();
  // Create the document node
  typename DataNodeType::Pointer document = DataNodeType::New();
  document->SetNodeType(otb::DOCUMENT);
  // Adding the layer to the data tree
  this->GetOutput(0)->GetDataTree()->Add(document, root);
  // Create the folder node
  typename DataNodeType::Pointer folder = DataNodeType::New();
  folder->SetNodeType(otb::FOLDER);
  this->GetOutput(0)->GetDataTree()->Add(folder, document);
  this->GetOutput(0)->SetProjectionRef(vData->GetProjectionRef());

  // Itterate on the vector data
  TreeIteratorType itVectorRef(vData->GetDataTree());      // Reference
  itVectorRef.GoToBegin();

  while (!itVectorRef.IsAtEnd())
    {
    if (!itVectorRef.Get()->IsLineFeature())
      {
      ++itVectorRef;
      continue; // do not process if it's not a line
      }
    TreeIteratorType itVectorCur = itVectorRef;    // Current

    while (!itVectorCur.IsAtEnd())
      {
      if (!itVectorCur.Get()->IsLineFeature())
        {
        ++itVectorCur;
        continue; // do not process if it's not a line
        }
      // Compute the angle formed by the two segments
      double Angle = this->ComputeAngleFormedBySegments(itVectorRef.Get()->GetLine(),
                                                        itVectorCur.Get()->GetLine());

      // Check if the angle is a right one
      if (vcl_abs(Angle - CONST_PI_2) <= m_AngleThreshold)
        {
        // Right angle coordinate
        PointType RightAngleCoordinate;
        RightAngleCoordinate = this->ComputeRightAngleCoordinate(itVectorRef.Get()->GetLine(),
                                                                 itVectorCur.Get()->GetLine());

        // Compute the distance between the two segments and the right angle formed by this segments
        double dist1_2 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate,
                                                               itVectorRef.Get()->GetLine());
        double dist2_2 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate,
                                                               itVectorCur.Get()->GetLine());

        double threshold_2 = m_DistanceThreshold * m_DistanceThreshold;
       if (dist1_2 < threshold_2 && dist2_2 < threshold_2)
          {
          // If Right Angle & not so far from segments: Add to the output
          typename DataNodeType::Pointer CurrentGeometry = DataNodeType::New();
          CurrentGeometry->SetNodeId("FEATURE_POINT");
          CurrentGeometry->SetNodeType(otb::FEATURE_POINT);
          CurrentGeometry->SetPoint(RightAngleCoordinate);
          this->GetOutput(0)->GetDataTree()->Add(CurrentGeometry, folder);
          }
        }

      ++itVectorCur;
      }
    ++itVectorRef;
    }
}

template <class TVectorData>
double
VectorDataToRightAngleVectorDataFilter<TVectorData>
::ComputeDistanceFromPointToSegment(PointType rAngle, LineType * line)
{
  VertexListType * vertexList = const_cast<VertexListType *>(line->GetVertexList());

  VertexType v0 = vertexList->GetElement(0);
  VertexType v1 = vertexList->GetElement(1);

  // Case the extremities of the segment are the same
  double l2 = v0.SquaredEuclideanDistanceTo(v1);

  // Is the projection of rAngle on the segment inside (0<u<1) or
  // inside the segment bounds
  double u = ((rAngle[0] - v0[0]) *(v1[0] - v0[0] ) +
    (rAngle[1] - v0[1]) *(v1[1] - v0[1])) / l2;

  if( u < 1e-10 ) u = 0.;
  if( u -1. > 1e-10 ) u = 1.;

  double x = v0[0] + u *(v1[0] - v0[0] );
  double y = v0[1] + u *(v1[1] - v0[1] );

  double dx = x - rAngle[0];
  double dy = y - rAngle[1];

  return dx*dx + dy*dy;
}

template <class TVectorData>
double
VectorDataToRightAngleVectorDataFilter<TVectorData>
::ComputeAngleFormedBySegments(LineType * lineDst, LineType * lineSrc)
{
  double oriDst = this->ComputeOrientation(lineDst);
  double oriSrc = this->ComputeOrientation(lineSrc);

  return vcl_abs(oriDst  - oriSrc);
}

template <class TVectorData>
double
VectorDataToRightAngleVectorDataFilter<TVectorData>
::ComputeOrientation(LineType * line)
{
  VertexListType * vertexList = const_cast<VertexListType *>(line->GetVertexList());

  double Xp1 = vertexList->GetElement(0)[0];
  double Yp1 = vertexList->GetElement(0)[1];

  double Xp2 = vertexList->GetElement(1)[0];
  double Yp2 = vertexList->GetElement(1)[1];

  //Compute the orientation
  double dx = Xp1 - Xp2;
  double dy = Yp1 - Yp2;
  double orientation = vcl_atan2(dy, dx);
  if (orientation < 0) orientation += CONST_PI;

  return orientation;
}

template <class TVectorData>
typename VectorDataToRightAngleVectorDataFilter<TVectorData>
::PointType
VectorDataToRightAngleVectorDataFilter<TVectorData>
::ComputeRightAngleCoordinate(LineType * lineDst, LineType * lineSrc)
{
  PointType P;

  VertexListType * vertexListSrc = const_cast<VertexListType *>(lineSrc->GetVertexList());
  double Xp1 = vertexListSrc->GetElement(0)[0];
  double Yp1 = vertexListSrc->GetElement(0)[1];
  double Xp2 = vertexListSrc->GetElement(1)[0];
  double Yp2 = vertexListSrc->GetElement(1)[1];

  VertexListType * vertexListDst = const_cast<VertexListType *>(lineDst->GetVertexList());
  double Xq1 = vertexListDst->GetElement(0)[0];
  double Yq1 = vertexListDst->GetElement(0)[1];
  double Xq2 = vertexListDst->GetElement(1)[0];
  double Yq2 = vertexListDst->GetElement(1)[1];

  double numU = (Xq2 - Xq1)*(Yp1-Yq1) - (Yq2-Yq1)*(Xp1-Xq1);
  double denumU = (Yq2-Yq1)*(Xp2-Xp1)-(Xq2-Xq1)*(Yp2-Yp1);

  double u  = numU/ denumU;

  P[0] = Xp1 + u*(Xp2-Xp1);
  P[1] = Yp1 + u * (Yp2-Yp1);

  return P;
}

template <class TVectorData>
void
VectorDataToRightAngleVectorDataFilter<TVectorData>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
