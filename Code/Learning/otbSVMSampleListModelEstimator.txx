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
::SVMSampleListModelEstimator(void):  SVMModelEstimator<ITK_TYPENAME TInputSampleList::MeasurementType,
    ITK_TYPENAME TTrainingSampleList::MeasurementType>()
{}

template<class TInputSampleList,
class TTrainingSampleList, class TMeasurementFunctor>
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::~SVMSampleListModelEstimator(void)
{}

/*
 * PrintSelf
 */
template<class TInputSampleList,
class TTrainingSampleList, class TMeasurementFunctor>
void
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList, TMeasurementFunctor>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
}// end PrintSelf


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
  InputSampleListPointer  inputSampleList = this->GetInputSampleList();
  TrainingSampleListPointer  trainingSampleList = this->GetTrainingSampleList();
  typename Superclass::ModelType * model = this->GetModel();

  int  inputSampleListSize = inputSampleList->Size();
  int trainingSampleListSize = trainingSampleList->Size();

  // Check if size of the two inputs are same
  if ( inputSampleListSize != trainingSampleListSize ) throw itk::ExceptionObject(__FILE__, __LINE__,"Input pointset size is not the same as the training pointset size.",ITK_LOCATION);
  
  // Declaration of the iterators on the input and training images
  InputSampleListIteratorType inIt = inputSampleList->Begin();
  TrainingSampleListIteratorType trIt = trainingSampleList->Begin();

  InputSampleListIteratorType inEnd = inputSampleList->End();
  TrainingSampleListIteratorType trEnd = trainingSampleList->End();

  // Clear previous samples
  model->ClearSamples();

  otbMsgDebugMacro(  << " Input nb points " << inputSampleListSize );
  otbMsgDebugMacro(  << " Training nb points " << trainingSampleListSize );

  MeasurementFunctorType mfunctor;
  while (inIt!=inEnd && trIt!=trEnd)
  {
    typename TTrainingSampleList::MeasurementType label =
      trIt.GetMeasurementVector()[0];
    typename TInputSampleList::MeasurementVectorType value =
      inIt.GetMeasurementVector();
    model->AddSample(mfunctor(value),label);
    ++inIt;
    ++trIt;
  }
}
}//End namespace OTB
#endif
