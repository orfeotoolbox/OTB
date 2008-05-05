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
class ITK_EXPORT MRFSamplerRandomMAP : public MRFSampler< TInput1, TInput2>
  {
  public:
    
    typedef MRFSamplerRandomMAP           Self;
    typedef MRFSampler< TInput1, TInput2> Superclass;
    typedef itk::SmartPointer<Self>       Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
    
    typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
    typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
    typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
    typedef typename Superclass::InputImagePixelType               InputImagePixelType;
    typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
    typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
    typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
    typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;
  
    /*
    typedef itk::ConstNeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
    //typedef itk::NeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
    typedef itk::NeighborhoodIterator< TInput2 >       LabelledImageNeighborhoodIterator;
    typedef typename TInput2::PixelType                LabelledImagePixelType;
    
    typedef MRFEnergy<TInput1, TInput2>                EnergyFidelityType;
    typedef MRFEnergy<TInput2, TInput2>                EnergyRegularizationType;
    typedef typename EnergyFidelityType::Pointer       EnergyFidelityPointer;
    typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;
    */
    itkNewMacro(Self);
    
    itkTypeMacro(MRFSamplerRandomMAP, MRFSampler);
    
    /*  
	void SetNumberOfClasses(unsigned int arg) 
	{ 
        otbDebugMacro("setting NumberOfClasses to " << arg); 
        if (this->m_NumberOfClasses != arg) 
	{
	this->m_NumberOfClasses = arg;
	m_RepartitionFunction = (double *) calloc(m_NumberOfClasses, sizeof(double));
	m_Energy = (double *) calloc(m_NumberOfClasses, sizeof(double));
	this->Modified();
	} 
	}
	itkGetMacro(NumberOfClasses, unsigned int);
	
	itkSetMacro(Lambda, double);
	itkGetMacro(Lambda, double);
	
	itkGetMacro(DeltaEnergy, double);
	itkGetMacro(Value, LabelledImagePixelType);
	
	itkSetObjectMacro( EnergyRegularization, EnergyRegularizationType);
	itkSetObjectMacro( EnergyFidelity, EnergyFidelityType);
    */    
    
    inline int Compute( const InputImageNeighborhoodIterator & itData, const LabelledImageNeighborhoodIterator & itRegul)             
      {
	this->SetEnergyBefore( this->GetEnergyFidelity()->GetValue(itData, itRegul.GetCenterPixel())
			       + this->GetLambda() * this->GetEnergyRegularization()->GetValue(itRegul, itRegul.GetCenterPixel()) );
	
	//Try all possible value (how to be generic ?)
	this->SetEnergyAfter( this->GetEnergyBefore() ); //default values to current one
	this->SetValue( itRegul.GetCenterPixel() );
	// otbDebugMacro(<< "Computing MAP for pix " << itData.GetIndex());
	// Compute probability for each possibility
	double totalProba=0.0;
	for (unsigned int valueCurrent = 0; valueCurrent < this->GetNumberOfClasses(); ++valueCurrent)
	  {
	    // otbDebugMacro(<< " --> Proposed value " << static_cast<double>(valueCurrent)); 
	    this->SetEnergyCurrent( this->GetEnergyFidelity()->GetValue(itData, valueCurrent)
	      + this->GetLambda() * this->GetEnergyRegularization()->GetValue(itRegul, valueCurrent) );
	    
	    m_Energy[valueCurrent] = this->GetEnergyCurrent();
	    m_RepartitionFunction[valueCurrent] = vcl_exp(-this->GetEnergyCurrent())+totalProba;
	    totalProba = m_RepartitionFunction[valueCurrent];
	    // otbDebugMacro("valueCurrent, m_RepartitionFunction[valueCurrent] " << (unsigned int)  valueCurrent << ", " << m_RepartitionFunction[valueCurrent]);
	    
	  }
	
	//Pick a value according to probability
	
	double select;
	if (m_ValueInsteadRandom == itk::NumericTraits<double>::min())
	  {
	    select = (rand()/(double(RAND_MAX)+1) * totalProba);
	  } 
	else
	  {
	    select = m_ValueInsteadRandom;
	  }
	// otbDebugMacro("m_RepartitionFunction " <<  m_RepartitionFunction[0] << " " 
	//                 <<  m_RepartitionFunction[1] << " " <<  m_RepartitionFunction[2] << " " 
	//                 <<  m_RepartitionFunction[3] << " ");
	
	// otbDebugMacro("select, totalProba " <<  select << ", " << totalProba);
	unsigned int valueCurrent = 0;
	for (valueCurrent = 0; valueCurrent < this->m_NumberOfClasses; ++valueCurrent)
	  {
	    if (m_RepartitionFunction[valueCurrent] > select) break;
	  }
	
	if ( this->GetValue() != static_cast<LabelledImagePixelType>(valueCurrent))
	  {
	    this->SetValue( valueCurrent );
	    this->SetEnergyAfter( m_Energy[valueCurrent] );
	  }
	
	this->SetDeltaEnergy( this->GetEnergyAfter() - this->GetEnergyBefore() );
	// otbDebugMacro("Decision " << (unsigned int) valueCurrent);
	return 0;
      }
    
    /** Store a value to be used instead of random value.. FOR TEST ONLY*/
    void SetValueInsteadRandom( double val )
      {
	m_ValueInsteadRandom = val;
	std::cout<<"The m_ValueInsteadRandom varaible has to be used only for tests..."<<std::endl;
	this->Modified();
      };

  private:
    double * m_RepartitionFunction ;   
    double * m_Energy; 
    
    
    
  protected:
    // The constructor and destructor.
    MRFSamplerRandomMAP() 
      {
	m_Energy = (double *) calloc(this->GetNumberOfClasses(), sizeof(double));
	m_RepartitionFunction = (double *) calloc(this->GetNumberOfClasses(), sizeof(double));
	m_ValueInsteadRandom = itk::NumericTraits<double>::min();
      }
    virtual ~MRFSamplerRandomMAP()
      {
	free(m_Energy);
	free(m_RepartitionFunction);
      }
    
    /** Store a value to be used instead of random value.. FOR TEST ONLY*/
    double m_ValueInsteadRandom;
    
  };
  
}

#endif
