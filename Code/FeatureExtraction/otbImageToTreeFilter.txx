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

#ifndef __otbImageToTreeFilter_txx
#define __otbImageToTreeFilter_txx

#include "otbImageToTreeFilter.h"


namespace otb
{
/**
 *
 */
template <class TInputImage, class TOutputTree>
ImageToTreeFilter<TInputImage,TOutputTree>
::ImageToTreeFilter()
{
  this->itk::ProcessObject::SetNumberOfRequiredInputs(1);

  OutputTreePointer output
  = dynamic_cast<OutputTreeType*>(this->MakeOutput(0).GetPointer());

  this->itk::ProcessObject::SetNumberOfRequiredOutputs(1);
  this->itk::ProcessObject::SetNthOutput(0, output.GetPointer());

}

/**
 *
 */
template <class TInputImage, class TOutputTree>
ImageToTreeFilter<TInputImage,TOutputTree>
::~ImageToTreeFilter()
{
}


/**
 *   Make Output
 */
template <class TInputImage, class TOutputTree>
itk::DataObject::Pointer
ImageToTreeFilter<TInputImage,TOutputTree>
::MakeOutput(unsigned int)
{
  OutputTreePointer  outputTree = OutputTreeType::New();
  return dynamic_cast< DataObject *>( outputTree.GetPointer() );
}




/**
 *
 */
template <class TInputImage, class TOutputTree>
void
ImageToTreeFilter<TInputImage,TOutputTree>
::SetInput(const InputImageType *input)
{
  // process object is not const-correct, the const_cast
  // is required here.
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputImageType * >(input) );
}



/**
 *
 */
template <class TInputImage, class TOutputTree>
const typename ImageToTreeFilter<TInputImage,TOutputTree>::InputImageType *
ImageToTreeFilter<TInputImage,TOutputTree>
::GetInput(void)
{
  return dynamic_cast<const InputImageType*>
         (this->itk::ProcessObject::GetInput(0));
}


/**
 *
 */
template <class TInputImage, class TOutputTree>
typename ImageToTreeFilter<TInputImage,TOutputTree>::OutputTreeType *
ImageToTreeFilter<TInputImage,TOutputTree>
::GetOutput(void)
{
  return dynamic_cast<OutputTreeType*>
         (this->itk::ProcessObject::GetOutput(0));
}


/**
 *
 */
template <class TInputImage, class TOutputTree>
void
ImageToTreeFilter<TInputImage,TOutputTree>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}



/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template <class TInputImage, class TOutputTree>
void
ImageToTreeFilter<TInputImage,TOutputTree>
::GenerateOutputInformation()
{
}



} // end namespace otb

#endif
