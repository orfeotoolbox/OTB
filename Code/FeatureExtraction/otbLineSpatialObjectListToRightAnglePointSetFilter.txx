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
  
  m_OutputPointSet = OutputPointSetType::New();
  
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
  /** Get The input Lines*/
  typename InputLinesListType::Pointer inputLinesList = const_cast<InputLinesListType *>(this->GetInput());
    
  /** Loop to get all the lines in the listLine */
  InputLinesListTypeIterator            itLinesListTest  = inputLinesList->begin();   /** Current tested Line */
  InputLinesListTypeIterator            itLinesListCur  = inputLinesList->begin();    /** Line tested with with current Line*/
  InputLinesListTypeIterator            itLinesListEnd  = inputLinesList->end();
  
  while(itLinesListTest != itLinesListEnd )
    {
      while(itLinesListCur != itLinesListEnd)
	{
	  /** Compute the distance from CurLine to DstLine*/
	  double SegmentDist = this->ComputeDistanceBetweenSegments(*itLinesListTest , *itLinesListCur);
	  
	  /** Check if the distance separating the segments is under the threshold*/
	  if(SegmentDist < 5. /* Threshold : 5 Pixels*/)
	    {
	      /** Compute the angle formed by the two segments */
	      float Angle = this->ComputeAngleFormedBySegments(*itLinesListTest, *itLinesListCur);
	      
	      /** Check if the angle is a right one */
	      if(this->IsRightAngle(Angle))
		{
		  /** If Right Angle:  Add it to the pointSet*/
		  
		}
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
double
LineSpatialObjectListToRightAnglePointSetFilter<TImage,TLinesList ,TPointSet>
::ComputeDistanceBetweenSegments(LineType * lineDst , LineType * lineSrc)
{
  /** Extract the begining and the */
  
  
  /** Define a line iterator */
  //itk::LineIterator<InputImageType> itLine(this->GetInputImage() , indexBegin , indexEnd);
  
  return 0.;
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
