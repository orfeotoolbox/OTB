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

#ifndef _MRFSamplerMAP_h
#define _MRFSamplerMAP_h

#include "otbMRFSampler.h"

namespace otb
{
  /**
   * \class MRFSamplerMAP
   * \brief This is the base class for sampler methods used in the MRF framework.
   * 
   * This is one sampler to be used int he MRF framework. This sampler select the 
   * value which maximizes the apriori probability (minimum energy).
   *
   */

  template< class TInput1, class TInput2>    
      class ITK_EXPORT MRFSamplerMAP : public MRFSampler< TInput1, TInput2>
      {
        public:
            
          typedef MRFSamplerMAP Self;
          typedef MRFSampler< TInput1, TInput2> Superclass;
          typedef itk::SmartPointer<Self>  Pointer;
          typedef itk::SmartPointer<const Self>  ConstPointer;
            
//           typedef itk::ConstNeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
//           typedef itk::NeighborhoodIterator< TInput2 >  LabelledImageNeighborhoodIterator;
//           typedef typename TInput2::PixelType LabelledImagePixelType;
//             
//           typedef MRFEnergy<TInput1, TInput2>  EnergyFidelityType;
//           typedef MRFEnergy<TInput2, TInput2>  EnergyRegularizationType;
//             
//           typedef typename EnergyFidelityType::Pointer EnergyFidelityPointer;
//           typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;
          
          typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
          typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
          typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
          typedef typename Superclass::InputImagePixelType               InputImagePixelType;
          typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
          typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
          typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
          typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;     
           
          itkNewMacro(Self);
    
          itkTypeMacro(MRFSamplerMAP,MRFSampler);
            
            
          inline int Compute( const InputImageNeighborhoodIterator & itData, 
                              const LabelledImageNeighborhoodIterator & itRegul)             
          {
            this->m_EnergyBefore=this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel());
            this->m_EnergyBefore  +=  this->m_Lambda 
                * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());
              
              //Try all possible value (how to be generic ?)
            this->m_EnergyAfter = this->m_EnergyBefore; //default values to current one
            this->m_Value = itRegul.GetCenterPixel();
//             for (LabelledImagePixelType valueCurrent = 0; 
//                  valueCurrent< this->m_NumberOfClasses; ++valueCurrent)
//             {
            LabelledImagePixelType valueCurrent = 0;
            while( valueCurrent<static_cast<LabelledImagePixelType>(this->GetNumberOfClasses()) && valueCurrent != itk::NumericTraits<LabelledImagePixelType>::max() )
            {
              this->m_EnergyCurrent = this->m_EnergyFidelity->GetValue(itData, valueCurrent);
              this->m_EnergyCurrent += this->m_Lambda 
                  * this->m_EnergyRegularization->GetValue(itRegul, valueCurrent);  
              if (this->m_EnergyCurrent < this->m_EnergyAfter)
              {
                this->m_EnergyAfter = this->m_EnergyCurrent;
                this->m_Value = valueCurrent;
              }
              valueCurrent++;
            }
              
            this->m_DeltaEnergy=  this->m_EnergyAfter - this->m_EnergyBefore;
               
            return 0;
          }
            
            
        protected:
            // The constructor and destructor.
          MRFSamplerMAP() {}
          virtual ~MRFSamplerMAP() {}
    
      };
  
  
}

#endif
