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
#ifndef __otbJoinHistogramMI_h
#define __otbJoinHistogramMI_h

#include "itkHistogram.h"

namespace otb
{

namespace Functor
{

template<class TInput1, class TInput2, class TOutput>
class JoinHistogramMI
{
public:
  typedef double                                                         HistogramFrequencyType;
  typedef typename itk::Statistics::Histogram<HistogramFrequencyType,
                             itk::Statistics::DenseFrequencyContainer2 > HistogramType;
  JoinHistogramMI() {}
  virtual ~JoinHistogramMI() {}
  inline TOutput operator ()(const TInput1& itA,
                             const TInput2& itB, const HistogramType* histogram)
  {
    TOutput                jointEntropy = itk::NumericTraits<TOutput>::Zero;
    HistogramFrequencyType totalFreq = histogram->GetTotalFrequency();

    typename HistogramType::MeasurementVectorType sample(2);
    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {
      double valueA = static_cast<double>(itA.GetPixel(pos));
      double valueB = static_cast<double>(itB.GetPixel(pos));

      sample[0] = valueA;
      sample[1] = valueB;

      typename HistogramType::IndexType index;
      histogram->GetIndex(sample, index);
      HistogramFrequencyType freq = histogram->GetFrequency(index);
      if (freq > 0)
        {
        jointEntropy += freq * vcl_log(freq);
        }

      }

    jointEntropy = -jointEntropy / static_cast<TOutput>(totalFreq) +
                   vcl_log(totalFreq);

    return jointEntropy;
  }

};
}
} // end namespace otb

#endif
