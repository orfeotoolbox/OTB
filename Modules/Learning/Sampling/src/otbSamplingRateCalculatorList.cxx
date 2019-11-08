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


#include "otbSamplingRateCalculatorList.h"
#include "otbMacro.h"

namespace otb
{

void SamplingRateCalculatorList::SetNthClassCount(unsigned int index, const ClassCountMapType& map)
{
  if (index >= this->Size())
  {
    unsigned int currentSize = this->Size();
    for (unsigned int i = currentSize; i < (index + 1); i++)
    {
      this->PushBack(SamplingRateCalculator::New());
    }
  }
  this->GetNthElement(index)->SetClassCount(map);
}

const SamplingRateCalculatorList::MapRateType& SamplingRateCalculatorList::GetRatesByClass(unsigned int index)
{
  if (index >= this->Size())
  {
    itkGenericExceptionMacro("Requesting an index (" << index << ") larger than list size (" << this->Size() << ")");
  }
  return this->GetNthElement(index)->GetRatesByClass();
}

void SamplingRateCalculatorList::ClearRates(void)
{
  for (unsigned int i = 0; i < this->Size(); i++)
  {
    this->GetNthElement(i)->ClearRates();
  }
}

void SamplingRateCalculatorList::SetAllSamples(PartitionType t)
{
  this->UpdateGlobalCounts();
  switch (t)
  {
  case PROPORTIONAL:
  case EQUAL:
  case CUSTOM:
  {
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetAllSamples();
    }
    break;
  }
  default:
    itkGenericExceptionMacro("Unknown partition mode");
    break;
  }
}

void SamplingRateCalculatorList::SetMinimumNbOfSamplesByClass(PartitionType t)
{
  this->UpdateGlobalCounts();
  switch (t)
  {
  case PROPORTIONAL:
  case EQUAL:
  {
    // Use the smallest class globally to derive the constant needed
    unsigned long               smallest = itk::NumericTraits<unsigned long>::max();
    ClassCountMapType::iterator it       = m_GlobalCountMap.begin();
    for (; it != m_GlobalCountMap.end(); ++it)
    {
      if (smallest > it->second && it->second > 0UL)
      {
        smallest = it->second;
      }
    }
    if (smallest == itk::NumericTraits<unsigned long>::max())
    {
      otbWarningMacro("All classes are empty !");
      smallest = 0UL;
    }
    std::vector<unsigned long> needed;
    needed.push_back(smallest);
    this->SetNbOfSamplesAllClasses(needed, t);
    break;
  }
  case CUSTOM:
  {
    // Use the smallest class in each input
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetMinimumNbOfSamplesByClass();
    }
    break;
  }
  default:
    itkGenericExceptionMacro("Unknown partition mode");
    break;
  }
}

void SamplingRateCalculatorList::SetNbOfSamplesAllClasses(std::vector<unsigned long>& nb, PartitionType t)
{
  if (nb.empty())
  {
    itkGenericExceptionMacro("No number of samples given");
  }
  this->UpdateGlobalCounts();
  ClassCountMapType::const_iterator it;
  ClassCountMapType                 needed;
  switch (t)
  {
  case PROPORTIONAL:
  {
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      needed.clear();
      for (it = m_GlobalCountMap.begin(); it != m_GlobalCountMap.end(); ++it)
      {
        const MapRateType&          rates = this->GetNthElement(i)->GetRatesByClass();
        MapRateType::const_iterator curIt = rates.find(it->first);
        if (curIt != rates.end() && it->second > 0UL)
        {
          unsigned long curTotal = (curIt->second).Tot;
          needed[it->first] =
              static_cast<unsigned long>(std::floor(static_cast<double>(nb[0]) * static_cast<double>(curTotal) / static_cast<double>(it->second)));
        }
        else
        {
          needed[it->first] = 0UL;
        }
      }
      this->GetNthElement(i)->SetNbOfSamplesByClass(needed);
    }
    break;
  }
  case EQUAL:
  {
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetNbOfSamplesAllClasses(static_cast<unsigned long>(std::floor((double)nb[0] / (double)this->Size())));
    }
    break;
  }
  case CUSTOM:
  {
    if (nb.size() < this->Size())
    {
      itkGenericExceptionMacro("Not enough values present to set custom requested numbers in all inputs");
    }
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetNbOfSamplesAllClasses(nb[i]);
    }
    break;
  }
  default:
    itkGenericExceptionMacro("Unknown partition mode");
    break;
  }
}

void SamplingRateCalculatorList::SetNbOfSamplesByClass(const std::vector<ClassCountMapType>& required, PartitionType t)
{
  if (required.empty())
  {
    itkGenericExceptionMacro("No number of samples given");
  }
  this->UpdateGlobalCounts();
  ClassCountMapType::const_iterator it;
  ClassCountMapType::const_iterator inputIt;
  ClassCountMapType                 needed;
  switch (t)
  {
  case PROPORTIONAL:
  {
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      needed.clear();
      for (it = m_GlobalCountMap.begin(); it != m_GlobalCountMap.end(); ++it)
      {
        const MapRateType&          rates = this->GetNthElement(i)->GetRatesByClass();
        MapRateType::const_iterator curIt = rates.find(it->first);
        inputIt                           = required[0].find(it->first);
        if (curIt != rates.end() && inputIt != required[0].end() && it->second > 0UL)
        {
          unsigned long curTotal = (curIt->second).Tot;
          needed[it->first] =
              static_cast<unsigned long>(std::floor(static_cast<double>(inputIt->second) * static_cast<double>(curTotal) / static_cast<double>(it->second)));
        }
        else
        {
          needed[it->first] = 0UL;
        }
      }
      this->GetNthElement(i)->SetNbOfSamplesByClass(needed);
    }
    break;
  }
  case EQUAL:
  {
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      needed.clear();
      for (it = m_GlobalCountMap.begin(); it != m_GlobalCountMap.end(); ++it)
      {
        const MapRateType&          rates = this->GetNthElement(i)->GetRatesByClass();
        MapRateType::const_iterator curIt = rates.find(it->first);
        inputIt                           = required[0].find(it->first);
        if (curIt != rates.end() && inputIt != required[0].end() && it->second > 0UL)
        {
          unsigned long curTotal  = (curIt->second).Tot;
          unsigned long curNeeded = static_cast<unsigned long>(std::floor(static_cast<double>(inputIt->second) / static_cast<double>(this->Size())));
          needed[it->first]       = std::min(curTotal, curNeeded);
        }
        else
        {
          needed[it->first] = 0UL;
        }
      }
      this->GetNthElement(i)->SetNbOfSamplesByClass(needed);
    }
    break;
  }
  case CUSTOM:
  {
    if (required.size() < this->Size())
    {
      itkGenericExceptionMacro("Not enough values present to set custom requested numbers in all inputs");
    }
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetNbOfSamplesByClass(required[i]);
    }
    break;
  }
  default:
    itkGenericExceptionMacro("Unknown partition mode");
    break;
  }
}

void SamplingRateCalculatorList::SetPercentageOfSamples(std::vector<double>& p, PartitionType t)
{
  if (p.empty())
  {
    itkGenericExceptionMacro("No percentage given");
  }

  this->UpdateGlobalCounts();

  ClassCountMapType::const_iterator it;
  ClassCountMapType                 needed;
  switch (t)
  {
  case PROPORTIONAL:
  {
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetPercentageOfSamples(p[0]);
    }
    break;
  }
  case EQUAL:
  {
    needed.clear();
    for (it = m_GlobalCountMap.begin(); it != m_GlobalCountMap.end(); ++it)
    {
      needed[it->first] = static_cast<unsigned long>(std::floor(0.5 + it->second * p[0] / (double)this->Size()));
    }


    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetNbOfSamplesByClass(needed);
    }
    break;
  }
  case CUSTOM:
  {
    if (p.size() < this->Size())
    {
      itkGenericExceptionMacro("Not enough values present to set custom percents in all inputs");
    }
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetPercentageOfSamples(p[i]);
    }
    break;
  }
  default:
    itkGenericExceptionMacro("Unknown partition mode");
    break;
  }
}

void SamplingRateCalculatorList::SetTotalNumberOfSamples(std::vector<unsigned long>& tot, PartitionType t)
{
  if (tot.empty())
  {
    itkGenericExceptionMacro("No percentage given");
  }

  this->UpdateGlobalCounts();

  ClassCountMapType needed;
  switch (t)
  {
  case PROPORTIONAL:
  {
    unsigned long total_nb_samples = 0UL;

    std::vector<unsigned long> nb_samples(this->Size(), 0UL);

    for (unsigned int i = 0; i < this->Size(); i++)
    {
      // Compute the total number of samples for image i
      const MapRateType& rates = this->GetNthElement(i)->GetRatesByClass();

      for (MapRateType::const_iterator it = rates.begin(); it != rates.end(); ++it)
      {
        nb_samples[i] += it->second.Tot;
      }

      total_nb_samples += nb_samples[i];
    }

    const Superclass::InternalContainerSizeType sampleListSize = this->Size();
    for (unsigned int i = 0; i < sampleListSize; i++)
    {
      if (total_nb_samples != 0)
      {
        this->GetNthElement(i)->SetTotalNumberOfSamples(std::floor(0.5 + tot[0] * nb_samples[i] / static_cast<double>(total_nb_samples)));
      }
      else
      {
        this->GetNthElement(i)->SetTotalNumberOfSamples(0);
      }
    }
    break;
  }
  case EQUAL:
  {
    const Superclass::InternalContainerSizeType sampleListSize = this->Size();

    unsigned long total_by_image = 0;

    if (sampleListSize != 0)
    {
      total_by_image = static_cast<unsigned long>(std::floor(tot[0] / static_cast<double>(sampleListSize)));
    }

    for (unsigned int i = 0; i < sampleListSize; i++)
    {
      this->GetNthElement(i)->SetTotalNumberOfSamples(total_by_image);
    }
    break;
  }
  case CUSTOM:
  {
    if (tot.size() < this->Size())
    {
      itkGenericExceptionMacro("Not enough values present to set total samples in all inputs");
    }
    for (unsigned int i = 0; i < this->Size(); i++)
    {
      this->GetNthElement(i)->SetTotalNumberOfSamples(tot[i]);
    }
    break;
  }
  default:
    itkGenericExceptionMacro("Unknown partition mode");
    break;
  }
}


void SamplingRateCalculatorList::UpdateGlobalCounts()
{
  m_GlobalCountMap.clear();
  for (unsigned int i = 0; i < this->Size(); i++)
  {
    const MapRateType&          rates = this->GetNthElement(i)->GetRatesByClass();
    MapRateType::const_iterator it    = rates.begin();
    for (; it != rates.end(); ++it)
    {
      m_GlobalCountMap[it->first] += it->second.Tot;
    }
  }
}

} // end of namespace otb
