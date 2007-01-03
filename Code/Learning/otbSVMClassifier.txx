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
//   otbMsgDevMacro(  << "SVMClassifier::SetSample enter" );
//   if ( m_Sample != sample )
//     {
//     m_Sample = sample ;
//     m_Output->SetSample(sample) ;
//     }
//   otbMsgDevMacro(  << "SVMClassifier::SetSample exit"  );
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


  otbMsgDevMacro(  << "Before Resize 0" );
/*  unsigned int i ;
  typename TSample::ConstIterator iter = this->GetSample()->Begin() ;
  typename TSample::ConstIterator end  = this->GetSample()->End() ;
  typename TSample::MeasurementVectorType measurements ;
*/
  otbMsgDevMacro(  << "Before Resize " );
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
      /*fprintf(output,"labels");		
      for(j=0;j<nr_class;j++)
	fprintf(output," %d",labels[j]);
      fprintf(output,"\n");*/
    }
//  while(1)

  otbMsgDevMacro(  << "Starting iterations " );
  while (iter != end && iterO != endO)
    {
    
    int i = 0;
    double v;
    
    /*
    double target;
    int c;
    if (fscanf(input,"%lf",&target)==EOF)
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
    otbMsgDevMacro(  << "Loop on components " << svm_type );
    for(i=0; i<numberOfComponentsPerSample; i++)
      {
      otbMsgDevMacro(  << i << " " << measurements[i] );
      otbMsgDevMacro(  << "Index "<< x[i].index );
      otbMsgDevMacro(  << "Value "<< x[i].value );
      x[i].index = i+1 ;
      x[i].value = measurements[i];
      otbMsgDevMacro(  << "Index "<< x[i].index );
      otbMsgDevMacro(  << "Value "<< x[i].value );
      otbMsgDevMacro(  << "-------------------" );
	
      }


    otbMsgDevMacro(  << "Starting prediction" );

    if (predict_probability && (svm_type==C_SVC || svm_type==NU_SVC))
      {
      otbMsgDevMacro(  << "With predict" );
      v = svm_predict_probability(model,x,prob_estimates);
      otbMsgDevMacro(  << "Value : " << v );
      /*fprintf(output,"%g ",v);
      for(j=0;j<nr_class;j++)
	fprintf(output,"%g ",prob_estimates[j]);
      fprintf(output,"\n");*/
      }
    else
      {
      otbMsgDevMacro(  << "Without predict" );
      v = svm_predict(model,x);
      otbMsgDevMacro(  << "Value : " << v );
      //fprintf(output,"%g\n",v);
      }
    

    ClassLabelType classLabel;
//     if(nr_class == 2)
//       classLabel = static_cast<ClassLabelType>(v+2);
//   else
    classLabel = static_cast<ClassLabelType>(v);

  otbMsgDevMacro(  << "Add instance " << classLabel ); 
  otbMsgDevMacro(  << "Add instance ident " << iterO.GetInstanceIdentifier() );
  m_Output->AddInstance(classLabel, iterO.GetInstanceIdentifier()) ;
  otbMsgDevMacro(  << "After add instance " << iterO.GetClassLabel() );  

  ++iter;
  ++iterO;
        
}

otbMsgDevMacro(  << "End of iterations " );
if(predict_probability)
  {
  free(prob_estimates);
  free(labels);
  }
  
otbMsgDevMacro(  << "End of iterations and free" );  
//  free(x);

delete [] x;
}

} // end of namespace otb

#endif








