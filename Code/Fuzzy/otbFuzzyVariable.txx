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
#ifndef __otbFuzzyVariable_txx
#define __otbFuzzyVariable_txx


#include "otbFuzzyVariable.h"
#include "otbMacro.h"

namespace otb
{

template <class TLabel, class TPrecision>
FuzzyVariable<TLabel, TPrecision>::FuzzyVariable()
{}

template <class TLabel, class TPrecision>
void
FuzzyVariable<TLabel, TPrecision>::SetMembership(const LabelType& var,
                                                 const PrecisionType & v1,
                                                 const PrecisionType & v2,
                                                 const PrecisionType & v3,
                                                 const PrecisionType & v4)
{
  // Check if values are ordered correctly
  if( v1>v2 || v2>v3 || v3>v4)
    itkExceptionMacro(<< "Values have to be v1<=v2<=v3<=v4");

  // Build the membership parameters
  ParametersType parameters;
  parameters[0] = v1;
  parameters[1] = v2;
  parameters[2] = v3;
  parameters[3] = v4;
  parameters[4] = static_cast<TPrecision>(0);
  parameters[5] = static_cast<TPrecision>(1);

  // Insert it in the parameters map
  m_MembershipFunctions[var]=parameters;

  // Call modified
  this->Modified();
}

template <class TLabel, class TPrecision>
void
FuzzyVariable<TLabel, TPrecision>::SetMembership(const LabelType& var,
                                                 const PrecisionType & v1,
                                                 const PrecisionType & v2,
                                                 const PrecisionType & v3,
                                                 const PrecisionType & v4,
                                                 const PrecisionType & min,
                                                 const PrecisionType & max)
{
  // Check if values are ordered correctly
  if( v1>v2 || v2>v3 || v3>v4)
    itkExceptionMacro(<< "Values have to be v1<=v2<=v3<=v4");
  if( min>=max)
    itkExceptionMacro(<< "Values have to be min<max");

  // Build the membership parameters
  ParametersType parameters;
  parameters[0] = v1;
  parameters[1] = v2;
  parameters[2] = v3;
  parameters[3] = v4;
  parameters[4] = min;
  parameters[5] = max;

  // Insert it in the parameters map
  m_MembershipFunctions[var]=parameters;

  // Call modified
  this->Modified();
}

template <class TLabel, class TPrecision>
void
FuzzyVariable<TLabel, TPrecision>
::RemoveMembership(const LabelType & var)
{
  // Erase one parameter
  m_MembershipFunctions.erase(var);

  // Call Modified()
  this->Modified();
}

template <class TLabel, class TPrecision>
void
FuzzyVariable<TLabel, TPrecision>
::Clear()
{
  // Clear the membership parameters map
  m_MembershipFunctions.clear();

  // Call Modified()
  this->Modified();
}


template <class TLabel, class TPrecision>
typename FuzzyVariable<TLabel, TPrecision>
::PrecisionType
FuzzyVariable<TLabel, TPrecision>
::GetMembership(const LabelType & var, const PrecisionType & value) const
{
  // Declare output
  PrecisionType output = itk::NumericTraits<PrecisionType>::Zero;

  // Retrieve parameters for the given membership function
  typename ParametersMapType::const_iterator mapIt = m_MembershipFunctions.find(var);

  // If var exists in the parameters map
  if(mapIt!=m_MembershipFunctions.end())
    {
    // Retrieve parameters
    ParametersType parameters = mapIt->second;

    // Remaining of the code is trapezoidal function
    if( value < parameters[0] || value >= parameters[3] )
      {
      output = parameters[4];
      }
    else if( value >= parameters[0]
             && value < parameters[1] )
      {
      if(parameters[1]>parameters[0])
        {
        output = static_cast<TPrecision>(parameters[4] + (value - parameters[0])
                                         /(parameters[1] - parameters[0])
                                         *(parameters[5] - parameters[4]));
        }
      else
        {
        output = parameters[5];
        }
      }

    if( value >= parameters[1]
        && value < parameters[2] )
      {
      output = parameters[5];
      }

    if( value >= parameters[2]
        && value < parameters[3] )
      {
      if(parameters[3]>parameters[2])
        {
        output = static_cast<TPrecision>(parameters[4] + (parameters[3] - value)
                                         /(parameters[3] - parameters[2])
                                         *(parameters[5] - parameters[4]));
        }
      else
        {
        output = parameters[5];
        }
      }

    }

  // Return the membership value
  return output;
}

template <class TLabel, class TPrecision>
typename FuzzyVariable<TLabel, TPrecision>
::MembershipValueType
FuzzyVariable<TLabel, TPrecision>
::GetMembership(const PrecisionType & value) const
{
  // Build the output membership map
  MembershipValueType output;

  // Walk the membership parameters map
  for(typename ParametersMapType::const_iterator mapIt = m_MembershipFunctions.begin();
      mapIt!=m_MembershipFunctions.end(); ++mapIt)
    {
    // Compute the membership
    output[mapIt->first] = this->GetMembership(mapIt->first, value);
    }

  // Return output
  return output;
}

template <class TLabel, class TPrecision>
typename FuzzyVariable<TLabel, TPrecision>
::LabelType
FuzzyVariable<TLabel, TPrecision>
::GetMaxVar(const PrecisionType & value) const
{
  // If parameters map is empty, throw an exception
  if(m_MembershipFunctions.empty())
    {
    itkExceptionMacro(<<"Membership parameters map is empty");
    }

  // First retrieve all membership values
  MembershipValueType memberships = this->GetMembership(value);

  // Define an iterator on the membership parameters map
  typename MembershipValueType::const_iterator mapIt = memberships.begin();

  // Look for the higher value
  LabelType higherVar = mapIt->first;
  PrecisionType higherVal = mapIt->second;
  ++mapIt;

  while(mapIt!=memberships.end())
    {
    if(mapIt->second > higherVal)
      {
      higherVal = mapIt->second;
      higherVar = mapIt->first;
      }
    ++mapIt;
    }

  // Return the higher var
  return higherVar;
}

template <class TLabel, class TPrecision>
void
FuzzyVariable<TLabel, TPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TLabel, class TPrecision>
std::ostream &
FuzzyVariable<TLabel, TPrecision>
::PrintMembershipValueType(std::ostream & out,
                           const MembershipValueType & labelMap)
{
  // Define an iterator on the label set
  typedef std::map<TLabel, TPrecision> LabelMapType;
  typename LabelMapType::const_iterator it = labelMap.begin();

  // Open the set
  out<<"{";

  // Append the set elements
  while(it != labelMap.end())
    {
    out<<it->first<<" : "<<it->second;
    ++it;
    if(it != labelMap.end())
      out<<", ";
    }

  // close the set
  out<<"}";

  // Return
  return out;
}

} // end namespace otb


#endif
