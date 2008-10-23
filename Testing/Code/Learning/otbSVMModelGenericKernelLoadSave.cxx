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

namespace otb
{

class LinearKernelFunctor : public GenericKernelFunctorBase
{
public:
  LinearKernelFunctor(): GenericKernelFunctorBase() {};
  virtual ~LinearKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      return this->dot(x,y);
    }
};

}

int otbSVMModelGenericKernelLoadSave( int argc, char* argv[] )
{
  typedef unsigned char                                   InputPixelType;
  typedef unsigned char                                   LabelPixelType;
  const   unsigned int        	                        Dimension = 2;
  
  typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
  
  typedef otb::SVMModel< InputPixelType, LabelPixelType >   ModelType;
  
  ModelType::Pointer svmModel = ModelType::New();
  
  otb::LinearKernelFunctor lFunctor;
  
  svmModel->SetKernelFunctor(&lFunctor);
  
  svmModel->LoadModel(argv[1]);
  svmModel->SaveModel(argv[2]);
  
  
  return EXIT_SUCCESS;
}


