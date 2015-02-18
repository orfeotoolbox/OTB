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
#ifndef __otbPointSetFunction_txx
#define __otbPointSetFunction_txx

#include "otbPointSetFunction.h"

#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TPointSet, class TOutput>
PointSetFunction<TPointSet,   TOutput>
::PointSetFunction()
{
  m_PointSet = PointSetType::New();
}

/**
 *
 */
template <class TPointSet, class TOutput>
void
PointSetFunction<TPointSet,   TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

// /**
//  * SetDetector method
//  */
// template <class TPointSet, class  TOutput >
// void
// PointSetFunction< TPointSet,   TOutput>
// ::SetPointSet(PointSetType* PointSet) const
// {
//   m_PointSet = PointSet;
// }

// /**
//  * GetDetector method
//  */
// template <class TPointSet, class  TOutput >
// const
// typename PointSetFunction< TPointSet, TOutput>
// ::PointSetType *
// PointSetFunction< TPointSet,   TOutput>
// ::GetPointSet() const
// {
//   return m_PointSet;
// }
} // end namespace otb

#endif
