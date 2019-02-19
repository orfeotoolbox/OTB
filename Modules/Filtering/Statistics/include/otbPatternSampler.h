/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbPatternSampler_h
#define otbPatternSampler_h

#include "otbSamplerBase.h"
#include <string>
#include "OTBStatisticsExport.h"

namespace otb
{

/**
 * \class PatternSampler
 *
 * \brief Periodic sampler for iteration loops
 * 
 * This class allows doing periodic sampling during an iteration loop.
 *
 * \ingroup OTBStatistics
 */
class OTBStatistics_EXPORT PatternSampler : public SamplerBase
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
    unsigned long MaxPatternSize = 0;

    /** First sampling pattern */
    std::vector<bool> Pattern1;

    /** Second sampling pattern (can be empty).
     * It is called when the first pattern returns false */
    std::vector<bool> Pattern2;

    /** Seed used to randomly generate patterns (used only if greater than 0) */
    unsigned int Seed = 0;

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
  void Reset(void) override;

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
  ~PatternSampler() override {}

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
