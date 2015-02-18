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
#ifndef __otbPathFunction_txx
#define __otbPathFunction_txx

#include "otbPathFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputPath, class TOutput>
PathFunction<TInputPath, TOutput>
::PathFunction()
{
  m_Path = NULL;
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputPath, class TOutput>
void
PathFunction<TInputPath, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "InputPath: " << m_Path.GetPointer() << std::endl;
}

/**
 * Initialize by setting the input image
 */
template <class TInputPath, class TOutput>
void
PathFunction<TInputPath, TOutput>
::SetInputPath(const InputPathType * ptr)
{
  // set the input path
  m_Path = ptr;
}

} // end namespace otb

#endif
