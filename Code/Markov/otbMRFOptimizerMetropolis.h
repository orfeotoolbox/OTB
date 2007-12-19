

#ifndef _MRFOptimizerMetropolis_h
#define _MRFOptimizerMetropolis_h

#include "otbMRFOptimizer.h"

namespace otb
{
  class ITK_EXPORT MRFOptimizerMetropolis :
      public MRFOptimizer
        {       
          public:

            typedef MRFOptimizerMetropolis Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self>  Pointer;
            typedef itk::SmartPointer<const Self>  ConstPointer;
            
            itkNewMacro(Self);
    
            itkTypeMacro(MRFOptimizerMetropolis,Object);
            
            itkSetMacro(Temperature, double);
            itkGetMacro(Temperature, double);
            
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
                double proba = vcl_exp(-(deltaEnergy)/m_Temperature);
                if ( (rand() % 10000) < proba*10000)
                {
                  return true;
                }
              }
              return false;
            }
            
            
          protected:
            MRFOptimizerMetropolis() {}
            virtual ~MRFOptimizerMetropolis() {}
            double m_Temperature;
        };       
  
}

#endif

