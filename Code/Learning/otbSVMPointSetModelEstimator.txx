/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   4 May 2006
  Version   :   
  Role      :   SVM PointSet Model Estimator
  $Id$


=========================================================================*/
#ifndef _otbSVMPointSetModelEstimator_txx
#define _otbSVMPointSetModelEstimator_txx

#include "otbSVMPointSetModelEstimator.h"
#include "itkCommand.h"


namespace otb
{
template<class TInputPointSet, 
         class TTrainingPointSet>
SVMPointSetModelEstimator<TInputPointSet, TTrainingPointSet>
::SVMPointSetModelEstimator(void):  SVMModelEstimator<typename TInputPointSet::PixelType::value_type,
    typename TTrainingPointSet::PixelType>::SVMModelEstimator()

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
  

  std::cout << " Input nb points " << inputPointSetSize <<std::endl;
  std::cout << " Training nb points " << trainingPointSetSize <<std::endl;
  

  std::cout << " Before while " << std::endl;
  while(inIt!=inEnd && trIt!=trEnd)
    {

    // If label != 0 

      this->m_Labels.push_back(trIt.Value()[0]);

      std::cout << " Label " <<trIt.Value()[0] << std::endl;

      typename Superclass::MeasurementVectorType v;

      typename InputPointType::ConstIterator pIt = inIt.Value().Begin();
      typename InputPointType::ConstIterator pEnd = inIt.Value().End();

      while(pIt!=pEnd)
	{
	v.push_back(*pIt);
	++pIt;
	}

      this->m_Measures.push_back(v);

    ++inIt;
    ++trIt;
    }

  std::cout << " Before prepare data " << std::endl;
  this->PrepareData();
  std::cout << " After prepare data " << std::endl;
}



}//End namespace OTB
#endif
