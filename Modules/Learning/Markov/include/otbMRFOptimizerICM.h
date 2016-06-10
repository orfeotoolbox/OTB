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

#ifndef __otbMRFOptimizerICM_h
#define __otbMRFOptimizerICM_h

#include "otbMRFOptimizer.h"

namespace otb
{
/**
 * \class MRFOptimizerICM
 * \brief This is the optimizer class implementing the ICM algorithm
 *
 * This is one optimizer to be used in the MRF framework. This optimizer
 * follows the ICM algorithm to accept of reject the value proposed by the sampler
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */
class ITK_EXPORT MRFOptimizerICM : public MRFOptimizer
{
public:

  typedef MRFOptimizerICM               Self;
  typedef MRFOptimizer                  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(MRFOptimizerICM, MRFOptimizer);

  inline bool Compute(double deltaEnergy) ITK_OVERRIDE
  {
    if (deltaEnergy < 0)
      {
      return true;
      }
    else
      {
      return false;
      }
  }

protected:
  MRFOptimizerICM() {}
  ~MRFOptimizerICM() ITK_OVERRIDE {}

};

}

#endif
