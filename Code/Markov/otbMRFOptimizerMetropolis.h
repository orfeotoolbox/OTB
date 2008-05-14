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
#ifndef _MRFOptimizerMetropolis_h
#define _MRFOptimizerMetropolis_h

#include "otbMRFOptimizer.h"
#include "otbMath.h"
#include "itkNumericTraits.h"

namespace otb
{
  /**
   * \class MRFOptimizerMetropolis
   * \brief This is the optimizer class implementing the Metropolis algorithm 
   *  
   * This is one optimizer to be used in the MRF framework. This optimizer 
   * follows the metropolis algorithm to accept of reject the value proposed by the sampler.
   *
   * The MRFOptimizerMetropolis has one parameter corresponding to the temperature T used
   * to accept or reject proposed values. The proposed value is accepted with a probability:
   *
   *  \f[ e^{\frac{-\Delta E}{T}} \f]
   * 
   */
  
class ITK_EXPORT MRFOptimizerMetropolis : public MRFOptimizer
  {
  public:
    
    typedef MRFOptimizerMetropolis Self;
    typedef MRFOptimizer Superclass;
    typedef itk::SmartPointer<Self>  Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;
    
    itkNewMacro(Self);
    
    itkTypeMacro(MRFOptimizerMetropolis,MRFOptimizer);
    
    inline bool Compute(double deltaEnergy)
      {
	if (deltaEnergy < 0)
	  {
	    return true;
	  }
	if (deltaEnergy == 0)
	  {
	    return false;
	  }
	else
              {
                double proba = vcl_exp(-(deltaEnergy)/this->m_Parameters[0]);
                if ( (rand() % 10000) < proba*10000)
		  {
                    std::cerr << "Opti true " << std::endl;
		    return true;
		  }
              }
	return false;
      }
    
    
  protected:
    MRFOptimizerMetropolis() {
      this->m_NumberOfParameters = 1;
      this->m_Parameters.SetSize(this->m_NumberOfParameters);
      this->m_Parameters[0]=1.0;
    }
    virtual ~MRFOptimizerMetropolis() {}
    double m_Temperature;
  };       
 
}

#endif

