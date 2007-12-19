
#ifndef _otbMRFEnergyPotts_h
#define _otbMRFEnergyPotts_h

#include "otbMRFEnergy.h"

namespace otb
{
  /**
   * \class MRFEnergyPotts
   * \brief This is the implementation of the Potts model for Markov classification.
   *
   * This is the implementation of the Potts model for Markov classification, to be used for
   * regularization. Energy is:
             * \f[  U(x_s,x_t) = -\beta \text{ if } x_s = x_t \f]
             * \f[  U(x_s,x_t) = +\beta \text{ if } x_s \neq x_t \f]
             * with
             * - \f$ x_s \f$ the label on site s
             * - \f$ y_s \f$ the value on the reference image
             * - \f$ \sigma^2_{x_s} \f$ the noise variance
   */
  
     template< class TInput1, class TInput2>    
                class ITK_EXPORT MRFEnergyPotts:public MRFEnergy< TInput1, TInput2>
    {
      public:
        typedef MRFEnergyPotts Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self>  Pointer;
        typedef itk::SmartPointer<const Self>  ConstPointer;
        
        typedef TInput1 InputImageType;
        typedef TInput2 LabelledImageType;
        typedef typename InputImageType::PixelType InputImagePixelType;
        typedef typename LabelledImageType::PixelType LabelledImagePixelType;
        
        typedef itk::Array< double > ParametersType;
    
        itkNewMacro(Self);
        
        itkTypeMacro(MRFEnergy,Object);
  
        
          
        double GetSingleValue(const InputImagePixelType & value1,  const LabelledImagePixelType & value2)   
        {
          if (value1 != value2)
          {
            return this->m_Parameters[0];
          }
          else 
          {
            return -this->m_Parameters[0];
          }
        }
     

    
      protected:
      // The constructor and destructor.
        MRFEnergyPotts() {
          this->m_NumberOfParameters = 1;
          this->m_Parameters.SetSize(this->m_NumberOfParameters);
          this->m_Parameters[0]=1.0;
        };
        virtual ~MRFEnergyPotts() {};

    };
}

#endif
