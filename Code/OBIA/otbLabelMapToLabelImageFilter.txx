/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToLabelImageFilter.txx, v $
  Language:  C++
  Date:      $Date: 2009-05-16 22:19:31 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLabelMapToLabelImageFilter_txx
#define __otbLabelMapToLabelImageFilter_txx

#include "otbLabelMapToLabelImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb {

template <class TInputImage, class TOutputImage>
LabelMapToLabelImageFilter<TInputImage, TOutputImage>
::LabelMapToLabelImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
LabelMapToLabelImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{


  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
      {
       InputImagePointer input = const_cast<InputImageType *>(this->GetInput(idx));
      if (!input.IsNull())
        {
        input->SetRequestedRegionToLargestPossibleRegion();
        // Check whether the input is an image of the appropriate
        // dimension (use ProcessObject's version of the GetInput()
        // method since it returns the input as a pointer to a
        // DataObject as opposed to the subclass version which
        // static_casts the input to an TInputImage).

        // Use the function object RegionCopier to copy the output region
        // to the input.  The default region copier has default implementations
        // to handle the cases where the input and output are the same
        // dimension, the input a higher dimension than the output, and the
        // input a lower dimension than the output.
        InputImageRegionType inputRegion;
        this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
        input->SetRequestedRegion( inputRegion );
        }
      }


}


}// end namespace otb
#endif
