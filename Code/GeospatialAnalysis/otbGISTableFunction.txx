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
#ifndef __otbGISTableFunction_txx
#define __otbGISTableFunction_txx

#include "otbGISTableFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputTable, class TOutput>
GISTableFunction<TInputTable, TOutput>
::GISTableFunction()
{
  m_Table = NULL;
}


/**
 * Standard "PrintSelf" method
 */
template <class TInputTable, class TOutput>
void
GISTableFunction<TInputTable, TOutput>
::PrintSelf(
  std::ostream& os,
  Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "InputTable: " << m_Table.GetPointer() << std::endl;

}


/**
 * Initialize by setting the input table
 */
template <class TInputTable, class TOutput>
void
GISTableFunction<TInputTable, TOutput>
::SetInputTable(
  const InputTableType* ptr )
{
  // set the input table
  m_Table = ptr;

}


} // end namespace itk

#endif
