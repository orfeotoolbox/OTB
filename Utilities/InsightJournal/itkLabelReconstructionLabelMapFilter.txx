/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelReconstructionLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelReconstructionLabelMapFilter_txx
#define __itkLabelReconstructionLabelMapFilter_txx

#include "itkLabelReconstructionLabelMapFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage, class TMarkerImage, class TAttributeAccessor>
LabelReconstructionLabelMapFilter<TImage, TMarkerImage, TAttributeAccessor>
::LabelReconstructionLabelMapFilter()
{
  this->SetNumberOfRequiredInputs(2);
}


template <class TImage, class TMarkerImage, class TAttributeAccessor>
void
LabelReconstructionLabelMapFilter<TImage, TMarkerImage, TAttributeAccessor>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  AttributeAccessorType accessor;

  const MarkerImageType * maskImage = this->GetMarkerImage();
  const PixelType & label = labelObject->GetLabel();

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  // iterate over all the lines to find a pixel inside the object
  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    const IndexType & firstIdx = lit->GetIndex();
    unsigned long length = lit->GetLength();

    long endIdx0 = firstIdx[0] + length;
    for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++ )
      {
      const MarkerImagePixelType & v = maskImage->GetPixel( idx );
      if( v == label )
        {
        // keep the object
        accessor( labelObject, true );
        return;
        }
      }
    }

  // remove the object
  accessor( labelObject, false );

}


}// end namespace itk
#endif
