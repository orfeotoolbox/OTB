/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCropLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/07/11 14:56:39 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkCropLabelMapFilter_txx
#define _itkCropLabelMapFilter_txx
#include "itkCropLabelMapFilter.h"


namespace itk
{


template <class TInputImage>
void 
CropLabelMapFilter<TInputImage>
::GenerateOutputInformation()
{
  const TInputImage * inputPtr = this->GetInput();
  if( !inputPtr )
    {
    return;
    }
  
  // Compute the new region size.
  RegionType croppedRegion;
  SizeType   sz;
  IndexType   idx;

  SizeType input_sz =
    inputPtr->GetLargestPossibleRegion().GetSize();
  IndexType input_idx =
    inputPtr->GetLargestPossibleRegion().GetIndex();
  
  idx = input_idx + m_LowerBoundaryCropSize; 
  sz  = input_sz  - (m_UpperBoundaryCropSize + m_LowerBoundaryCropSize); 

  croppedRegion.SetSize(sz);
  croppedRegion.SetIndex(idx);
  
  // Set extraction region in the superclass.
  this->SetRegion(croppedRegion);

  // 
  Superclass::GenerateOutputInformation();
}


template <class TInputImage>
void 
CropLabelMapFilter<TInputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "UpperBoundaryCropSize: " << m_UpperBoundaryCropSize <<
    std::endl;
  os << indent << "LowerBoundaryCropSize: " << m_LowerBoundaryCropSize <<
    std::endl;
}

} // end namespace itk

#endif
