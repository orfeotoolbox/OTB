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


#include "otbSamplingRateCalculator.h"
#include "otbStringUtils.h"
#include "otbMacro.h"
#include <sstream>
#include <fstream>
#include <iterator>
#include "itksys/SystemTools.hxx"


namespace otb
{

bool
SamplingRateCalculator::TripletType::operator==(const SamplingRateCalculator::TripletType & triplet) const
{
  return bool((Required == triplet.Required)||
              (Tot == triplet.Tot)||
              (Rate == triplet.Rate));
}

SamplingRateCalculator
::SamplingRateCalculator()
{
}

void 
SamplingRateCalculator
::SetMinimumNbOfSamplesByClass(void)
{
  unsigned long smallestNbofSamples = itk::NumericTraits<unsigned long>::max();
  MapRateType::iterator it = m_RatesByClass.begin();
  for (; it != m_RatesByClass.end() ; ++it)
    {
    if (smallestNbofSamples > it->second.Tot)
      {
      smallestNbofSamples = it->second.Tot;
      }
    }
  // Check if there is an empty class
  if (smallestNbofSamples == 0UL)
    {
    otbWarningMacro("There is an empty class, sample size is set to zero!");
    }
  this->SetNbOfSamplesAllClasses( smallestNbofSamples );
}

void 
SamplingRateCalculator
::SetNbOfSamplesAllClasses(unsigned long dRequiredNbSamples)
{
  MapRateType::iterator it = m_RatesByClass.begin();
  for (; it != m_RatesByClass.end() ; ++it)
    {
    it->second.Required = dRequiredNbSamples;
    this->UpdateRate(it->first);
    }
}

void 
SamplingRateCalculator
::SetNbOfSamplesByClass(const ClassCountMapType &required)
{
  ClassCountMapType::const_iterator it = required.begin();
  for (; it != required.end() ; ++it)
    {
    if (m_RatesByClass.count(it->first))
      {
      m_RatesByClass[it->first].Required = it->second;
      this->UpdateRate(it->first);
      }
    else
      {
      TripletType triplet;
      triplet.Tot = it->second;
      triplet.Required = 0UL;
      triplet.Rate = 0.0;
      m_RatesByClass[it->first] = triplet;
      }
    }
}

void
SamplingRateCalculator
::SetAllSamples(void)
{
  MapRateType::iterator it = m_RatesByClass.begin();
  for (; it != m_RatesByClass.end() ; ++it)
    {
    it->second.Required = it->second.Tot;
    it->second.Rate = 1.0;
    }
}

void 
SamplingRateCalculator
::Write(std::string filename)
{
  std::ofstream file(filename.c_str(), std::ios::out | std::ios::trunc);

  if (file)
    {
    itk::Indent indent(0);
    this->PrintSelf(file,indent);
    file.close();
    }
  else
    {
    itkExceptionMacro(<< " Couldn't open " << filename);
    }
}

void
SamplingRateCalculator
::Read(std::string filename)
{
  std::ifstream ifs(filename.c_str());

  if (ifs)
    {
    this->ClearRates();
    std::string line;
    TripletType tpt;
    std::string sep("");
    while(!ifs.eof())
      {
      std::getline(ifs,line);
      if (line.empty()) continue;
      std::string::size_type pos = line.find_first_not_of(" \t");
      if (pos != std::string::npos && line[pos] == '#') continue;

      if (sep.size() == 0)
        {
        // Try to detect the separator
        std::string separators("\t;,");
        for (unsigned int k=0 ; k<separators.size() ; k++)
          {
          std::vector<itksys::String> words = itksys::SystemTools::SplitString(line,separators[k]);
          if (words.size() == 4)
            {
            sep.push_back(separators[k]);
            break;
            }
          }
        if (sep.size() == 0) continue;
        }
      // parse the line
      std::vector<itksys::String> parts = itksys::SystemTools::SplitString(line,sep[0]);
      if (parts.size() == 4)
        {
        std::string::size_type pos1 = parts[0].find_first_not_of(" \t");
        std::string::size_type pos2 = parts[0].find_last_not_of(" \t");
        std::string::size_type pos3 = parts[1].find_first_not_of(" \t");
        std::string::size_type pos4 = parts[1].find_last_not_of(" \t");
        std::string::size_type pos5 = parts[2].find_first_not_of(" \t");
        std::string::size_type pos6 = parts[2].find_last_not_of(" \t");
        std::string::size_type pos7 = parts[3].find_first_not_of(" \t");
        std::string::size_type pos8 = parts[3].find_last_not_of(" \t");
        if (pos1 != std::string::npos && pos3 != std::string::npos &&
            pos5 != std::string::npos && pos7 != std::string::npos)
          {
          std::string name = parts[0].substr(pos1, pos2 - pos1 + 1);
          std::string val1 = parts[1].substr(pos3, pos4 - pos3 + 1);
          std::string val2 = parts[2].substr(pos5, pos6 - pos5 + 1);
          std::string val3 = parts[3].substr(pos7, pos8 - pos7 + 1);
          tpt.Required = boost::lexical_cast<unsigned long>(val1);
          tpt.Tot = boost::lexical_cast<unsigned long>(val2);
          tpt.Rate = boost::lexical_cast<double>(val3);
          m_RatesByClass[name] = tpt;
          }
        }
      }
    ifs.close();
    }
  else
    {
    itkExceptionMacro(<< " Couldn't open " << filename);
    }
}

void
SamplingRateCalculator
::SetClassCount(const ClassCountMapType& map)
{
  ClassCountMapType::const_iterator it = map.begin();
  for (; it != map.end() ; ++it)
    {
    if (m_RatesByClass.count(it->first))
      {
      m_RatesByClass[it->first].Tot = it->second;
      this->UpdateRate(it->first);
      }
    else
      {
      TripletType triplet;
      triplet.Tot = it->second;
      triplet.Required = 0UL;
      triplet.Rate = 0.0;
      m_RatesByClass[it->first] = triplet;
      }
    }
}

void
SamplingRateCalculator
::ClearRates(void)
{
  m_RatesByClass.clear();
}

void
SamplingRateCalculator
::UpdateRate(const std::string &name)
{
  if (m_RatesByClass[name].Tot)
    {
    m_RatesByClass[name].Rate = std::min(
      static_cast<double>(m_RatesByClass[name].Required) / 
        static_cast<double>(m_RatesByClass[name].Tot),
      1.0);
    }
  else
    {
    // Set to 0 as rate is undefined
    m_RatesByClass[name].Rate = 0.0;
    }
}

void
SamplingRateCalculator
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << indent << "#className requiredSamples totalSamples rate" << std::endl;
  MapRateType::const_iterator itRates = m_RatesByClass.begin();
  for(; itRates != m_RatesByClass.end(); ++itRates)
    {
    TripletType tpt=itRates->second;
    os << indent << itRates->first << "\t" << tpt.Required << "\t" << tpt.Tot << "\t" << tpt.Rate << std::endl;
    }
}

} // End namespace otb

