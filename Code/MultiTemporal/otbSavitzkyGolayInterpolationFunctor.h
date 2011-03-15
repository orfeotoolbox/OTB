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
#ifndef __otbSavitzkyGolayInterpolationFunctor_h
#define __otbSavitzkyGolayInterpolationFunctor_h


namespace otb
{
namespace Functor
{
template <unsigned int Radius, class TSeries, class TDates>
class SavitzkyGolayInterpolationFunctor
{
public:


  /// Constructor
  SavitzkyGolayInterpolationFunctor()
  {
  }
  /// Destructor
  virtual ~SavitzkyGolayInterpolationFunctor() {}

  inline TSeries operator ()(const TSeries& series)
  {

  }

private:
  
};
}
}
#endif
