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
#ifndef __otbMeanDifference_h
#define __otbMeanDifference_h

namespace otb
{

namespace Functor
{

template<class TInput1, class TInput2, class TOutput>
class MeanDifference
{
public:
  MeanDifference() {}
  virtual ~MeanDifference() {}
  inline TOutput operator ()(const TInput1& itA,
                             const TInput2& itB)
  {

    TOutput meanA = 0.0;
    TOutput meanB = 0.0;

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {

      meanA += static_cast<TOutput>(itA.GetPixel(pos));
      meanB += static_cast<TOutput>(itB.GetPixel(pos));

      }
    return static_cast<TOutput>((meanA - meanB) / itA.Size());
  }
};
}
}

#endif
