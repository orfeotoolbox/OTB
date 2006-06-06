/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Classifier
  $Id$


=========================================================================*/
#ifndef __otbSVMClassifier_txx
#define __otbSVMClassifier_txx

#include "otbSVMClassifier.h"

namespace otb{ 

template< class TSample, class TLabel >
SVMClassifier< TSample, TLabel >
::SVMClassifier()
{
  m_Sample = 0 ;
  m_Output = OutputType::New() ;
  m_Model = SVMModelType::New();
}

template< class TSample, class TLabel >
void
SVMClassifier< TSample, TLabel >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);

//   os << indent << "Sample: " ;
//   if ( m_Sample != 0 )
//     {
//     os << m_Sample << std::endl;
//     }
//   else
//     {
//     os << "not set." << std::endl ;
//     }

//   os << indent << "Output: " << m_Output << std::endl;
}

// template< class TSample, class TLabel >
// void
// SVMClassifier< TSample, TLabel >
// ::SetSample(const TSample* sample)
// {
//   std::cout << "SVMClassifier::SetSample enter"  << std::endl;
//   if ( m_Sample != sample )
//     {
//     m_Sample = sample ;
//     m_Output->SetSample(sample) ;
//     }
//   std::cout << "SVMClassifier::SetSample exit"  << std::endl;
// }

// template< class TSample, class TLabel >
// const TSample*
// SVMClassifier< TSample, TLabel >
// ::GetSample() const
// {
//   return m_Sample ;
// }

// template< class TSample, class TLabel >
// void
// SVMClassifier< TSample, TLabel >
// ::SetMembershipFunctionClassLabels(ClassLabelVectorType& labels)
// {
//   m_ClassLabels = labels ;
// }


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


  //std::cout << "Before Resize 0" <<  std::endl;
/*  unsigned int i ;
  typename TSample::ConstIterator iter = this->GetSample()->Begin() ;
  typename TSample::ConstIterator end  = this->GetSample()->End() ;
  typename TSample::MeasurementVectorType measurements ;
*/
  //std::cout << "Before Resize " <<  std::endl;
  m_Output->SetSample(this->GetSample()) ;
  //std::cout << "m_Output " << m_Output <<  std::endl;

  m_Output->Resize( this->GetSample()->Size() ) ;

  
  //std::cout << "Resize to " << this->GetSample()->Size() << std::endl;
  //std::cout << "Resize to " << m_Output->GetSample()->Size() << std::endl;
  
  //std::vector< double > discriminantScores ;
  unsigned int numberOfClasses = this->GetNumberOfClasses() ;
  //std::cout << "NbClass " << numberOfClasses << std::endl;
  //discriminantScores.resize(numberOfClasses) ;
  //unsigned int classLabel ;

  m_Output->SetNumberOfClasses(numberOfClasses) ;


/*typename Superclass::DecisionRuleType::Pointer rule = 
    this->GetDecisionRule() ;*/

  //std::cout << "Do Classif "  << std::endl;
  this->DoClassification();
  //std::cout << "End of classif" << std::endl;  

//   if ( m_ClassLabels.size() != this->GetNumberOfMembershipFunctions() )
//     {
//     while (iter != end)
//       {
//       measurements = iter.GetMeasurementVector() ;
//       for (i = 0 ; i < numberOfClasses ; i++)
//         {
//         discriminantScores[i] =
//           (this->GetMembershipFunction(i))->Evaluate(measurements) ;
//         }
//       classLabel = rule->Evaluate(discriminantScores) ;
//       m_Output->AddInstance(classLabel, iter.GetInstanceIdentifier()) ;
//       ++iter ;
//       }
//     }
//   else
//     {
//     while (iter != end)
//       {
//       measurements = iter.GetMeasurementVector() ;
//       for (i = 0 ; i < numberOfClasses ; i++)
//         {
//         discriminantScores[i] = 
//           (this->GetMembershipFunction(i))->Evaluate(measurements) ;
//         }
//       classLabel = rule->Evaluate(discriminantScores) ;
//       m_Output->AddInstance(m_ClassLabels[classLabel], 
//                             iter.GetInstanceIdentifier()) ;
//       ++iter ;
//       }
//     }
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

  int max_line_len = 1024;
  struct svm_node *x;
  int max_nr_attr = 64;
  bool predict_probability = 1;


  const struct svm_model* model = m_Model->GetModel();

//  char*  line = (char *) malloc(max_line_len*sizeof(char));
  //  x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct
  //  svm_node));

/*   m_Model->AllocateProblem(1, numberOfComponentsPerSample);*/

  x = new svm_node[numberOfComponentsPerSample+1];//m_Model->GetXSpace();

  //std::cout << "XSpace Allocated" << std::endl;
  if(svm_check_probability_model(model)==0)
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,
			       "Model does not support probabiliy estimates",ITK_LOCATION);
    //    predict_probability=0;
    }


  int correct = 0;
  int total = 0;
  double error = 0;
  double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;


  int svm_type=svm_get_svm_type(model);
  //std::cout << "SVM Type = " << svm_type << std::endl;

  int nr_class=svm_get_nr_class(model);
  //std::cout << "SVM nr_class = " << nr_class << std::endl;

  int *labels=(int *) malloc(nr_class*sizeof(int));
  double *prob_estimates=NULL;
  int j;
  
  if(predict_probability) 
    {
    if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
      printf("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",svm_get_svr_probability(model));
    else
      {

      svm_get_labels(model,labels);

      prob_estimates = (double *) malloc(nr_class*sizeof(double));
	}
      /*fprintf(output,"labels");		
      for(j=0;j<nr_class;j++)
	fprintf(output," %d",labels[j]);
      fprintf(output,"\n");*/
    }
//  while(1)

//  std::cout << "Starting iterations " << std::endl;
  while (iter != end && iterO != endO)
    {
    
    int i = 0;
    int c;
    double target,v;
    
    /*if (fscanf(input,"%lf",&target)==EOF)
      break;*/
    
//     while(1)
//       {
//       if(i>=max_nr_attr-1)	// need one more for index = -1
// 	{
// 	max_nr_attr *= 2;
// 	x = (struct svm_node *) realloc(x,max_nr_attr*sizeof(struct svm_node));
// 	}
      
//       do {
//       c = getc(input);
//       if(c=='\n' || c==EOF) goto out2;
//       } while(isspace(c));
//       ungetc(c,input);
//       fscanf(input,"%d:%lf",&x[i].index,&x[i].value);
//       ++i;
//       }
    
//     out2:
//     x[i++].index = -1;


    measurements = iter.GetMeasurementVector() ;
    //std::cout << "Loop on components " << svm_type << std::endl;
    for(i=0; i<numberOfComponentsPerSample; i++)
      {
      //std::cout << i << " " << measurements[i] << std::endl;
      //std::cout << "Index "<< x[i].index << std::endl;
      //std::cout << "Value "<< x[i].value << std::endl;
      x[i].index = i+1 ;
      x[i].value = measurements[i];
      //std::cout << "Index "<< x[i].index << std::endl;
      //std::cout << "Value "<< x[i].value << std::endl;
      //std::cout << "-------------------" << std::endl;
	
      }


    //std::cout << "Starting prediction" << std::endl;

    if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
      {
      //std::cout << "With predict" << std::endl;
      v = svm_predict_probability(model,x,prob_estimates);
      //std::cout << "Value : " << v << std::endl;
      /*fprintf(output,"%g ",v);
      for(j=0;j<nr_class;j++)
	fprintf(output,"%g ",prob_estimates[j]);
      fprintf(output,"\n");*/
      }
    else
      {
      //std::cout << "Without predict" << std::endl;
      v = svm_predict(model,x);
      //std::cout << "Value : " << v << std::endl;
      //fprintf(output,"%g\n",v);
      }
    

    ClassLabelType classLabel;
//     if(nr_class == 2)
//       classLabel = static_cast<ClassLabelType>(v+2);
//   else
    classLabel = static_cast<ClassLabelType>(v);

//  std::cout << "Add instance " << classLabel << std::endl;
//std::cout << "Add instance ident " << iterO.GetInstanceIdentifier() << std::endl;
m_Output->AddInstance(classLabel, iterO.GetInstanceIdentifier()) ;
//std::cout << "After add instance " << iterO.GetClassLabel() << std::endl;  


  ++iter;
  ++iterO;
    
    
}

//std::cout << "End of iterations " << std::endl;
if(predict_probability)
  {
  free(prob_estimates);
  free(labels);
  }
  
//std::cout << "End of iterations and free" << std::endl;  
//  free(x);

delete [] x;
}

} // end of namespace itk

#endif








