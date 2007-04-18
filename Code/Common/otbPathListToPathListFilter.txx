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
#ifndef _otbPathListToPathListFilter_txx
#define _otbPathListToPathListFilter_txx

#include "otbPathListToPathListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputPathList, class TOutputPathList>
PathListToPathListFilter<TInputPathList,TOutputPathList>
::PathListToPathListFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TInputPathList, class TOutputPathList>
void
PathListToPathListFilter<TInputPathList,TOutputPathList>
::SetInput(const InputPathListType * pathList)
{
  this->itk::ProcessObject::SetNthInput(0,const_cast<InputPathListType *>(pathList));
}

template <class TInputPathList, class TOutputPathList>
const typename PathListToPathListFilter<TInputPathList,TOutputPathList>
::InputPathListType *
PathListToPathListFilter<TInputPathList,TOutputPathList>
::GetInput(void)
{
  if(this->GetNumberOfInputs()<1)
    {
      return 0;
    }
  return static_cast<const InputPathListType *>(this->itk::ProcessObject::GetInput(0));
}
/**
 * PrintSelf Method
 */
template <class TInputPathList, class TOutputPathList>
void
PathListToPathListFilter<TInputPathList,TOutputPathList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
