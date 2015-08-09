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

                TPrecision inputValue = sMin+ i*sampleStep;
                TPrecision outputValue = m_a*inputValue+m_b;
                m_isl->PushBack(inputSample);
                m_tsl->PushBack(outputValue);
                }
            }
            InputListSampleRegressionType* GetInputSampleList()
            {
              return m_isl;
            }
            TargetListSampleRegressionType* GetTargetSampleList()
            {
              return m_tsl;
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
  lfsg.GenerateSamples(0.0, 1.0, 100);
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
  regression->SetBeta(0.01);
  regression->SetBackPropDWScale(0.1);
  regression->SetBackPropMomentScale(0.1);
  regression->SetRegPropDW0(0.1);
  regression->SetRegPropDWMin(1e-7);
  regression->SetTermCriteriaType(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS);
  regression->SetEpsilon(1e-10);
  regression->SetMaxIter(1e7);

  regression->SetInputListSample(lfsg.GetInputSampleList());
  regression->SetTargetListSample(lfsg.GetTargetSampleList());
  regression->Train();

  return EXIT_SUCCESS;
}

