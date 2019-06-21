/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbLineSpatialObjectListToRightAnglePointSetFilter_hxx
#define otbLineSpatialObjectListToRightAnglePointSetFilter_hxx

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

  /** Instancition of Checking availability Matrix*/
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
        if (std::abs(Angle - CONST_PI_2) <= m_ThresholdAngle)
          {
          /** Right angle coordinate*/
          PointType RightAngleCoordinate;
          RightAngleCoordinate = this->ComputeAngleRightCoordinate(*itLinesListTest, *itLinesListCur);

          /** Compute the distance between the two segments and the right angle formed by this segments*/
          double dist1 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate, *itLinesListTest);
          double dist2 = this->ComputeDistanceFromPointToSegment(RightAngleCoordinate, *itLinesListCur);

          /** Use Pythagore to compute the distance between the two segments*/
          double SegmentDistance = std::sqrt(dist1 * dist1 + dist2 * dist2);

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
#if ITK_VERSION_MAJOR < 5
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType&                pointsListDst = line->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();
  double                                           X1 = (*itPointsDst).GetPosition()[0]; //xq1
  double                                           Y1 = (*itPointsDst).GetPosition()[1]; //yq1
  ++itPointsDst;
  double X2 = (*itPointsDst).GetPosition()[0];  //xq2
  double Y2 = (*itPointsDst).GetPosition()[1];  //yq2
#else
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::LinePointListType&                pointsListDst = line->GetPoints();
  typename LineType::LinePointListType::const_iterator itPointsDst = pointsListDst.begin();
  double                                           X1 = (*itPointsDst).GetPositionInObjectSpace()[0]; //xq1
  double                                           Y1 = (*itPointsDst).GetPositionInObjectSpace()[1]; //yq1
  ++itPointsDst;
  double X2 = (*itPointsDst).GetPositionInObjectSpace()[0];  //xq2
  double Y2 = (*itPointsDst).GetPositionInObjectSpace()[1];  //yq2
#endif
  double dist1 = std::sqrt((X1 - rAngle[0]) * (X1 - rAngle[0]) + (Y1 - rAngle[1]) * (Y1 - rAngle[1]));
  double dist2 = std::sqrt((X2 - rAngle[0]) * (X2 - rAngle[0]) + (Y2 - rAngle[1]) * (Y2 - rAngle[1]));

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

  return std::abs(oriDst  - oriSrc);
}

/**
 * Method : ComputeDistanceBetweenSegments
 */
template <class TImage, class TLinesList, class TPointSet>
double
LineSpatialObjectListToRightAnglePointSetFilter<TImage, TLinesList, TPointSet>
::ComputeOrientation(LineType * line)
{
#if ITK_VERSION_MAJOR < 5
  typename LineType::PointListType&                pointsList = line->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();
  double                                           Xp1 = (*itPoints).GetPosition()[0];
  double                                           Yp1 = (*itPoints).GetPosition()[1];

  ++itPoints;
  double Xp2 = (*itPoints).GetPosition()[0];
  double Yp2 = (*itPoints).GetPosition()[1];
#else
  typename LineType::LinePointListType&                pointsList = line->GetPoints();
  typename LineType::LinePointListType::const_iterator itPoints = pointsList.begin();
  double                                           Xp1 = (*itPoints).GetPositionInObjectSpace()[0];
  double                                           Yp1 = (*itPoints).GetPositionInObjectSpace()[1];

  ++itPoints;
  double Xp2 = (*itPoints).GetPositionInObjectSpace()[0];
  double Yp2 = (*itPoints).GetPositionInObjectSpace()[1];
#endif
  //Compute the orientation
  double dx = Xp1 - Xp2;
  double dy = Yp1 - Yp2;
  double orientation = std::atan2(dy, dx);
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


#if ITK_VERSION_MAJOR < 5
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
#else
  /** ----- */
  typename LineType::LinePointListType&                pointsList = lineSrc->GetPoints();
  typename LineType::LinePointListType::const_iterator itPoints = pointsList.begin();

  double Xp1 = (*itPoints).GetPositionInObjectSpace()[0];
  double Yp1 = (*itPoints).GetPositionInObjectSpace()[1];
  ++itPoints;
  double Xp2 = (*itPoints).GetPositionInObjectSpace()[0];
  double Yp2 = (*itPoints).GetPositionInObjectSpace()[1];

  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::LinePointListType&                pointsListDst = lineDst->GetPoints();
  typename LineType::LinePointListType::const_iterator itPointsDst = pointsListDst.begin();

  double Xq1 = (*itPointsDst).GetPositionInObjectSpace()[0];  //xq1
  double Yq1 = (*itPointsDst).GetPositionInObjectSpace()[1];  //yq1
  ++itPointsDst;
  double Xq2 = (*itPointsDst).GetPositionInObjectSpace()[0];  //xq2
  double Yq2 = (*itPointsDst).GetPositionInObjectSpace()[1];  //yq2
#endif

  /** Compute the equation of the lines A and B  which are support of the segments Src & Dst*/
  // - Line 1 : slope and origin
  double originA = 0., slopeA = 0.;
  double originB = 0., slopeB = 0.;
  double LengthSegmentAy = 0., lengthSegmentBy = 0.;

  /** Equation of the first Line*/
  if (std::abs(Xp2 - Xp1) < 1e-10) Xp2 = 0.0001;

  if (Xp1 < Xp2) LengthSegmentAy = Yp2 - Yp1;
  else LengthSegmentAy = Yp1 - Yp2;

  slopeA = LengthSegmentAy / (Xp2 - Xp1);
  originA = Yp1 - (slopeA * Xp1);

  /** Equation of the second Line*/
  if (std::abs(Xq2 - Xq1) < 1e-10) Xq2 = 0.0001;

  if (Xq1 < Xq2) lengthSegmentBy = Yq2 - Yq1;
  else lengthSegmentBy = Yq1 - Yq2;

  slopeB = lengthSegmentBy / (Xq2 - Xq1);
  originB = Yq1 - (slopeB * Xq1);

  /** Avoid the case of parallel lines*/
  double denum = 0.;
  if (std::abs(slopeA - slopeB) < 1e-5) denum = 0.001;
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
