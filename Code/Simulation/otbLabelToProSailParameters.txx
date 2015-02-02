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
#ifndef __otbLabelToProSailParameters_txx
#define __otbLabelToProSailParameters_txx

#include "itkNumericTraits.h"

#include "otbMacro.h"
#include "otbLabelToProSailParameters.h"

namespace otb
{

template <class TLabel>
LabelToProSailParameters<TLabel>
::LabelToProSailParameters()
{
  //default parameters
  ParametersType Step1Parameters;
  ParametersType Step2Parameters;

  Step1Parameters.SetSize(6);
  Step2Parameters.SetSize(8);

  Step1Parameters[0] = 30.0;
  Step1Parameters[1] = 10.0;
  Step1Parameters[2] = 0.0;
  Step1Parameters[3] = 0.015;
  Step1Parameters[4] = 0.009;
  Step1Parameters[5] = 1.2;

  Step2Parameters[0] = 2;
  Step2Parameters[1] = 50;
  Step2Parameters[2] = 1;
  Step2Parameters[3] = 70;
  Step2Parameters[4] = 0.2;
  Step2Parameters[5] = 30;
  Step2Parameters[6] = 0;
  Step2Parameters[7] = 0;

  this->SetStep1Parameters(Step1Parameters);
  this->SetStep2Parameters(Step2Parameters);
}

//template <class TLabel>
//const typename LabelToProSailParameters<TLabel>::ParametersType &
//LabelToProSailParameters<TLabel>
//::GetStep1Parameters()
//{
//   return this->m_Step1Parameters;
//}

//template <class TLabel>
//const typename LabelToProSailParameters<TLabel>::ParametersType &
//LabelToProSailParameters<TLabel>
//::GetStep2Parameters()
//{
//  return this->m_Step2Parameters;
//}


template <class TLabel>
void
LabelToProSailParameters<TLabel>
::GenerateData()
{
  ParametersType Step1Parameters=this->GetStep1Parameters();
  ParametersType Step2Parameters=this->GetStep2Parameters();
  otbMsgDevMacro(<<" Step1Parameters"<<Step1Parameters<<" Step2Parameters"<<Step2Parameters);
}


} // end namespace otb

#endif

