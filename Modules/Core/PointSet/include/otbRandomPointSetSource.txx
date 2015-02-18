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

#ifndef __otbRandomPointSetSource_txx
#define __otbRandomPointSetSource_txx

#include "otbRandomPointSetSource.h"

namespace otb
{

template<class TOutputPointSet>
RandomPointSetSource<TOutputPointSet>
::RandomPointSetSource()
{
  m_NumberOfPoints = 1;
  m_Generator = GeneratorType::New();
  m_Generator->Initialize();

  m_MinPoint.Fill(0.0);
  m_MaxPoint.Fill(1.0);
}

//init seed

template<class TOutputPointSet>
void
RandomPointSetSource<TOutputPointSet>
::GenerateData(void)
{
  OutputPointSetPointer outputPointSet = this->GetOutput();
  outputPointSet->Initialize();

  PointsContainerPointer outPoints = outputPointSet->GetPoints();
  outPoints->Reserve(m_NumberOfPoints);

  typename PointsContainerType::Iterator outputPoint = outPoints->Begin();
  while (outputPoint != outPoints->End())
    {
    PointType point;
    for (unsigned int i = 0; i < OutputPointSetType::PointDimension; ++i)
      {
      double v = m_Generator->GetVariateWithClosedRange(1.0);
      point[i] = (1.0 - v) * m_MinPoint[i] + v * m_MaxPoint[i];
      }
    outputPoint.Value() = point;
    ++outputPoint;

    }
}

}

#endif
