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
#ifndef __otbImageToPointSetFilter_txx
#define __otbImageToPointSetFilter_txx

#include "otbImageToPointSetFilter.h"


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::ImageToPointSetFilter()
{
  this->ProcessObjectType::SetNumberOfRequiredInputs(1);

  OutputPointSetPointer output
    = dynamic_cast<OutputPointSetType*>(this->MakeOutput(0).GetPointer());

  ProcessObjectType::SetNumberOfRequiredOutputs(1);
  ProcessObjectType::SetNthOutput(0, output.GetPointer());

}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::~ImageToPointSetFilter()
{
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::SetInput(unsigned int idx,const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(idx,
                                   const_cast< InputImageType * >(input) );
}
/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::SetInput(const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->ProcessObjectType::SetNthInput(0,
                                   const_cast< InputImageType * >(input) );
}



/**
 *
 */
template <class TInputImage, class TOutputPointSet>
const typename ImageToPointSetFilter<TInputImage,TOutputPointSet>::InputImageType *
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::GetInput(unsigned int idx)
{
    
  return dynamic_cast<const InputImageType*>
    (this->ProcessObjectType::GetInput(idx));
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
const typename ImageToPointSetFilter<TInputImage,TOutputPointSet>::InputImageType *
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::GetInput(void)
{
  if(this->GetNumberOfInputs() < 1)
    return 0;
  
  return dynamic_cast<const InputImageType*>
    (this->ProcessObjectType::GetInput(0));
}

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}



/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template <class TInputImage, class TOutputPointSet>
void
ImageToPointSetFilter<TInputImage,TOutputPointSet>
::GenerateOutputInformation()
{
}


} // end namespace otb

#endif
