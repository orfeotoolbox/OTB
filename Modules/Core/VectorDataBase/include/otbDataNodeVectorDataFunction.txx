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
#ifndef otbDataNodeVectorDataFunction_txx
#define otbDataNodeVectorDataFunction_txx

#include "otbDataNodeVectorDataFunction.h"

namespace otb
{

/**
 * Constructor
 */
template <class TOutput, class TCoordRep, class TPrecision>
DataNodeVectorDataFunction<TOutput, TCoordRep, TPrecision>
::DataNodeVectorDataFunction()
{
  m_VectorData = NULL;
}


/**
 * Standard "PrintSelf" method
 */
template <class TOutput, class TCoordRep, class TPrecision>
void
DataNodeVectorDataFunction<TOutput, TCoordRep, TPrecision>
::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "VetorData: " << m_VectorData.GetPointer() << std::endl;
}


/**
 * Initialize by setting the input VectorData
 */
template <class TOutput, class TCoordRep, class TPrecision>
void
DataNodeVectorDataFunction<TOutput, TCoordRep, TPrecision>
::SetInputVectorData(
  const VectorDataType * ptr )
{
  // set the input VectorData
  m_VectorData = ptr;
}

} // end namespace otb

#endif
