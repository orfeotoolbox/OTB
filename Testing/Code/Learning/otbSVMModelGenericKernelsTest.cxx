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
 


int otbSVMModelGenericKernelsTest( int argc, char* argv[] )
{
  typedef unsigned char                                   InputPixelType;
  typedef unsigned char                                   LabelPixelType;

  typedef otb::SVMModel< InputPixelType, LabelPixelType >   ModelType;
  
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



  invMultiQuadricFunctor.SetValue<double>("const_coef", 2.);
  invMultiQuadraticSAMFunctor.SetValue<double>("const_coef", 3.);
  kModFunctor.SetValue<double>("const_coef", 1.5);
  KModSAMFunctor.SetValue<double>("const_coef", 1.8);
  RBFRBFSAMFunctor.SetValue<double>("const_coef", 1.7);
  polyRBFSAMFunctor.SetValue<double>("const_coef", 1.);
  polyRBFSAMFunctor.SetValue<double>("lin_coef", 1.2);
  groupingAdaptiveFunctor.SetValue<double>("const_coef", 8.2);
  groupingAdaptiveFunctor.SetValue<double>("lin_coef", 5.2);

  struct svm_model *model;
  model = (struct svm_model *)malloc(sizeof(struct svm_model));
  model->param.kernel_type = 5;
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

  struct svm_node *p =  Malloc(struct svm_node,20);
  for(unsigned int n = 0;n<20;++n)
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

  model->label = Malloc(int,2);
  model->label[0] = 1;
  model->label[1] = -1;
  model->nSV = Malloc(int,2);
  model->nSV[0] = 3;
  model->nSV[1] = 2;
			
  model->param.kernel_generic = &customFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes1");
  model->param.kernel_generic = &invMultiQuadricFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes2");
  model->param.kernel_generic = &SAMFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes3");
  model->param.kernel_generic = &kModFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes4");
  model->param.kernel_generic = &radialSAMFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes5");
  model->param.kernel_generic = &invMultiQuadraticSAMFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes6");
  model->param.kernel_generic = &KModSAMFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes7");
  model->param.kernel_generic = &RBFRBFSAMFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes8");
  model->param.kernel_generic = &polyRBFSAMFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes9");
  model->param.kernel_generic = &RBFDiffFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes10");
  model->param.kernel_generic = &customLinearFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes11");
  model->param.kernel_generic = &groupedRBFFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes12");
  model->param.kernel_generic = &groupingAdaptiveFunctor;
  svmModel->SetModel(model);
  svmModel->SaveModel("../../Temporary/leTuKernelsTestRes13");


  return EXIT_SUCCESS;
}


