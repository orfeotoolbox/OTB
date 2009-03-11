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
#include "itkCovariantVector.h"
#include "vnl/vnl_math.h"

namespace otb
{

/**
 *
 */
template <class TImage, class TLinesList , class TPointSet>
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::LineSpatialObjectListToRightAnglePointSetFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_ThresholdDistance = 5.;
  m_ThresholdAngle = M_PI/36.;  //36 cause we want 5 degrees threshold

}

/**
 * Method : Set The Input Image
 */
template <class TImage, class TLinesList , class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::SetInputImage(InputImageType * inputImage)
{
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast< InputImageType * >( inputImage ) );
}

 /**
 * Method : Get The Input Image
 */
template <class TImage, class TLinesList , class TPointSet>
typename LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::InputImageType *
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::GetInputImage()
{
  return static_cast<InputImageType *>
         (this->ProcessObjectType::GetInput(1) );
}

/**
 * Method : GenerateData()
 */
template <class TImage, class TLinesList , class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::GenerateData()
{
  unsigned int counterTest = 0, counterCur = 0;
  
  /** Output*/
  m_OutputPointSet = this->GetOutput();

  /** Get The input Lines*/
  typename InputLinesListType::Pointer inputLinesList = const_cast<InputLinesListType *>(this->GetInput());

  /** Instancition of Cheking availability Matrix*/
  unsigned int numberOfLines = inputLinesList->size();
  checkMatrixType              segmentsUsedMatrix(numberOfLines,numberOfLines);
  segmentsUsedMatrix.Fill(0);
  
  /** Loop to get all the lines in the listLine */
  InputLinesListTypeIterator             itLinesListTest  = inputLinesList->begin();   /** Current tested Line */
  
  while(itLinesListTest != inputLinesList->end() )
    {
      counterCur = 0;
      InputLinesListTypeIterator            itLinesListCur   = inputLinesList->begin();    /** Line tested with with current Line*/
      InputLinesListTypeIterator            itLinesListCurEnd   = inputLinesList->end();
      
      while(itLinesListCur != itLinesListCurEnd )
        {
          /** Check If segments are already computed */
           if( segmentsUsedMatrix[counterTest][counterCur]== 0 && segmentsUsedMatrix[counterCur][counterTest] == 0 )
             {
              /** Set the segments to USED (== 1)*/
              segmentsUsedMatrix[counterTest][counterCur] = 1;
              segmentsUsedMatrix[counterCur][counterTest] = 1;
              
              /** Compute the distance from CurLine to DstLine*/
              float SegmentDist = this->ComputeDistanceBetweenSegments(*itLinesListTest , *itLinesListCur);
                 
              /** Check if the distance separating the segments is under the threshold*/
              if(SegmentDist < m_ThresholdDistance /* Threshold : 5 Pixels*/)
                {
                  /** Compute the angle formed by the two segments */
                  float Angle = this->ComputeAngleFormedBySegments(*itLinesListTest, *itLinesListCur);
                                                    
                  /** Check if the angle is a right one */
                  if(vcl_abs(Angle - M_PI/2.) <= m_ThresholdAngle ) 
                    {
                      /** Right angle coordinate*/
                      PointType              RightAngleCoordinate;
                      RightAngleCoordinate = this->ComputeAngleRightCoordinate(*itLinesListTest, *itLinesListCur);
                      
                      /** If Right Angle:  Add it to the pointSet*/
                      this->AddRightAngleToPointSet(RightAngleCoordinate , *itLinesListTest , *itLinesListCur );
                      
                    }
                }
            }
          counterCur++;
          ++itLinesListCur;
        }
      counterTest++;
      ++itLinesListTest;
    }
}

/**
 * Method : ComputeDistanceBetweenSegments 
 */
template <class TImage, class TLinesList , class TPointSet>
float
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::ComputeDistanceBetweenSegments(LineType * lineDst , LineType * lineSrc)
{
  /** Local variable*/
  InputIndexType IndexBeginSrc, IndexEndSrc;  
  
  /** Extract Indexes from the Src line for segment distance computation*/
  typename LineType::PointListType & pointsList = lineSrc->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();
  
  IndexBeginSrc[0] = static_cast<unsigned int>((*itPoints).GetPosition()[0]);  
  IndexBeginSrc[1] = static_cast<unsigned int>((*itPoints).GetPosition()[1]);  
  ++itPoints;
  IndexEndSrc[0] = static_cast<unsigned int>((*itPoints).GetPosition()[0]);  
  IndexEndSrc[1] = static_cast<unsigned int>((*itPoints).GetPosition()[1]);  
  
  /** Verify if the indexes of the line are inside the region*/
  typename InputImageType::SizeType size = this->GetInputImage()->GetRequestedRegion().GetSize();
  //unsigned int width = size[0];
  //unsigned int length = size[1];
  
//   if(IndexBeginSrc[0] > width)  IndexBeginSrc[0] = width-1;
//   if(IndexBeginSrc[1] > length) IndexBeginSrc[1] = length-1;

//   if(IndexEndSrc[0] > width)   IndexEndSrc[0] = width-1;
//   if(IndexEndSrc[1] > length)  IndexEndSrc[1] = length-1;

//   if(IndexBeginSrc[0] <0) IndexBeginSrc[0] = 0;
//   if(IndexBeginSrc[1] <0) IndexBeginSrc[1] = 0;

//   if(IndexEndSrc[0]<0)   IndexEndSrc[0] = 0;
//   if(IndexEndSrc[1]<0)   IndexEndSrc[1] = 0;

 
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType &pointsListDst = lineDst->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();

  float Xq1 = (*itPointsDst).GetPosition()[0];  //xq1
  float Yq1 = (*itPointsDst).GetPosition()[1];  //yq1
  ++itPointsDst;
  float Xq2 = (*itPointsDst).GetPosition()[0];  //xq2
  float Yq2 = (*itPointsDst).GetPosition()[1];  //yq2
  
  /**  Those constants are used in the loop :  compute them once
   *   d(P,[Q1Q2]) = |PQ1^PQ2|/|Q1Q2|
   */
  
  float SegmentLength = vcl_sqrt((Xq1-Xq2)* (Xq1-Xq2) + (Yq1-Yq2) *(Yq1-Yq2));
  float CrossProduct  =   Xq1*Yq2 - Xq2*Yq1 ;

  /** Define a line iterator */
  itk::LineIterator<InputImageType> itLine(this->GetInputImage() , IndexBeginSrc , IndexEndSrc);
  itLine.GoToBegin();
  
  float MinDistance = 10000.;

  while(!itLine.IsAtEnd())
    {
      InputIndexType IndexCur = itLine.GetIndex();
      
      if(this->GetInputImage()->GetRequestedRegion().IsInside(IndexCur) )
        {
          float xp = static_cast<float>(itLine.GetIndex()[0]);
          float yp = static_cast<float>(itLine.GetIndex()[1]);
          float Num   = vcl_abs(xp*(Yq1-Yq2) + yp*(Xq2-Xq1) + CrossProduct);
          
          /** distance from Point P to Segment Q1Q2*/
          float CurDistance = Num/SegmentLength;

          if(CurDistance <  MinDistance )
            MinDistance = CurDistance;

          if(MinDistance < 1e-50)  //MinDistance supposed to be positive
          return 0.;
        }
      ++itLine;
    }
  
  return MinDistance;
}

/**
 * Method : Compute Angle formed by the two segments
 * 
 */
template <class TImage, class TLinesList , class TPointSet>
float
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::ComputeAngleFormedBySegments(LineType * lineDst , LineType * lineSrc)
{
  /** ----- */
  typename LineType::PointListType & pointsList = lineSrc->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();
  
  float Xp1 = (*itPoints).GetPosition()[0];  
  float Yp1 = (*itPoints).GetPosition()[1];  
  ++itPoints;
  float Xp2 = (*itPoints).GetPosition()[0];  
  float Yp2 = (*itPoints).GetPosition()[1];  
 
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType &pointsListDst = lineDst->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();

  float Xq1 = (*itPointsDst).GetPosition()[0];  //xq1
  float Yq1 = (*itPointsDst).GetPosition()[1];  //yq1
  ++itPointsDst;
  float Xq2 = (*itPointsDst).GetPosition()[0];  //xq2
  float Yq2 = (*itPointsDst).GetPosition()[1];  //yq2

  /** Vectors support computation */
  float SegmentLengthP = vcl_sqrt( (Xp1-Xp2)* (Xp1-Xp2) + (Yp1-Yp2) *(Yp1-Yp2) ) ;
  float SegmentLengthQ = vcl_sqrt( (Xq1-Xq2)* (Xq1-Xq2) + (Yq1-Yq2) *(Yq1-Yq2) ) ;
  
  float X = vcl_abs((Xp1 - Xp2)*(Xq1-Xq2));
  float Y = vcl_abs((Yp1 - Yp2)*(Yq1-Yq2));

  float angle = vcl_acos( ( X + Y )/( SegmentLengthP * SegmentLengthQ ));  //Scalar Product
      
  return angle;
}

/**
 * Method : ComputeDistanceBetweenSegments 
 */
template <class TImage, class TLinesList , class TPointSet>
typename LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::PointType
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::ComputeAngleRightCoordinate(LineType * lineDst , LineType * lineSrc)
{
  PointType P;
    
  /** ----- */
  typename LineType::PointListType & pointsList = lineSrc->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();
  
  float Xp1 = (*itPoints).GetPosition()[0];  
  float Yp1 = (*itPoints).GetPosition()[1];  
  ++itPoints;
  float Xp2 = (*itPoints).GetPosition()[0];  
  float Yp2 = (*itPoints).GetPosition()[1];  
 
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType &pointsListDst = lineDst->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();
  
  float Xq1 = (*itPointsDst).GetPosition()[0];  //xq1
  float Yq1 = (*itPointsDst).GetPosition()[1];  //yq1
  ++itPointsDst;
  float Xq2 = (*itPointsDst).GetPosition()[0];  //xq2
  float Yq2 = (*itPointsDst).GetPosition()[1];  //yq2

  //std::cout << "Xp1 " <<Xp1 <<" Yp1 " << Yp1 << " Xp2 " << Xp2 << " Yp2 "<<Yp2  << " Xq1 " <<Xq1  << " Yq1 "<< Yq1 << " Xq2 " << Xq2<< " Yq2 " << Yq2 << std::endl;
  
  /** Compute the equation of the lines A and B  which are support of the segments Src & Dst*/
  // - Line 1 : slope and origin
  float originA = 0. , slopeA = 0.;
  float originB = 0. , slopeB = 0.;
  float LengthSegmentAy = 0., lengthSegmentBy = 0.;

  /** Equation of the first Line*/
  if(vcl_abs(Xp2 - Xp1) <1e-10)
    Xp2 = 0.0001;

  if(Xp1 < Xp2 )
    LengthSegmentAy = Yp2 - Yp1;
  else 
    LengthSegmentAy = Yp1 - Yp2;
  
  slopeA = LengthSegmentAy/(Xp2-Xp1);
  originA = Yp1 - (slopeA * Xp1);

  /** Equation of the second Line*/
  if(vcl_abs(Xq2 - Xq1) <1e-10)
    Xq2 = 0.0001;

  if(Xq1 < Xq2 )
    lengthSegmentBy = Yq2 - Yq1;
  else 
     lengthSegmentBy = Yq1 - Yq2;
  
  slopeB = lengthSegmentBy/(Xq2-Xq1);
  originB = Yq1 - (slopeB * Xq1);

  /** Avoid the case of parallel lines*/
  float denum = 0.;
  if(vcl_abs(slopeA - slopeB ) < 1e-5)
    denum = 0.001;
  else
    denum = slopeA - slopeB;
 
  /** Compute the coordinate of the intersection point Y =AX+B*/
  P[0] =  (originB - originA)/denum;
  P[1] =  slopeA * static_cast<float>(P[0]) + originA;

  return P;
}



/**
 * AddRightAngleToPointSet
 */
template <class TImage, class TLinesList , class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::AddRightAngleToPointSet(PointType rAngle  , LineType * LineDst , LineType* LineCur )
{
  unsigned int CurrentPos = m_OutputPointSet->GetNumberOfPoints();

  m_OutputPointSet->SetPoint(CurrentPos ,rAngle  );
  VectorLineType   vectorLine;
  vectorLine.push_back(LineDst);
  vectorLine.push_back(LineCur);
  m_OutputPointSet->SetPointData(CurrentPos , vectorLine); 

}

/**
 * Standard "PrintSelf" method
 */
template <class TImage, class TLinesList , class TPointSet>
void
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


} // end namespace otb


#endif
