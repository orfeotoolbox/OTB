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
#ifndef __otbRandomSampler_h
#define __otbRandomSampler_h

#include "otbSamplerBase.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{

/**
 * \class RandomSampler
 *
 * \brief Random sampler for iteration loops
 * 
 * This class allows to do random sampling during an iteration loop.
 * It uses the MersenneTwisterRandomGenerator.
 *
 * \ingroup OTBStatistics
 */
class RandomSampler : public SamplerBase
{
public:
  typedef RandomSampler  Self;
  typedef SamplerBase      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Internal parameters, only contains an offset to shift the periodic 
   * sampling
   */
  typedef struct Parameter
    {
    /** Flag for adaptative mode : adapt the rate during iteration so that
     *  the exact number of requested samples is selected */
    bool Adaptative;
    
    bool operator!=(const struct Parameter  & param) const;
    } ParameterType; 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RandomSampler,SamplerBase);
  
  /** Setter/Getter for internal parameters */
  itkSetMacro(Parameters,ParameterType);
  itkGetMacro(Parameters,ParameterType);

  /**
   * Reset internal counter (to be called before starting iteration)
   */
  virtual void Reset(void);

  /**
   * Method to call during iteration, returns true if the sample is selected,
   * and false otherwise.
   */
  bool TakeSample(void);

protected:
  /** Constructor */
  RandomSampler();
   
  /** Destructor */
  virtual ~RandomSampler() {}

private:
  // Not implemented
  RandomSampler(const Self&);
  void operator=(const Self&);
  
  /** Internal parameters for the sampler */
  ParameterType m_Parameters;

  /** Pointer to the random number generator instance */
  itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer m_Generator;
};

} // namespace otb
#endif
