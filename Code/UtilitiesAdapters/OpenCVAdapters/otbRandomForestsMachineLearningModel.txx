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
#ifndef __otbRandomForestsMachineLearningModel_txx
#define __otbRandomForestsMachineLearningModel_txx

#include <fstream>

#include "otbRandomForestsMachineLearningModel.h"
#include "otbOpenCVUtils.h"
#include <opencv2/opencv.hpp>       // opencv general include file

namespace otb
{

template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::RandomForestsMachineLearningModel() :
 m_RFModel (new CvRTrees),
 m_MaxDepth(5),
 m_MinSampleCount(10),
 m_RegressionAccuracy(0),
 m_ComputeSurrogateSplit(false),
 m_MaxNumberOfCategories(10),
 m_CalculateVariableImportance(false),
 m_MaxNumberOfVariables(0),
 m_MaxNumberOfTrees(100),
 m_ForestAccuracy(0.01),
 m_TerminationCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS)
{
}


template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::~RandomForestsMachineLearningModel()
{
  delete m_RFModel;
}

template <class TInputValue, class TOutputValue>
float
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::GetTrainError()
{
  return m_RFModel->get_train_error();
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
  //Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
  //std::cout << "priors " << m_Priors[0] << std::endl;
  //Define random forests paramneters
  //FIXME do this in the constructor?

  float * priors = m_Priors.empty() ? 0 : &m_Priors.front();

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

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  //train the RT model
  m_RFModel->train(samples, CV_ROW_SAMPLE, labels,
              cv::Mat(), cv::Mat(), var_type, cv::Mat(), params);
}

template <class TInputValue, class TOutputValue>
typename RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & value) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(value,sample);

    double result = m_RFModel->predict(sample);

    TargetSampleType target;

    target[0] = static_cast<TOutputValue>(result);

    return target[0];
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_RFModel->save(filename.c_str(), 0);
  else
    m_RFModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_RFModel->load(filename.c_str(), 0);
  else
    m_RFModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
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

      //if (line.find(m_RFModel->getName()) != std::string::npos)
      if (line.find(CV_TYPE_NAME_ML_RTREES) != std::string::npos)
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
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
