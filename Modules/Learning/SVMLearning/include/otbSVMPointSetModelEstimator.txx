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

#ifndef otbSVMPointSetModelEstimator_txx
#define otbSVMPointSetModelEstimator_txx

#include "otbSVMPointSetModelEstimator.h"
#include "itkCommand.h"
#include "otbMacro.h"

namespace otb
{
template<class TInputPointSet,
    class TTrainingPointSet>
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::SVMPointSetModelEstimator(void) :  SVMModelEstimator<typename TInputPointSet::PixelType::value_type,
      typename TTrainingPointSet::PixelType>()
{
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputPointSet,
    class TTrainingPointSet>
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::~SVMPointSetModelEstimator(void)
{}

/*
 * PrintSelf
 */
template<class TInputPointSet,
    class TTrainingPointSet>
void
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
} // end PrintSelf

template<class TInputPointSet, class TTrainingPointSet>
void
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::SetInputPointSet(const TInputPointSet * inputPointSet)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<TInputPointSet *>(inputPointSet));
}

template<class TInputPointSet, class TTrainingPointSet>
void
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::SetTrainingPointSet(const TTrainingPointSet * trainingPointSet)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<TTrainingPointSet*>(trainingPointSet));
}

template<class TInputPointSet, class TTrainingPointSet>
const TInputPointSet *
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::GetInputPointSet()
{
  if (this->GetNumberOfInputs() < 1)
    {
    return ITK_NULLPTR;
    }
  return static_cast<TInputPointSet *>(this->itk::ProcessObject::GetInput(0));
}

template<class TInputPointSet, class TTrainingPointSet>
const TTrainingPointSet *
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::GetTrainingPointSet()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }
  return static_cast<TTrainingPointSet *>(this->itk::ProcessObject::GetInput(1));
}

/**
 * Generate data (start the model building process)
 */
template<class TInputPointSet,
    class TTrainingPointSet>
void
SVMPointSetModelEstimator<TInputPointSet,  TTrainingPointSet>
::PrepareData()
{

  //Do some error checking
  const TInputPointSet *           inputPointSet = this->GetInputPointSet();
  const TTrainingPointSet *        trainingPointSet = this->GetTrainingPointSet();
  typename Superclass::ModelType * model = this->GetModel();

  int inputPointSetSize = inputPointSet->GetNumberOfPoints();
  int trainingPointSetSize = trainingPointSet->GetNumberOfPoints();

  // Check if size of the two inputs are same
  if (inputPointSetSize != trainingPointSetSize)
    throw itk::ExceptionObject(
      __FILE__,
      __LINE__,
      "Input pointset size is not the same as the training pointset size.",
      ITK_LOCATION);

  // Declaration of the iterators on the input and training images
  InputPointSetIteratorType    inIt = inputPointSet->GetPoints()->Begin();
  TrainingPointSetIteratorType trIt = trainingPointSet->GetPoints()->Begin();

  InputPointSetIteratorType    inEnd = inputPointSet->GetPoints()->End();
  TrainingPointSetIteratorType trEnd = trainingPointSet->GetPoints()->End();

  // Erase previous samples
  model->ClearSamples();

  otbMsgDevMacro(<< " Input nb points " << inputPointSetSize);
  otbMsgDevMacro(<< " Training nb points " << trainingPointSetSize);

  unsigned int dataId = 0;
  while (inIt != inEnd && trIt != trEnd)
    {
    typename TTrainingPointSet::PixelType label = itk::NumericTraits<typename TTrainingPointSet::PixelType>::Zero;
    trainingPointSet->GetPointData(dataId, &label);

    otbMsgDevMacro(<< " Label " << label);

    typename TInputPointSet::PixelType value;
    inputPointSet->GetPointData(dataId, &value);

    typename Superclass::ModelType::MeasurementType v;

    typename TInputPointSet::PixelType::iterator pIt = value.begin();
    typename TInputPointSet::PixelType::iterator pEnd = value.end();

    while (pIt != pEnd)
      {
      v.push_back(*pIt);
      ++pIt;
      }

    model->AddSample(v, label);

    ++inIt;
    ++trIt;
    ++dataId;
    }
}
} //End namespace OTB
#endif
