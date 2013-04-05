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
#ifndef __otbNeuralNetworkMachineLearningModel_txx
#define __otbNeuralNetworkMachineLearningModel_txx


#include "otbNeuralNetworkMachineLearningModel.h"
#include "otbOpenCVUtils.h"
#include "itkMacro.h" // itkExceptionMacro
#include <opencv2/opencv.hpp>

namespace otb
{

template <class TInputValue, class TOutputValue>
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::NeuralNetworkMachineLearningModel() :
 m_TrainMethod(CvANN_MLP_TrainParams::RPROP), m_ActivateFunction(CvANN_MLP::SIGMOID_SYM),
 m_Alpha(0.), m_Beta(0.), m_BackPropDWScale(0.1), m_BackPropMomentScale(0.1),
 m_RegPropDW0(0.1), m_RegPropDWMin(FLT_EPSILON), m_TermCriteriaType(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS),
 m_MaxIter(1000), m_Epsilon(0.01)
{
  m_ANNModel = new CvANN_MLP;
  m_LayerSizes.clear();
}


template <class TInputValue, class TOutputValue>
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::~NeuralNetworkMachineLearningModel()
{
  delete m_ANNModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::SetLayerSizes(const std::vector<unsigned int> layers)
{
  const unsigned int nbLayers = layers.size();
  if (nbLayers < 3)
    itkExceptionMacro(<< "Number of layers in the Neural Network must be >= 3")

  m_LayerSizes = layers;
}

/** Converts a ListSample of VariableLengthVector to a CvMat. The user
 *  is responsible for freeing the output pointer with the
 *  cvReleaseMat function.  A null pointer is resturned in case the
 *  conversion failed.
 */
template <class TInputValue, class TOutputValue>
void
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::LabelsToMat(const TargetListSampleType * labels, cv::Mat & output)
{
    // Sample index
    unsigned int sampleIdx = 0;

    // Check for valid listSample
    if(labels != NULL && labels->Size() > 0)
      {
      // Retrieve samples count
      unsigned int sampleCount = labels->Size();

      std::vector<unsigned int> classes;

      // Build an iterator
      typename TargetListSampleType::ConstIterator sampleIt = labels->Begin();
      // Retrieve samples size alike
      const unsigned int sampleSize = labels->GetMeasurementVectorSize();

      for(; sampleIt!=labels->End(); ++sampleIt,++sampleIdx)
        {
        // Retrieve sample
        typename TargetListSampleType::MeasurementVectorType sample = sampleIt.GetMeasurementVector();
        // Loop on sample size
        for(unsigned int i = 0; i < sampleSize; ++i)
          {
          classes.push_back(sample[i]);
          }
        }
      std::sort(classes.begin(), classes.end());
      std::vector<unsigned int>::iterator it = std::unique(classes.begin(), classes.end());
      classes.resize(std::distance(classes.begin(),it));
      const unsigned int nbClasses = classes.size();

      // Allocate CvMat
      sampleIdx = 0;
      sampleIt = labels->Begin();
      output.create(sampleCount,nbClasses,CV_32FC1);
      output.setTo(0);
      // Fill the cv matrix
      for(; sampleIt!=labels->End(); ++sampleIt,++sampleIdx)
        {
        // Retrieve sample
        typename TargetListSampleType::MeasurementVectorType sample = sampleIt.GetMeasurementVector();

        // Loop on sample size
        for(unsigned int i = 0; i < sampleSize; ++i)
          {
          output.at<float>(sampleIdx,sample[i]) = 1;
          }
        }
      }
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //Create the neural network
  const unsigned int nbLayers = m_LayerSizes.size();
  if (nbLayers == 0)
    itkExceptionMacro(<< "Number of layers in the Neural Network must be >= 3")

  cv::Mat layers = cv::Mat(nbLayers,1,CV_32SC1);
  for (unsigned int i=0; i<nbLayers; i++)
  {
    layers.row(i) = m_LayerSizes[i];
  }

  m_ANNModel->create(layers, m_ActivateFunction, m_Alpha, m_Beta);

  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  LabelsToMat(this->GetTargetListSample(),labels);
  //otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  CvANN_MLP_TrainParams params;
  params.train_method = m_TrainMethod;
  params.bp_dw_scale = m_BackPropDWScale;
  params.bp_moment_scale = m_BackPropMomentScale;
  params.rp_dw0 = m_RegPropDW0;
  params.rp_dw_min = m_RegPropDWMin;
  CvTermCriteria term_crit   = cvTermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon);
  params.term_crit = term_crit;

  //train the Neural network model
  m_ANNModel->train(samples,labels,cv::Mat(),cv::Mat(),params);
}

template <class TInputValue, class TOutputValue>
typename NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  cv::Mat response(1, 1, CV_32FC1);
  m_ANNModel->predict(sample,response);

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(response.at<float>(0,0));

  return target;
}

template <class TInputValue, class TOutputValue>
void
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_ANNModel->save(filename.c_str(), 0);
  else
    m_ANNModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_ANNModel->load(filename.c_str(), 0);
  else
    m_ANNModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
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

    if (line.find(CV_TYPE_NAME_ML_ANN_MLP) != std::string::npos)
    {
       std::cout<<"Reading a "<<CV_TYPE_NAME_ML_ANN_MLP<<" model !!!"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & file)
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
NeuralNetworkMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
