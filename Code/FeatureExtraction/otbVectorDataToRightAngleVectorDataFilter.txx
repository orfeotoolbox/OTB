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
::SetInput(VectorDataType * inputVData)
{
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<VectorDataType *>(inputVData));
}

template <class TVectorData>
typename VectorDataToRightAngleVectorDataFilter<TVectorData>
::VectorDataType *
VectorDataToRightAngleVectorDataFilter<TVectorData>
::GetInput()
{
  return static_cast<VectorDataType *>
           (this->ProcessObjectType::GetInput(0));
}

template <class TVectorData>
void
VectorDataToRightAngleVectorDataFilter<TVectorData>
::GenerateData()
{
  // Output
  this->GetOutput(0)->SetMetaDataDictionary(this->GetInput()->GetMetaDataDictionary());
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

  // Get the input segments
  typename VectorDataType::Pointer vData = const_cast<VectorDataType *>(this->GetInput());

  // Itterate on the vector data
  TreeIteratorType itVectorRef(vData->GetDataTree());      // Reference
  itVectorRef.GoToBegin();
  
  while (!itVectorRef.IsAtEnd())
    {
    if (itVectorRef.Get()->IsLineFeature())
      {
      TreeIteratorType itVectorCur = itVectorRef;    // Current
      
      while (!itVectorCur.IsAtEnd())
        {
        if (itVectorCur.Get()->IsLineFeature())
          {
          // Compute the angle formed by the two segments
          double Angle = this->ComputeAngleFormedBySegments(itVectorRef.Get()->GetLine(),
                                                            itVectorCur.Get()->GetLine());

          // Check if the angle is a right one
          if (vcl_abs(Angle - CONST_PI_2) <= m_AngleThreshold)
            {
//            std::cout << "ComputeAngleFormedBySegments: " << Angle << std::endl;

            // Right angle coordinate
            PointType RightAngleCoordinate;
            RightAngleCoordinate = this->ComputeRightAngleCoordinate(itVectorRef.Get()->GetLine(),
                                                                     itVectorCur.Get()->GetLine());
            
            // Compute the distance between the two segments and the right angle formed by this segments
            double dist1 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate,
                                                                   itVectorRef.Get()->GetLine());
            double dist2 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate,
                                                                   itVectorCur.Get()->GetLine());
            
            // Use Pythagore to compute the distance between the two segments
            double SegmentDistance = vcl_sqrt(dist1 * dist1 + dist2 * dist2);
            
            if (SegmentDistance < m_DistanceThreshold)
              {
              // If Right Angle & not so far from segments: Add to the output
              typename DataNodeType::Pointer CurrentGeometry = DataNodeType::New();
              CurrentGeometry->SetNodeId("FEATURE_POINT");
              CurrentGeometry->SetNodeType(otb::FEATURE_POINT);
              CurrentGeometry->SetPoint(RightAngleCoordinate);
              this->GetOutput(0)->GetDataTree()->Add(CurrentGeometry, folder);
              }
            }
          }
        ++itVectorCur;
        }
      }
    ++itVectorRef;
    }
}
 
template <class TVectorData>
double
VectorDataToRightAngleVectorDataFilter<TVectorData>
::ComputeDistanceFromPointToSegment(PointType rAngle, LineType * line)
{
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  
  VertexListType * vertexList = const_cast<VertexListType *>(line->GetVertexList());

  double X1 = vertexList->GetElement(0)[0];
  double Y1 = vertexList->GetElement(0)[1];

  double X2 = vertexList->GetElement(1)[0];
  double Y2 = vertexList->GetElement(1)[1];

  double dist1 = vcl_sqrt((X1 - rAngle[0]) * (X1 - rAngle[0]) + (Y1 - rAngle[1]) * (Y1 - rAngle[1]));
  double dist2 = vcl_sqrt((X2 - rAngle[0]) * (X2 - rAngle[0]) + (Y2 - rAngle[1]) * (Y2 - rAngle[1]));

  return std::min(dist1, dist2);
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

  // Compute the equation of the lines A and B  which are support of the segments Src & Dst
  // - Line 1 : slope and origin
  double originA = 0., slopeA = 0.;
  double originB = 0., slopeB = 0.;
  double LengthSegmentAy = 0., lengthSegmentBy = 0.;

  // Equation of the first Line
  if (vcl_abs(Xp2 - Xp1) < 1e-10) Xp2 = 0.0001;

  if (Xp1 < Xp2) LengthSegmentAy = Yp2 - Yp1;
  else LengthSegmentAy = Yp1 - Yp2;

  slopeA = LengthSegmentAy / (Xp2 - Xp1);
  originA = Yp1 - (slopeA * Xp1);

  // Equation of the second Line
  if (vcl_abs(Xq2 - Xq1) < 1e-10) Xq2 = 0.0001;

  if (Xq1 < Xq2) lengthSegmentBy = Yq2 - Yq1;
  else lengthSegmentBy = Yq1 - Yq2;

  slopeB = lengthSegmentBy / (Xq2 - Xq1);
  originB = Yq1 - (slopeB * Xq1);

  // Avoid the case of parallel lines
  double denum = 0.;
  if (vcl_abs(slopeA - slopeB) < 1e-5) denum = 0.001;
  else denum = slopeA - slopeB;

  // Compute the coordinate of the intersection point Y =AX+B
  P[0] =  (originB - originA) / denum;
  P[1] =  slopeA * static_cast<double>(P[0]) + originA;

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
