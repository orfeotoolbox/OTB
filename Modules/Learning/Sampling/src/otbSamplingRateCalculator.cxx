/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
    if (it->second.Tot)
      {
      if (smallestNbofSamples > it->second.Tot)
        {
        smallestNbofSamples = it->second.Tot;
        }
      }
    else
      {
      otbWarningMacro("Ignoring empty class " << it->first);
      }
    }
  // Check if there is at least one non-empty class
  if (smallestNbofSamples == itk::NumericTraits<unsigned long>::max())
    {
    otbWarningMacro("There are only empty classes, sample size is set to zero!");
    smallestNbofSamples = 0UL;
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
    if (it->second.Tot)
      {
      it->second.Required = dRequiredNbSamples;
      }
    else
      {
      // ignore empty classes
      it->second.Required = 0UL;
      }
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
      triplet.Tot = 0UL;
      triplet.Required = it->second;
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


void SamplingRateCalculator
::SetPercentageOfSamples(double percent)
{
  MapRateType::iterator it = m_RatesByClass.begin();
  for (; it != m_RatesByClass.end() ; ++it)
    {
    it->second.Required = static_cast<unsigned long>(std::floor(0.5+percent * it->second.Tot));
    it->second.Rate = percent;
    }
}

void SamplingRateCalculator
::SetTotalNumberOfSamples(unsigned long value)
{
  // First, get total number of samples
  unsigned long totalNumberOfSamplesAvailable = 0;

  MapRateType::iterator it = m_RatesByClass.begin();
  for (; it != m_RatesByClass.end() ; ++it)
    {
    totalNumberOfSamplesAvailable+=it->second.Tot;
    }

  // Then compute number of samples for each class
  for (it = m_RatesByClass.begin(); it != m_RatesByClass.end() ; ++it)
    {
    double ratio = totalNumberOfSamplesAvailable != 0 ? it->second.Tot / static_cast<double>(totalNumberOfSamplesAvailable): 0;

    it->second.Required = static_cast<unsigned long>(0.5+ratio*value);
    this->UpdateRate(it->first);
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

  typedef std::vector<boost::iterator_range<std::string::const_iterator> > ListType;

  if (ifs)
    {
    this->ClearRates();
    std::string line;
    TripletType tpt;
    std::string sep("");
    ListType parts;
    while(std::getline(ifs,line))
      {
      if (line.empty()) continue;
      std::string::size_type pos = line.find_first_not_of(" \t");
      if (pos != std::string::npos && line[pos] == '#') continue;

      if (sep.empty())
        {
        // Try to detect the separator
        std::vector<std::string> separators(4);
        separators[0] = "\t";
        separators[1] = ";";
        separators[2] = ",";
        separators[3] = " ";
        ListType words;
        for (unsigned int k=0 ; k<separators.size() ; k++)
          {
          boost::split(words, line, boost::is_any_of(separators[k]));
          if (words.size() == 4)
            {
            sep = separators[k];
            break;
            }
          }
        if (sep.empty()) continue;
        }
      // parse the line
      boost::split(parts, line, boost::is_any_of(sep));
      if (parts.size() == 4)
        {
        std::string::size_type pos1 = line.find_first_not_of(" \t", parts[0].begin() - line.begin());
        std::string::size_type pos2 = line.find_last_not_of(" \t", parts[0].end() - line.begin() -1);
        std::string::size_type pos3 = line.find_first_not_of(" \t", parts[1].begin() - line.begin());
        std::string::size_type pos4 = line.find_last_not_of(" \t", parts[1].end() - line.begin() -1);
        std::string::size_type pos5 = line.find_first_not_of(" \t", parts[2].begin() - line.begin());
        std::string::size_type pos6 = line.find_last_not_of(" \t", parts[2].end() - line.begin() -1);
        std::string::size_type pos7 = line.find_first_not_of(" \t", parts[3].begin() - line.begin());
        std::string::size_type pos8 = line.find_last_not_of(" \t\r", parts[3].end() - line.begin() -1);
        if (pos2 != std::string::npos && pos1 <= pos2 &&
            pos4 != std::string::npos && pos3 <= pos4 &&
            pos6 != std::string::npos && pos5 <= pos6 &&
            pos8 != std::string::npos && pos7 <= pos8)
          {
          std::string name = line.substr(pos1, pos2 - pos1 + 1);
          std::string val1 = line.substr(pos3, pos4 - pos3 + 1);
          std::string val2 = line.substr(pos5, pos6 - pos5 + 1);
          std::string val3 = line.substr(pos7, pos8 - pos7 + 1);
          tpt.Required = Utils::LexicalCast<unsigned long>(val1, "number of samples");
          tpt.Tot = Utils::LexicalCast<unsigned long>(val2, "number of samples");
          tpt.Rate = Utils::LexicalCast<double>(val3, "rate");
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

SamplingRateCalculator::ClassCountMapType
SamplingRateCalculator
::ReadRequiredSamples(const std::string& filename)
{
  ClassCountMapType output;
  std::ifstream ifs(filename.c_str());

  typedef std::vector<boost::iterator_range<std::string::const_iterator> > ListType;

  if (ifs)
    {
    std::string line;
    std::string sep("");
    ListType parts;

    while(std::getline(ifs,line))
      {
      if (line.empty()) continue;
      std::string::size_type pos = line.find_first_not_of(" \t");
      if (pos != std::string::npos && line[pos] == '#') continue;

      if (sep.empty())
        {
        // Try to detect the separator
        std::vector<std::string> separators(4);
        separators[0] = "\t";
        separators[1] = ";";
        separators[2] = ",";
        separators[3] = " ";
        ListType words;
        for (unsigned int k=0 ; k<separators.size() ; k++)
          {
          boost::split(words, line, boost::is_any_of(separators[k]));
          if (words.size() >= 2)
            {
            sep = separators[k];
            break;
            }
          }
        if (sep.empty()) continue;
        }
      // parse the line
      boost::split(parts, line, boost::is_any_of(sep));
      if (parts.size() >= 2)
        {
        std::string::size_type pos1 = line.find_first_not_of(" \t", parts[0].begin() - line.begin());
        std::string::size_type pos2 = line.find_last_not_of(" \t", parts[0].end() - line.begin() -1);
        std::string::size_type pos3 = line.find_first_not_of(" \t", parts[1].begin() - line.begin());
        std::string::size_type pos4 = line.find_last_not_of(" \t\r", parts[1].end() - line.begin() -1);
        if (pos2 != std::string::npos && pos1 <= pos2 &&
            pos4 != std::string::npos && pos3 <= pos4)
          {
          std::string name = line.substr(pos1, pos2 - pos1 + 1);
          std::string value = line.substr(pos3, pos4 - pos3 + 1);
          output[name] = Utils::LexicalCast<unsigned long>(value, "number of samples");
          }
        }
      }
    ifs.close();
    }
  else
    {
    itkGenericExceptionMacro(<< " Couldn't open " << filename);
    }
  return output;
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

