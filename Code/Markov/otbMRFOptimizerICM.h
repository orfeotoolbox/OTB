

#ifndef _MRFOptimizerICM_h
#define _MRFOptimizerICM_h

#include "otbMRFOptimizer.h"

namespace otb
{
              /**
   * \class MRFOptimizerICM
   * \brief This is the optimizer class implementing the ICM algorithm 
   *  
   * This is one optimizer to be used in the MRF framework. This optimizer 
   * follows the ICM algorithm to accept of reject the value proposed by the sampler
               */
  class ITK_EXPORT MRFOptimizerICM : 
      public MRFOptimizer
      {       
        public:

          typedef MRFOptimizerICM Self;
          typedef itk::Object Superclass;
          typedef itk::SmartPointer<Self>  Pointer;
          typedef itk::SmartPointer<const Self>  ConstPointer;
            
          itkNewMacro(Self);
    
          itkTypeMacro(MRFOptimizerICM,Object);
            
            
          inline bool operator()(double deltaEnergy)
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
          virtual ~MRFOptimizerICM() {}
  
      };       
  
}

#endif

