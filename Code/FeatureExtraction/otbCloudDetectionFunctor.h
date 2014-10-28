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
#ifndef __otbCloudDetectionFunctor_h
#define __otbCloudDetectionFunctor_h

#include "otbCloudEstimatorFunctor.h"

namespace otb
{
/** \class CloudDetectionFunctor
 *  \brief This functor first uses CloudEstimatorFunctor
 *  \brief And then binarise the image with two thresholds
 */
namespace Functor
{
template<class TInput, class TOutputValue>
class CloudDetectionFunctor
{
public:
  typedef CloudEstimatorFunctor<TInput, TOutputValue> CloudEstimatorFunctorType;

  CloudDetectionFunctor()
  {
    m_MinThreshold = 0.0;
    m_MaxThreshold = 1.0;
  }

  virtual ~CloudDetectionFunctor() {}
  inline TOutputValue operator ()(const TInput& inPix)
  {
    if ((m_CloudEstimatorFunctor(inPix) > m_MinThreshold) && (m_CloudEstimatorFunctor(inPix) <= m_MaxThreshold))
      {
      return 1;
      }
    else
      {
      return 0;
      }
  }

  void SetReferencePixel(TInput ref)
  {
    m_CloudEstimatorFunctor.SetReferencePixel(ref);
  }
  void SetVariance(double variance)
  {
    m_CloudEstimatorFunctor.SetVariance(variance);
  }
  void SetMinThreshold(double threshold)
  {
    m_MinThreshold = threshold;
  }
  void SetMaxThreshold(double threshold)
  {
    m_MaxThreshold = threshold;
  }
  double GetMinThreshold()
  {
    return m_MinThreshold;
  }
  double GetMaxThreshold()
  {
    return m_MaxThreshold;
  }
  double GetVariance()
  {
    return m_CloudEstimatorFunctor.GetVariance();
  }
  TInput GetReferencePixel()
  {
    return m_CloudEstimatorFunctor.GetReferencePixel();
  }

protected:
  CloudEstimatorFunctorType m_CloudEstimatorFunctor;
  double                    m_MinThreshold;
  double                    m_MaxThreshold;

};

} // end namespace functor
} // end namespace otb

#endif
