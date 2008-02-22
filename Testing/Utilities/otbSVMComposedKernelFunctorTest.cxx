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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "otbImage.h"
#include <iostream>

#include "otbSVMModel.h"
#include "otbSVMKernels.h"
#include "svm.h"



int otbSVMComposedKernelFunctorTest( int argc, char* argv[] )
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
  model = (struct svm_model *)malloc(sizeof(struct svm_model));
  model->param.kernel_type = 6;
  model->nr_class = 2;
  model->l = 5;
  model->sv_coef = Malloc(double *,model->nr_class-1);
 
  for(int i=0; i<model->nr_class-1; i++)
    model->sv_coef[i] = Malloc(double,model->l);
  model->SV = Malloc(svm_node*,model->l);
  for(int n = 0; n<model->l; ++n)
    {
      model->SV[n]=Malloc(svm_node,1);
      model->SV[n]->index = -1;
      model->SV[n]->value = 0.;
    }

  model->sv_coef[0][0] = 0.125641;
  model->sv_coef[0][1] = 1;
  model->sv_coef[0][2] = 0;
  model->sv_coef[0][3] = -1;
  model->sv_coef[0][4] = -0.54994;

  model->rho = Malloc(double,1);
  model->probA = Malloc(double,1);
  model->probB = Malloc(double,1);
  model->rho[0] = 22.3117;
  model->probA[0] = -0.541009;
  model->probB[0] = -0.687381;
  model->param.const_coef = 2.;
  model->param.lin_coef = 5.;
  model->param.gamma = 1.5;
  model->param.degree = 2;


  model->label = Malloc(int,2);
  model->label[0] = 1;
  model->label[1] = -1;
  model->nSV = Malloc(int,2);
  model->nSV[0] = 3;
  model->nSV[1] = 2;
			
  model->param.kernel_composed = &composedKernelFunctor;
  svmModel->SetModel(model);

  struct svm_node *x =  Malloc(struct svm_node,3);
  struct svm_node *y =  Malloc(struct svm_node,3);
  
  struct svm_node **SVx = Malloc(svm_node*,1);
  struct svm_node **SVy = Malloc(svm_node*,1);
  SVx[0] = Malloc(svm_node,1);
  SVy[0] = Malloc(svm_node,1);
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
  double res1 = 0.;
  double res2 = 0.;

  res1 = customFunctor(SVx[0], SVy[0], model->param);
  std::cout<<"customFunctor : "<<res1<<std::endl;
  res2 = SAMFunctor(SVx[0], SVy[0], model->param);
  std::cout<<"SAMFunctor : "<<res2<<std::endl;

  resAdd = (*(svmModel->GetModel()->param.kernel_composed))(SVx[0], SVy[0], svmModel->GetModel()->param);

  std::cout<<"composed : "<<resAdd<<std::endl;

  svmModel->GetModel()->param.kernel_composed->SetMultiplyKernelFunctor(true);
  resAdd = (*(svmModel->GetModel()->param.kernel_composed))(SVx[0], SVy[0], svmModel->GetModel()->param);
  
  std::cout<<"composed : "<<resAdd<<std::endl;

  svmModel->GetModel()->param.kernel_composed->print_parameters();

  svmModel->SaveModel("test.txt");

  ModelType::Pointer svmModelBis = ModelType::New();
  svmModelBis->LoadModel("test.txt");

  svmModelBis->GetModel()->param.kernel_composed->print_parameters();

  svmModelBis->SaveModel("testBis.txt");


  return EXIT_SUCCESS;
}




