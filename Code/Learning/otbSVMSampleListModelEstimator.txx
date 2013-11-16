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

#ifndef __otbSVMSampleListModelEstimator_txx
#define __otbSVMSampleListModelEstimator_txx

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
    return 0;
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
    return 0;
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
