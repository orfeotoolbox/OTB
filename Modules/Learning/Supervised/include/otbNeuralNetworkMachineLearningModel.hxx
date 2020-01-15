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

#ifndef otbNeuralNetworkMachineLearningModel_hxx
#define otbNeuralNetworkMachineLearningModel_hxx

#include <fstream>
#include "otbNeuralNetworkMachineLearningModel.h"
#include "itkMacro.h" // itkExceptionMacro

namespace otb
{

template <class TInputValue, class TOutputValue>
NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::NeuralNetworkMachineLearningModel()
  :
    m_ANNModel(cv::ml::ANN_MLP::create()),
    m_TrainMethod(CvANN_MLP_TrainParams::RPROP),
    m_ActivateFunction(CvANN_MLP::SIGMOID_SYM),
    m_Alpha(1.),
    m_Beta(1.),
    m_BackPropDWScale(0.1),
    m_BackPropMomentScale(0.1),
    m_RegPropDW0(0.1),
    m_RegPropDWMin(FLT_EPSILON),
    m_TermCriteriaType(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS),
    m_MaxIter(1000),
    m_Epsilon(0.01)
{
  this->m_ConfidenceIndex       = true;
  this->m_IsRegressionSupported = true;
}

/** Sets the topology of the NN */
template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::SetLayerSizes(const std::vector<unsigned int> layers)
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
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::LabelsToMat(const TargetListSampleType* labels, cv::Mat& output)
{
  unsigned int nbSamples = 0;
  if (labels != nullptr)
  {
    nbSamples = labels->Size();
  }

  // Check for valid listSample
  if (nbSamples > 0)
  {
    // Build an iterator
    typename TargetListSampleType::ConstIterator labelSampleIt = labels->Begin();

    TargetValueType classLabel;
    for (; labelSampleIt != labels->End(); ++labelSampleIt)
    {
      // Retrieve labelSample
      typename TargetListSampleType::MeasurementVectorType labelSample = labelSampleIt.GetMeasurementVector();
      classLabel                                                       = labelSample[0];
      if (m_MapOfLabels.count(classLabel) == 0)
      {
        m_MapOfLabels[classLabel] = -1;
      }
    }

    unsigned int nbClasses = m_MapOfLabels.size();

    m_MatrixOfLabels = cv::Mat(1,nbClasses, CV_32FC1);
    unsigned int itLabel = 0;
    for (auto& kv : m_MapOfLabels)
    {
      classLabel = kv.first;
      kv.second = itLabel;
      m_MatrixOfLabels.at<float>(0,itLabel) = classLabel;
      ++itLabel;
    }

    // Sample index
    unsigned int sampleIdx = 0;
    labelSampleIt          = labels->Begin();
    output.create(nbSamples, nbClasses, CV_32FC1);
    output.setTo(-m_Beta);
    // Fill the cv matrix
    for (; labelSampleIt != labels->End(); ++labelSampleIt, ++sampleIdx)
    {
      // Retrieve labelSample
      typename TargetListSampleType::MeasurementVectorType labelSample = labelSampleIt.GetMeasurementVector();
      classLabel                                                       = labelSample[0];
      unsigned int indexLabel                                          = m_MapOfLabels[classLabel];
      output.at<float>(sampleIdx, indexLabel) = m_Beta;
    }
  }
}

template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::CreateNetwork()
{
  // Create the neural network
  const unsigned int nbLayers = m_LayerSizes.size();

  if (nbLayers == 0)
    itkExceptionMacro(<< "Number of layers in the Neural Network must be >= 3")

        cv::Mat layers = cv::Mat(nbLayers, 1, CV_32SC1);
  for (unsigned int i = 0; i < nbLayers; i++)
  {
    layers.row(i) = m_LayerSizes[i];
  }

  m_ANNModel->setLayerSizes(layers);
  m_ANNModel->setActivationFunction(m_ActivateFunction, m_Alpha, m_Beta);
}

template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::SetupNetworkAndTrain(cv::Mat& labels)
{
  // convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);
  this->CreateNetwork();
  int flags = (this->m_RegressionMode ? 0 : cv::ml::ANN_MLP::NO_OUTPUT_SCALE);
  m_ANNModel->setTrainMethod(m_TrainMethod);
  m_ANNModel->setBackpropMomentumScale(m_BackPropMomentScale);
  m_ANNModel->setBackpropWeightScale(m_BackPropDWScale);
  m_ANNModel->setRpropDW0(m_RegPropDW0);
  // m_ANNModel->setRpropDWMax( );
  m_ANNModel->setRpropDWMin(m_RegPropDWMin);
  // m_ANNModel->setRpropDWMinus( );
  // m_ANNModel->setRpropDWPlus( );
  m_ANNModel->setTermCriteria(cv::TermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon));
  m_ANNModel->train(cv::ml::TrainData::create(samples, cv::ml::ROW_SAMPLE, labels), flags);
}

/** Train the machine learning model for classification*/
template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::Train()
{
  // Transform the targets into a matrix of labels
  cv::Mat matOutputANN;
  if (this->m_RegressionMode)
  {
    // MODE REGRESSION
    otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(), matOutputANN);
  }
  else
  {
    // MODE CLASSIFICATION : store the map between internal labels and output labels
    LabelsToMat(this->GetTargetListSample(), matOutputANN);
  }
  this->SetupNetworkAndTrain(matOutputANN);
}

template <class TInputValue, class TOutputValue>
typename NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType
NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::DoPredict(const InputSampleType& input, ConfidenceValueType* quality,
                                                                        ProbaSampleType* proba) const
{
  TargetSampleType target;
  // convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input, sample);

  cv::Mat response; //(1, 1, CV_32FC1);
  m_ANNModel->predict(sample, response);

  float currentResponse = 0;
  float maxResponse     = response.at<float>(0, 0);

  if (this->m_RegressionMode)
  {
    // MODE REGRESSION : only output first response
    target[0] = maxResponse;
    return target;
  }

  // MODE CLASSIFICATION : find the highest response
  float secondResponse = -1e10;

  target[0] = m_MatrixOfLabels.at<TOutputValue>(0);
  unsigned int nbClasses = m_MatrixOfLabels.size[1];

  for (unsigned itLabel = 1; itLabel < nbClasses; ++itLabel)
  {
    currentResponse = response.at<float>(0, itLabel);
    if (currentResponse > maxResponse)
    {
      secondResponse = maxResponse;

      maxResponse = currentResponse;
      target[0] = m_MatrixOfLabels.at<TOutputValue>(itLabel);
    }
    else
    {
      if (currentResponse > secondResponse)
      {
        secondResponse = currentResponse;
      }
    }
  }

  if (quality != nullptr)
  {
    (*quality) = static_cast<ConfidenceValueType>(maxResponse) - static_cast<ConfidenceValueType>(secondResponse);
  }
  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  return target;
}

template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_ANNModel->getDefaultName() : cv::String(name)) << "{";
  m_ANNModel->write(fs);

  if (!m_MatrixOfLabels.empty())
  {
    fs << "class_labels" << m_MatrixOfLabels;
  }
  fs << "}";
  fs.release();
}

template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  cv::FileNode    model_node(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
  m_ANNModel->read(model_node);
  model_node["class_labels"] >> m_MatrixOfLabels;
  fs.release();
}

template <class TInputValue, class TOutputValue>
bool NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string& file)
{
  std::ifstream ifs;
  ifs.open(file);

  if (!ifs)
  {
    std::cerr << "Could not read file " << file << std::endl;
    return false;
  }

  while (!ifs.eof())
  {
    std::string line;
    std::getline(ifs, line);

    if (line.find(CV_TYPE_NAME_ML_ANN_MLP) != std::string::npos
        || line.find(m_ANNModel->getDefaultName()) != std::string::npos
        )
    {
      return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
