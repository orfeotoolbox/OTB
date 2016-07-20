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
#ifndef otbPatternSampler_h
#define otbPatternSampler_h

#include "otbSamplerBase.h"

namespace otb
{

/**
 * \class PatternSampler
 *
 * \brief Periodic sampler for iteration loops
 * 
 * This class allows to do periodic sampling during an iteration loop.
 *
 * \ingroup OTBStatistics
 */
class ITK_EXPORT PatternSampler : public SamplerBase
{
public:
  typedef PatternSampler  Self;
  typedef SamplerBase      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Internal parameters, only contains an offset to shift the periodic 
   * sampling
   */
  typedef struct Parameter
    {
    /** Maximum size of the internal patterns */
    unsigned long MaxPatternSize;

    /** First sampling pattern */
    std::vector<bool> Pattern1;

    /** Second sampling pattern (can be empty).
     * It is called when the first pattern returns false */
    std::vector<bool> Pattern2;

    /** Seed used to randomly generate patterns (used only if greater than 0) */
    unsigned int Seed;

    bool operator!=(const struct Parameter  & param) const;
    } ParameterType; 

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PatternSampler,SamplerBase);

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
   * Method that resets the internal state of the sampler
   */
  virtual void Reset(void);

  /**
   * Method to call during iteration, returns true if the sample is selected,
   * and false otherwise.
   */
  bool TakeSample(void);

  /** Import sampling patterns from an input string
   *  Two patterns may be given, separated by a slash
   *  Beware, the second pattern may be empty
   */
  static void ImportPatterns(
    const std::string &data,
    ParameterType &param);

  /** Export the sampling patterns in the input parameter to
   *  an output string. If the second pattern is not empty, it will be
   *  concatenated to the output string, separated by a slash
   */
  static void ExportPatterns(
    const ParameterType &param,
    std::string &data);

protected:
  /** Constructor */
  PatternSampler();

  /** Destructor */
  virtual ~PatternSampler() {}

private:
  // Not implemented
  PatternSampler(const Self&);
  void operator=(const Self&);

  /** Generate a random array of booleans */
  std::vector<bool> RandArray(unsigned long N,unsigned long T);

  /** Find a suitable size for internal patterns */
  unsigned long FindBestSize(unsigned long tot);

  /** Internal parameters for the sampler */
  ParameterType m_Parameters;

  /** Index tracking the position in first internal pattern */
  unsigned long m_Index1;

  /** Index tracking the position in the second internal pattern */
  unsigned long m_Index2;

  /** Helper function to decode boolean sequence
   *  Caracters converted into true : '1' 'X' 'y' 'Y' '|' '+'
   *  Caracters converted into false : '0' '_' 'n' 'N' '.' '-'
   *  Other caracters will return a 2 */
  static unsigned int ParseSymbol(const char &s);
};

} // namespace otb
#endif
