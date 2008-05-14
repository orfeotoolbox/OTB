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

#ifndef _MRFSamplerRandomMAP_h
#define _MRFSamplerRandomMAP_h

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
      class ITK_EXPORT MRFSamplerRandomMAP: public MRFSampler< TInput1, TInput2>
      {
        public:
            
          typedef MRFSamplerRandomMAP Self;
          typedef otb::MRFSampler< TInput1, TInput2> Superclass;
          typedef itk::SmartPointer<Self>  Pointer;
          typedef itk::SmartPointer<const Self>  ConstPointer;
            
          typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
          typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
          typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
          typedef typename Superclass::InputImagePixelType               InputImagePixelType;
          typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
          typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
          typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
          typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;     
          
          itkNewMacro(Self);
    
          itkTypeMacro(MRFSamplerRandomMAP,MRFSampler);
            
            
          void SetNumberOfClasses(const unsigned int nClasses)
          {
            if (nClasses != this->m_NumberOfClasses)
            {
              this->m_NumberOfClasses = nClasses;
              if (energy != NULL) free(energy);
              if (repartitionFunction != NULL) free(repartitionFunction);
              energy = (double *) calloc(this->m_NumberOfClasses, sizeof(double));
              repartitionFunction = (double *) calloc(this->m_NumberOfClasses, sizeof(double));
              this->Modified();
            }
          }
      
          inline int Compute( const InputImageNeighborhoodIterator & itData, 
                              const LabelledImageNeighborhoodIterator & itRegul)
          {
            
            this->m_EnergyBefore = this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel());
            this->m_EnergyBefore += this->m_Lambda 
                 * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());
              
              //Try all possible value (how to be generic ?)
            this->m_EnergyAfter = this->m_EnergyBefore; //default values to current one
            this->m_Value = itRegul.GetCenterPixel();
              
            //Compute probability for each possibility
            double totalProba=0.0;
            for (LabelledImagePixelType valueCurrent = 0; 
                 valueCurrent < static_cast<LabelledImagePixelType>(this->m_NumberOfClasses); ++valueCurrent)
            {
              this->m_EnergyCurrent = this->m_EnergyFidelity->GetValue(itData, valueCurrent);
              this->m_EnergyCurrent += this->m_Lambda 
                  * this->m_EnergyRegularization->GetValue(itRegul, valueCurrent);

              energy[static_cast<unsigned int>(valueCurrent)] = this->m_EnergyCurrent;
              repartitionFunction[static_cast<unsigned int>(valueCurrent)] = vcl_exp(-this->m_EnergyCurrent)+totalProba;
              totalProba = repartitionFunction[static_cast<unsigned int>(valueCurrent)];

            }
              
              //Pick a value according to probability
              
            double select = (rand()/(double(RAND_MAX)+1) * totalProba);
            
        /*** POUR EVITER LE SEGFAULT : on sort de la taille du tableau si la condition du break n'est pas respectee...*/
        /*
            LabelledImagePixelType valueCurrent = 0;
            for (valueCurrent = 0; 
                 valueCurrent < static_cast<LabelledImagePixelType>(this->m_NumberOfClasses); ++valueCurrent)
            {
              if (repartitionFunction[static_cast<unsigned int>(valueCurrent)] > select) break;
            }
              */
        unsigned int valueCurrent = 0;
        while( valueCurrent<this->GetNumberOfClasses() && repartitionFunction[valueCurrent] <= select)
        {
            valueCurrent++;
        }
   
        // TODO avoir la confirmation cnesienne : premier indince ou dernier
        if ( valueCurrent==this->GetNumberOfClasses() )
        {
            valueCurrent = 0;
        }
    
            if ( this->m_Value != static_cast<LabelledImagePixelType>(valueCurrent))
            {
              this->m_Value = static_cast<LabelledImagePixelType>(valueCurrent);
              this->m_EnergyAfter = energy[valueCurrent];
            }
                            
            this->m_DeltaEnergy=  this->m_EnergyAfter - this->m_EnergyBefore;
            
            return 0;
          }
            
        private:
          double * repartitionFunction;
          double * energy; 
            
        protected:
          // The constructor and destructor.
          MRFSamplerRandomMAP() {
          energy=NULL;
          repartitionFunction=NULL;
          };
          virtual ~MRFSamplerRandomMAP() {
            if (energy != NULL) free(energy);
            if (repartitionFunction != NULL) free(repartitionFunction);
          };
    
      };
  
  
}

#endif
