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

#include "otbConfigure.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"

#ifdef OTB_USE_OPENCV
#include "otbNeuralNetworkMachineLearningModel.h"
#include "otbSVMMachineLearningModel.h"
#include "otbBoostMachineLearningModel.h"
#include "otbDecisionTreeMachineLearningModel.h"
#include "otbGradientBoostedTreeMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#endif

#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModel.h"
#endif

typedef float PrecisionType;
typedef otb::MachineLearningModel<PrecisionType,PrecisionType>   MachineLearningModelRegressionType;
typedef MachineLearningModelRegressionType::InputValueType       InputValueRegressionType;
typedef MachineLearningModelRegressionType::InputSampleType      InputSampleRegressionType;
typedef MachineLearningModelRegressionType::InputListSampleType  InputListSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetValueType      TargetValueRegressionType;
typedef MachineLearningModelRegressionType::TargetSampleType     TargetSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetListSampleType TargetListSampleRegressionType;
typedef itk::Statistics::MersenneTwisterRandomVariateGenerator   RandomGeneratorType;

typedef struct RegressionTestParamStruct
{
  double vMin;
  double vMax;
  size_t count;
  double eps;
} RegressionTestParam;

template <typename TPrecision>
struct LinearFunctionSampleGenerator
{
  typedef TPrecision PrecisionType;
  LinearFunctionSampleGenerator(TPrecision a, TPrecision b)
    : m_a(a), m_b(b), m_NbInputVars(1), m_NbOutputVars(1) {
    m_isl = InputListSampleRegressionType::New();
    m_tsl = TargetListSampleRegressionType::New();
  };
  void GenerateSamples(TPrecision sMin, TPrecision sMax, size_t nbSamples)
  {
    m_isl->Clear();
    m_tsl->Clear();
    m_isl->SetMeasurementVectorSize(m_NbInputVars);
    m_tsl->SetMeasurementVectorSize(m_NbOutputVars);

    RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();
    InputSampleRegressionType inputSample;
    inputSample.SetSize(m_NbInputVars);
    TargetSampleRegressionType outputSample;

    TPrecision sampleStep = (sMax-sMin)/nbSamples;
    for(size_t i=0; i<nbSamples; ++i)
      {
      TPrecision x = randomGenerator->GetUniformVariate(0.0, 1.0) * static_cast<TPrecision>(nbSamples);
      TPrecision inputValue = sMin+ x*sampleStep;
      inputSample[0] = inputValue;
      outputSample[0] = m_a*inputValue+m_b;
      m_isl->PushBack(inputSample);
      m_tsl->PushBack(outputSample);
      }
  }

  TPrecision m_a;
  TPrecision m_b;
  const size_t m_NbInputVars;
  const size_t m_NbOutputVars;
  InputListSampleRegressionType::Pointer m_isl;
  TargetListSampleRegressionType::Pointer m_tsl;
};

template <typename TPrecision>
struct BilinearFunctionSampleGenerator
{
  typedef TPrecision PrecisionType;
  BilinearFunctionSampleGenerator(TPrecision a, TPrecision b, TPrecision c)
    : m_a(a), m_b(b), m_c(c), m_NbInputVars(2), m_NbOutputVars(1) {
    m_isl = InputListSampleRegressionType::New();
    m_tsl = TargetListSampleRegressionType::New();
  };
  void GenerateSamples(TPrecision sMin, TPrecision sMax, size_t nbSamples)
  {
    m_isl->Clear();
    m_tsl->Clear();
    m_isl->SetMeasurementVectorSize(m_NbInputVars);
    m_tsl->SetMeasurementVectorSize(m_NbOutputVars);

    RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();
    InputSampleRegressionType inputSample;
    inputSample.SetSize(m_NbInputVars);
    TargetSampleRegressionType outputSample;

    TPrecision sampleStep = (sMax-sMin)/nbSamples;
    for(size_t i=0; i<nbSamples; ++i)
      {
      TPrecision x = randomGenerator->GetUniformVariate(0.0, 1.0) * static_cast<TPrecision>(nbSamples);
      TPrecision inputValue1 = sMin+ x*sampleStep;
      x = randomGenerator->GetUniformVariate(0.0, 1.0) * static_cast<TPrecision>(nbSamples);
      TPrecision inputValue2 = sMin+ x*sampleStep;
      inputSample[0] = inputValue1;
      inputSample[1] = inputValue2;
      outputSample[0] = m_a*inputValue1+m_b*inputValue2+m_c;
      m_isl->PushBack(inputSample);
      m_tsl->PushBack(outputSample);
      }
  }

  TPrecision m_a;
  TPrecision m_b;
  TPrecision m_c;
  const size_t m_NbInputVars;
  const size_t m_NbOutputVars;
  InputListSampleRegressionType::Pointer m_isl;
  TargetListSampleRegressionType::Pointer m_tsl;
};

template <typename TPrecision>
struct PolynomialFunctionSampleGenerator
{
  typedef TPrecision PrecisionType;
  PolynomialFunctionSampleGenerator(std::vector<TPrecision> c)
    : m_c(c), m_NbInputVars(1), m_NbOutputVars(1) {
    m_isl = InputListSampleRegressionType::New();
    m_tsl = TargetListSampleRegressionType::New();
  };
  void GenerateSamples(TPrecision sMin, TPrecision sMax, size_t nbSamples)
  {
    m_isl->Clear();
    m_tsl->Clear();
    m_isl->SetMeasurementVectorSize(m_NbInputVars);
    m_tsl->SetMeasurementVectorSize(m_NbOutputVars);

    RandomGeneratorType::Pointer randomGenerator = RandomGeneratorType::GetInstance();
    InputSampleRegressionType inputSample;
    inputSample.SetSize(m_NbInputVars);
    TargetSampleRegressionType outputSample;

    TPrecision sampleStep = (sMax-sMin)/nbSamples;
    for(size_t i=0; i<nbSamples; ++i)
      {
      TPrecision x = randomGenerator->GetUniformVariate(0.0, 1.0) * static_cast<TPrecision>(nbSamples);
      TPrecision inputValue = sMin+ x*sampleStep;
      inputSample[0] = inputValue;
      TPrecision y = 0.0;
      for (unsigned int j=0; j<m_c.size() ; ++j)
        {
        y += m_c[j] * pow(static_cast<double>(inputValue), static_cast<double>(j));
        }
      outputSample[0] = y;
      m_isl->PushBack(inputSample);
      m_tsl->PushBack(outputSample);
      }
  }

  std::vector<TPrecision> m_c;
  const size_t m_NbInputVars;
  const size_t m_NbOutputVars;
  InputListSampleRegressionType::Pointer m_isl;
  TargetListSampleRegressionType::Pointer m_tsl;
};

template <typename SampleGeneratorType, typename RegressionType>
int testRegression(SampleGeneratorType& sg, RegressionType& rgrsn, RegressionTestParam param)
{
  std::cout << "Generating training samples" << std::endl;
  sg.GenerateSamples(param.vMin, param.vMax, param.count);

  rgrsn->SetInputListSample(sg.m_isl);
  rgrsn->SetTargetListSample(sg.m_tsl);
  std::cout << "Training" << std::endl;
  rgrsn->Train();

  std::cout << "Generate validation samples"<<std::endl;
  sg.GenerateSamples(param.vMin, param.vMax, param.count);

  std::cout << "Validation" << std::endl;
  //Check the prediction accuracy
  typename InputListSampleRegressionType::Iterator sampleIt = sg.m_isl->Begin();
  typename TargetListSampleRegressionType::Iterator resultIt = sg.m_tsl->Begin();
  typename InputListSampleRegressionType::Iterator sampleLast = sg.m_isl->End();
  typename TargetListSampleRegressionType::Iterator resultLast = sg.m_tsl->End();
  typename SampleGeneratorType::PrecisionType rmse = 0.0;
  while(sampleIt != sampleLast && resultIt != resultLast)
    {
    //typename SampleGeneratorType::PrecisionType invalue = sampleIt.GetMeasurementVector()[0];
    typename SampleGeneratorType::PrecisionType prediction = rgrsn->Predict(sampleIt.GetMeasurementVector())[0];
    typename SampleGeneratorType::PrecisionType expected = resultIt.GetMeasurementVector()[0];
    rmse += std::pow(prediction - expected, static_cast<PrecisionType>(2.0));
    ++sampleIt;
    ++resultIt;
    } 

  rmse = sqrt( rmse / static_cast<double>(param.count) );
  std::cout << "RMSE = "<< rmse << std::endl;
  if(rmse > param.eps)
    {
    std::cout << "Failed : RMSE above expected precision !" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

#ifdef OTB_USE_LIBSVM
MachineLearningModelRegressionType::Pointer getLibSVMRegressionModel()
{
  typedef otb::LibSVMMachineLearningModel<InputValueRegressionType,
                                       TargetValueRegressionType>
    libsvmType;
  libsvmType::Pointer regression = libsvmType::New();
  regression->SetRegressionMode(true);
  regression->SetSVMType(EPSILON_SVR);
  regression->SetKernelType(RBF);
  regression->SetEpsilon(1e-5);
  regression->SetParameterOptimization(true);
  return regression.GetPointer();
}

int otbLibSVMRegressionTests(int itkNotUsed(argc),
                             char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 200;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getLibSVMRegressionModel();
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  regression = getLibSVMRegressionModel();
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  regression = getLibSVMRegressionModel();
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}

#endif

#ifdef OTB_USE_OPENCV
MachineLearningModelRegressionType::Pointer getNeuralNetworkRegressionModel(unsigned int nbInputVar)
{
  typedef otb::NeuralNetworkMachineLearningModel<InputValueRegressionType,
                                                 TargetValueRegressionType>
    NeuralNetworkType;
  NeuralNetworkType::Pointer regression = NeuralNetworkType::New();

  regression->SetRegressionMode(1);
  regression->SetTrainMethod(CvANN_MLP_TrainParams::BACKPROP);
  std::vector<unsigned int> layerSizes;
  layerSizes.push_back(nbInputVar);
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
  regression->SetMaxIter(1e4);
  return regression.GetPointer();
}

int otbNeuralNetworkRegressionTests(int itkNotUsed(argc),
                                    char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 20000;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getNeuralNetworkRegressionModel(1);
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  regression = getNeuralNetworkRegressionModel(2);
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  regression = getNeuralNetworkRegressionModel(1);
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}


MachineLearningModelRegressionType::Pointer getSVMRegressionModel()
{
  typedef otb::SVMMachineLearningModel<InputValueRegressionType,
                                       TargetValueRegressionType>
    SVMType;
  SVMType::Pointer regression = SVMType::New();

  regression->SetRegressionMode(1);
  regression->SetSVMType(CvSVM::NU_SVR);
  regression->SetNu(0.5);
  regression->SetKernelType(CvSVM::RBF);
  regression->SetTermCriteriaType(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS);
  regression->SetMaxIter(100000);
  regression->SetEpsilon(1e-5);
  regression->SetParameterOptimization(true);
  return regression.GetPointer();
}

int otbSVMRegressionTests(int itkNotUsed(argc),
                                      char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 200;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getSVMRegressionModel();
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  regression = getSVMRegressionModel();
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  regression = getSVMRegressionModel();
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}


MachineLearningModelRegressionType::Pointer getDecisionTreeRegressionModel()
{
  typedef otb::DecisionTreeMachineLearningModel<InputValueRegressionType,
                                       TargetValueRegressionType>
    DTreeType;
  DTreeType::Pointer regression = DTreeType::New();
  regression->SetRegressionMode(true);
  regression->SetRegressionAccuracy(0.005);
  return regression.GetPointer();
}

int otbDecisionTreeRegressionTests(int itkNotUsed(argc),
                                      char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 200;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getDecisionTreeRegressionModel();
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  regression = getDecisionTreeRegressionModel();
  // increase the number of training samples for bilinear function
  param.count = 1000;
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  param.count = 200;
  regression = getDecisionTreeRegressionModel();
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}


MachineLearningModelRegressionType::Pointer getGradientBoostedTreeRegressionModel()
{
  typedef otb::GradientBoostedTreeMachineLearningModel<InputValueRegressionType,
                                       TargetValueRegressionType>
    GBTreeType;
  GBTreeType::Pointer regression = GBTreeType::New();
  regression->SetRegressionMode(true);
  regression->SetShrinkage(0.1);
  regression->SetSubSamplePortion(0.8);
  regression->SetLossFunctionType(CvGBTrees::SQUARED_LOSS);
  return regression.GetPointer();
}

int otbGradientBoostedTreeRegressionTests(int itkNotUsed(argc),
                                      char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 200;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getGradientBoostedTreeRegressionModel();
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  // increase number of training samples for bilinear function
  param.count = 1000;
  regression = getGradientBoostedTreeRegressionModel();
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  param.count = 200;
  regression = getGradientBoostedTreeRegressionModel();
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}


MachineLearningModelRegressionType::Pointer getKNearestNeighborsRegressionModel()
{
  typedef otb::KNearestNeighborsMachineLearningModel<InputValueRegressionType,
                                       TargetValueRegressionType>
    KNNType;
  KNNType::Pointer regression = KNNType::New();
  regression->SetRegressionMode(true);
  regression->SetK(5);
  regression->SetDecisionRule(KNNType::KNN_MEDIAN);
  return regression.GetPointer();
}

int otbKNearestNeighborsRegressionTests(int itkNotUsed(argc),
                                      char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 200;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getKNearestNeighborsRegressionModel();
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  regression = getKNearestNeighborsRegressionModel();
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  regression = getKNearestNeighborsRegressionModel();
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}


MachineLearningModelRegressionType::Pointer getRandomForestsRegressionModel()
{
  typedef otb::RandomForestsMachineLearningModel<InputValueRegressionType,
                                       TargetValueRegressionType>
    RFType;
  RFType::Pointer regression = RFType::New();
  regression->SetRegressionMode(true);
  regression->SetRegressionAccuracy(0.005);
  return regression.GetPointer();
}


int otbRandomForestsRegressionTests(int itkNotUsed(argc),
                                      char * itkNotUsed(argv) [])
{
  int status = EXIT_SUCCESS;
  int ret;
  MachineLearningModelRegressionType::Pointer regression;

  RegressionTestParam param;
  param.vMin = -0.5;
  param.vMax = 0.5;
  param.count = 200;
  param.eps = epsilon;

  std::cout << "Testing regression on a linear monovariate function" << std::endl;
  LinearFunctionSampleGenerator<PrecisionType> lfsg(2.0, 1.0);
  regression = getRandomForestsRegressionModel();
  ret = testRegression(lfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a bilinear function" << std::endl;
  BilinearFunctionSampleGenerator<PrecisionType> bfsg(2.0,-1.0,1.0);
  // increase number of training samples for bilinear function
  param.count = 1000;
  regression = getRandomForestsRegressionModel();
  ret = testRegression(bfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  std::cout << "Testing regression on a polynomial function" << std::endl;
  std::vector<PrecisionType> coeffs;
  coeffs.push_back(0.0);
  coeffs.push_back(-1.0);
  coeffs.push_back(0.0);
  coeffs.push_back(4.0);
  PolynomialFunctionSampleGenerator<PrecisionType> pfsg(coeffs);
  param.count = 200;
  regression = getRandomForestsRegressionModel();
  ret = testRegression(pfsg,regression,param);
  if (ret == EXIT_FAILURE)
    {
    status = EXIT_FAILURE;
    }
  return status;
}
#endif
