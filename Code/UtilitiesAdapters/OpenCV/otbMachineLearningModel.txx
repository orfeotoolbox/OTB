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
#ifndef __otbMachineLearningModel_txx
#define __otbMachineLearningModel_txx

#include "otbMachineLearningModel.h"

namespace otb 
{

template <class TInputValue, class TOutputValue>
MachineLearningModel<TInputValue,TOutputValue>
::MachineLearningModel()
{}


template <class TInputValue, class TOutputValue>
MachineLearningModel<TInputValue,TOutputValue>
::~MachineLearningModel()
{}

template <class TInputValue, class TOutputValue>
void
MachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}
}

#endif
