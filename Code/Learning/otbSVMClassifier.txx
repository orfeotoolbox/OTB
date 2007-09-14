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
#ifndef __otbSVMClassifier_txx
#define __otbSVMClassifier_txx

#include "otbSVMClassifier.h"
#include "otbMacro.h"

namespace otb{ 

template< class TSample, class TLabel >
SVMClassifier< TSample, TLabel >
::SVMClassifier()
{
  m_Output = OutputType::New() ;
  m_Model = SVMModelType::New();
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::Update()
{
  this->GenerateData();
}



template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::GenerateData()
{

  m_Output->SetSample(this->GetSample()) ;
  otbMsgDevMacro(  << "m_Output " << m_Output );

  m_Output->Resize( this->GetSample()->Size() ) ;

  
  otbMsgDevMacro(  << "Resize to " << this->GetSample()->Size() );
  otbMsgDevMacro(  << "Resize to " << m_Output->GetSample()->Size() );
  
  //std::vector< double > discriminantScores ;
  unsigned int numberOfClasses = this->GetNumberOfClasses() ;
  otbMsgDevMacro(  << "NbClass " << numberOfClasses );
  //discriminantScores.resize(numberOfClasses) ;
  //unsigned int classLabel ;

  m_Output->SetNumberOfClasses(numberOfClasses) ;


/*typename Superclass::DecisionRuleType::Pointer rule = 
    this->GetDecisionRule() ;*/

  otbMsgDevMacro(  << "Do Classif "  );
  this->DoClassification();
  otbMsgDevMacro(  << "End of classif" );  

}

template< class TSample, class TLabel >
typename SVMClassifier< TSample, TLabel >::OutputType*
SVMClassifier< TSample, TLabel >
::GetOutput() 
{
  return m_Output ;
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::DoClassification()
{

  typename TSample::ConstIterator iter = this->GetSample()->Begin() ;
  typename TSample::ConstIterator end  = this->GetSample()->End() ;

  typename OutputType::ConstIterator iterO = m_Output->Begin() ;
  typename OutputType::ConstIterator endO  = m_Output->End() ;
  typename TSample::MeasurementVectorType measurements ;


  int numberOfComponentsPerSample  = iter.GetMeasurementVector().Size() ;//this->GetSample().GetMeasurementVectorSize();//
 
  struct svm_node *x;
  bool predict_probability = 1;


  const struct svm_model* model = m_Model->GetModel();

//  char*  line = (char *) malloc(max_line_len*sizeof(char));
  //  x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct
  //  svm_node));

/*   m_Model->AllocateProblem(1, numberOfComponentsPerSample);*/

  x = new svm_node[numberOfComponentsPerSample+1];//m_Model->GetXSpace();

  otbMsgDevMacro(  << "XSpace Allocated" );
  if(svm_check_probability_model(model)==0)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,
			       "Model does not support probabiliy estimates",ITK_LOCATION);
    //    predict_probability=0;
    }

  int svm_type=svm_get_svm_type(model);
  otbMsgDevMacro(  << "SVM Type = " << svm_type );

  int nr_class=svm_get_nr_class(model);
  otbMsgDevMacro(  << "SVM nr_class = " << nr_class );

  int *labels=(int *) malloc(nr_class*sizeof(int));
  double *prob_estimates=NULL;
  
  if(predict_probability) 
    {
    if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
      printf("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
    else
      {

      svm_get_labels(model,labels);

      prob_estimates = (double *) malloc(nr_class*sizeof(double));
	}
    }

  otbMsgDevMacro(  << "Starting iterations " );
  while (iter != end && iterO != endO)
    {
    
    int i = 0;
    double v;

    measurements = iter.GetMeasurementVector() ;
    // otbMsgDevMacro(  << "Loop on components " << svm_type );
    for(i=0; i<numberOfComponentsPerSample; i++)
      {
      x[i].index = i+1 ;
      x[i].value = measurements[i];
	
      }
    x[i].index = -1;


    if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
      {
      v = svm_predict_probability(model,x,prob_estimates);
      }
    else
      {
      v = svm_predict(model,x);
      }
    

    ClassLabelType classLabel;
    classLabel = static_cast<ClassLabelType>(v);

  m_Output->AddInstance(classLabel, iterO.GetInstanceIdentifier()) ;

  ++iter;
  ++iterO;
        
}

if(predict_probability)
  {
  free(prob_estimates);
  free(labels);
  }
  

delete [] x;
}

} // end of namespace otb

#endif
