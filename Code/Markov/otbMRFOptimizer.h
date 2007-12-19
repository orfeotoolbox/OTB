

#ifndef _MRFOptimizer_h
#define _MRFOptimizer_h

namespace otb
{
      /**
   * \class MRFOptimizer
   * \brief This is the base class for optimizer used in the MRF framework.
       */
  class ITK_EXPORT MRFOptimizer : 
      public itk::Object
        {       
          public:

            typedef MRFOptimizer Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self>  Pointer;
            typedef itk::SmartPointer<const Self>  ConstPointer;
            
            itkNewMacro(Self);
    
            itkTypeMacro(MRFOptimizer,Object);
            
            
            virtual bool Compute(double deltaEnergy)
            {
                return false;
            }
            

            
          protected:
            MRFOptimizer() {}
            virtual ~MRFOptimizer() {}
   
        };       
  
}

#endif

