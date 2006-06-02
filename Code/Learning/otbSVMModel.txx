/*=========================================================================

  Program :   OTB (ORFEO ToolBox)
  Authors   :   CNES - J. Inglada
  Language  :   C++
  Date      :   26 April 2006
  Version   :   
  Role      :   SVM Model
  $Id$


=========================================================================*/
#ifndef __otbSVMModel_txx
#define __otbSVMModel_txx
#include "otbSVMModel.h"

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
  //std::cout << "SVMModel::SVMModel - m_Problem.l = " <<  m_Problem.l << std::endl;
  m_Problem.y = new double[1];
  m_Problem.x = new struct svm_node*[1];
  x_space = new struct svm_node[1];

  

}

template <class TInputPixel, class TLabel >
SVMModel<TInputPixel, TLabel>::~SVMModel()
{
// FIXME: pbs. when deleting de problem ....
//  std::cout << "SVMModel destructor" << std::endl;

  /*
  delete [] m_Problem.y;//free(m_Problem.y);
  std::cout << "SVMModel destructor - y done" << std::endl;
  delete [] m_Problem.x; //free(m_Problem.x);
  std::cout << "SVMModel destructor - x done" << std::endl;
  delete [] x_space;//
  std::cout << "SVMModel destructor - x_space done" << std::endl;
  //free(x_space);
*/


  //svm_destroy_model(m_Model);

}

template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::AllocateProblem(int l, long int elements)
  {
    //std::cout << "SVMModel::AllocateProblem - enter" << std::endl;
    //std::cout << "SVMModel::AllocateProblem - l = " << l << std::endl;
    //std::cout << "SVMModel::AllocateProblem - elements = " << elements << std::endl;

    //std::cout << "SVMModel::AllocateProblem - m_Problem.l = " <<  m_Problem.l << std::endl;
    m_Problem.l = l;
    //std::cout << "SVMModel::AllocateProblem - m_Problem.l = " <<  m_Problem.l << std::endl;
    //std::cout << "SVMModel::AllocateProblem - l done" << std::endl;
    delete [] m_Problem.y;
    m_Problem.y = new double[l];//Malloc(double,l);
    //std::cout << "SVMModel::AllocateProblem - y done" << std::endl;
    delete [] m_Problem.x;
    m_Problem.x = new struct svm_node*[l];//Malloc(struct svm_node* ,l);
    //std::cout << "SVMModel::AllocateProblem - x done" << std::endl;
    delete [] x_space;
    x_space = new struct svm_node[elements];
    //free(x_space);
    //x_space = Malloc(struct svm_node,elements);
    //std::cout << "SVMModel::AllocateProblem - x_space done" << std::endl;

  }


template <class TInputPixel, class TLabel >
void
SVMModel<TInputPixel, TLabel>
::SetModel(struct svm_model* aModel)
  {
    //std::cout << "SVMModel::SetModel - enter" << std::endl;
    //svm_destroy_model(m_Model);
    //std::cout << "SVMModel::SetModel - destroyed" << std::endl;
    m_Model = aModel;
    //std::cout << "SVMModel::SetModel - out" << std::endl;
  }


/*template <class TInputPixel, class TLabel >
struct svm_problem&
SVMModel<TInputPixel, TLabel>
::GetProblem()
  {
    return m_Problem;
    
//     //std::cout << "SVMModel::GetProblem - enter" << std::endl;
//     aProblem.l = m_Problem.l;
//     aProblem.y = m_Problem.y;
//     aProblem.x = m_Problem.x;

//     //std::cout << "SVMModel::GetProblem - x_space " << x_space << std::endl;
// //    aNode = x_space;
//     //std::cout << "SVMModel::GetProblem - out" << std::endl;
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
    m_Model = svm_load_model(model_file_name);
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
