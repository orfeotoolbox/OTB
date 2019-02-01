/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbDataNodeVectorDataFunction_hxx
#define otbDataNodeVectorDataFunction_hxx

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
  m_VectorData = nullptr;
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
