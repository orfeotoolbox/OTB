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
#ifndef _otbBinaryImageMinimalBoundingRegionCalculator_txx
#define _otbBinaryImageMinimalBoundingRegionCalculator_txx

#include "otbBinaryImageMinimalBoundingRegionCalculator.h"
#include "itkImageSliceConstIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/*
 * Constructor
 */
template <class TInputImage>
BinaryImageMinimalBoundingRegionCalculator<TInputImage>
::BinaryImageMinimalBoundingRegionCalculator() 
{
  // The pad option is desactivated by default
  m_Pad=0;
  // Set the default region
  typename InputImageType::SizeType size;
  typename InputImageType::IndexType index;
  size[0]=0;
  size[1]=0;
  index[0]=0;
  index[1]=0;
  m_Region.SetSize(size);
  m_Region.SetIndex(index);
  m_InsideValue=static_cast<PixelType>(255);
}
/**
 * Main computation method
 */
template <class TInputImage>
void
BinaryImageMinimalBoundingRegionCalculator<TInputImage>
::GenerateData(void)
{ 
    // Input images pointers
    const InputImageType* image = this->GetInput();
    // Iterator definition
    typedef itk::ImageSliceConstIteratorWithIndex<InputImageType> SliceIteratorType;
    // Indexes containing upper-left and lower-right corner
    typename InputImageType::IndexType min;
    typename InputImageType::IndexType max;
    min[0]=0;
    min[1]=0;
    max[1]=0;
    max[1]=0;
    for ( unsigned int axis = 0; axis < InputImageType::ImageDimension; axis++ )
      { // Create the forward iterator to find lower bound
	SliceIteratorType fit(image,image->GetLargestPossibleRegion());

	fit.SetFirstDirection( !axis );
	fit.SetSecondDirection( axis );
	fit.GoToBegin();
;
	// Walk through the two images line by line
	while (!fit.IsAtEnd())
	  {
	    while (!fit.IsAtEndOfSlice())
	      {
		while(!fit.IsAtEndOfLine())
		  {
		    // If a common intersection is found
		    if (fit.Get()==m_InsideValue)
		      {
			// then the lower bound is found
			min[axis]=fit.GetIndex()[axis];
			fit.GoToReverseBegin(); // skip to the end
			break;
		      }
		    ++fit;
		  }
		fit.NextLine();
	      }
	    fit.NextSlice();
	  }
	// Create the reverse iterator to find upper bound
	SliceIteratorType rit(image,image->GetLargestPossibleRegion());
	rit.SetFirstDirection(!axis);
	rit.SetSecondDirection(axis);
	rit.GoToReverseBegin();
	// Walk through the two images line by line
	while (!rit.IsAtReverseEnd())
	  {
	    while (!rit.IsAtReverseEndOfSlice())
	      {
		while (!rit.IsAtReverseEndOfLine())
		  {
		    // If a common intersection is found
		    if (rit.Get()==m_InsideValue)
		      {
			max[axis]=rit.GetIndex()[axis];
			rit.GoToBegin(); //Skip to reverse end
			break;
		      }
		    --rit;

		  }
		rit.PreviousLine();

	      }
	    rit.PreviousSlice();
	  }
      }
    
    // Compute size and index of the region
    typename InputImageType::SizeType size;
    typename InputImageType::IndexType index;
    RegionType maxRegion = image->GetLargestPossibleRegion();
    // If the pad option is activated
    if(m_Pad>0)
      {
	for(int i=0;i<InputImageType::ImageDimension;i++)
	  {
	    // If we are not on boundary case, we can do what we want
	    if((int)(min[i]-m_Pad)> maxRegion.GetIndex()[i])
	      {
		index[i]= min[i]-m_Pad;
	      }
	    // else we are at beginning of the image, so don't pad
	    else
	      {
		index[i]= maxRegion.GetIndex()[i]; 
	      }
	    // If are not on boundary case, we can pad the size
	    if (index[i]+max[i]-min[i]+2*m_Pad+1<=maxRegion.GetIndex()[i]+maxRegion.GetSize()[i])
	      {
		size[i]=max[i]-min[i]+2*m_Pad+1;
	      }
	    // Else we must restrain ourselves to the image boundaries
	    else 
	      {
		size[i]=maxRegion.GetSize()[i]+maxRegion.GetIndex()[i]
		  -max[i]+min[i]-index[i];
	      }
	  }
      }
    else
      // If the pad option is not activated, the result is simple
      {
	for(int i=0;i<InputImageType::ImageDimension;i++)
	  {
	    size[i]=max[i]-min[i]+1;
	    index[i]=min[i];
	  }
      }
    // otbMsgDebugMacro(<<"BinaryImageMinimalBoundingBoxCalculator: index "<<index);
    // otbMsgDebugMacro(<<"BinaryImageMinimalBoundingBoxCalculator: size "<<size);
    // Set the size and index of the output region
    m_Region.SetIndex(index);
    m_Region.SetSize(size);
}
/**
 * PrintSelf method
 */
template <class TInputImage>
void
BinaryImageMinimalBoundingRegionCalculator<TInputImage>
::PrintSelf( std::ostream& os,itk::Indent indent ) const
  { 
    Superclass::PrintSelf(os,indent);
  }

} // End namespace otb

#endif
