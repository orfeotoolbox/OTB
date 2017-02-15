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
#ifndef otbPointSetDensityEpanechnikovFunction_txx
#define otbPointSetDensityEpanechnikovFunction_txx

#include "otbPointSetDensityEpanechnikovFunction.h"
#include "otbMath.h"

namespace otb
{
/**
 * Evaluate
 */
template <class TPointSet, class TOutput>
typename PointSetDensityEpanechnikovFunction<TPointSet,   TOutput>
::OutputType
PointSetDensityEpanechnikovFunction<TPointSet,   TOutput>
::Evaluate(const InputType& input) const
{
  double accu = 0;
  double radiussq = m_Radius * m_Radius;

  if (this->GetPointSet()->GetNumberOfPoints() != 0)
    {
    typedef typename TPointSet::PointsContainer::ConstIterator iteratorType;
    iteratorType it = this->GetPointSet()->GetPoints()->Begin();

    while (it != this->GetPointSet()->GetPoints()->End())
      {
      float distX = input[0] - it.Value()[0];
      float distY = input[1] - it.Value()[1];
      float distsq = distX * distX + distY * distY;

      if (distsq < radiussq)
        {
        accu += 0.75 * (1 - distsq / radiussq);
        }
      ++it;
      }
    }

  return accu;
}

/**
 * PrintSelf
 */
template <class TPointSet, class TOutput>
void
PointSetDensityEpanechnikovFunction<TPointSet,   TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
