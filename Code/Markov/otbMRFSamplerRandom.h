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

#include "otbMRFSampler.h"
#include "itkNumericTraits.h"

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
class ITK_EXPORT MRFSamplerRandom : public MRFSampler< TInput1, TInput2>
  {
  public:
    
    typedef MRFSamplerRandom               Self;
    typedef MRFSampler<TInput1, TInput2>   Superclass;
    typedef itk::SmartPointer<Self>        Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;
    
    typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
    typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
    typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
    typedef typename Superclass::InputImagePixelType               InputImagePixelType;
    typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
    typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
    typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
    typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;
    // Now use Superclass tepedefs
    //typedef itk::ConstNeighborhoodIterator< TInput1 > InputImageNeighborhoodIterator;
    //typedef itk::NeighborhoodIterator< TInput2 >      LabelledImageNeighborhoodIterator;
    //typedef typename TInput2::PixelType               LabelledImagePixelType;
    //typedef MRFEnergy<TInput1, TInput2>  EnergyFidelityType;
    //typedef MRFEnergy<TInput2, TInput2>  EnergyRegularizationType;
    //typedef typename EnergyFidelityType::Pointer                  EnergyFidelityPointer;
    //typedef typename EnergyRegularizationType::Pointer            EnergyRegularizationPointer;
    
    itkNewMacro(Self);
    
    itkTypeMacro(MRFSamplerRandom,MRFSampler);

    /** Store a value to be used instead of random value.. FOR TEST ONLY*/
    void SetValueInsteadRandom( LabelledImagePixelType val )
      {
	m_ValueInsteadRandom = val;
	std::cout<<"The m_ValueInsteadRandom varaible has to be used only for tests..."<<std::endl;
	this->Modified();
      };

    inline int Compute( const InputImageNeighborhoodIterator & itData, const LabelledImageNeighborhoodIterator & itRegul)
      {	
	this->SetEnergyBefore ( this->GetEnergyFidelity()->GetValue(itData, itRegul.GetCenterPixel()) );
	this->m_EnergyBefore +=  this->GetLambda() * this->GetEnergyRegularization()->GetValue(itRegul, itRegul.GetCenterPixel());
	LabelledImagePixelType val;
	if (m_ValueInsteadRandom == itk::NumericTraits<LabelledImagePixelType>::min())
	  {
	    val = static_cast<LabelledImagePixelType> (rand() % this->GetNumberOfClasses());
	  }
	else
	  {
	    val = m_ValueInsteadRandom; 
	  }

	this->SetValue( val );
	this->SetEnergyAfter ( this->GetEnergyFidelity()->GetValue(itData, this->GetValue()) );
	this->m_EnergyAfter +=  this->GetLambda() * this->GetEnergyRegularization()->GetValue(itRegul, this->GetValue());
	this->SetDeltaEnergy( this->GetEnergyAfter() - this->GetEnergyBefore() );

	return 0;
      }
    
  protected:
    // The constructor and destructor.
    MRFSamplerRandom() { m_ValueInsteadRandom=itk::NumericTraits<LabelledImagePixelType>::min(); }
    virtual ~MRFSamplerRandom() {}
    /** Store a value to be used instead of random value.. FOR TEST ONLY*/
    LabelledImagePixelType m_ValueInsteadRandom;
  }; 
}

#endif
