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
#ifndef __otbSVMMachineLearningModel_txx
#define __otbSVMMachineLearningModel_txx

#include <fstream>
#include "otbSVMMachineLearningModel.h"
#include "otbOpenCVUtils.h"
#include <opencv2/opencv.hpp>       // opencv general include file

namespace otb
{

template <class TInputValue, class TOutputValue>
SVMMachineLearningModel<TInputValue,TOutputValue>
::SVMMachineLearningModel() :
 m_SVMType(CvSVM::C_SVC), m_KernelType(CvSVM::LINEAR), m_TermCriteriaType(CV_TERMCRIT_ITER),
 m_MaxIter(100), m_Epsilon(1e-6), m_Degree(0), m_Gamma(1),
 m_Coef0(0), m_C(1), m_Nu(0), m_P(0)
{
  m_SVMModel = new CvSVM;
}


template <class TInputValue, class TOutputValue>
SVMMachineLearningModel<TInputValue,TOutputValue>
::~SVMMachineLearningModel()
{
  delete m_SVMModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  // Set up SVM's parameters
  CvTermCriteria term_crit   = cvTermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon);
  CvMat* class_weights;
  CvSVMParams params( m_SVMType, m_KernelType, m_Degree, m_Gamma, m_Coef0, m_C, m_Nu, m_P, NULL , term_crit );

  // Train the SVM
  if (!m_ParameterOptimization)
    m_SVMModel->train(samples, labels, cv::Mat(), cv::Mat(), params);
  else
    //Trains SVM with optimal parameters.
    //train_auto(const Mat& trainData, const Mat& responses, const Mat& varIdx, const Mat& sampleIdx,
    //CvSVMParams params, int k_fold=10, CvParamGrid Cgrid=CvSVM::get_default_grid(CvSVM::C),
    //CvParamGrid gammaGrid=CvSVM::get_default_grid(CvSVM::GAMMA),
    //CvParamGrid pGrid=CvSVM::get_default_grid(CvSVM::P), CvParamGrid nuGrid=CvSVM::get_default_grid(CvSVM::NU),
    //CvParamGrid coeffGrid=CvSVM::get_default_grid(CvSVM::COEF), CvParamGrid degreeGrid=CvSVM::get_default_grid(CvSVM::DEGREE),
    //bool balanced=false)
    //We used default parameters grid. If not enough, those grids should be expose to the user.
    m_SVMModel->train_auto(samples, labels, cv::Mat(), cv::Mat(), params);
}

template <class TInputValue, class TOutputValue>
typename SVMMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
SVMMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_SVMModel->predict(sample,false);

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
       if (name == "")
              m_SVMModel->save(filename.c_str(), 0);
       else
              m_SVMModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
         m_SVMModel->load(filename.c_str(), 0);
  else
         m_SVMModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
SVMMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
   std::ifstream ifs;
   ifs.open(file.c_str());
   
   if(!ifs)
   {
      std::cerr<<"Could not read file "<<file<<std::endl;
      return false;
   }
   
   while (!ifs.eof())
   {
      std::string line;
      std::getline(ifs, line);
      
      //if (line.find(m_SVMModel->getName()) != std::string::npos)
      if (line.find(CV_TYPE_NAME_ML_SVM) != std::string::npos)
      {
         std::cout<<"Reading a "<<CV_TYPE_NAME_ML_SVM<<" model !!!"<<std::endl;
         return true;
      }
   }
   ifs.close();
   return false;
}

template <class TInputValue, class TOutputValue>
bool
SVMMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & file)
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
