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
#ifndef __otbPathListToPathListFilter_txx
#define __otbPathListToPathListFilter_txx

#include "otbPathListToPathListFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath>
PathListToPathListFilter<TPath>
::PathListToPathListFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template <class TPath>
void
PathListToPathListFilter<TPath>
::SetInput(const PathListType * pathList)
{
  this->itk::ProcessObject::SetNthInput(0,const_cast<PathListType *>(pathList));
}

template <class TPath>
const typename PathListToPathListFilter<TPath>
::PathListType *
PathListToPathListFilter<TPath>
::GetInput(void)
{
  if(this->GetNumberOfInputs()<1)
    {
      return 0;
    }
  return static_cast<const PathListType *>(this->itk::ProcessObject::GetInput(0));
}
/**
 * PrintSelf Method
 */
template <class TPath>
void
PathListToPathListFilter<TPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
