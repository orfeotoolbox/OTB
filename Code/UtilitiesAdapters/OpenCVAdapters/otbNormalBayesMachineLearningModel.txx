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
#ifndef __otbNormalBayesMachineLearningModel_txx
#define __otbNormalBayesMachineLearningModel_txx

#include <fstream>
#include "itkMacro.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::NormalBayesMachineLearningModel() :
 m_NormalBayesModel (new CvNormalBayesClassifier)
{
}


template <class TInputValue, class TOutputValue>
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::~NormalBayesMachineLearningModel()
{
  delete m_NormalBayesModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  m_NormalBayesModel->train(samples,labels,cv::Mat(),cv::Mat(),false);
}

template <class TInputValue, class TOutputValue>
typename NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  cv::Mat missing  = cv::Mat(1,input.Size(), CV_8U );
  missing.setTo(0);
  double result = m_NormalBayesModel->predict(sample);

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_NormalBayesModel->save(filename.c_str(), 0);
  else
    m_NormalBayesModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_NormalBayesModel->load(filename.c_str(), 0);
  else
    m_NormalBayesModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
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

    if (line.find(CV_TYPE_NAME_ML_NBAYES) != std::string::npos)
    {
       //std::cout<<"Reading a "<<CV_TYPE_NAME_ML_NBAYES<<" model"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
NormalBayesMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
