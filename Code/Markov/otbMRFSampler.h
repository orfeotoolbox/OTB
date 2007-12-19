
#ifndef _MRFSampler_h
#define _MRFSampler_h

#include "otbMRFEnergy.h"
// #include "otbMRFEnergyFunctorBase.h"
#include "itkNeighborhoodIterator.h"

namespace otb
{
    /**
   * \class MRFSampler
   * \brief This is the base class for sampler methods used in the MRF framework.
     */
    template< class TInput1, class TInput2>    
        class ITK_EXPORT MRFSampler:public itk::Object
        {
          public:
            
            typedef MRFSampler Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self>  Pointer;
            typedef itk::SmartPointer<const Self>  ConstPointer;
            
            typedef itk::ConstNeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
//             typedef itk::NeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
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
    
            itkTypeMacro(MRFSampler,Object);
            
            itkSetMacro(NumberOfClasses, unsigned int);
            itkGetMacro(NumberOfClasses, unsigned int);
            
            itkSetMacro(Lambda, double);
            itkGetMacro(Lambda, double);
            
            itkGetMacro(DeltaEnergy, double);
            itkGetMacro(Value, LabelledImagePixelType);
            
            itkSetObjectMacro( EnergyRegularization, EnergyRegularizationType);
            itkSetObjectMacro( EnergyFidelity, EnergyFidelityType);
            
            virtual int Compute( const InputImageNeighborhoodIterator & itData, 
                                   const LabelledImageNeighborhoodIterator & itRegul)             
            {
              
              m_DeltaEnergy=  0;
               
              return 0;
            }
            
            
          protected:
            unsigned int m_NumberOfClasses;
            double m_EnergyBefore;
            double m_EnergyAfter;
            double m_DeltaEnergy;
            LabelledImagePixelType m_Value;
            EnergyRegularizationPointer  m_EnergyRegularization;
            EnergyFidelityPointer  m_EnergyFidelity;
            LabelledImagePixelType m_ValueCurrent;
            double  m_EnergyCurrent;      
            double  m_Lambda;
            
            
          protected:
            // The constructor and destructor.
            MRFSampler() {}
            virtual ~MRFSampler() {}
    
        };
  
  
}

#endif
