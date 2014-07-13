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

template<class TInputValue, class TOutputValue>
NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::NeuralNetworkMachineLearningModel() :
  m_ANNModel (new CvANN_MLP),
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
  m_Epsilon(0.01),
  m_CvMatOfLabels(0)
{
}

template<class TInputValue, class TOutputValue>
NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::~NeuralNetworkMachineLearningModel()
{
  delete m_ANNModel;
  cvReleaseMat(&m_CvMatOfLabels);
}

/** Train the machine learning model */
template<class TInputValue, class TOutputValue>
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
template<class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::LabelsToMat(const TargetListSampleType * labels,
                                                                               cv::Mat & output)
{
  unsigned int nbSamples = labels->Size();

  // Check for valid listSample
  if (labels != NULL && nbSamples > 0)
    {
    // Build an iterator
    typename TargetListSampleType::ConstIterator labelSampleIt = labels->Begin();

    TargetValueType classLabel;
    for (; labelSampleIt != labels->End(); ++labelSampleIt)
      {
      // Retrieve labelSample
      typename TargetListSampleType::MeasurementVectorType labelSample = labelSampleIt.GetMeasurementVector();
      classLabel = labelSample[0];
      if (m_MapOfLabels.count(classLabel) == 0)
        {
        m_MapOfLabels[classLabel] = -1;
        }
      }

    unsigned int nbClasses = m_MapOfLabels.size();
    typename MapOfLabelsType::iterator itMapOfLabels = m_MapOfLabels.begin();
    unsigned itLabel = 0;
    for (; itMapOfLabels != m_MapOfLabels.end(); ++itMapOfLabels)
      {
      classLabel = itMapOfLabels->first;
      m_MapOfLabels[classLabel] = itLabel;

      if (itLabel == 0)
        {
        if (m_CvMatOfLabels)
          {
          cvReleaseMat(&m_CvMatOfLabels);
          }
        m_CvMatOfLabels = cvCreateMat(1, nbClasses, CV_32SC1);
        }
      m_CvMatOfLabels->data.i[itLabel] = classLabel;
      ++itLabel;
      }

    // Allocate CvMat
    // Sample index
    unsigned int sampleIdx = 0;
    labelSampleIt = labels->Begin();
    output.create(nbSamples, nbClasses, CV_32FC1);
    output.setTo(-m_Beta);
    // Fill the cv matrix
    for (; labelSampleIt != labels->End(); ++labelSampleIt, ++sampleIdx)
      {
      // Retrieve labelSample
      typename TargetListSampleType::MeasurementVectorType labelSample = labelSampleIt.GetMeasurementVector();
      classLabel = labelSample[0];
      unsigned int indexLabel = m_MapOfLabels[classLabel];
      output.at<float> (sampleIdx, indexLabel) = m_Beta;
      }
    }
}

/** Train the machine learning model */
template<class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::Train()
{
  //Create the neural network
  const unsigned int nbLayers = m_LayerSizes.size();

  if ( nbLayers == 0 )
    itkExceptionMacro(<< "Number of layers in the Neural Network must be >= 3")

  cv::Mat layers = cv::Mat(nbLayers, 1, CV_32SC1);
  for (unsigned int i = 0; i < nbLayers; i++)
    {
    layers.row(i) = m_LayerSizes[i];
    }

  m_ANNModel->create(layers, m_ActivateFunction, m_Alpha, m_Beta);

  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat matOutputANN;
  LabelsToMat(this->GetTargetListSample(), matOutputANN);

  CvANN_MLP_TrainParams params;
  params.train_method = m_TrainMethod;
  params.bp_dw_scale = m_BackPropDWScale;
  params.bp_moment_scale = m_BackPropMomentScale;
  params.rp_dw0 = m_RegPropDW0;
  params.rp_dw_min = m_RegPropDWMin;
  CvTermCriteria term_crit = cvTermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon);
  params.term_crit = term_crit;

  //train the Neural network model
  m_ANNModel->train(samples, matOutputANN, cv::Mat(), cv::Mat(), params);
}

template<class TInputValue, class TOutputValue>
typename NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType NeuralNetworkMachineLearningModel<
    TInputValue, TOutputValue>::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input, sample);

  cv::Mat response; //(1, 1, CV_32FC1);
  m_ANNModel->predict(sample, response);

  TargetSampleType target;
  float currentResponse = 0;
  float maxResponse = response.at<float> (0, 0);
  target[0] = m_CvMatOfLabels->data.i[0];

  unsigned int nbClasses = m_CvMatOfLabels->cols;
  for (unsigned itLabel = 1; itLabel < nbClasses; ++itLabel)
    {
    currentResponse = response.at<float> (0, itLabel);
    if (currentResponse > maxResponse)
      {
      maxResponse = currentResponse;
      target[0] = m_CvMatOfLabels->data.i[itLabel];
      }
    }

  return target;
}

template<class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string & filename,
                                                                        const std::string & name)
{
  const char* lname = "my_nn";
  if ( !name.empty() )
    lname = name.c_str();

  CvFileStorage* fs = 0;
  fs = cvOpenFileStorage(filename.c_str(), 0, CV_STORAGE_WRITE);
  if ( !fs )
    {
    itkExceptionMacro("Could not open the file " << filename << " for writing");
    }

  m_ANNModel->write(fs, lname);
  cvWrite(fs, "class_labels", m_CvMatOfLabels);

  cvReleaseFileStorage(&fs);
}

template<class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string & filename,
                                                                        const std::string & name)
{
  const char* lname = 0;
  if ( !name.empty() )
    lname = name.c_str();

  CvFileStorage* fs = 0;
  CvFileNode* model_node = 0;
  fs = cvOpenFileStorage(filename.c_str(), 0, CV_STORAGE_READ);
  if ( !fs )
    {
    itkExceptionMacro("Could not open the file " << filename << " for reading");
    }

  if( lname )
    model_node = cvGetFileNodeByName(fs, 0, lname);
  else
    {
    CvFileNode* root = cvGetRootFileNode( fs );
    if( root->data.seq->total > 0 )
      model_node = (CvFileNode*)cvGetSeqElem( root->data.seq, 0 );
    }

  m_ANNModel->read(fs, model_node);
  m_CvMatOfLabels = (CvMat*)cvReadByName( fs, model_node, "class_labels" );

  cvReleaseFileStorage(&fs);
}

template<class TInputValue, class TOutputValue>
bool NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string & file)
{
  std::ifstream ifs;
  ifs.open(file.c_str());

  if (!ifs)
    {
    std::cerr << "Could not read file " << file << std::endl;
    return false;
    }

  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);

    if (line.find(CV_TYPE_NAME_ML_ANN_MLP) != std::string::npos)
      {
      //std::cout << "Reading a " << CV_TYPE_NAME_ML_ANN_MLP << " model" << std::endl;
      return true;
      }
    }
  ifs.close();
  return false;
}

template<class TInputValue, class TOutputValue>
bool NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template<class TInputValue, class TOutputValue>
void NeuralNetworkMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} //end namespace otb

#endif
