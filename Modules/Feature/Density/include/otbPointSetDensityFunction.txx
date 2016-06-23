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
#ifndef otbPointSetDensityFunction_txx
#define otbPointSetDensityFunction_txx

#include "otbPointSetDensityFunction.h"
#include "otbMath.h"

namespace otb
{
/**
 * Evaluate
 */
template <class TPointSet, class TOutput>
typename PointSetDensityFunction<TPointSet,   TOutput>
::OutputType
PointSetDensityFunction<TPointSet,   TOutput>
::Evaluate(const InputType& input) const
{
  int    accu = 0;
  double surface = CONST_PI * m_Radius * m_Radius;

  if (this->GetPointSet()->GetNumberOfPoints() != 0)
    {
    typedef typename TPointSet::PointsContainer::ConstIterator iteratorType;
    iteratorType it = this->GetPointSet()->GetPoints()->Begin();

    while (it != this->GetPointSet()->GetPoints()->End())
      {
      float distX = input[0] - it.Value()[0];
      float distY = input[1] - it.Value()[1];
      float distsq = distX * distX + distY * distY;

      if (distsq <= m_Radius * m_Radius)
        {
        accu++;
        }

      ++it;
      }
    }

  return static_cast<float>(accu / surface);
}

/**
 * PrintSelf
 */
template <class TPointSet, class TOutput>
void
PointSetDensityFunction<TPointSet,   TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
