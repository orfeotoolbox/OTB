
#ifndef _MRFSamplerRandomMAP_h
#define _MRFSamplerRandomMAP_h

// #include "otbMRFEnergy.h"
#include "otbMRFSampler.h"

namespace otb
{
        /**
        * \class MRFSamplerRandomMAP
        * \brief This is the base class for sampler methods used in the MRF framework.
        * 
        * This is one sampler to be used int he MRF framework. This sampler select the 
        * value randomly according to the apriori probability.
        * 
        * The probability is defined from the energy as:
        * 
        *  \f[ P(X=x)= \frac{1}{Z} \exp^{-U(x)}  \f]
        * 
        * where \f$ Z = \sum_x \exp^{-U(x)}\f$
        * 
        */

    template< class TInput1, class TInput2>    
        class ITK_EXPORT MRFSamplerRandomMAP :
            public MRFSampler< TInput1, TInput2>
        {
          public:
            
            typedef MRFSamplerRandomMAP Self;
            typedef itk::Object Superclass;
            typedef itk::SmartPointer<Self>  Pointer;
            typedef itk::SmartPointer<const Self>  ConstPointer;
            
            typedef itk::ConstNeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
//             typedef itk::NeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
            typedef itk::NeighborhoodIterator< TInput2 >  LabelledImageNeighborhoodIterator;
            typedef typename TInput2::PixelType LabelledImagePixelType;
            
            typedef MRFEnergy<TInput1, TInput2>  EnergyFidelityType;
            typedef MRFEnergy<TInput2, TInput2>  EnergyRegularizationType;
            
            typedef typename EnergyFidelityType::Pointer EnergyFidelityPointer;
            typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;
                
            itkNewMacro(Self);
    
            itkTypeMacro(MRFSamplerRandomMAP,Object);
            
//             void SetNumberOfClasses(unsigned int arg) 
//             { 
//               otbDebugMacro("setting NumberOfClasses to " << arg); 
//               if (this->m_NumberOfClasses != arg) 
//               {
//                 this->m_NumberOfClasses = arg;
//                 repartitionFunction = (double *) calloc(m_NumberOfClasses, sizeof(double));
//                 energy = (double *) calloc(m_NumberOfClasses, sizeof(double));
//                 this->Modified();
//               } 
//             }
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
              this->m_EnergyBefore=this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel())
                  +  this->m_Lambda * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());
              
              //Try all possible value (how to be generic ?)
              this->m_EnergyAfter = this->m_EnergyBefore; //default values to current one
              this->m_Value = itRegul.GetCenterPixel();
//               otbDebugMacro(<< "Computing MAP for pix " << itData.GetIndex());
              //Compute probability for each possibility
              double totalProba=0.0;
              for (LabelledImagePixelType valueCurrent = 0; 
                   valueCurrent < this->m_NumberOfClasses; ++valueCurrent)
              {
//                     otbDebugMacro(<< " --> Proposed value " << static_cast<double>(valueCurrent)); 
                this->m_EnergyCurrent = this->m_EnergyFidelity->GetValue(itData, valueCurrent)
                    + this->m_Lambda * this->m_EnergyRegularization->GetValue(itRegul, valueCurrent);

                energy[valueCurrent] = this->m_EnergyCurrent;
                repartitionFunction[valueCurrent] = vcl_exp(-this->m_EnergyCurrent)+totalProba;
                totalProba = repartitionFunction[valueCurrent];
//                 otbDebugMacro("valueCurrent, repartitionFunction[valueCurrent] " << (unsigned int)  valueCurrent << ", " << repartitionFunction[valueCurrent]);

              }
              
              //Pick a value according to probability
              
              double select = (rand()/(double(RAND_MAX)+1) * totalProba);
//               otbDebugMacro("repartitionFunction " <<  repartitionFunction[0] << " " 
//                   <<  repartitionFunction[1] << " " <<  repartitionFunction[2] << " " 
//                   <<  repartitionFunction[3] << " ");
              
//               otbDebugMacro("select, totalProba " <<  select << ", " << totalProba);
              LabelledImagePixelType valueCurrent = 0;
              for (valueCurrent = 0; 
                   valueCurrent < this->m_NumberOfClasses; ++valueCurrent)
              {
                if (repartitionFunction[valueCurrent] > select) break;
              }
              
              if ( this->m_Value != valueCurrent)
              {
                this->m_Value = valueCurrent;
                this->m_EnergyAfter = energy[valueCurrent];
              }
                            
              this->m_DeltaEnergy=  this->m_EnergyAfter - this->m_EnergyBefore;
//               otbDebugMacro("Decision " << (unsigned int) valueCurrent);
              return 0;
            }
            
            
          private:
//             unsigned int m_NumberOfClasses;
//             double m_EnergyBefore;
//             double m_EnergyAfter;
//             double m_DeltaEnergy;
//             LabelledImagePixelType m_Value;
//             EnergyRegularizationPointer  m_EnergyRegularization;
//             EnergyFidelityPointer  m_EnergyFidelity;
//             LabelledImagePixelType valueCurrent;
//             double  m_EnergyCurrent;  
            double * repartitionFunction ;   
            double * energy; 
//             double  m_Lambda;
            
            
          protected:
            // The constructor and destructor.
            MRFSamplerRandomMAP() {
              energy = (double *) calloc(this->m_NumberOfClasses, sizeof(double));
              repartitionFunction = (double *) calloc(this->m_NumberOfClasses, sizeof(double));
            }
            virtual ~MRFSamplerRandomMAP() {}
    
        };
  
  
}

#endif
