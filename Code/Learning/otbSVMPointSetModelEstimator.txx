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
#ifndef _otbSVMPointSetModelEstimator_txx
#define _otbSVMPointSetModelEstimator_txx

#include "otbSVMPointSetModelEstimator.h"
#include "itkCommand.h"
#include "otbMacro.h"

namespace otb
{
template<class TInputPointSet, 
         class TTrainingPointSet>
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::SVMPointSetModelEstimator(void):  SVMModelEstimator<ITK_TYPENAME TInputPointSet::PixelType::value_type,
    ITK_TYPENAME TTrainingPointSet::PixelType>()

{


}


template<class TInputPointSet, 
         class TTrainingPointSet>
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::~SVMPointSetModelEstimator(void)
{
}

/*
 * PrintSelf
 */
template<class TInputPointSet, 
         class TTrainingPointSet>
void
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{  
  // FIXME : print useful SVM information
//   os << indent << "                   " << std::endl;
//   os << indent << "Gaussian Models generated from the training data." << std::endl;
//   os << indent << "TrainingPointSet: " ;
//   os << m_TrainingPointSet.GetPointer() << std::endl;
//   os << indent << "Results printed in the superclass " << std::endl;
//   os << indent << "                   " << std::endl;

  Superclass::PrintSelf(os,indent);

}// end PrintSelf


/**
 * Generate data (start the model building process)
 */


template<class TInputPointSet, 
         class TTrainingPointSet>
void
SVMPointSetModelEstimator<TInputPointSet,  TTrainingPointSet>
::BuildProblem()
{

    //Do some error checking
  InputPointSetPointer  inputPointSet = this->GetInputPointSet();
  TrainingPointSetPointer  trainingPointSet = this->GetTrainingPointSet();

  // Check if the training and input image dimensions are same
  if( (int)(TInputPointSet::PointType::Dimension) != (int)(TTrainingPointSet::PointType::Dimension) )
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,"Training and input pointsets dimensions are not the same.",ITK_LOCATION);
    }

  int  inputPointSetSize = inputPointSet->GetNumberOfPoints();

  int trainingPointSetSize = trainingPointSet->GetNumberOfPoints();

  // Check if size of the two inputs are same
  if( inputPointSetSize != trainingPointSetSize ) throw itk::ExceptionObject(__FILE__, __LINE__,"Input pointset size is not the same as the training pointset size.",ITK_LOCATION); 
  

  // Declaration of the iterators on the input and training images


  InputPointSetIteratorType inIt = inputPointSet->GetPoints()->Begin();
  TrainingPointSetIteratorType trIt = trainingPointSet->GetPoints()->Begin();

  InputPointSetIteratorType inEnd = inputPointSet->GetPoints()->End();
  TrainingPointSetIteratorType trEnd = trainingPointSet->GetPoints()->End();


  
  // Erase the vector contents
  this->m_Measures.resize(0);
  this->m_Labels.resize(0);
  

  otbMsgDevMacro(  << " Input nb points " << inputPointSetSize );
  otbMsgDevMacro(  << " Training nb points " << trainingPointSetSize );
  

  otbMsgDevMacro(  << " Before while " );

  unsigned int dataId = 0;
  while(inIt!=inEnd && trIt!=trEnd)
    {

    // If label != 0 

    typename TTrainingPointSet::PixelType label;
    trainingPointSet->GetPointData( dataId, & label );   
    this->m_Labels.push_back(label);

      otbMsgDevMacro(  << " Label " << label );

      typename TInputPointSet::PixelType value;
      inputPointSet->GetPointData( dataId, & value );   

      typename Superclass::MeasurementVectorType v;

      typename TInputPointSet::PixelType::iterator pIt = value.begin();
      typename TInputPointSet::PixelType::iterator pEnd = value.end();

      while(pIt!=pEnd)
	{
	v.push_back(*pIt);
	++pIt;
	}

      this->m_Measures.push_back(v);

    ++inIt;
    ++trIt;
    ++dataId;
    }

  otbMsgDevMacro(  << " Before prepare data " );
  this->PrepareData();
  otbMsgDevMacro(  << " After prepare data " );
}



}//End namespace OTB
#endif
