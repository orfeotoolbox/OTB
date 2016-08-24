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
#ifndef otbRandomSampler_h
#define otbRandomSampler_h

#include "otbSamplerBase.h"

namespace otb
{

/**
 * \class RandomSampler
 *
 * \brief Random sampler for iteration loops
 * 
 * This class allows doing random sampling during an iteration loop.
 * It uses the MersenneTwisterRandomGenerator.
 *
 * \ingroup OTBStatistics
 */
class ITK_EXPORT RandomSampler : public SamplerBase
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
    /** Maximum buffer size to generate permutations */
    unsigned long MaxBufferSize;
    
    bool operator!=(const struct Parameter  & param) const;
    } ParameterType; 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(RandomSampler,SamplerBase);

  /** Setter for internal parameters */
  void SetParameters(const ParameterType &param)
    {
    if (m_Parameters != param)
      {
      this->Modified();
      m_Parameters = param;
      }
    }

  /** Getter for internal parameters */
  ParameterType GetParameters()
    {
    return m_Parameters;
    }

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

  /** Internal list of positions to select */
  std::vector<unsigned long> m_Positions;

  /** Internal current position */
  std::vector<unsigned long>::iterator m_CurrentPosition;
};

} // namespace otb
#endif
