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

#include "otbSVMModel.h"

namespace otb
{

class LinearKernelFunctor : public GenericKernelFunctorBase
{
public:
  typedef LinearKernelFunctor                 Self;
  typedef GenericKernelFunctorBase            Superclass;

  LinearKernelFunctor() : GenericKernelFunctorBase() {}
  virtual ~LinearKernelFunctor() {}

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  virtual double operator ()(const svm_node *x, const svm_node *y, const svm_parameter&) const
  {
    return this->dot(x, y);
  }

protected:
  LinearKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  LinearKernelFunctor& operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    return *this;
  }
};

}

int otbSVMModelGenericKernelLoadSave(int argc, char* argv[])
{
  typedef unsigned char InputPixelType;
  typedef unsigned char LabelPixelType;

  typedef otb::SVMModel<InputPixelType, LabelPixelType> ModelType;

  ModelType::Pointer svmModel = ModelType::New();

  otb::LinearKernelFunctor lFunctor;

  svmModel->SetKernelFunctor(&lFunctor);

  svmModel->LoadModel(argv[1]);
  svmModel->SaveModel(argv[2]);

  return EXIT_SUCCESS;
}
