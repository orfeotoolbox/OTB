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



#include "otbImage.h"
#include <iostream>

#include "otbSVMModel.h"
#include "otbSVMKernels.h"

//For second test
#include "otbSVMSampleListModelEstimator.h"
#include "otbSVMKernels.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbListSampleGenerator.h"

int otbSVMModelGenericKernelsTest(int argc, char* argv[])
{
  if (argc != 18)
    {
    return EXIT_FAILURE;
    }

  typedef unsigned char InputPixelType;
  typedef unsigned char LabelPixelType;

  typedef otb::SVMModel<InputPixelType, LabelPixelType> ModelType;

  ModelType::Pointer svmModel = ModelType::New();

  otb::CustomKernelFunctor               customFunctor;
  otb::InvMultiQuadricKernelFunctor      invMultiQuadricFunctor;
  otb::SAMKernelFunctor                  SAMFunctor;
  otb::KModKernelFunctor                 kModFunctor;
  otb::RadialSAMKernelFunctor            radialSAMFunctor;
  otb::InvMultiQuadraticSAMKernelFunctor invMultiQuadraticSAMFunctor;
  otb::KModSAMKernelFunctor              KModSAMFunctor;
  otb::RBFRBFSAMKernelFunctor            RBFRBFSAMFunctor;
  otb::PolyRBFSAMKernelFunctor           polyRBFSAMFunctor;
  otb::RBFDiffKernelFunctor              RBFDiffFunctor;
  otb::CustomLinearKernelFunctor         customLinearFunctor;
  otb::GroupedRBFKernelFunctor           groupedRBFFunctor;
  otb::GroupingAdaptiveKernelFunctor     groupingAdaptiveFunctor;
  otb::SpectralAngleKernelFunctor        spectAngleFunctor;
  otb::NonGaussianRBFKernelFunctor       nonGaussRBFFunctor;
  otb::ChangeProfileKernelFunctor        changeProFunctor;
  otb::MixturePolyRBFKernelFunctor       mixPolRBFFunctor;

  invMultiQuadricFunctor.SetValue<double>("const_coef", 2.);
  invMultiQuadraticSAMFunctor.SetValue<double>("const_coef", 3.);
  kModFunctor.SetValue<double>("const_coef", 1.5);
  KModSAMFunctor.SetValue<double>("const_coef", 1.8);
  RBFRBFSAMFunctor.SetValue<double>("const_coef", 1.7);
  polyRBFSAMFunctor.SetValue<double>("const_coef", 1.);
  polyRBFSAMFunctor.SetValue<double>("lin_coef", 1.2);
  groupingAdaptiveFunctor.SetValue<double>("const_coef", 8.2);
  groupingAdaptiveFunctor.SetValue<double>("lin_coef", 5.2);
  spectAngleFunctor.SetValue<double>("Coef", 2.5);
  nonGaussRBFFunctor.SetValue<double>("Alpha", 1.1);
  nonGaussRBFFunctor.SetValue<double>("Beta", 2.1);
  nonGaussRBFFunctor.SetValue<double>("Gamma", 1.1);
  changeProFunctor.SetValue<double>("Coef", 0.1);
  changeProFunctor.SetValue<double>("Degree", 1.1);
  changeProFunctor.SetValue<double>("Gamma", 1.1);
  mixPolRBFFunctor.SetValue<double>("Mixture", 0.5);
  mixPolRBFFunctor.SetValue<double>("GammaPoly", 1.0);
  mixPolRBFFunctor.SetValue<double>("CoefPoly", 1.0);
  mixPolRBFFunctor.SetValue<double>("DegreePoly", 2.0);
  mixPolRBFFunctor.SetValue<double>("GammaRBF", 1.0);


  struct svm_model *model;
  model = new svm_model;
  model->param.svm_type = 0;
  model->param.kernel_type = 5;
  model->nr_class = 2;
  model->l = 5;
  model->sv_coef = new double*[model->nr_class - 1];

  for (int i = 0; i < model->nr_class - 1; ++i)
    model->sv_coef[i] = new double[model->l];
  model->SV = new svm_node *[model->l];
 
  model->sv_coef[0][0] = 0.125641;
  model->sv_coef[0][1] = 1;
  model->sv_coef[0][2] = 0;
  model->sv_coef[0][3] = -1;
  model->sv_coef[0][4] = -0.54994;

  model->rho = new double[1];
  model->probA = new double[1];
  model->probB = new double[1];
  model->rho[0] = 22.3117;
  model->probA[0] = -0.541009;
  model->probB[0] = -0.687381;
  model->param.const_coef = 2.;
  model->param.lin_coef = 5.;
  model->param.gamma = 1.5;
  model->param.degree = 2;

  struct svm_node *p =  new struct svm_node[20];
  for (unsigned int n = 0; n < 20; ++n)
    {
    p[n].index = -1;
    p[n].value = 0.;
    }
  model->SV[0] = &p[0];
  model->SV[1] = &p[4];
  model->SV[2] = &p[8];
  model->SV[3] = &p[12];
  model->SV[4] = &p[16];
  p[0].index = 1;
  p[0].value = 89;
  p[1].index = 2;
  p[1].value = 123;
  p[2].index = 3;
  p[2].value = 134;
  p[3].index = -1;
  p[4].index = 1;
  p[4].value = 80;
  p[5].index = 2;
  p[5].value = 113;
  p[6].index = 3;
  p[6].value = 120;
  p[7].index = -1;
  p[8].index = 1;
  p[8].value = 75;
  p[9].index = 2;
  p[9].value = 108;
  p[10].index = 3;
  p[10].value = 105;
  p[11].index = -1;
  p[12].index = 1;
  p[12].value = 34;
  p[13].index = 2;
  p[13].value = 60;
  p[14].index = 3;
  p[14].value = 67;
  p[15].index = -1;
  p[16].index = 1;
  p[16].value = 208;
  p[17].index = 2;
  p[17].value = 255;
  p[18].index = 1;
  p[18].value = 255;
  p[19].index = -1;

  model->label = new int[2];
  model->label[0] = 1;
  model->label[1] = -1;
  model->nSV = new int[2];
  model->nSV[0] = 3;
  model->nSV[1] = 2;

  model->param.kernel_generic = customFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[1]);
  model->param.kernel_generic = invMultiQuadricFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[2]);
  model->param.kernel_generic = SAMFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[3]);
  model->param.kernel_generic = kModFunctor.Clone(); ;
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[4]);
  model->param.kernel_generic = radialSAMFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[5]);
  model->param.kernel_generic = invMultiQuadraticSAMFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[6]);
  model->param.kernel_generic = KModSAMFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[7]);
  model->param.kernel_generic = RBFRBFSAMFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[8]);
  model->param.kernel_generic = polyRBFSAMFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[9]);
  model->param.kernel_generic = RBFDiffFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[10]);
  model->param.kernel_generic = customLinearFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[11]);
  model->param.kernel_generic = groupedRBFFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[12]);
  model->param.kernel_generic = groupingAdaptiveFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[13]);
  model->param.kernel_generic = spectAngleFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[14]);
  model->param.kernel_generic = nonGaussRBFFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[15]);
  model->param.kernel_generic = changeProFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[16]);
  model->param.kernel_generic = mixPolRBFFunctor.Clone();
  svmModel->SetModel(model);
  svmModel->SaveModel(argv[17]);

  // Free all memory

  
  for (int i = 0; i < model->nr_class - 1; ++i)
    {
    delete [] model->sv_coef[i];
    }
  delete [] model->sv_coef;
  
  delete [] model->SV;

  delete model->rho;
  delete model->probA;
  delete model->probB;
  delete [] model->label;
  delete [] model->nSV;
  delete model;
  delete [] p;

  return EXIT_SUCCESS;
}

template<class KernelType>
int otbSVMKernelsTest_generic(int argc, char* argv[])
{
  if (argc != 5)
    {
    return EXIT_FAILURE;
    }

  std::string imageFilename = argv[1];
  std::string vectorDataFilename = argv[2];
  std::string outputModelFileName= argv[3];
  int         maxTrainingSize = 500;
  int         maxValidationSize = 500;
  double      validationTrainingProportion = 0.5;

  std::string classKey = "Class";

  typedef double                          PixelType;
  typedef otb::VectorImage<PixelType, 2>  ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->UpdateOutputInformation();

  typedef otb::VectorData<float, 2>                 VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

  VectorDataReaderType::Pointer vectorReader = VectorDataReaderType::New();
  vectorReader->SetFileName(vectorDataFilename);
  vectorReader->Update();

  typedef otb::ListSampleGenerator<ImageType, VectorDataType> ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer generator = ListSampleGeneratorType::New();
  generator->SetMaxTrainingSize(maxTrainingSize);
  generator->SetMaxValidationSize(maxValidationSize);
  generator->SetValidationTrainingProportion(validationTrainingProportion);

  generator->SetInput(reader->GetOutput());
  generator->SetInputVectorData(vectorReader->GetOutput());

  generator->Update();
  std::cout << generator << std::endl;

  typedef ListSampleGeneratorType::ListSampleType                                  ListSampleType;
  typedef ListSampleGeneratorType::ListLabelType                                   TrainingListSampleType;
  typedef otb::SVMSampleListModelEstimator<ListSampleType, TrainingListSampleType> EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetInputSampleList(generator->GetTrainingListSample());
  estimator->SetTrainingSampleList(generator->GetTrainingListLabel());


//  KernelType::Pointer kernel = KernelType::New();
  KernelType kernel;
  std::cout << "Kernel: " << kernel.GetName() << std::endl;
  estimator->SetKernelFunctor(&kernel);
  estimator->SetKernelType(GENERIC);

  estimator->Update();

  estimator->GetModel()->SaveModel(outputModelFileName);

  return EXIT_SUCCESS;
}

int otbSVMKernelsTest(int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0] << " inputImage inputVectorData outputModelFileName kernelType"
              << std::endl;
    return EXIT_FAILURE;
    }

  int kernelType = atoi(argv[4]);
  switch (kernelType)
    {
    case 0:
      return otbSVMKernelsTest_generic<otb::CustomKernelFunctor>(argc, argv);
      break;
    case 1:
      return otbSVMKernelsTest_generic<otb::InvMultiQuadricKernelFunctor>(argc, argv);
      break;
    case 2:
      return otbSVMKernelsTest_generic<otb::KModKernelFunctor>(argc, argv);
      break;
    case 3:
      return otbSVMKernelsTest_generic<otb::SAMKernelFunctor>(argc, argv);
      break;
    case 4:
      return otbSVMKernelsTest_generic<otb::RadialSAMKernelFunctor>(argc, argv);
      break;
    case 5:
      return otbSVMKernelsTest_generic<otb::InverseCosSAMKernelFunctor>(argc, argv);
      break;
    case 6:
      return otbSVMKernelsTest_generic<otb::InvMultiQuadraticSAMKernelFunctor>(argc, argv);
      break;
    case 7:
      return otbSVMKernelsTest_generic<otb::KModSAMKernelFunctor>(argc, argv);
      break;
    case 8:
      return otbSVMKernelsTest_generic<otb::RBFKernelFunctor>(argc, argv);
      break;
    case 9:
      return otbSVMKernelsTest_generic<otb::RBFRBFSAMKernelFunctor>(argc, argv);
      break;
    case 10:
      return otbSVMKernelsTest_generic<otb::PolyRBFSAMKernelFunctor>(argc, argv);
      break;
    case 11:
      return otbSVMKernelsTest_generic<otb::RBFDiffKernelFunctor>(argc, argv);
      break;
    case 12:
      return otbSVMKernelsTest_generic<otb::CustomLinearKernelFunctor>(argc, argv);
      break;
    case 13:
      return otbSVMKernelsTest_generic<otb::GroupedRBFKernelFunctor>(argc, argv);
      break;
    case 14:
      return otbSVMKernelsTest_generic<otb::GroupingAdaptiveKernelFunctor>(argc, argv);
      break;
    case 15:
      return otbSVMKernelsTest_generic<otb::SpectralAngleKernelFunctor>(argc, argv);
      break;
    case 16:
      return otbSVMKernelsTest_generic<otb::NonGaussianRBFKernelFunctor>(argc, argv);
      break;
    case 17:
      return otbSVMKernelsTest_generic<otb::ChangeProfileKernelFunctor>(argc, argv);
      break;
    case 18:
      return otbSVMKernelsTest_generic<otb::MixturePolyRBFKernelFunctor>(argc, argv);
      break;
    default:
       std::cerr << "No more kernel available\n";
       return EXIT_FAILURE;
    }
}
