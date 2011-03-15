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
template <unsigned int Radius, class TSeries, class TDates, class TWeight = TSeries>
class SavitzkyGolayInterpolationFunctor
{
public:


  /// Constructor
  SavitzkyGolayInterpolationFunctor()
  {
  }
  /// Destructor
  virtual ~SavitzkyGolayInterpolationFunctor() {}

  inline void SetWeights(const TWeight weights)
  {
    for(unsigned int i=0; i<m_WeightSeries.Size(); ++i)
      m_WeightSeries[i] = weights[i];
  }

  inline void SetDates(const TDates doy)
  {
    for(unsigned int i=0; i<m_DoySeries.Size(); ++i)
      m_DoySeries[i] = doy[i];
  }


  inline TSeries operator ()(const TSeries& series)
  {

  }

private:

  TWeight m_WeightSeries;
  TDates m_DoySeries;
  
};
}
}
#endif
