/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   4 May 2006
  Version   :   
  Role      :   SVM Model Estimator
  $Id$


=========================================================================*/
#ifndef _otbSVMModelEstimator_txx
#define _otbSVMModelEstimator_txx

#include "otbSVMModelEstimator.h"
#include "itkCommand.h"


namespace otb
{
template<class InputPixelType, class LabelPixelType>
SVMModelEstimator<InputPixelType, LabelPixelType>
::SVMModelEstimator(void):
m_NumberOfClasses( 0 )
{
  // FIXME initialize SVMModel

  m_Model = SVMModelType::New();

  m_Done = 0;
  // default values
  param.svm_type = C_SVC;
  param.kernel_type = LINEAR;
  param.degree = 3;
  param.gamma = 0;	// 1/k
  param.coef0 = 0;
  param.nu = 0.5;
  param.cache_size = 400;
  param.C = 1;
  param.eps = 1e-3;
  param.p = 0.1;
  param.shrinking = 1;
  param.probability = 1;
  param.nr_weight = 0;
  param.weight_label = NULL;
  param.weight = NULL;
  //cross_validation = 0;
}


template<class InputPixelType, class LabelPixelType>
SVMModelEstimator<InputPixelType, LabelPixelType>
::~SVMModelEstimator(void)
{
  
   svm_destroy_param(&param);

}

/*
 * PrintSelf
 */
template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{  
  // FIXME : print useful SVM information
//   os << indent << "                   " << std::endl;
//   os << indent << "Gaussian Models generated from the training data." << std::endl;
//   os << indent << "TrainingImage: " ;
//   os << m_TrainingImage.GetPointer() << std::endl;
//   os << indent << "Results printed in the superclass " << std::endl;
//   os << indent << "                   " << std::endl;

  Superclass::PrintSelf(os,indent);

}// end PrintSelf

template<class InputPixelType, class LabelPixelType>
void 
SVMModelEstimator<InputPixelType, LabelPixelType>
::Update()
{
  GenerateData() ;
}


/**
 * Generate data (start the model building process)
 */
template<class InputPixelType, class LabelPixelType>
void 
SVMModelEstimator<InputPixelType, LabelPixelType>
::GenerateData( )
{
  if(!m_Done)
    {
    m_Done = 1;
    this->EstimateModels();
    }

}// end Generate data


template<class InputPixelType, class LabelPixelType>
void 
SVMModelEstimator<InputPixelType, LabelPixelType>
::EstimateModels()
{
  // FIXME : the core of the model estimation


  //-------------------------------------------------------------------
  // Build the SVM problem
  //-------------------------------------------------------------------



  this->BuildProblem();
  
  const char* error_msg = svm_check_parameter(&prob,&param);

  if(error_msg)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,error_msg,ITK_LOCATION);
    }

  //std::cout << "Starting training" << std::endl;

  svm_model* tempModel = svm_train(&prob,&param);

  //std::cout << "Training done" << std::endl;
    
  m_Model->SetModel(tempModel);
  
  //std::cout << "Training done" << std::endl;
    

}// end train classifier 


template<class InputPixelType, class LabelPixelType>
void 
SVMModelEstimator< InputPixelType, LabelPixelType >
::PrepareData()
{

  
  int probl = m_Measures.size();  

  long int elements = (m_Measures[0].size()+1)*probl;


  m_Model->AllocateProblem(probl, elements);

  //struct svm_problem prob;
  //struct svm_node *x_space;

  prob = m_Model->GetProblem();
  x_space = m_Model->GetXSpace();

  //std::cout << "x_space " <<  x_space << std::endl;

  //std::cout << "prob = " << &prob << std::endl;
  //std::cout << "prob.l = " << prob.l << std::endl;

  long int j=0;
  long int i=0;
  long int max_index = 0;

  typename TrainingMeasuresType::iterator measIt = m_Measures.begin();
  typename TrainingMeasuresType::iterator measEnd = m_Measures.end();
  typename TrainingLabelsType::iterator labelsIt = m_Labels.begin();
  typename TrainingLabelsType::iterator labelsEnd = m_Labels.end();


  //std::cout << " Before while " << std::endl;
  while(measIt!=measEnd && labelsIt!=labelsEnd)
    {

      double label = static_cast<double>(*labelsIt);
/*      std::cout << label << std::endl;
      std::cout << prob.x[i] << std::endl;
      std::cout << prob.y[i] << std::endl;
      std::cout << &x_space[j] << std::endl;*/
      prob.x[i] = &x_space[j];
      prob.y[i] = label;

      std::cout << "Label " << label << " " << i <<"/" << probl<<std::endl;

      typename MeasurementVectorType::iterator compIt = (*measIt).begin();
      typename MeasurementVectorType::iterator compEnd = (*measIt).end();

      int k=0;
      
      while(compIt!=compEnd)
	{
/*	std::cout << "Index " << x_space[j].index << std::endl;	
	std::cout << "Value " << x_space[j].value << std::endl;*/
	x_space[j].index = k+1;
	x_space[j].value = (*compIt);
	std::cout << x_space[j].index << ":" << x_space[j].value << " ";
	std::cout << "j: " << j << " " << std::endl;
	++j;
	++k;
	++compIt;
	}
      if(j>=1 && x_space[j-1].index > max_index)
	max_index = x_space[j-1].index;
      x_space[j++].index = -1;
      //    std::cout << std::endl;
      ++i;

      ++measIt;
      ++labelsIt;
	
		  
    }

  std::cout << "Processed " << i << " examples" << std::endl;

  if(param.gamma == 0)
    param.gamma = 1.0/max_index;

}

template<class InputPixelType, class LabelPixelType >
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::SaveModel(const char* model_file_name)
  {
    m_Model->SaveModel(model_file_name);
  }


}//End namespace OTB
#endif
