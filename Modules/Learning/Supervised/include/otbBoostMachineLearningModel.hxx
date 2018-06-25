/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbBoostMachineLearningModel_hxx
#define otbBoostMachineLearningModel_hxx

#include "otbBoostMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <fstream>
#include "itkMacro.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
BoostMachineLearningModel<TInputValue,TOutputValue>
::BoostMachineLearningModel() :
#ifdef OTB_OPENCV_3
 m_BoostModel(cv::ml::Boost::create()),
#else
 m_BoostModel (new CvBoost),
#endif
 m_BoostType(CvBoost::REAL),
 m_WeakCount(100),
 m_WeightTrimRate(0.95),
#ifdef OTB_OPENCV_3
 m_SplitCrit(0), // not used in OpenCV 3.x
#else
 m_SplitCrit(CvBoost::DEFAULT),
#endif
 m_MaxDepth(1)
{
  this->m_ConfidenceIndex = true;
}


template <class TInputValue, class TOutputValue>
BoostMachineLearningModel<TInputValue,TOutputValue>
::~BoostMachineLearningModel()
{
#ifndef OTB_OPENCV_3
  delete m_BoostModel;
#endif
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical
  var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

#ifdef OTB_OPENCV_3
  m_BoostModel->setBoostType(m_BoostType);
  m_BoostModel->setWeakCount(m_WeakCount);
  m_BoostModel->setWeightTrimRate(m_WeightTrimRate);
  m_BoostModel->setMaxDepth(m_MaxDepth);
  m_BoostModel->setUseSurrogates(false);
  m_BoostModel->setPriors(cv::Mat());
  m_BoostModel->train(cv::ml::TrainData::create(
    samples,
    cv::ml::ROW_SAMPLE,
    labels,
    cv::noArray(),
    cv::noArray(),
    cv::noArray(),
    var_type));
#else
  CvBoostParams params = CvBoostParams(m_BoostType, m_WeakCount, m_WeightTrimRate, m_MaxDepth, false, nullptr);
  params.split_criteria = m_SplitCrit;
  m_BoostModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params);
#endif
}

template <class TInputValue, class TOutputValue>
typename BoostMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
BoostMachineLearningModel<TInputValue,TOutputValue>
::DoPredict(const InputSampleType & input, ConfidenceValueType *quality) const
{
  TargetSampleType target;

  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);
  double result = 0.;

#ifdef OTB_OPENCV_3
  result = m_BoostModel->predict(sample);
#else
  cv::Mat missing  = cv::Mat(1,input.Size(), CV_8U );
  missing.setTo(0);
  result = m_BoostModel->predict(sample,missing);
#endif

  if (quality != nullptr)
    {
    (*quality) = static_cast<ConfidenceValueType>(
#ifdef OTB_OPENCV_3
      m_BoostModel->predict(sample,cv::noArray(), cv::ml::StatModel::RAW_OUTPUT)
#else
      m_BoostModel->predict(sample,missing,cv::Range::all(),false,true)
#endif
      );
    }

  target[0] = static_cast<TOutputValue>(result);
  return target;
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
#ifdef OTB_OPENCV_3
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_BoostModel->getDefaultName() : cv::String(name)) << "{";
  m_BoostModel->write(fs);
  fs << "}";
  fs.release();
#else
  if (name == "")
    m_BoostModel->save(filename.c_str(), nullptr);
  else
    m_BoostModel->save(filename.c_str(), name.c_str());
#endif
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
#ifdef OTB_OPENCV_3
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  m_BoostModel->read(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
#else
  if (name == "")
      m_BoostModel->load(filename.c_str(), nullptr);
  else
      m_BoostModel->load(filename.c_str(), name.c_str());
#endif
}

template <class TInputValue, class TOutputValue>
bool
BoostMachineLearningModel<TInputValue,TOutputValue>
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
    if (line.find(CV_TYPE_NAME_ML_BOOSTING) != std::string::npos
#ifdef OTB_OPENCV_3
        || line.find(m_BoostModel->getDefaultName()) != std::string::npos
#endif
      )
    {
       //std::cout<<"Reading a "<<CV_TYPE_NAME_ML_BOOSTING<<" model"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
BoostMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
