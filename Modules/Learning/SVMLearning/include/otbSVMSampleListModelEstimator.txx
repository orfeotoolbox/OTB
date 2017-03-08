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


#ifndef otbSVMSampleListModelEstimator_txx
#define otbSVMSampleListModelEstimator_txx

#include "otbSVMSampleListModelEstimator.h"
#include "itkCommand.h"
#include "otbMacro.h"

namespace otb
{
template<class TInputSampleList,
    class TTrainingSampleList, class TMeasurementFunctor>
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::SVMSampleListModelEstimator(void) :  SVMModelEstimator<typename TInputSampleList::MeasurementType,
      typename TTrainingSampleList::MeasurementType>()
{
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputSampleList,
    class TTrainingSampleList, class TMeasurementFunctor>
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::~SVMSampleListModelEstimator(void)
{}

//Set the input sample list
template<class TInputSampleList,
         class TTrainingSampleList,
         class TMeasurementFunctor>
void
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::SetInputSampleList( const InputSampleListType* inputSampleList )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputSampleListType* >(inputSampleList) );
}


// Set the label sample list
template<class TInputSampleList,
         class TTrainingSampleList,
         class TMeasurementFunctor>
void
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::SetTrainingSampleList(const TrainingSampleListType* trainingSampleList)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<TrainingSampleListType*>(trainingSampleList) );
}

// Get the input sample list
template<class TInputSampleList,
         class TTrainingSampleList,
         class TMeasurementFunctor>
const typename SVMSampleListModelEstimator<TInputSampleList,
                                           TTrainingSampleList,
                                           TMeasurementFunctor>::InputSampleListType *
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::GetInputSampleList( )
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const InputSampleListType* >
    (this->itk::ProcessObject::GetInput(0) );
}

// Get the input label sample list
template<class TInputSampleList,
         class TTrainingSampleList,
         class TMeasurementFunctor>
const typename SVMSampleListModelEstimator<TInputSampleList,
                                           TTrainingSampleList,
                                           TMeasurementFunctor>::TrainingSampleListType *
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::GetTrainingSampleList( )
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const TrainingSampleListType* >
    (this->itk::ProcessObject::GetInput(1));
}


/*
 * PrintSelf
 */
template<class TInputSampleList,
    class TTrainingSampleList, class TMeasurementFunctor>
void
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
} // end PrintSelf

/**
 * Generate data (start the model building process)
 */
template<class TInputSampleList,
    class TTrainingSampleList, class TMeasurementFunctor>
void
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::PrepareData()
{
  //Do some error checking
  InputSampleListPointer           inputSampleList    = const_cast<InputSampleListType*>(this->GetInputSampleList());
  TrainingSampleListPointer        trainingSampleList = const_cast<TrainingSampleListType*>(this->GetTrainingSampleList());
  typename Superclass::ModelType * model              = this->GetModel();

  int inputSampleListSize = inputSampleList->Size();
  int trainingSampleListSize = trainingSampleList->Size();

  // Check if size of the two inputs are same
  if (inputSampleListSize != trainingSampleListSize)
    {
    /*throw itk::ExceptionObject(
      __FILE__,
      __LINE__,
      "Input pointset size is not the same as the training pointset size.",
      ITK_LOCATION); */
    itkExceptionMacro(<< "Input pointset size is not the same as the training pointset size ("
                      << inputSampleListSize << " vs "<< trainingSampleListSize << ").");
    }

  // Declaration of the iterators on the input and training images
  InputSampleListIteratorType    inIt = inputSampleList->Begin();
  TrainingSampleListIteratorType trIt = trainingSampleList->Begin();

  InputSampleListIteratorType    inEnd = inputSampleList->End();
  TrainingSampleListIteratorType trEnd = trainingSampleList->End();

  // Clear previous samples
  model->ClearSamples();

  otbMsgDebugMacro(<< " Input nb points " << inputSampleListSize);
  otbMsgDebugMacro(<< " Training nb points " << trainingSampleListSize);

  MeasurementFunctorType mfunctor;
  while (inIt != inEnd && trIt != trEnd)
    {
    typename TTrainingSampleList::MeasurementType label =
      trIt.GetMeasurementVector()[0];
    typename TInputSampleList::MeasurementVectorType value =
      inIt.GetMeasurementVector();
    model->AddSample(mfunctor(value), label);
    ++inIt;
    ++trIt;
    }
}
} //End namespace OTB
#endif
