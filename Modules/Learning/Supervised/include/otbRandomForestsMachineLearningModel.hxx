/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRandomForestsMachineLearningModel_hxx
#define otbRandomForestsMachineLearningModel_hxx

#include <fstream>
#include "itkMacro.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::RandomForestsMachineLearningModel() :
#ifdef OTB_OPENCV_3
  m_RFModel(CvRTreesWrapper::create()),
#else
  m_RFModel (new CvRTreesWrapper),
#endif
  m_MaxDepth(5),
  m_MinSampleCount(10),
  m_RegressionAccuracy(0.01),
  m_ComputeSurrogateSplit(false),
  m_MaxNumberOfCategories(10),
  m_CalculateVariableImportance(false),
  m_MaxNumberOfVariables(0),
  m_MaxNumberOfTrees(100),
  m_ForestAccuracy(0.01),
  m_TerminationCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS), // identic for v3 ?
  m_ComputeMargin(false)
{
  this->m_ConfidenceIndex = true;
  this->m_ProbaIndex = false;
  this->m_IsRegressionSupported = true;
}


template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::~RandomForestsMachineLearningModel()
{
#ifndef OTB_OPENCV_3
  delete m_RFModel;
#endif
}

template <class TInputValue, class TOutputValue>
float
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::GetTrainError()
{
#ifdef OTB_OPENCV_3
  // TODO
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  if(this->m_RegressionMode)
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_NUMERICAL;
  else
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  return m_RFModel->calcError(
    cv::ml::TrainData::create(
      samples,
      cv::ml::ROW_SAMPLE,
      labels,
      cv::noArray(),
      cv::noArray(),
      cv::noArray(),
      var_type),
    false,
    cv::noArray());
#else
  return m_RFModel->get_train_error();
#endif
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  if(this->m_RegressionMode)
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_NUMERICAL;
  else
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  //Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
  //std::cout << "priors " << m_Priors[0] << std::endl;
  //Define random forests paramneters
  //FIXME do this in the constructor?
#ifdef OTB_OPENCV_3
  m_RFModel->setMaxDepth(m_MaxDepth);
  m_RFModel->setMinSampleCount(m_MinSampleCount);
  m_RFModel->setRegressionAccuracy(m_RegressionAccuracy);
  m_RFModel->setUseSurrogates(m_ComputeSurrogateSplit);
  m_RFModel->setMaxCategories(m_MaxNumberOfCategories);
  m_RFModel->setPriors(cv::Mat(m_Priors) ); // TODO
  m_RFModel->setCalculateVarImportance(m_CalculateVariableImportance);
  m_RFModel->setActiveVarCount(m_MaxNumberOfVariables);
  m_RFModel->setTermCriteria( cv::TermCriteria(
    m_TerminationCriteria,
    m_MaxNumberOfTrees,
    m_ForestAccuracy) );
  m_RFModel->train(cv::ml::TrainData::create(
    samples,
    cv::ml::ROW_SAMPLE,
    labels,
    cv::noArray(),
    cv::noArray(),
    cv::noArray(),
    var_type));
#else
  float * priors = m_Priors.empty() ? nullptr : &m_Priors.front();

  CvRTParams params = CvRTParams(m_MaxDepth,                    // max depth
                                 m_MinSampleCount,              // min sample count
                                 m_RegressionAccuracy,          // regression accuracy: N/A here
                                 m_ComputeSurrogateSplit,       // compute surrogate split, no missing data
                                 m_MaxNumberOfCategories,       // max number of categories (use sub-optimal algorithm for larger numbers)
                                 priors,                        // the array of priors
                                 m_CalculateVariableImportance, // calculate variable importance
                                 m_MaxNumberOfVariables,        // number of variables randomly selected at node and used to find the best split(s).
                                 m_MaxNumberOfTrees,            // max number of trees in the forest
                                 m_ForestAccuracy,              // forest accuracy
                                 m_TerminationCriteria          // termination criteria
    );
  //train the RT model
  m_RFModel->train(samples, CV_ROW_SAMPLE, labels,
                   cv::Mat(), cv::Mat(), var_type, cv::Mat(), params);
#endif
}

template <class TInputValue, class TOutputValue>
typename RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::DoPredict(const InputSampleType & value, ConfidenceValueType *quality, ProbaSampleType *proba) const
{
  //std::cout << "Enter predict" << std::endl;
  TargetSampleType target;
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(value,sample);

  double result = m_RFModel->predict(sample);

  target[0] = static_cast<TOutputValue>(result);

  if (quality != nullptr)
    {
    if(m_ComputeMargin)
      (*quality) = m_RFModel->predict_margin(sample);
    else
      (*quality) = m_RFModel->predict_confidence(sample);
    }

  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  return target[0];
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
#ifdef OTB_OPENCV_3
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_RFModel->getDefaultName() : cv::String(name)) << "{";
  m_RFModel->write(fs);
  fs << "}";
  fs.release();
#else
  if (name == "")
    m_RFModel->save(filename.c_str(), nullptr);
  else
    m_RFModel->save(filename.c_str(), name.c_str());
#endif
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
#ifdef OTB_OPENCV_3
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  m_RFModel->read(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
#else
  if (name == "")
    m_RFModel->load(filename.c_str(), nullptr);
  else
    m_RFModel->load(filename.c_str(), name.c_str());
#endif
}

template <class TInputValue, class TOutputValue>
bool
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  std::ifstream ifs;
  ifs.open(file);

  if(!ifs)
    {
    std::cerr<<"Could not read file "<<file<<std::endl;
    return false;
    }


  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);

    //if (line.find(m_RFModel->getName()) != std::string::npos)
    if (line.find(CV_TYPE_NAME_ML_RTREES) != std::string::npos
#ifdef OTB_OPENCV_3
        || line.find(m_RFModel->getDefaultName()) != std::string::npos
#endif
        )
      {
      //std::cout<<"Reading a "<<CV_TYPE_NAME_ML_RTREES<<" model"<<std::endl;
      return true;
      }
    }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
typename RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::VariableImportanceMatrixType
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::GetVariableImportance()
{
  cv::Mat cvMat = m_RFModel->getVarImportance();
  VariableImportanceMatrixType itkMat(cvMat.rows,cvMat.cols);
  for(int i =0; i<cvMat.rows; i++)
    {
    for(int j =0; j<cvMat.cols; j++)
      {
      itkMat(i,j)=cvMat.at<float>(i,j);
      }
    }
  return itkMat;
}


template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
