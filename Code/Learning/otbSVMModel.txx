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
  otbMsgDebugMacro(  << "SVMModel::SVMModel - m_Problem.l = " <<  m_Problem.l );
  m_Problem.y = new double[1];
  m_Problem.x = new struct svm_node*[1];
  x_space = new struct svm_node[1];
  m_GenericKernelFunctor = NULL;

}

template <class TInputPixel, class TLabel >
SVMModel<TInputPixel, TLabel>::~SVMModel()
{
// FIXME: pbs. when deleting de problem ....
  otbMsgDebugMacro(  << "SVMModel destructor" );

  /*
  delete [] m_Problem.y;//free(m_Problem.y);
  otbMsgDebugMacro(  << "SVMModel destructor - y done" );
  delete [] m_Problem.x; //free(m_Problem.x);
  otbMsgDebugMacro(  << "SVMModel destructor - x done" );
  delete [] x_space;//
  otbMsgDebugMacro(  << "SVMModel destructor - x_space done" );
  //free(x_space);
*/


  //svm_destroy_model(m_Model);

}

template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::AllocateProblem(int l, long int elements)
  {
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - enter" );
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - l = " << l );
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - elements = " << elements );
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - m_Problem.l = " << m_Problem.l );
    m_Problem.l = l;
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - m_Problem.l = " <<  m_Problem.l );
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - l done" );
    delete [] m_Problem.y;
    m_Problem.y = new double[l];//Malloc(double,l);
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - y done" );
    delete [] m_Problem.x;
    m_Problem.x = new struct svm_node*[l];//Malloc(struct svm_node* ,l);
    otbMsgDebugMacro(  << "SVMModel::AllocateProblem - x done" );
    delete [] x_space;
    x_space = new struct svm_node[elements];
    //free(x_space);
    //x_space = Malloc(struct svm_node,elements);
    //otbMsgDebugMacro(  << "SVMModel::AllocateProblem - x_space done" );

  }


template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::SetModel(struct svm_model* aModel)
  {
    otbMsgDebugMacro(  << "SVMModel::SetModel - enter");
    //svm_destroy_model(m_Model);
    otbMsgDebugMacro(  << "SVMModel::SetModel - destroyed" );
    m_Model = aModel;
    otbMsgDebugMacro(  << "SVMModel::SetModel - out" );
  }


/*template <class TInputPixel, class TLabel >
struct svm_problem&
SVMModel<TInputPixel, TLabel>
::GetProblem()
  {
    return m_Problem;
    
//     otbMsgDebugMacro(  << "SVMModel::GetProblem - enter" );
//     aProblem.l = m_Problem.l;
//     aProblem.y = m_Problem.y;
//     aProblem.x = m_Problem.x;

//     otbMsgDebugMacro(  << "SVMModel::GetProblem - x_space " << x_space );
// //    aNode = x_space;
//     otbMsgDebugMacro(  << "SVMModel::GetProblem - out" );
//     return x_space;
  }

template <class TInputPixel, class TLabel >
struct svm_node*
SVMModel<TInputPixel, TLabel>
::GetXSpace()
  {
    return x_space;

  }*/


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
    m_Model = svm_load_model(model_file_name,m_GenericKernelFunctor);
    if(m_Model == 0)
      {
      itkExceptionMacro( << "Problem while loading SVM model "
			 << std::string(model_file_name) );
      }
  }

template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{  Superclass::PrintSelf(os,indent); }
// FIXME
}// end namespace otb




#endif
