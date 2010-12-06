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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbFuzzyVariable.h"
#include "otbMacro.h"

namespace otb
{

template <unsigned int TNValues, class TPrecision>
FuzzyVariable<TNValues, TPrecision>::FuzzyVariable()
{
  this->m_Value = 0;
  for(unsigned int i=0; i<(4*TNValues); i++)
    m_MembershipFunctions[i] = static_cast<TPrecision>(0);
  this->UpdateMembershipValues();
}

template <unsigned int TNValues, class TPrecision>
void
FuzzyVariable<TNValues, TPrecision>::SetMembership(unsigned int var, TPrecision v1, TPrecision v2, TPrecision v3, TPrecision v4)
{
  if( v1>v2 || v2>v3 || v3>v4)
    itkExceptionMacro(<< "Values have to be v1<=v2<=v3<=v4");
  m_MembershipFunctions[var*4] = v1;
  m_MembershipFunctions[var*4+1] = v2;
  m_MembershipFunctions[var*4+2] = v3;
  m_MembershipFunctions[var*4+3] = v4;

  this->UpdateMembershipValues();
}

template <unsigned int TNValues, class TPrecision>
TPrecision
FuzzyVariable<TNValues, TPrecision>::GetMembership(unsigned int var, TPrecision val)
{

  this->m_Value = val;
  this->UpdateMembershipValues();
  return this->m_MembershipValues[var];

}


template <unsigned int TNValues, class TPrecision>
void
FuzzyVariable<TNValues, TPrecision>::UpdateMembershipValues()
{
  for(unsigned int i=0; i<TNValues; i++)
    {
    TPrecision v1 = this->m_MembershipFunctions[i*4+0];
    TPrecision v2 = this->m_MembershipFunctions[i*4+1];
    TPrecision v3 = this->m_MembershipFunctions[i*4+2];
    TPrecision v4 = this->m_MembershipFunctions[i*4+3];
    if( this->m_Value < v1 || this->m_Value > v4 )
      m_MembershipValues[i] = 0;
    if( this->m_Value >= v1 && this->m_Value < v2 )
      {
      if(v2>v1)
        m_MembershipValues[i] = static_cast<TPrecision>((this->m_Value - v1)/(v2 - v1));
      else m_MembershipValues[i] = static_cast<TPrecision>(1);
      }

    if( this->m_Value >= v2 && this->m_Value < v3 )
      {
      m_MembershipValues[i] = static_cast<TPrecision>(1);
      }

    if( this->m_Value >= v3 && this->m_Value < v4 )
      {
      std::cout << " here " << std::endl;
      if(v4>v3)
        m_MembershipValues[i] = static_cast<TPrecision>((v4 - this->m_Value)/(v4 - v3));
      else m_MembershipValues[i] = static_cast<TPrecision>(1);
      }
    
    }

}


template <unsigned int TNValues, class TPrecision>
void
FuzzyVariable<TNValues, TPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
