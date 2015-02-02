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

#include "itkMacro.h"
#include "otbImage.h"
#include <iostream>
#include <fstream>

#include "otbSVMModel.h"
#include "otbSVMKernels.h"
#include "svm.h"


int otbSVMComposedKernelFunctorTest( int itkNotUsed(argc), char* argv[] )
{
  typedef unsigned char                                   InputPixelType;
  typedef unsigned char                                   LabelPixelType;

  typedef otb::SVMModel< InputPixelType, LabelPixelType >   ModelType;

  ModelType::Pointer svmModel = ModelType::New();

  otb::CustomKernelFunctor         customFunctor;
  otb::SAMKernelFunctor            SAMFunctor;
  ComposedKernelFunctor            composedKernelFunctor;
  composedKernelFunctor.AddKernelFunctorModelToKernelList(&customFunctor);
  composedKernelFunctor.AddKernelFunctorModelToKernelList(&SAMFunctor);
  composedKernelFunctor.AddPonderationToPonderationList(1.5);
  composedKernelFunctor.AddPonderationToPonderationList(2.0);
  composedKernelFunctor.SetName("compositionFilter");

  struct svm_model *model;
  model = new svm_model;
  model->param.svm_type = 0;
  model->param.kernel_type = 6;
  model->nr_class = 2;
  model->l = 5;
  model->sv_coef = new double*[model->nr_class-1];

  for(int i=0; i<model->nr_class-1; i++)
    model->sv_coef[i] = new double[model->l];
  model->SV = new svm_node*[model->l];
  for(int n = 0; n<model->l; ++n)
    {
      model->SV[n]= new svm_node;
      model->SV[n]->index = -1;
      model->SV[n]->value = 0.;
    }

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


  model->label = new int[2];
  model->label[0] = 1;
  model->label[1] = -1;
  model->nSV = new int[2];
  model->nSV[0] = 3;
  model->nSV[1] = 2;

  model->param.kernel_composed = &composedKernelFunctor;
  svmModel->SetModel(model);

  struct svm_node *x = new svm_node[3];
  struct svm_node *y = new svm_node[3];

  struct svm_node **SVx = new svm_node*[1];
  struct svm_node **SVy = new svm_node*[1];
  SVx[0] = new svm_node[1];
  SVy[0] = new svm_node[1];
  SVx[0] = &x[0];
  SVy[0] = &y[0];

  x[0].index = 1;
  x[0].value = 10;
  x[1].index = -1;
  x[1].value = 10000;

  y[0].index = 1;
  y[0].value = 5;
  y[1].index = -1;
  y[1].value = 10000;

  double resAdd =0.;
  double resMul =0.;
  double res1 = 0.;
  double res2 = 0.;

  std::ofstream file;
  file.open(argv[1]);

  file<<"Inputs Values: 10, 5, Inputs Ponderation: 1.5, 2"<<std::endl;

  file<<std::endl;


  file<<"Functor Results:"<<std::endl;

  res1 = customFunctor(SVx[0], SVy[0], model->param);
  file<<"Custom Functor only: "<<res1<<std::endl;
  //std::cout<<"customFunctor : "<<res1<<std::endl;
  res2 = SAMFunctor(SVx[0], SVy[0], model->param);
  file<<"SAM Functor only: "<<res2<<std::endl;
  //std::cout<<"SAMFunctor : "<<res2<<std::endl;
  file<<"Composed Functor: "<<std::endl;
  resAdd = (*(svmModel->GetModel()->param.kernel_composed))(SVx[0], SVy[0], svmModel->GetModel()->param);

  //std::cout<<"composed : "<<resAdd<<std::endl;

  svmModel->GetModel()->param.kernel_composed->SetMultiplyKernelFunctor(true);
  resMul = (*(svmModel->GetModel()->param.kernel_composed))(SVx[0], SVy[0], svmModel->GetModel()->param);

  //std::cout<<"composed : "<<resAdd<<std::endl;
  file<<"Addition: "<<resAdd<<", "<<"Multiplication: "<<resMul<<std::endl;

  file.close();

  //svmModel->GetModel()->param.kernel_composed->print_parameters();

  svmModel->SaveModel(argv[2]);

  ModelType::Pointer svmModelBis = ModelType::New();
  svmModelBis->LoadModel(argv[2]);

  //svmModelBis->GetModel()->param.kernel_composed->print_parameters();

  svmModelBis->SaveModel(argv[3]);

  // Free all memory

  for(int i=0; i<model->nr_class-1; i++)
    delete [] model->sv_coef[i];
  delete model->sv_coef;

  for(int n = 0; n<model->l; ++n)
    delete model->SV[n];

  delete [] model->SV;  
  delete [] model->rho;
  delete [] model->probA;
  delete [] model->probB;
  delete [] model->label;
  delete [] model->nSV;
  delete [] x;
  delete [] y;
  delete [] SVx[0];
  delete [] SVy[0];
  delete [] SVx;
  delete [] SVy;

  delete model;

  return EXIT_SUCCESS;
}
