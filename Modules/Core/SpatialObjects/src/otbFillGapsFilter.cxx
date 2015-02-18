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

#include "otbFillGapsFilter.h"

namespace otb
{

/**
 * Constructor
 */

FillGapsFilter::FillGapsFilter()
{
  this->ProcessObjectType::SetNumberOfRequiredInputs(1);
  this->ProcessObjectType::SetNumberOfRequiredOutputs(1);

  LineSpatialObjectListPointer output = LineSpatialObjectListType::New();

  this->ProcessObjectType::SetNthOutput(0, output);

  m_Radius      =  4.0;
  m_AngularBeam =  1.0;
}

void
FillGapsFilter
::SetInput(const LineSpatialObjectListType * input)
{
  this->ProcessObjectType::SetNthInput(0,
                                       const_cast<LineSpatialObjectListType *>(input));
}

const FillGapsFilter::LineSpatialObjectListType *
FillGapsFilter
::GetInput(void)
{
  return static_cast<const LineSpatialObjectListType *>
         (this->ProcessObjectType::GetInput(0));
}

FillGapsFilter::LineSpatialObjectListType *
FillGapsFilter
::GetOutput(void)
{
  return static_cast<LineSpatialObjectListType *>
         (this->ProcessObjectType::GetOutput(0));
}

/**
 *
 */

void
FillGapsFilter
::GenerateData()
{
  // Get the LineSpatialObject

  const LineSpatialObjectList * inputLine = this->GetInput();
  LineSpatialObjectList *       outputLine = this->GetOutput();

  // Get the list of points which consists of two points to represent a
  // straight line

  LineSpatialObjectListType::const_iterator itLineListA    = inputLine->begin();
  LineSpatialObjectListType::const_iterator itLineListAEnd  = inputLine->end();
  LineSpatialObjectListType::const_iterator itLineListB;
  LineSpatialObjectListType::const_iterator itLineListBEnd  = inputLine->end();

  double x1(0.), y1(0.), x2(0.), y2(0.), x3(0.), y3(0.), x4(0.), y4(0.);
  double xTemp(0.), yTemp(0.);
  double R13(0.), R14(0.), R23(0.), R24(0.);
  double CosTheta(0.);

  PointListType::const_iterator itPoints;

  CosTheta = vcl_cos(m_AngularBeam);
  --itLineListAEnd;

  while (itLineListA != itLineListAEnd)
    {
    itLineListB = itLineListA;
    ++itLineListB;

    PointListType& pointsList = (*itLineListA)->GetPoints();
    itPoints = pointsList.begin();

    x1 = (*itPoints).GetPosition()[0];
    y1 = (*itPoints).GetPosition()[1];

    ++itPoints;
    x2 = (*itPoints).GetPosition()[0];
    y2 = (*itPoints).GetPosition()[1];

    PointType     point;
    PointListType pointList;

    point.SetPosition(x1, y1);
    pointList.push_back(point);
    point.SetPosition(x2, y2);
    pointList.push_back(point);

    LineSpatialObjectType::Pointer line = LineSpatialObjectType::New();
    line->SetId(0);
    line->SetPoints(pointList);
    line->ComputeBoundingBox();
    outputLine->push_back(line);

    while (itLineListB != itLineListBEnd)
      {

      pointsList = (*itLineListB)->GetPoints();
      itPoints = pointsList.begin();

      x3 = (*itPoints).GetPosition()[0];
      y3 = (*itPoints).GetPosition()[1];

      ++itPoints;
      x4 = (*itPoints).GetPosition()[0];
      y4 = (*itPoints).GetPosition()[1];

      // Calculate the radius for each point of each line

      R13 = vcl_sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
      R14 = vcl_sqrt((x1 - x4) * (x1 - x4) + (y1 - y4) * (y1 - y4));
      R23 = vcl_sqrt((x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3));
      R24 = vcl_sqrt((x2 - x4) * (x2 - x4) + (y2 - y4) * (y2 - y4));

      double Rmin = m_Radius;

      // Test the lower Radius
      if (R13 < Rmin) Rmin = R13;
      if (R14 < Rmin) Rmin = R14;
      if (R23 < Rmin) Rmin = R23;
      if (R24 < Rmin) Rmin = R24;

      if (Rmin < m_Radius)
        {
        // Sort Points such as the radius of P2 and P3 is the smallest one.
        if (Rmin == R24)
          {
          xTemp = x3;
          yTemp = y3;
          x3    = x4;
          y3    = y4;
          x4    = xTemp;
          y4    = yTemp;
          }
        if (Rmin == R13)
          {
          xTemp = x1;
          yTemp = y1;
          x1    = x2;
          y1    = y2;
          x2    = xTemp;
          y2    = yTemp;
          }
        if (Rmin == R14)
          {
          xTemp = x3;
          yTemp = y3;
          x3    = x4;
          y3    = y4;
          x4    = xTemp;
          y4    = yTemp;
          xTemp = x1;
          yTemp = y1;
          x1    = x2;
          y1    = y2;
          x2    = xTemp;
          y2    = yTemp;
          }

        //Estimate the norm each line
        /*  double Norm12, Norm23, Norm34;
          Norm12 = vcl_sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
          Norm23 = vcl_sqrt( (x2-x3)*(x2-x3) + (y2-y3)*(y2-y3) );
          Norm34 = vcl_sqrt( (x3-x4)*(x3-x4) + (y3-y4)*(y3-y4) );
          */
        double Angle12_23, Angle12_34, Angle23_34;
        //Estimate the angle between lines 12-23 and lines 12-34
        /*Angle12_23 = (x2-x1)*(x3-x2) + (y2-y1)*(y3-y2);
        Angle12_23 = Angle12_23 / Norm12 / Norm23;

        Angle12_34 = (x2-x1)*(x4-x3) + (y2-y1)*(y4-y3);
        Angle12_34 = Angle12_34 / Norm12 / Norm34; */

        Angle12_23 = vcl_cos(vcl_atan2((y2 - y1), (x2 - x1)) - vcl_atan2((y3 - y2), (x3 - x2)));
        Angle12_34 = vcl_cos(vcl_atan2((y2 - y1), (x2 - x1)) - vcl_atan2((y4 - y3), (x4 - x3)));
        Angle23_34 = vcl_cos(vcl_atan2((y3 - y2), (x3 - x2)) - vcl_atan2((y4 - y3), (x4 - x3)));

        if ((Angle12_23 > CosTheta) && (Angle12_34 > CosTheta) && (Angle23_34 > CosTheta))
          {

          // Store 23-segment
          PointType     point;
          PointListType pointList;

          point.SetPosition(x2, y2);
          pointList.push_back(point);
          point.SetPosition(x3, y3);
          pointList.push_back(point);

          LineSpatialObjectType::Pointer line = LineSpatialObjectType::New();
          line->SetId(0);
          line->SetPoints(pointList);
          line->ComputeBoundingBox();
          outputLine->push_back(line);

          pointList.clear();

          }
        } // if(Rmin < m_Radius)

      ++itLineListB;
      } // while(itLineListB != itLineListBEnd)
    ++itLineListA;
    }

  // Insert the last element

  PointType     point;
  PointListType pointList;

  point.SetPosition(x3, y3);
  pointList.push_back(point);
  point.SetPosition(x4, y4);
  pointList.push_back(point);

  LineSpatialObjectType::Pointer line = LineSpatialObjectType::New();
  line->SetId(0);
  line->SetPoints(pointList);
  line->ComputeBoundingBox();
  outputLine->push_back(line);

  pointList.clear();

}

/**
 *
 */

void
FillGapsFilter
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
