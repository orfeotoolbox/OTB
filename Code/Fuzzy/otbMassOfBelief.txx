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
#ifndef __otbMassOfBelief_txx
#define __otbMassOfBelief_txx

#include "otbMassOfBelief.h"

#include <algorithm>
#include <iterator>

#include "otbMath.h"

namespace otb
{

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::SetMass(const LabelSetType & labelSet, const MassType & mass)
  {
    // Insert into the mass map
    m_MassesMap[labelSet] = mass;

    // Call modified
    this->Modified();
  }

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::RemoveMass(const LabelSetType & labelSet)
  {
    // Insert into the mass map
    m_MassesMap.erase(labelSet);

    // Call modified
    this->Modified();
  }


template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::MassType
MassOfBelief<TLabel, TMass>
::GetMass(const LabelSetType & labelSet) const
  {
    // Look for mass in the table
    typename MassMapType::const_iterator it = m_MassesMap.find(labelSet);

    if(it!=m_MassesMap.end())
      {
      return it->second;
      }
    else
      {
      return itk::NumericTraits<MassType>::Zero;
      }
  }

template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::LabelSetOfSetType
MassOfBelief<TLabel, TMass>
::GetSupport() const
  {
    // Build the output
    LabelSetOfSetType output;

    // Define an iterator on the mass map
    typename MassMapType::const_iterator it = m_MassesMap.begin();

    // Walk the mass map, gathering the element of the power set
    while(it != m_MassesMap.end())
      {
      output.insert(it->first);
      ++it;
      }

    // Return
    return output;
  }

template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::LabelSetType
MassOfBelief<TLabel, TMass>
::GetUniverse() const
  {
    // Build the output
    LabelSetType output;

    // Retrieve support set
    LabelSetOfSetType support = this->GetSupport();

    // Walk the support and perform union
    for(typename LabelSetOfSetType::iterator it = support.begin();
        it != support.end(); ++it)
      {
      // Temporary set
      LabelSetType tempSet;
      std::insert_iterator<LabelSetType> tmpIt(tempSet, tempSet.begin());

      // Perform set union
      std::set_union(output.begin(), output.end(), it->begin(), it->end(), tmpIt);

      // swap output and tempSet
      output.swap(tempSet);
      }

    // Return
    return output;
  }

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::Normalize()
{
  // Sum of masses
  MassType sum = itk::NumericTraits<MassType>::Zero;

  // Compute normalization factor
  for(typename MassMapType::const_iterator it = m_MassesMap.begin();
      it != m_MassesMap.end(); ++it)
    {
    sum+=it->second;
    }

  // check if sum is not null (maybe use and 1e-xxx)
  if(sum > 0.0)
    {
    // Apply normalization factor
    for(typename MassMapType::iterator it = m_MassesMap.begin();
      it != m_MassesMap.end(); ++it)
      {
      it->second/=sum;
      }
    // Call modified
    this->Modified();
    }
}

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::EstimateUncertainty()
{
  // Retrieve the universe of the mass of belief
  LabelSetType universe = this->GetUniverse();

  // Compute the sum of available masses
  MassType sum = itk::NumericTraits<MassType>::Zero;

  // Compute normalization factor
  for(typename MassMapType::const_iterator it = m_MassesMap.begin();
      it != m_MassesMap.end(); ++it)
    {
    sum+=it->second;
    }

  // Compute uncertainty mass
  MassType uncertaintyMass = 1 - sum;

  // Check if uncertainty mass is not negative
  if(uncertaintyMass > 0)
    {
    // Associate uncertainty mass
    this->SetMass(universe, uncertaintyMass);

    // Call modified
    this->Modified();
    }
}

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::Clear()
{
  m_MassesMap.clear();
}

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::InitializePowerSetMasses(const LabelSetType & universe)
{
  // Clear any previous mass
  this->Clear();

  // Compute number of elements
  unsigned long nbElements = 1 << universe.size(); // 2^universe.size()


  // Build each element
  for(unsigned long elementId = 1; elementId <= nbElements; ++elementId)
    {
    // Instantiate a new element
    LabelSetType newElement;

    unsigned long residu = elementId;

    // Walk the universe set
    for(typename LabelSetType::const_iterator it = universe.begin();
        residu >0 && it!=universe.end(); ++it)
      {
      // Retrieve the current bit
      unsigned long bit = residu%2;

      // If bit is 1, add the current element from universe to the new set
      if(bit)
        {
        newElement.insert(*it);
        }

      // Compute residu
      residu/=2;
      }
    this->SetMass(newElement, itk::NumericTraits<MassType>::Zero);
    }
}

template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::MassType
MassOfBelief<TLabel, TMass>
::GetBelief(const LabelSetType & labelSet) const
{
  // Retrieve support of mass function
  LabelSetOfSetType support = this->GetSupport();

  // Define an empty set which will contain contained elements of the power-set
  LabelSetOfSetType containedSet;

  // Look for elements in the support which are contained in labelSet
  for(typename LabelSetOfSetType::const_iterator it = support.begin();
      it!=support.end(); ++it)
    {
    // Temporary set containing intersection
    LabelSetType intersectionSet;
    std::insert_iterator<LabelSetType> interIt(intersectionSet, intersectionSet.begin());

      // Perform set union
      std::set_intersection(labelSet.begin(), labelSet.end(), it->begin(), it->end(), interIt);

      // If labelSet inter (*it) == (*it), then (*it) is contained
      // inside labelSet
      if(intersectionSet == (*it))
        {
        containedSet.insert((*it));
        }
    }

  // Call the generic implementation
  return this->GetBelief(containedSet);
}

template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::MassType
MassOfBelief<TLabel, TMass>
::GetPlausibility(const LabelSetType & labelSet) const
{
  // Retrieve support of mass function
  LabelSetOfSetType support = this->GetSupport();

  // Define an empty set which will contain contained elements of the power-set
  LabelSetOfSetType intersectedSet;

  // Look for elements in the support which are contained in labelSet
  for(typename LabelSetOfSetType::const_iterator it = support.begin();
      it!=support.end(); ++it)
    {
    // Temporary set containing intersection
    LabelSetType intersectionSet;
    std::insert_iterator<LabelSetType> interIt(intersectionSet, intersectionSet.begin());

      // Perform set intersection
      std::set_intersection(labelSet.begin(), labelSet.end(), it->begin(), it->end(), interIt);

      // If labelSet inter (*it) != {}, then (*it) intersects labelSet
      if(!intersectionSet.empty())
        {
        intersectedSet.insert((*it));
        }
    }

  // Call the generic implementation
  return this->GetPlausibility(intersectedSet);
}

template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::MassType
MassOfBelief<TLabel, TMass>
::GetBelief(const LabelSetOfSetType & containedLabelSet) const
{
  // Define output
  MassType belief = itk::NumericTraits<MassType>::Zero;

  // Sum masses of contained set
  for(typename LabelSetOfSetType::const_iterator it = containedLabelSet.begin();
      it!=containedLabelSet.end(); ++it)
    {
    belief+=this->GetMass((*it));
    }

  // return belief
  return belief;
}

template <class TLabel, class TMass>
typename MassOfBelief<TLabel, TMass>
::MassType
MassOfBelief<TLabel, TMass>
::GetPlausibility(const LabelSetOfSetType & intersectedLabelSet) const
{
  // Define output
  MassType plausibility = itk::NumericTraits<MassType>::Zero;

  // Sum masses of contained set
  for(typename LabelSetOfSetType::const_iterator it = intersectedLabelSet.begin();
      it!=intersectedLabelSet.end(); ++it)
    {
    plausibility+=this->GetMass((*it));
    }

  // return belief
  return plausibility;
}

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::Copy(const Self * massOfBelief)
{
  // Swap content of masses maps
  m_MassesMap = massOfBelief->m_MassesMap;

  // Call to Modified()
  this->Modified();
}

template <class TLabel, class TMass>
bool
MassOfBelief<TLabel, TMass>
::IsEmpty() const
{
  return m_MassesMap.empty();
}

template <class TLabel, class TMass>
void
MassOfBelief<TLabel, TMass>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);

  // Display mass of belief universe
  os << indent << "Mass of belief universe: ";
  PrintLabelSet(os, this->GetUniverse());
  os << std::endl;

  // Display mass of belief support
  os << indent << "Mass of belief support: ";
  PrintLabelSetOfSet(os, this->GetSupport());
  os << std::endl;

  // Display individual masses
  for(typename MassMapType::const_iterator it = m_MassesMap.begin();
      it!=m_MassesMap.end(); ++it)
    {
    os<< indent;
    PrintLabelSet(os, it->first);
    os << " has mass " << (it->second) << std::endl;
    }
  os << indent << "Other masses are null" << std::endl;
}

template <class TLabel, class TMass>
std::ostream &
MassOfBelief<TLabel, TMass>
::PrintLabelSet(std::ostream & out,
                const LabelSetType & labelSet)
{
  // Define an iterator on the label set
  typename LabelSetType::const_iterator it = labelSet.begin();

  // Open the set
  out<<"{";

  // Append the set elements
  while(it != labelSet.end())
    {
    out<<(*it);
    ++it;
    if(it != labelSet.end())
      out<<", ";
    }

  // close the set
  out<<"}";

  // Return
  return out;
}

template <class TLabel, class TMass>
std::ostream &
MassOfBelief<TLabel, TMass>
::PrintLabelSetOfSet(std::ostream & out,
                     const LabelSetOfSetType & labelSet)
{
  // Define an iterator on the label set
  typedef std::set<TLabel> LabelSetType;
  typename LabelSetOfSetType::const_iterator it = labelSet.begin();

  // Open the set
  out<<"{";

  // Append the set elements
  while(it != labelSet.end())
    {
    PrintLabelSet(out, *it);
    ++it;
    if(it != labelSet.end())
      out<<", ";
    }

  // close the set
  out<<"}";

  // Return
  return out;
}


} // end namespace otb

/** Define the << operator for label sets */
/*
template <class TLabel>
std::ostream &
operator<<(std::ostream & out,
           const std::set<TLabel> & labelSet)*/


#endif
