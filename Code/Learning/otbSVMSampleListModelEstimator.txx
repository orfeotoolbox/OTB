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
         class TTrainingSampleList>
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList>
::SVMSampleListModelEstimator(void):  SVMModelEstimator<ITK_TYPENAME TInputSampleList::MeasurementType,
              ITK_TYPENAME TTrainingSampleList::MeasurementType>()

{


}


template<class TInputSampleList,
         class TTrainingSampleList>
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList>
::~SVMSampleListModelEstimator(void)
{
}

/*
 * PrintSelf
 */
template<class TInputSampleList,
         class TTrainingSampleList>
void
SVMSampleListModelEstimator<TInputSampleList, TTrainingSampleList>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  // FIXME : print useful SVM information
//   os << indent << "                   " << std::endl;
//   os << indent << "Gaussian Models generated from the training data." << std::endl;
//   os << indent << "TrainingSampleList: " ;
//   os << m_TrainingSampleList.GetPointer() << std::endl;
//   os << indent << "Results printed in the superclass " << std::endl;
//   os << indent << "                   " << std::endl;

  Superclass::PrintSelf(os,indent);

}// end PrintSelf


/**
 * Generate data (start the model building process)
 */


template<class TInputSampleList,
         class TTrainingSampleList>
void
SVMSampleListModelEstimator<TInputSampleList,  TTrainingSampleList>
::BuildProblem()
{

    //Do some error checking
  InputSampleListPointer  inputSampleList = this->GetInputSampleList();
  TrainingSampleListPointer  trainingSampleList = this->GetTrainingSampleList();


  int  inputSampleListSize = inputSampleList->Size();

  int trainingSampleListSize = trainingSampleList->Size();

  // Check if size of the two inputs are same
  if( inputSampleListSize != trainingSampleListSize ) throw itk::ExceptionObject(__FILE__, __LINE__,"Input pointset size is not the same as the training pointset size.",ITK_LOCATION);


  // Declaration of the iterators on the input and training images


  InputSampleListIteratorType inIt = inputSampleList->Begin();
  TrainingSampleListIteratorType trIt = trainingSampleList->Begin();

  InputSampleListIteratorType inEnd = inputSampleList->End();
  TrainingSampleListIteratorType trEnd = trainingSampleList->End();



  // Erase the vector contents
  this->m_Measures.resize(0);
  this->m_Labels.resize(0);


  otbMsgDebugMacro(  << " Input nb points " << inputSampleListSize );
  otbMsgDebugMacro(  << " Training nb points " << trainingSampleListSize );


 //  otbMsgDebugMacro(  << " Before while " );

  while(inIt!=inEnd && trIt!=trEnd)
    {

    // If label != 0

    typename TTrainingSampleList::MeasurementType label =
      trIt.GetMeasurementVector()[0];

    this->m_Labels.push_back(label);

    // otbMsgDebugMacro(  << " Label " << label );

    typename TInputSampleList::MeasurementVectorType value =
      inIt.GetMeasurementVector();

    typename Superclass::MeasurementVectorType v;

    typename TInputSampleList::MeasurementVectorType::ConstIterator pIt = value.Begin();
    typename TInputSampleList::MeasurementVectorType::ConstIterator pEnd = value.End();

    while(pIt!=pEnd)
      {
      v.push_back(*pIt);
      ++pIt;
      }

    this->m_Measures.push_back(v);

    ++inIt;
    ++trIt;
    }

  // otbMsgDebugMacro(  << " Before prepare data " );
  this->PrepareData();
//   otbMsgDebugMacro(  << " After prepare data " );
}



}//End namespace OTB
#endif
