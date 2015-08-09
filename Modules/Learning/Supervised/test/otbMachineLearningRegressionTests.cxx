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


#include "otbNeuralNetworkMachineLearningModel.h"

typedef float PrecisionType;
typedef otb::MachineLearningModel<PrecisionType,PrecisionType>   MachineLearningModelRegressionType;
typedef MachineLearningModelRegressionType::InputValueType       InputValueRegressionType;
typedef MachineLearningModelRegressionType::InputSampleType      InputSampleRegressionType;
typedef MachineLearningModelRegressionType::InputListSampleType  InputListSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetValueType      TargetValueRegressionType;
typedef MachineLearningModelRegressionType::TargetSampleType     TargetSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetListSampleType TargetListSampleRegressionType;

const double epsilon = 0.1;

template <typename TPrecision>
struct LinearFunctionSampleGenerator
{
  LinearFunctionSampleGenerator(TPrecision a, TPrecision b)
    : m_a(a), m_b(b), m_NbInputVars(1), m_NbOutputVars(1) {
    m_isl = InputListSampleRegressionType::New();
    m_tsl = TargetListSampleRegressionType::New();
  };
  void GenerateSamples(TPrecision sMin, TPrecision sMax, size_t nbSamples)
  {
    m_isl->SetMeasurementVectorSize(m_NbInputVars);
    m_tsl->SetMeasurementVectorSize(m_NbOutputVars);

    TPrecision sampleStep = (sMax-sMin)/nbSamples;
    for(size_t i=0; i<nbSamples; ++i)
      {
      InputSampleRegressionType inputSample;
      inputSample.Reserve(m_NbInputVars);
      TPrecision x = std::rand()/static_cast<TPrecision>(RAND_MAX)*nbSamples;
      TPrecision inputValue = sMin+ x*sampleStep;
      inputSample[0] = inputValue;
      TPrecision outputValue = m_a*inputValue+m_b;
      m_isl->PushBack(inputSample);
      m_tsl->PushBack(outputValue);
      }
  }

  TPrecision m_a;
  TPrecision m_b;
  const size_t m_NbInputVars;
  const size_t m_NbOutputVars;
  InputListSampleRegressionType::Pointer m_isl;
  TargetListSampleRegressionType::Pointer m_tsl;

};


int otbNeuralNetworkRegressionLinearMonovariate(int itkNotUsed(argc), 
                                                char * itkNotUsed(argv) [])
{

  LinearFunctionSampleGenerator<PrecisionType> lfsg(1.0, 0.0);
  std::cout << "Generating samples\n";
  lfsg.GenerateSamples(-0.5, 0.5, 20000);
  typedef otb::NeuralNetworkMachineLearningModel<InputValueRegressionType,
                                                 TargetValueRegressionType> 
    NeuralNetworkType;
  NeuralNetworkType::Pointer regression = NeuralNetworkType::New();

  regression->SetRegressionMode(1);
  regression->SetTrainMethod(CvANN_MLP_TrainParams::BACKPROP);
  std::vector<unsigned int> layerSizes;
  layerSizes.push_back(1);
  layerSizes.push_back(5);
  layerSizes.push_back(1);
  regression->SetLayerSizes(layerSizes);
  regression->SetActivateFunction(CvANN_MLP::SIGMOID_SYM);
  regression->SetAlpha(1.0);
  regression->SetBeta(1.0);
  regression->SetBackPropDWScale(0.1);
  regression->SetBackPropMomentScale(0.1);
  regression->SetRegPropDW0(0.1);
  regression->SetRegPropDWMin(1e-7);
  regression->SetTermCriteriaType(CV_TERMCRIT_EPS);
  regression->SetEpsilon(1e-5);
  regression->SetMaxIter(1e20);

  regression->SetInputListSample(lfsg.m_isl);
  regression->SetTargetListSample(lfsg.m_tsl);
  std::cout << "Training\n";
  regression->Train();

  std::cout << "Validation\n";
  //Check the prediction accuracy
  typename InputListSampleRegressionType::Iterator sampleIt = lfsg.m_isl->Begin();
  typename TargetListSampleRegressionType::Iterator resultIt = lfsg.m_tsl->Begin();
  typename InputListSampleRegressionType::Iterator sampleLast = lfsg.m_isl->End();
  typename TargetListSampleRegressionType::Iterator resultLast = lfsg.m_tsl->End();
  PrecisionType rmse = 0.0;
  size_t nbSamples = 0;
  while(sampleIt != sampleLast && resultIt != resultLast)
    {
    PrecisionType invalue = sampleIt.GetMeasurementVector()[0];
    PrecisionType prediction = regression->Predict(sampleIt.GetMeasurementVector())[0];
    PrecisionType expected = resultIt.GetMeasurementVector()[0];
    rmse += pow(prediction - expected, 2.0);
    ++sampleIt;
    ++resultIt;
    ++nbSamples;
    } 

  rmse /= nbSamples;
  if(rmse > epsilon)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

  
