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
#ifndef __otbPointSetDensityFunction_txx
#define __otbPointSetDensityFunction_txx

#include "otbPointSetDensityFunction.h"
#include "otbImage.h"
#include "otbMath.h"


namespace otb
{

/**
 * Constructor
 */
template <class TPointSet, class  TOutput >
PointSetDensityFunction< TPointSet,   TOutput>
::PointSetDensityFunction()
{
  m_Radius = 1;
}


/**
 *
 */
template <class TPointSet, class  TOutput >
typename PointSetDensityFunction< TPointSet,   TOutput>
::OutputType
PointSetDensityFunction< TPointSet,   TOutput>
::Evaluate(const  InputType& input ) const
{

  typename otb::Image<OutputType,2>::IndexType  index;
  index[0] = input[0];
  index[1] = input[1];

  int accu = 0;
  double surface = M_PI*vcl_pow(2.,static_cast<double>(m_Radius));

  if (this->GetPointSet()->GetNumberOfPoints() != 0)
  {
    typedef typename TPointSet::PointsContainer::ConstIterator     iteratorType;
    iteratorType it = this->GetPointSet()->GetPoints()->Begin();

    while ( it != this->GetPointSet()->GetPoints()->End())
    {
      float distX2 =( index[0]-it.Value()[0])*( index[0]-it.Value()[0]);
      float distY2 =( index[1]-it.Value()[1])*( index[1]-it.Value()[1]);
      float dist = vcl_sqrt(distX2 + distY2);

      if (dist <= m_Radius)
        accu++;

      ++it;
    }
  }
  else
    return 0.;

  return static_cast<float>(accu/surface);
}


/**
 *
 */
template <class TPointSet, class  TOutput >
void
PointSetDensityFunction< TPointSet,   TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
}


} // end namespace otb

#endif
