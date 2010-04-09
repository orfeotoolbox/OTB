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
#ifndef __otbPointSetDensityGaussianFunction_txx
#define __otbPointSetDensityGaussianFunction_txx

#include "otbPointSetDensityGaussianFunction.h"
#include "otbMath.h"

namespace otb
{
/**
 * Evaluate
 */
template <class TPointSet, class TOutput>
typename PointSetDensityGaussianFunction<TPointSet,   TOutput>
::OutputType
PointSetDensityGaussianFunction<TPointSet,   TOutput>
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

      accu += vcl_exp(-distsq / radiussq / 2);

      ++it;
      }
    accu /= vcl_sqrt(2 * CONST_PI * radiussq);
    }

  return accu;
}

/**
 * PrintSelf
 */
template <class TPointSet, class TOutput>
void
PointSetDensityGaussianFunction<TPointSet,   TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
