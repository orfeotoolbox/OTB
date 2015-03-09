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
#ifndef __otbLineSpatialObjectListToRightAnglePointSetFilter_txx
#define __otbLineSpatialObjectListToRightAnglePointSetFilter_txx

#include "otbLineSpatialObjectListToRightAnglePointSetFilter.h"
#include "itkLineIterator.h"
#include "itkMatrix.h"
#include "vnl/vnl_math.h"

namespace otb
{

/**
 *
 */
template <class TImage, class TLinesList, class TPointSet>
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::LineSpatialObjectListToRightAnglePointSetFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_ThresholdDistance = 20.;
  m_ThresholdAngle = CONST_PI / 30.;  //36 cause we want 6 degrees threshold
}

/**
 * Method : Set The Input Image
 */
template <class TImage, class TLinesList, class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::SetInputImage(InputImageType * inputImage)
{
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<InputImageType *>(inputImage));
}

/**
* Method : Get The Input Image
*/
template <class TImage, class TLinesList, class TPointSet>
typename LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::InputImageType *
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::GetInputImage()
{
  return static_cast<InputImageType *>
           (this->ProcessObjectType::GetInput(1));
}

/**
 * Method : GenerateData()
 */
template <class TImage, class TLinesList, class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::GenerateData()
{
  unsigned int counterTest = 0, counterCur = 0;

  /** Output*/
  m_OutputPointSet = this->GetOutput();

  /** Get The input Lines*/
  typename InputLinesListType::Pointer inputLinesList = const_cast<InputLinesListType *>(this->GetInput());

  /** Instancition of Cheking availability Matrix*/
  unsigned int numberOfLines = inputLinesList->size();
  checkMatrixType              segmentsUsedMatrix(numberOfLines, numberOfLines);
  segmentsUsedMatrix.Fill(0);

  /** Loop to get all the lines in the listLine */
  InputLinesListTypeIterator itLinesListTest  = inputLinesList->begin();               /** Current tested Line */

  while (itLinesListTest != inputLinesList->end())
    {
    counterCur = 0;
    InputLinesListTypeIterator itLinesListCur   = inputLinesList->begin();                 /** Line tested with with current Line*/
    InputLinesListTypeIterator itLinesListCurEnd   = inputLinesList->end();

    while (itLinesListCur != itLinesListCurEnd)
      {
      /** Check If segments are already computed */
      if (segmentsUsedMatrix[counterTest][counterCur] == 0 && segmentsUsedMatrix[counterCur][counterTest] == 0)
        {
        /** Set the segments to USED (== 1)*/
        segmentsUsedMatrix[counterTest][counterCur] = 1;
        segmentsUsedMatrix[counterCur][counterTest] = 1;

        /** Compute the angle formed by the two segments */
        double Angle = this->ComputeAngleFormedBySegments(*itLinesListTest, *itLinesListCur);

        /** Check if the angle is a right one */
        if (vcl_abs(Angle - CONST_PI_2) <= m_ThresholdAngle)
          {
          /** Right angle coordinate*/
          PointType RightAngleCoordinate;
          RightAngleCoordinate = this->ComputeAngleRightCoordinate(*itLinesListTest, *itLinesListCur);

          /** Compute the distance between the two segments and the right angle formed by this segments*/
          double dist1 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate, *itLinesListTest);
          double dist2 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate, *itLinesListCur);

          /** Use Pythagore to compute the distance between the two segments*/
          double SegmentDistance = vcl_sqrt(dist1 * dist1 + dist2 * dist2);

//                 if(this->ComputeDistanceFromPointToSegment(RightAngleCoordinate, *itLinesListTest) <m_ThresholdDistance &&
//                    this->ComputeDistanceFromPointToSegment(RightAngleCoordinate, *itLinesListCur) <m_ThresholdDistance)
          if (SegmentDistance < m_ThresholdDistance)
            {
            /** If Right Angle & not so far from segments:  Add it to the pointSet*/
            this->AddRightAngleToPointSet(RightAngleCoordinate, *itLinesListTest, *itLinesListCur);
            }
          }
        }
      ++counterCur;
      ++itLinesListCur;
      }
    ++counterTest;
    ++itLinesListTest;
    }
}

/**
 * Method : Compute Distance between a point and a segment
 *
 */
template <class TImage, class TLinesList, class TPointSet>
double
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::ComputeDistanceFromPointToSegment(PointType rAngle, LineType * line)
{
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType&                pointsListDst = line->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();
  double                                           X1 = (*itPointsDst).GetPosition()[0]; //xq1
  double                                           Y1 = (*itPointsDst).GetPosition()[1]; //yq1
  ++itPointsDst;
  double X2 = (*itPointsDst).GetPosition()[0];  //xq2
  double Y2 = (*itPointsDst).GetPosition()[1];  //yq2

  double dist1 = vcl_sqrt((X1 - rAngle[0]) * (X1 - rAngle[0]) + (Y1 - rAngle[1]) * (Y1 - rAngle[1]));
  double dist2 = vcl_sqrt((X2 - rAngle[0]) * (X2 - rAngle[0]) + (Y2 - rAngle[1]) * (Y2 - rAngle[1]));

  return std::min(dist1, dist2);
}

/**
 * Method : Compute Angle formed by the two segments
 *
 */
template <class TImage, class TLinesList, class TPointSet>
double
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::ComputeAngleFormedBySegments(LineType * lineDst, LineType * lineSrc)
{
  double oriDst = this->ComputeOrientation(lineDst);
  double oriSrc = this->ComputeOrientation(lineSrc);

  return vcl_abs(oriDst  - oriSrc);
}

/**
 * Method : ComputeDistanceBetweenSegments
 */
template <class TImage, class TLinesList, class TPointSet>
double
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::ComputeOrientation(LineType * line)
{
  typename LineType::PointListType&                pointsList = line->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();
  double                                           Xp1 = (*itPoints).GetPosition()[0];
  double                                           Yp1 = (*itPoints).GetPosition()[1];

  ++itPoints;
  double Xp2 = (*itPoints).GetPosition()[0];
  double Yp2 = (*itPoints).GetPosition()[1];

  //Compute the orientation
  double dx = Xp1 - Xp2;
  double dy = Yp1 - Yp2;
  double orientation = vcl_atan2(dy, dx);
  if (orientation < 0) orientation += CONST_PI;

  return orientation;
}

/**
 * Method : ComputeDistanceBetweenSegments
 */
template <class TImage, class TLinesList, class TPointSet>
typename LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::PointType
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::ComputeAngleRightCoordinate(LineType * lineDst, LineType * lineSrc)
{
  PointType P;

  /** ----- */
  typename LineType::PointListType&                pointsList = lineSrc->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();

  double Xp1 = (*itPoints).GetPosition()[0];
  double Yp1 = (*itPoints).GetPosition()[1];
  ++itPoints;
  double Xp2 = (*itPoints).GetPosition()[0];
  double Yp2 = (*itPoints).GetPosition()[1];

  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType&                pointsListDst = lineDst->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();

  double Xq1 = (*itPointsDst).GetPosition()[0];  //xq1
  double Yq1 = (*itPointsDst).GetPosition()[1];  //yq1
  ++itPointsDst;
  double Xq2 = (*itPointsDst).GetPosition()[0];  //xq2
  double Yq2 = (*itPointsDst).GetPosition()[1];  //yq2

  /** Compute the equation of the lines A and B  which are support of the segments Src & Dst*/
  // - Line 1 : slope and origin
  double originA = 0., slopeA = 0.;
  double originB = 0., slopeB = 0.;
  double LengthSegmentAy = 0., lengthSegmentBy = 0.;

  /** Equation of the first Line*/
  if (vcl_abs(Xp2 - Xp1) < 1e-10) Xp2 = 0.0001;

  if (Xp1 < Xp2) LengthSegmentAy = Yp2 - Yp1;
  else LengthSegmentAy = Yp1 - Yp2;

  slopeA = LengthSegmentAy / (Xp2 - Xp1);
  originA = Yp1 - (slopeA * Xp1);

  /** Equation of the second Line*/
  if (vcl_abs(Xq2 - Xq1) < 1e-10) Xq2 = 0.0001;

  if (Xq1 < Xq2) lengthSegmentBy = Yq2 - Yq1;
  else lengthSegmentBy = Yq1 - Yq2;

  slopeB = lengthSegmentBy / (Xq2 - Xq1);
  originB = Yq1 - (slopeB * Xq1);

  /** Avoid the case of parallel lines*/
  double denum = 0.;
  if (vcl_abs(slopeA - slopeB) < 1e-5) denum = 0.001;
  else denum = slopeA - slopeB;

  /** Compute the coordinate of the intersection point Y =AX+B*/
  P[0] =  (originB - originA) / denum;
  P[1] =  slopeA * static_cast<double>(P[0]) + originA;

  return P;
}

/**
 * AddRightAngleToPointSet
 */
template <class TImage, class TLinesList, class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::AddRightAngleToPointSet(PointType rAngle, LineType * LineDst, LineType* LineCur)
{
  unsigned int CurrentPos = m_OutputPointSet->GetNumberOfPoints();

  m_OutputPointSet->SetPoint(CurrentPos, rAngle);
  VectorLineType vectorLine;
  vectorLine.push_back(LineDst);
  vectorLine.push_back(LineCur);
  m_OutputPointSet->SetPointData(CurrentPos, vectorLine);
}

/**
 * Standard "PrintSelf" method
 */
template <class TImage, class TLinesList, class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
