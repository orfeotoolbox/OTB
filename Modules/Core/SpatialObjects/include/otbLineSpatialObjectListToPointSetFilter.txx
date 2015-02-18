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
#ifndef __otbLineSpatialObjectListToPointSetFilter_txx
#define __otbLineSpatialObjectListToPointSetFilter_txx

#include "otbLineSpatialObjectListToPointSetFilter.h"

namespace otb
{

/**
 *
 */
template<class TLinesList, class TPointSet>
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>::LineSpatialObjectListToPointSetFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->SetNthOutput(0, PointSetType::New());

}

template<class TLinesList, class TPointSet>
void
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::SetInput(const LinesListType *list)
{
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<LinesListType  *>(list));
}

template<class TLinesList, class TPointSet>
const typename LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>::LinesListType*
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::GetInput(void)
{
  return static_cast<const LinesListType *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TLinesList, class TPointSet>
void
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::SetOutput(const PointSetType *pointSet)
{
  this->ProcessObjectType::SetNthOutput(0,
                                        const_cast<PointSetType *>(pointSet));
}

template<class TLinesList, class TPointSet>
typename LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>::PointSetType*
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::GetOutput(void)
{
  return static_cast<PointSetType *>
           (this->ProcessObjectType::GetOutput(0));
}

/**
 *
 */

template<class TLinesList, class TPointSet>
void
LineSpatialObjectListToPointSetFilter<TLinesList, TPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
