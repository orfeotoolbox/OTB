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
#ifndef __otbSVMModel_txx
#define __otbSVMModel_txx
#include "otbSVMModel.h"
#include "otbMacro.h"

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

namespace otb
{

template <class TInputPixel, class TLabel>
SVMModel< TInputPixel, TLabel >::SVMModel()
{
// FIXME:
  m_Model = Malloc(svm_model,1);

  m_Problem.l = 0;
  otbMsgDevMacro(  << "SVMModel::SVMModel - m_Problem.l = " <<  m_Problem.l );
  m_Problem.y = new double[1];
  m_Problem.x = new struct svm_node*[1];
  m_XSpace = new struct svm_node[1];
 

  //m_GenericKernelFunctor = NULL;
  m_Model->param.kernel_generic = NULL;
  m_Model->param.kernel_composed = NULL;
  m_Model->param.nr_weight = 0;
  m_Model->param.weight_label = NULL;
  m_Model->param.weight = NULL;
  m_Model->delete_composed = false;

  //m_GenericKernelFunctor = NULL;
  //m_Model->param.kernel_generic = NULL;
  //m_Model->param.nr_weight = 0;
  //m_Model->param.weight_label = NULL;
  //m_Model->param.weight = NULL;
  //m_Parameters.kernel_generic = NULL;
  //m_Parameters.nr_weight = 0;
  //m_Parameters.weight_label = NULL;
  //m_Parameters.weight = NULL;

}

template <class TInputPixel, class TLabel >
SVMModel<TInputPixel, TLabel>::~SVMModel()
{
// FIXME: pbs. when deleting de problem ....
  otbMsgDevMacro(  << "SVMModel destructor" );

  /*
  delete [] m_Problem.y;//free(m_Problem.y);
  otbMsgDevMacro(  << "SVMModel destructor - y done" );
  delete [] m_Problem.x; //free(m_Problem.x);
  otbMsgDevMacro(  << "SVMModel destructor - x done" );
  delete [] m_XSpace;//
  otbMsgDevMacro(  << "SVMModel destructor - m_XSpace done" );
  //free(m_XSpace);
*/

  //svm_destroy_model(m_Model);

}

template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::AllocateProblem(int l, long int elements)
 {
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - enter" );
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - l = " << l );
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - elements = " << elements );
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - m_Problem.l = " << m_Problem.l );
   m_Problem.l = l;
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - m_Problem.l = " <<  m_Problem.l );
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - l done" );
   delete [] m_Problem.y;
   m_Problem.y = new double[l];//Malloc(double,l);
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - y done" );
   delete [] m_Problem.x;
   m_Problem.x = new struct svm_node*[l];//Malloc(struct svm_node* ,l);
   otbMsgDevMacro(  << "SVMModel::AllocateProblem - x done" );
   delete [] m_XSpace;
   m_XSpace = new struct svm_node[elements];
   
   for(long int i = 0;i<elements;++i)
     {
       m_XSpace[i].value = 0;
       m_XSpace[i].index = -1;
     }
   
   //free(m_XSpace);
   //m_XSpace = Malloc(struct svm_node,elements);
   //otbMsgDevMacro(  << "SVMModel::AllocateProblem - m_XSpace done" );
   
 }


template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::SetModel(struct svm_model* aModel)
{
  otbMsgDevMacro(  << "SVMModel::SetModel - enter");
  //svm_destroy_model(m_Model);
  otbMsgDevMacro(  << "SVMModel::SetModel - destroyed" );
  m_Model = aModel;
  otbMsgDevMacro(  << "SVMModel::SetModel - out" );
}


/*template <class TInputPixel, class TLabel >
struct svm_problem&
SVMModel<TInputPixel, TLabel>
::GetProblem()
  {
    return m_Problem;
    
//     otbMsgDevMacro(  << "SVMModel::GetProblem - enter" );
//     aProblem.l = m_Problem.l;
//     aProblem.y = m_Problem.y;
//     aProblem.x = m_Problem.x;

//     otbMsgDevMacro(  << "SVMModel::GetProblem - m_XSpace " << m_XSpace );
// //    aNode = m_XSpace;
//     otbMsgDevMacro(  << "SVMModel::GetProblem - out" );
//     return m_XSpace;
  }

*/


template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::SaveModel(const char* model_file_name)
{
  if(svm_save_model(model_file_name, m_Model)!=0)
    {
      itkExceptionMacro( << "Problem while saving SVM model "
			 << std::string(model_file_name) );
    }
}

template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::LoadModel(const char* model_file_name)
{
  //m_Model = svm_load_model(model_file_name,m_Parameters.kernel_generic/*m_GenericKernelFunctor*/);
  m_Model = svm_load_model(model_file_name, m_Model->param.kernel_generic/*m_GenericKernelFunctor*/);
  if(m_Model == 0)
    {
      itkExceptionMacro( << "Problem while loading SVM model "
			 << std::string(model_file_name) );
    }
}

template <class TInputPixel, class TLabel >
typename SVMModel<TInputPixel, TLabel>::Pointer
SVMModel<TInputPixel, TLabel>
::GetCopy()
{
  Pointer modelCopy = New();
  modelCopy->SetModel( svm_copy_model(m_Model) ); 
  
  return modelCopy;
}


template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{  
  Superclass::PrintSelf(os,indent); 
}


template <class TInputPixel, class TLabel >
double 
SVMModel<TInputPixel, TLabel>
::Evaluate(void)
{
  return( svm_predict(m_Model,m_XSpace));
}

template <class TInputPixel, class TLabel >
typename SVMModel<TInputPixel, TLabel>::ValuesType 
SVMModel<TInputPixel, TLabel>
::EvaluateHyperplaneDistance(void)
{
  ValuesType values;
  
  values.SetSize( m_Model->nr_class*(m_Model->nr_class-1)/2);
  
  svm_predict_values(m_Model, m_XSpace, (double*)(values.GetDataPointer()));
  return (values);
}

// THE METHOD UPDATES THE l VALUE...
template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::SetSupportVectors(svm_node ** sv, int nbOfSupportVector)
{     
  // erase the old SV
  // delete just the first element, it destoyes the whole pointers (cf SV filling with x_space)
  delete [] (m_Model->SV[0]);

  for(int n = 0; n<m_Model->l; n++)
    {
      m_Model->SV[n] = NULL;
    }
  delete[] (m_Model->SV);
  m_Model->SV = NULL;

  this->SetNumberOfSupportVectors(nbOfSupportVector);
  m_Model->SV = Malloc(svm_node*,m_Model->l);

  // copy new SV values
  svm_node **SV = m_Model->SV;
  
  // Compute the total number of SV elements.
  unsigned int elements = 0;
  for (int p=0; p<nbOfSupportVector; p++)
    {
      std::cout<<p<<"  "; 
      const svm_node *tempNode = sv[p];
      std::cout<<p<<"  "; 
      while(tempNode->index != -1)
	{
	  tempNode++;
	  elements++;
	}
      elements++;// for -1 values
    }

  if(m_Model->l>0) 
      {
        SV[0] = Malloc(svm_node,elements);
	memcpy( SV[0],sv[0],sizeof(svm_node*)*elements);
      }
  svm_node *x_space =  SV[0];
  
  int j = 0;
  for(int i=0; i<m_Model->l; i++)
    {   
      // SV
      SV[i] = &x_space[j];   
      const svm_node *p = sv[i];
      svm_node *pCpy = SV[i];
      while(p->index != -1)
	{  
	  pCpy->index = p->index;
	  pCpy->value = p->value;
	  p++;
	  pCpy++;
	  j++;
	}
      pCpy->index = -1;
      j++;
    }
  this->Modified();
}

template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::SetAlpha( double ** alpha, int nbOfSupportVector )
{
  // Erase the old sv_coef
  for(int i=0; i<m_Model->nr_class-1; i++)
    {
      delete[] m_Model->sv_coef[i];
    }
  delete [] m_Model->sv_coef;
  
  this->SetNumberOfSupportVectors(nbOfSupportVector);
  
  // copy new sv_coef values
  m_Model->sv_coef = Malloc(double *,m_Model->nr_class-1);
  for(int i=0; i<m_Model->nr_class-1; i++)
    m_Model->sv_coef[i] = Malloc(double,m_Model->l);

  for(int i=0; i<m_Model->l; i++)
    { 
      // sv_coef
      for(int k=0; k<m_Model->nr_class-1; k++)
	{
	  m_Model->sv_coef[k][i] = alpha[k][i];
	}
    }
   this->Modified();
}


// FIXME
}// end namespace otb

#endif
