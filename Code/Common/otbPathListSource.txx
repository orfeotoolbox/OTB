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

#ifndef _otbPathListSource_txx
#define _otbPathListSource_txx
#include "otbPathListSource.h"

namespace otb
{

/**
 * Constructor
 */
template<class TOutputPath>
PathListSource<TOutputPath>
::PathListSource()
{
  this->Superclass::SetNumberOfRequiredOutputs(1);
  this->Superclass::SetNthOutput(0,ObjectList<TOutputPath>::New().GetPointer());
}
/**
 * Get the output path list
 * \return The output path list
 */
template<class TOutputPath>
typename PathListSource<TOutputPath>::OutputPathListType *
PathListSource<TOutputPath>
::GetOutput(void)
{
  if(this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<OutputPathListType *> (this->ProcessObject::GetOutput(0));
}

template<class TOutputPath>
void 
PathListSource<TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
