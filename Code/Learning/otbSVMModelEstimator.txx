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
#ifndef __otbSVMModelEstimator_txx
#define __otbSVMModelEstimator_txx

#include "otbSVMModelEstimator.h"
#include "otbMacro.h"
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

        m_Model->SetSVMType(C_SVC);
  m_Model->SetKernelType(LINEAR);
  m_Model->SetPolynomialKernelDegree(3);
  m_Model->SetKernelGamma(0.);  // 1/k
  m_Model->SetKernelCoef0(0);
        m_Model->SetKernelFunctor(NULL);
  m_Model->SetNu(0.5);
  m_Model->SetCacheSize(40);
  m_Model->SetC(1);
  m_Model->SetEpsilon(1e-3);
  m_Model->SetP(0.1);
  m_Model->DoShrinking(1);
  m_Model->DoProbabilityEstimates(true);

}


template<class InputPixelType, class LabelPixelType>
SVMModelEstimator<InputPixelType, LabelPixelType>
::~SVMModelEstimator(void)
{
   svm_destroy_param(&m_Model->GetParameters());
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
  Superclass::PrintSelf(os,indent);

}// end PrintSelf

template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::Update()
{
  GenerateData();
}


/**
 * Generate data (start the model building process)
 */
template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator<InputPixelType, LabelPixelType>
::GenerateData( )
{
  if(m_NumberOfClasses<2)
    {
      itkExceptionMacro(<<"Can not do SVM estimation with less than 2 classes");
    }

  if(m_Model->GetSVMType() == ONE_CLASS)
    {
      if(m_NumberOfClasses>2)
  {
    itkExceptionMacro(<<"Can not do ONE_CLASS SVM estimation with more than 2 classes");
  }
      if(m_Model->GetDoProbabilityEstimates())
  {
    otbMsgDebugMacro(<<"Disabling SVM probability estimates for ONE_CLASS SVM type.");
    m_Model->DoProbabilityEstimates(false);
  }
    }

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

  const char* error_msg = svm_check_parameter(&m_Model->GetProblem(),&m_Model->GetParameters());

  if(error_msg)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,error_msg,ITK_LOCATION);
    }

  otbMsgDebugMacro(  << "Starting training" );

  svm_model* tempModel = svm_train(&m_Model->GetProblem(),&m_Model->GetParameters());

  otbMsgDebugMacro(  << "Training done" );

  m_Model->SetModel(tempModel);
  m_Model->SetNumberOfClasses( this->GetNumberOfClasses() );

  otbMsgDebugMacro(  << "Training done" );

}// end train classifier


template<class InputPixelType, class LabelPixelType>
void
SVMModelEstimator< InputPixelType, LabelPixelType >
::PrepareData()
{
  int probl = m_Measures.size();

  long int elements = (m_Measures[0].size()+1)*probl;

  m_Model->AllocateProblem(probl, elements);
  struct svm_problem & prob = m_Model->GetProblem();
  struct svm_node *x_space = m_Model->GetXSpace();

   otbMsgDebugMacro(  << "x_space " <<  x_space );
   otbMsgDebugMacro(  << "prob = " << &prob );
   otbMsgDebugMacro(  << "prob.l = " << prob.l );

  long int j=0;
  long int i=0;
  long int max_index = 0;

  typename TrainingMeasuresType::iterator measIt = m_Measures.begin();
  typename TrainingMeasuresType::iterator measEnd = m_Measures.end();
  typename TrainingLabelsType::iterator labelsIt = m_Labels.begin();
  typename TrainingLabelsType::iterator labelsEnd = m_Labels.end();


  while(measIt!=measEnd && labelsIt!=labelsEnd)
    {

      double label = static_cast<double>(*labelsIt);
      prob.x[i] = &x_space[j];
      prob.y[i] = label;

      typename MeasurementVectorType::iterator compIt = (*measIt).begin();
      typename MeasurementVectorType::iterator compEnd = (*measIt).end();

      int k=0;

      while(compIt!=compEnd)
  {

  x_space[j].index = k+1;
  x_space[j].value = (*compIt);
  ++j;
  ++k;
  ++compIt;
  }
      if(j>=1 && x_space[j-1].index > max_index)
  max_index = x_space[j-1].index;
      x_space[j++].index = -1;
      ++i;

      ++measIt;
      ++labelsIt;


    }

  otbMsgDebugMacro(  << "Processed " << i << " examples" );

  if(m_Model->GetKernelGamma() == 0 && m_Model->GetParameters().kernel_type != COMPOSED && m_Model->GetParameters().kernel_type != GENERIC)
    m_Model->SetKernelGamma(1.0/static_cast<double>(max_index));
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
