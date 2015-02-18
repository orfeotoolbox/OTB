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

#ifndef __otbPathLengthFunctor_h
#define __otbPathLengthFunctor_h

#include "otbMath.h"

namespace otb
{

/** \class PathLengthFunctor
 *  \brief Select paths according to their length
 *
 * This functor gets the length of a path
 * and returns true if it is above the threshold, false otherwise.
 *
 *  \ingroup Functor
 *
 * \ingroup OTBPath
 */
template <class TInput1>
class PathLengthFunctor
{
public:

  void SetThreshold(double threshold)
  {
    m_Threshold = threshold;
  }
  double GetThreshold(void) const
  {
    return m_Threshold;
  }

  PathLengthFunctor()
  {
    m_Threshold = 10;
  }
  ~PathLengthFunctor() {}

  inline bool operator ()(const TInput1& input)
  {
    double length = input->GetLength();

    if (length > m_Threshold)
      {
      return true;
      }
    else
      {
      return false;
      }
  }

private:
  double m_Threshold;
};

}

#endif
