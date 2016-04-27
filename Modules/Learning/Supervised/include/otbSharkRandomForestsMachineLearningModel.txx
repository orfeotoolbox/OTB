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
#ifndef __otbSharkRandomForestsMachineLearningModel_txx
#define __otbSharkRandomForestsMachineLearningModel_txx

#include <fstream>
#include "itkMacro.h"
#include "otbSharkRandomForestsMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::SharkRandomForestsMachineLearningModel()
{
  this->m_ConfidenceIndex = true;
  this->m_IsRegressionSupported = true;
}


template <class TInputValue, class TOutputValue>
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::~SharkRandomForestsMachineLearningModel()
{
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Train()
{

}

template <class TInputValue, class TOutputValue>
typename SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & value, ConfidenceValueType *quality) const
{

  return SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>::TargetSampleType{};
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
}

template <class TInputValue, class TOutputValue>
bool
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{

  return true;
}

template <class TInputValue, class TOutputValue>
bool
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
