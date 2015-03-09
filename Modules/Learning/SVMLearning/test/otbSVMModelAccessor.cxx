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

int otbSVMModelAccessor(int itkNotUsed(argc), char* argv[])
{
  typedef unsigned char InputPixelType;
  typedef unsigned char LabelPixelType;

  typedef otb::SVMModel<InputPixelType, LabelPixelType> ModelType;

  ModelType::Pointer ptrModel = ModelType::New();

  ptrModel->LoadModel(argv[1]);

  std::ofstream f;
  unsigned int  nbClass = ptrModel->GetNumberOfClasses();
  unsigned int  nbSupportVector = ptrModel->GetNumberOfSupportVectors();

  f.open(argv[2]);
  f << "Test methods of SVMModel class:" << std::endl;
  f << " - GetNumberOfClasses()        " << nbClass << std::endl;
  f << " - GetNumberOfHyperplane()     " << ptrModel->GetNumberOfHyperplane() << std::endl;
  f << " - GetNumberOfSupportVectors() " << nbSupportVector << std::endl;

  f << " - GetSupportVectors() [nb support vector][]" << std::endl;
  svm_node ** SVs = ptrModel->GetSupportVectors();
  if (SVs == NULL)
    {
    itkGenericExceptionMacro(<< "SVs NULL");
    }
  for (unsigned int i = 0; i < nbSupportVector; ++i)
    {
    if (SVs[i] == NULL) itkGenericExceptionMacro(<< "SVs " << i << " NULL");
    f << std::endl;
    f << "  SV[" << i << "]:";
    const svm_node *p = SVs[i];
    while (p->index != -1)
      {
      f << " [" << p->index << ";" << p->value << "] ";
      p++;
      }
    f << std::endl;
    }

  f << " - GetRho() [nr_class*(nr_class-1)/2]" << std::endl;
  unsigned int taille = nbClass * (nbClass - 1) / 2;
  double *     rhos = ptrModel->GetRho();
  if (rhos == NULL)
    {
    itkGenericExceptionMacro(<< "rhos NULL");
    }
  f << "      ";
  for (unsigned int i = 0; i < taille; ++i)
    {
    f << " " << rhos[i];
    }

  f << std::endl;
  f << " - GetAlpha() [nb class-1][nb support vector]" << std::endl;
  double ** alphas = ptrModel->GetAlpha();
  if (alphas == NULL)
    {
    itkGenericExceptionMacro(<< "alphas NULL");
    }
  for (unsigned int i = 0; i < nbClass - 1; ++i)
    {
    if (alphas[i] == NULL) itkGenericExceptionMacro(<< "alphas " << i << " NULL");
    f << "     ";
    for (unsigned int j = 0; j < nbSupportVector; ++j)
      {
      f << "  " << alphas[i][j];
      }
    }
  f << std::endl;
//   f << " - Evaluate() (double) -> "<<ptrModel->Evaluate()<<std::endl;

//   typedef ModelType::ValuesType ValuesType;
//   ValuesType _evaluateHyperplaneDistance;
//   _evaluateHyperplaneDistance = ptrModel->EvaluateHyperplaneDistance();

//   f << " - EvaluateHyperplaneDistance() VariableLengthVector() nb value(s): "<<_evaluateHyperplaneDistance.Size()<<std::endl;
//   for (unsigned int i=0; i<_evaluateHyperplaneDistance.Size(); ++i)
//   {
//     f << "     "<<_evaluateHyperplaneDistance[i]<<std::endl;
//   }
  f << "end" << std::endl;
  f.close();

  return EXIT_SUCCESS;
}
