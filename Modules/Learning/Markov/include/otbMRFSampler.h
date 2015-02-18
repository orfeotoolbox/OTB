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
#ifndef __otbMRFSampler_h
#define __otbMRFSampler_h

#include "otbMRFEnergy.h"
#include "itkNeighborhoodIterator.h"

namespace otb
{
/**
   * \class MRFSampler
   * \brief This is the base class for sampler methods used in the MRF framework.
   *
   * Derived class must reimplement Compute() method.
   *
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
   * \ingroup Markov
 */

template<class TInput1, class TInput2>
class ITK_EXPORT MRFSampler : public itk::Object
{
public:

  typedef MRFSampler                    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ConstNeighborhoodIterator
  <TInput1>         InputImageNeighborhoodIterator;
  typedef typename TInput1::PixelType InputImagePixelType;

  typedef itk::NeighborhoodIterator
  <TInput2>         LabelledImageNeighborhoodIterator;
  typedef typename TInput2::PixelType LabelledImagePixelType;

  typedef MRFEnergy<TInput1, TInput2> EnergyFidelityType;
  typedef MRFEnergy<TInput2, TInput2> EnergyRegularizationType;

  typedef typename EnergyFidelityType::Pointer       EnergyFidelityPointer;
  typedef typename EnergyRegularizationType::Pointer EnergyRegularizationPointer;

  itkTypeMacro(MRFSampler, Object);

  itkSetMacro(NumberOfClasses, unsigned int);
  itkGetMacro(NumberOfClasses, unsigned int);

  itkSetMacro(Lambda, double);
  itkGetMacro(Lambda, double);

  itkGetMacro(DeltaEnergy, double);
  itkGetMacro(Value, LabelledImagePixelType);

  // Accessor for validation tests pour les tests
  itkGetConstMacro(EnergyBefore, double);
  itkGetConstMacro(EnergyAfter, double);

  itkSetObjectMacro(EnergyRegularization, EnergyRegularizationType);
  itkSetObjectMacro(EnergyFidelity, EnergyFidelityType);

  virtual int Compute(const InputImageNeighborhoodIterator& itData,
                      const LabelledImageNeighborhoodIterator& itRegul) = 0;

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
  MRFSampler() :
    m_NumberOfClasses(1),
    m_EnergyBefore(1.0),
    m_EnergyAfter(1.0),
    m_DeltaEnergy(1.0),
    m_EnergyCurrent(1.0),
    m_Lambda(1.0)
    {
    m_EnergyRegularization = EnergyRegularizationType::New();
    m_EnergyFidelity =  EnergyFidelityType::New();
    };
  virtual ~MRFSampler() {}

};

}

#endif
