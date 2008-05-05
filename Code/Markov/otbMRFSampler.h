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
    typedef MRFSampler                    Self;
    typedef itk::Object                   Superclass;
    typedef itk::SmartPointer<Self>       Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    // typedef itk::NeighborhoodIterator< TInput1 >  InputImageNeighborhoodIterator;
    typedef itk::ConstNeighborhoodIterator< TInput1 > InputImageNeighborhoodIterator;
    typedef itk::NeighborhoodIterator< TInput2 >      LabelledImageNeighborhoodIterator;
    typedef typename TInput2::PixelType               LabelledImagePixelType;
    typedef typename TInput1::PixelType               InputImagePixelType;

    // typedef MRFEnergyFunctorBase<TInput1, TInput2> EnergyFidelityFunctorType;
    // typedef MRFEnergy<EnergyFidelityFunctorType>  EnergyFidelityType;
    // typedef MRFEnergyFunctorBase<TInput2, TInput2> EnergyRegularizationFunctorType;
    // typedef MRFEnergy<EnergyRegularizationFunctorType>  EnergyRegularizationType;
    typedef MRFEnergy<TInput1, TInput2>                EnergyFidelityType;
    typedef MRFEnergy<TInput2, TInput2>                EnergyRegularizationType;    
    typedef typename EnergyFidelityType::Pointer       EnergyFidelityPointer;
    typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;
    
    itkNewMacro(Self);
    
    itkTypeMacro(MRFSampler,itk::Object);
 
    // Accessors
    itkSetMacro(NumberOfClasses, unsigned int);
    itkGetMacro(NumberOfClasses, unsigned int);

    itkSetMacro(Lambda, double);
    itkGetMacro(Lambda, double);
    itkSetMacro(EnergyBefore, double);
    itkGetMacro(EnergyBefore, double);
    itkSetMacro(EnergyAfter, double);
    itkGetMacro(EnergyAfter, double);
    itkSetMacro(DeltaEnergy, double);
    itkGetMacro(DeltaEnergy, double);
    itkSetMacro(EnergyCurrent, double);
    itkGetMacro(EnergyCurrent, double);
 
    itkSetMacro(Value, LabelledImagePixelType);
    itkGetMacro(Value, LabelledImagePixelType);
    
    void SetEnergyRegularization( EnergyRegularizationPointer enerReg ){ m_EnergyRegularization=enerReg; this->Modified(); };
    EnergyRegularizationPointer GetEnergyRegularization(){ return m_EnergyRegularization; };
    void SetEnergyFidelity( EnergyFidelityPointer enerFid ){ m_EnergyFidelity=enerFid; this->Modified(); };
    EnergyFidelityPointer GetEnergyFidelity(){ return m_EnergyFidelity; };
    
    virtual int Compute( const InputImageNeighborhoodIterator & itData, const LabelledImageNeighborhoodIterator & itRegul)             
      {
	m_DeltaEnergy =  0;
	
	return 0;
      }
    
    
  protected:
    unsigned int m_NumberOfClasses;
    double       m_EnergyBefore;
    double       m_EnergyAfter;
    double       m_DeltaEnergy;
    double       m_EnergyCurrent;      
    double       m_Lambda;
    LabelledImagePixelType      m_Value;
    EnergyRegularizationPointer m_EnergyRegularization; 
    EnergyFidelityPointer       m_EnergyFidelity; 
    LabelledImagePixelType      m_ValueCurrent;
    
    
  protected:
    // The constructor and destructor.
    MRFSampler() 
      {
	m_EnergyRegularization = EnergyRegularizationType::New();
	m_EnergyFidelity = EnergyFidelityType::New();

	m_Lambda = 1.;
	m_EnergyCurrent = 1.;
	m_DeltaEnergy = 1.;
	m_EnergyAfter = 1.;
	m_EnergyBefore = 1.;
      }
    virtual ~MRFSampler() {}
    
  }; 
}

#endif
