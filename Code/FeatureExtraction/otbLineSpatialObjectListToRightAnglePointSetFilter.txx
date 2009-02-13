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
  /** Output*/
  m_OutputPointSet = this->GetOutput();

  /** Get The input Lines*/
  typename InputLinesListType::Pointer inputLinesList = const_cast<InputLinesListType *>(this->GetInput());
  
  
  /** Loop to get all the lines in the listLine */
  InputLinesListTypeIterator            itLinesListTest  = inputLinesList->begin();   /** Current tested Line */
  
  
  while(itLinesListTest != inputLinesList->end() )
    {
      std::cout << " ----------------------------- "<<std::endl;
      std::cout << "Test First Postion : " << ((*itLinesListTest)->GetPoint(0)->GetPosition())[0]<<std::endl;
      InputLinesListTypeIterator            itLinesListCur   = inputLinesList->begin();    /** Line tested with with current Line*/
      InputLinesListTypeIterator            itLinesListCurEnd   = inputLinesList->end();
      
      while(itLinesListCur != itLinesListCurEnd )
	{
	  std::cout <<  "    Cur First  Postion : " << ((*itLinesListCur)->GetPoint(0)->GetPosition())[0]<<std::endl;
	  

	  /** Compute the distance from CurLine to DstLine*/
	  float SegmentDist = this->ComputeDistanceBetweenSegments(*itLinesListTest , *itLinesListCur);
	  std::cout << " Distance " << SegmentDist  <<std::endl;



	 
 	  /** Check if the distance separating the segments is under the threshold*/
 	  if(SegmentDist < 5. /* Threshold : 5 Pixels*/)
 	    {
 	      /** Compute the angle formed by the two segments */
 	      float Angle = this->ComputeAngleFormedBySegments(*itLinesListTest, *itLinesListCur);
	      
// 	      /** Check if the angle is a right one */
// 	      if(this->IsRightAngle(Angle))
// 		{
// 		  /** If Right Angle:  Add it to the pointSet*/
		  
// 		}
 	    }




	  ++itLinesListCur;
	}
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
  InputIndexType IndexBeginDst, IndexEndDst,IndexBeginSrc, IndexEndSrc;  
  
  /** Extract Indexes from the Src line for segment distance computation*/
  typename LineType::PointListType & pointsList = lineSrc->GetPoints();
  typename LineType::PointListType::const_iterator itPoints = pointsList.begin();
  
  IndexBeginSrc[0] = static_cast<unsigned int>((*itPoints).GetPosition()[0]);  
  IndexBeginSrc[1] = static_cast<unsigned int>((*itPoints).GetPosition()[1]);  
  ++itPoints;
  IndexEndSrc[0] = static_cast<unsigned int>((*itPoints).GetPosition()[0]);  
  IndexEndSrc[1] = static_cast<unsigned int>((*itPoints).GetPosition()[1]);  
 
  /** Extract Indexes from the Dst line to instantiate the line iterator*/
  typename LineType::PointListType &pointsListDst = lineDst->GetPoints();
  typename LineType::PointListType::const_iterator itPointsDst = pointsListDst.begin();

  float Xq1 = (*itPointsDst).GetPosition()[0];  //xq1
  float Yq1 = (*itPointsDst).GetPosition()[1];  //yq1
  ++itPointsDst;
  float Xq2 = (*itPointsDst).GetPosition()[0];  //xq2
  float Yq2 = (*itPointsDst).GetPosition()[1];  //yq2
  
  //This constants is used in the loop : let compute it once
  float SegmentLength = vcl_sqrt( (Xq1-Xq2)* (Xq1-Xq2) + (Yq1-Yq2) *(Yq1-Yq2) ) ;
  float CrossProduct  = Xq1*Yq2 -Xq2*Yq1;

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
	  
	  float CurDistance = Num/SegmentLength;

	  if(CurDistance -  MinDistance < 1e-10)
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
 */
template <class TImage, class TLinesList , class TPointSet>
float
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::ComputeAngleFormedBySegments(LineType * lineDst , LineType * lineSrc)
{
  
  
  
  
  return 0.;
}

/**
 * Method : ComputeDistanceBetweenSegments 
 */
template <class TImage, class TLinesList , class TPointSet>
bool
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::IsRightAngle(float Angle)
{
  return true;
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
