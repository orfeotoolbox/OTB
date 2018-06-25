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

#ifndef otbSOMClassifier_hxx
#define otbSOMClassifier_hxx

#include "otbSOMClassifier.h"
#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template<class TSample, class TSOMMap, class TLabel>
SOMClassifier<TSample, TSOMMap, TLabel>
::SOMClassifier()
{
  this->SetNumberOfRequiredInputs(1);
  SOMMapPointerType sommap = SOMMapType::New();
  this->SetNthInput(0, sommap.GetPointer());
  m_Sample = SampleType::New();

  OutputPointerType outputPtr = OutputType::New();
  this->SetNthOutput(0, outputPtr.GetPointer());
}

template<class TSample, class TSOMMap, class TLabel>
typename SOMClassifier<TSample, TSOMMap, TLabel>
::OutputType *
SOMClassifier<TSample, TSOMMap, TLabel>
::GetOutput()
{
  return dynamic_cast<OutputType *>(this->itk::ProcessObject::GetOutput(0));
}

/**
 * SOMMap getter.
 */
template<class TSample, class TSOMMap, class TLabel>
typename SOMClassifier<TSample, TSOMMap, TLabel>
::SOMMapType *
SOMClassifier<TSample, TSOMMap, TLabel>
::GetMap(void)
{
  return dynamic_cast<SOMMapType *>(this->itk::ProcessObject::GetInput(0));
}
/**
 * SOMMap Setter.
 */
template<class TSample, class TSOMMap, class TLabel>
void
SOMClassifier<TSample, TSOMMap, TLabel>
::SetMap(SOMMapType * sommap)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<SOMMapType *>(sommap));
}
/**
 * PrintSelf method
 */
template<class TSample, class TSOMMap, class TLabel>
void
SOMClassifier<TSample, TSOMMap, TLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
/**
 * Main computation class.
 */
template<class TSample, class TSOMMap, class TLabel>
void
SOMClassifier<TSample, TSOMMap, TLabel>
::GenerateData()
{
  typename OutputType::Pointer outputPtr = this->GetOutput();
  outputPtr->SetSample(this->GetSample());
//  outputPtr->Resize(this->GetSample()->Size()); //FIXME Check if still required
  typename SOMMapType::SizeType size = this->GetMap()->GetLargestPossibleRegion().GetSize();
  unsigned int                  numberOfClasses = 1;
  for (unsigned int i = 0; i < SOMMapType::ImageDimension; ++i)
    {
    numberOfClasses *= size[i];
    }
  outputPtr->SetNumberOfClasses(numberOfClasses);

  typename TSample::Iterator iter = this->GetSample()->Begin();
  typename TSample::Iterator end  = this->GetSample()->End();

  typename OutputType::ConstIterator      iterO = outputPtr->Begin();
  typename OutputType::ConstIterator      endO  = outputPtr->End();
  typename TSample::MeasurementVectorType measurements;

  typename SOMMapType::IndexType index;

  SOMMapPointerType somMap = this->GetMap();

  otbMsgDebugMacro(<< "Starting iterations ");
  while (iter != end && iterO != endO)
    {

    measurements = iter.GetMeasurementVector();
    index = somMap->GetWinner(measurements);
    ClassLabelType classLabel = static_cast<ClassLabelType>((index[1] * size[1]) + index[0]);
    outputPtr->AddInstance(classLabel, iterO.GetInstanceIdentifier());
    ++iter;
    ++iterO;
    }
}
} // end of namespace otb

#endif
