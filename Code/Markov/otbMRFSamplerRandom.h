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
#ifndef _MRFSamplerRandom_h
#define _MRFSamplerRandom_h

// #include "otbMRFEnergy.h"
#include "otbMRFSampler.h"

namespace otb
{
          /**
   * \class MRFSamplerRandom
   * \brief This is the base class for sampler methods used in the MRF framework.
   * 
   * This is one sampler to be used int he MRF framework. This sampler select the 
   * value randomly according to a uniform probability.
           * 
           */
          
    template< class TInput1, class TInput2>    
        class ITK_EXPORT MRFSamplerRandom : 
            public MRFSampler< TInput1, TInput2>
        {
          public:
            
            typedef MRFSamplerRandom Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self>  Pointer;
            typedef itk::SmartPointer<const Self>  ConstPointer;
            
            typedef itk::ConstNeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
            typedef itk::NeighborhoodIterator< TInput2 >  LabelledImageNeighborhoodIterator;
            typedef typename TInput2::PixelType LabelledImagePixelType;
            
//             typedef MRFEnergyFunctorBase<TInput1, TInput2> EnergyFidelityFunctorType;
//             typedef MRFEnergy<EnergyFidelityFunctorType>  EnergyFidelityType;
//             typedef MRFEnergyFunctorBase<TInput2, TInput2> EnergyRegularizationFunctorType;
//             typedef MRFEnergy<EnergyRegularizationFunctorType>  EnergyRegularizationType;
            typedef MRFEnergy<TInput1, TInput2>  EnergyFidelityType;
            typedef MRFEnergy<TInput2, TInput2>  EnergyRegularizationType;
            
            typedef typename EnergyFidelityType::Pointer EnergyFidelityPointer;
            typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;
                
            itkNewMacro(Self);
    
            itkTypeMacro(MRFSamplerRandom,Object);
            
//             itkSetMacro(NumberOfClasses, unsigned int);
//             itkGetMacro(NumberOfClasses, unsigned int);
//             
//             itkSetMacro(Lambda, double);
//             itkGetMacro(Lambda, double);
//             
//             itkGetMacro(DeltaEnergy, double);
//             itkGetMacro(Value, LabelledImagePixelType);
//             
//             itkSetObjectMacro( EnergyRegularization, EnergyRegularizationType);
//             itkSetObjectMacro( EnergyFidelity, EnergyFidelityType);
            
            inline int Compute( const InputImageNeighborhoodIterator & itData, 
                                   const LabelledImageNeighborhoodIterator & itRegul)             
            {
              this->m_EnergyBefore= this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel());
               this->m_EnergyBefore +=  this->m_Lambda * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());
              
              this->m_Value = static_cast<LabelledImagePixelType> (rand() % this->m_NumberOfClasses);
                  
              this->m_EnergyAfter = this->m_EnergyFidelity->GetValue(itData, this->m_Value);
              this->m_EnergyAfter +=  this->m_Lambda * this->m_EnergyRegularization->GetValue(itRegul, this->m_Value);
              
              this->m_DeltaEnergy=  this->m_EnergyAfter - this->m_EnergyBefore;
               
              return 0;
            }
            
            
//           private:
//             unsigned int m_NumberOfClasses;
//             double m_EnergyBefore;
//             double m_EnergyAfter;
//             double m_DeltaEnergy;
//             LabelledImagePixelType m_Value;
//             EnergyFidelityPointer  m_EnergyFidelity;
//             EnergyRegularizationPointer  m_EnergyRegularization;
//             double  m_Lambda;
            
            
          protected:
            // The constructor and destructor.
            MRFSamplerRandom() {}
            virtual ~MRFSamplerRandom() {}
    
        };
  
  
}

#endif
