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

#ifndef otbPointSetSource_hxx
#define otbPointSetSource_hxx

#include "otbPointSetSource.h"

namespace otb
{

/**
 *
 */
template <class TOutputPointSet>
PointSetSource<TOutputPointSet>::PointSetSource()
{
  // Create the output. We use static_cast<> here because we know the default
  // output must be of type TOutputPointSet
  OutputPointSetPointer output = static_cast<TOutputPointSet*>(this->MakeOutput(0).GetPointer());

  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput(0, output.GetPointer());

  m_GenerateDataRegion          = 0;
  m_GenerateDataNumberOfRegions = 0;
}

/**
 *
 */
template <class TOutputPointSet>
typename PointSetSource<TOutputPointSet>::DataObjectPointer PointSetSource<TOutputPointSet>::MakeOutput(DataObjectPointerArraySizeType)
{
  return static_cast<itk::DataObject*>(TOutputPointSet::New().GetPointer());
}

/**
 *
 */
template <class TOutputPointSet>
typename PointSetSource<TOutputPointSet>::OutputPointSetType* PointSetSource<TOutputPointSet>::GetOutput(void)
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return nullptr;
  }

  return static_cast<TOutputPointSet*>(this->ProcessObject::GetOutput(0));
}

/**
 *
 */
template <class TOutputPointSet>
typename PointSetSource<TOutputPointSet>::OutputPointSetType* PointSetSource<TOutputPointSet>::GetOutput(DataObjectPointerArraySizeType idx)
{
  return static_cast<TOutputPointSet*>(this->ProcessObject::GetOutput(idx));
}

/**
 *
 */
template <class TOutputPointSet>
void PointSetSource<TOutputPointSet>::SetOutput(OutputPointSetType* output)
{
  itkWarningMacro(
      << "SetOutput(): This method is slated to be removed from ITK.  Please use GraftOutput() in possible combination with DisconnectPipeline() instead.");
  this->ProcessObject::SetNthOutput(0, output);
}

/**
 *
 */
template <class TOutputPointSet>
void PointSetSource<TOutputPointSet>::GenerateInputRequestedRegion()
{
  Superclass::GenerateInputRequestedRegion();
}

/**
 *
 */
template <class TOutputPointSet>
void PointSetSource<TOutputPointSet>::GraftOutput(itk::DataObject* graft)
{
  this->GraftNthOutput(0, graft);
}

/**
 *
 */
template <class TOutputPointSet>
void PointSetSource<TOutputPointSet>::GraftNthOutput(DataObjectPointerArraySizeType idx, itk::DataObject* graft)
{
  if (idx >= this->GetNumberOfOutputs())
  {
    itkExceptionMacro(<< "Requested to graft output " << idx << " but this filter only has " << this->GetNumberOfOutputs() << " Outputs.");
  }

  if (!graft)
  {
    itkExceptionMacro(<< "Requested to graft output that is a NULL pointer");
  }

  itk::DataObject* output = this->GetOutput(idx);

  // Call Graft on the PointSet in order to copy meta-information, and containers.
  output->Graft(graft);
}

/**
 *
 */
template <class TOutputPointSet>
void PointSetSource<TOutputPointSet>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
