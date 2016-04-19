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
#ifndef __otbPeriodicSampler_h
#define __otbPeriodicSampler_h

#include "otbSamplerBase.h"

namespace otb
{

/**
 * \class PeriodicSampler
 *
 * \brief Periodic sampler for iteration loops
 * 
 * This class allows to do periodic sampling during an iteration loop.
 *
 * \ingroup OTBStatistics
 */
class PeriodicSampler : public SamplerBase
{
public:
  typedef PeriodicSampler  Self;
  typedef SamplerBase      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Internal parameters, only contains an offset to shift the periodic 
   * sampling
   */
  typedef struct Parameter
    {
    unsigned long Offset;
    
    bool operator!=(const struct Parameter  & param) const;
    } ParameterType; 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PeriodicSampler,SamplerBase);
  
  /** Setter/Getter for internal parameters */
  itkSetMacro(Parameters,ParameterType);
  itkGetMacro(Parameters,ParameterType);
  
  /**
   * Method that resets the internal state of the sampler
   */
  virtual void Reset(void);
  
  /**
   * Method to call during iteration, returns true if the sample is selected,
   * and false otherwise.
   */
  bool TakeSample(void);

protected:
  /** Constructor */
  PeriodicSampler();
   
  /** Destructor */
  virtual ~PeriodicSampler() {}

private:
  // Not implemented
  PeriodicSampler(const Self&);
  void operator=(const Self&);
  
  /** Internal parameters for the sampler */
  ParameterType m_Parameters;
  
  /** Internal temporary value : 
   * (m_ProcessedElements+Offset)*m_NeededElements / m_TotalElements
   */
  double m_Value;
};

} // namespace otb
#endif
