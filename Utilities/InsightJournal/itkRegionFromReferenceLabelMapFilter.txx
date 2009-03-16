/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionFromReferenceLabelMapFilter.txx,v $
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
#ifndef _itkRegionFromReferenceLabelMapFilter_txx
#define _itkRegionFromReferenceLabelMapFilter_txx
#include "itkRegionFromReferenceLabelMapFilter.h"


namespace itk
{


template <class TInputImage>
void 
RegionFromReferenceLabelMapFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->SetRegion( this->GetReferenceImage()->GetLargestPossibleRegion() );
  this->GetOutput()->SetLargestPossibleRegion( this->GetRegion() );
}


template <class TInputImage>
const typename RegionFromReferenceLabelMapFilter<TInputImage>::ReferenceImageType *
RegionFromReferenceLabelMapFilter<TInputImage>
::GetReferenceImage() const
{
  Self * surrogate = const_cast< Self * >( this );
  const ReferenceImageType * referenceImage = 
    static_cast<const ReferenceImageType *>(surrogate->ProcessObject::GetInput(1));
  return referenceImage;
}


} // end namespace itk

#endif
