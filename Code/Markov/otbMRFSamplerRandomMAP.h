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

#include "itkMersenneTwisterRandomVariateGenerator.h"
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
    
    typedef MRFSamplerRandomMAP               Self;
    typedef otb::MRFSampler<TInput1, TInput2> Superclass;
    typedef itk::SmartPointer<Self>           Pointer;
    typedef itk::SmartPointer<const Self>     ConstPointer;
    
    typedef typename Superclass::InputImageNeighborhoodIterator    InputImageNeighborhoodIterator;
    typedef typename Superclass::LabelledImageNeighborhoodIterator LabelledImageNeighborhoodIterator;
    typedef typename Superclass::LabelledImagePixelType            LabelledImagePixelType;
    typedef typename Superclass::InputImagePixelType               InputImagePixelType;
    typedef typename Superclass::EnergyFidelityType                EnergyFidelityType;
    typedef typename Superclass::EnergyRegularizationType          EnergyRegularizationType;
    typedef typename Superclass::EnergyFidelityPointer             EnergyFidelityPointer;
    typedef typename Superclass::EnergyRegularizationPointer       EnergyRegularizationPointer;     
    typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
    
    itkNewMacro(Self);
    
    itkTypeMacro(MRFSamplerRandomMAP,MRFSampler);
    
    
    void SetNumberOfClasses(const unsigned int nClasses)
      {
	if (nClasses != this->m_NumberOfClasses || energiesInvalid == true)
	  {
	    this->m_NumberOfClasses = nClasses;
	    if (energy != NULL) 
	      free(energy);
	    if (repartitionFunction != NULL) 
	      free(repartitionFunction);
	    energy = (double *) calloc(this->m_NumberOfClasses, sizeof(double));
	    repartitionFunction = (double *) calloc(this->m_NumberOfClasses, sizeof(double));
	    this->Modified();
	  }
      }
    
    inline int Compute( const InputImageNeighborhoodIterator & itData, const LabelledImageNeighborhoodIterator & itRegul)
      {
	if (this->m_NumberOfClasses == 0)
	  {
            itkExceptionMacro(<<"NumberOfClasse has to be greater than 0.");
	  }
	
	this->m_EnergyBefore = this->m_EnergyFidelity->GetValue(itData, itRegul.GetCenterPixel());
	this->m_EnergyBefore += this->m_Lambda 
	                        * this->m_EnergyRegularization->GetValue(itRegul, itRegul.GetCenterPixel());
	
	//Try all possible value (how to be generic ?)
	this->m_EnergyAfter = this->m_EnergyBefore; //default values to current one
	this->m_Value = itRegul.GetCenterPixel();
	
	//Compute probability for each possibility
	double totalProba=0.0;
	for (unsigned int  valueCurrent = 0; valueCurrent < this->m_NumberOfClasses; ++valueCurrent)
	  {
	    this->m_EnergyCurrent = this->m_EnergyFidelity->GetValue(itData, static_cast<LabelledImagePixelType>(valueCurrent));
	    this->m_EnergyCurrent += this->m_Lambda 
	                             * this->m_EnergyRegularization->GetValue(itRegul, static_cast<LabelledImagePixelType>(valueCurrent));
	    
	    energy[valueCurrent] = this->m_EnergyCurrent;
	    repartitionFunction[valueCurrent] = vcl_exp(-this->m_EnergyCurrent)+totalProba;
	    totalProba = repartitionFunction[valueCurrent];
	    
	  }
	
	//Pick a value according to probability
	
	double select = (m_Generator->GetIntegerVariate()/(double(RAND_MAX)+1) * totalProba);
	unsigned int valueCurrent = 0;
        while( valueCurrent<this->GetNumberOfClasses() && repartitionFunction[valueCurrent] <= select)
	  {
            valueCurrent++;
	  }
	
        if ( valueCurrent==this->GetNumberOfClasses() )
	  {
            valueCurrent = this->GetNumberOfClasses()-1;
	  }
	
	
	if ( this->m_Value != static_cast<LabelledImagePixelType>(valueCurrent))
	  {
	    this->m_Value = static_cast<LabelledImagePixelType>(valueCurrent);
	    this->m_EnergyAfter = energy[static_cast<unsigned int>(valueCurrent)];
	  }
	
	this->m_DeltaEnergy=  this->m_EnergyAfter - this->m_EnergyBefore;
	
	return 0;
      }
    
    /** Methods to cancel random effects.*/
    void InitializeSeed(int seed){ m_Generator->SetSeed(seed); }
    void InitializeSeed(){ m_Generator->SetSeed(); }
    
  private:
    double * repartitionFunction;
    double * energy; 
    bool energiesInvalid;
    RandomGeneratorType::Pointer m_Generator;
    
  protected:
    // The constructor and destructor.
    MRFSamplerRandomMAP() 
      {
	energy=NULL;
	repartitionFunction=NULL;
	energiesInvalid = true;
	m_Generator = RandomGeneratorType::New();
	m_Generator->SetSeed();
      }
    virtual ~MRFSamplerRandomMAP() 
      {
	if (energy != NULL) 
	  free(energy);
	if (repartitionFunction != NULL) 
	  free(repartitionFunction);
      }
  };
}

#endif
