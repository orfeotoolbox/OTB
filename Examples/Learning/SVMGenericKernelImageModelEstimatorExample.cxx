/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) GET / ENST Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginLatex
// This example illustrates the modifications to be added to the
// use of \doxygen{otb}{SVMImageModelEstimator} in order to add a
// user defined kernel. This initial program has been explained in section
// \ref{ssec:LearningFromImages}.
//
// The first thing to do is to include the header file for the new kernel.
//
//  Software Guide : EndLatex

#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

//  Software Guide : BeginCodeSnippet
#include "otbSVMImageModelEstimator.h"
#include "otbMixturePolyRBFKernelFunctor.h"
//  Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"

int main(int argc, char* argv[])
{

  if (argc != 4)
    {
    std::cerr << "Usage : " << argv[0] << " inputImage mask modelFile\n";
    return EXIT_FAILURE;
    }

  const char* inputImageFileName = argv[1];
  const char* trainingImageFileName = argv[2];
  const char* outputModelFileName = argv[3];

  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;

  typedef otb::Image<InputPixelType,  Dimension> TrainingImageType;

  typedef otb::SVMImageModelEstimator<InputImageType,
      TrainingImageType>   EstimatorType;

  typedef otb::ImageFileReader<InputImageType>    InputReaderType;
  typedef otb::ImageFileReader<TrainingImageType> TrainingReaderType;

  InputReaderType::Pointer    inputReader = InputReaderType::New();
  TrainingReaderType::Pointer trainingReader = TrainingReaderType::New();

  inputReader->SetFileName(inputImageFileName);
  trainingReader->SetFileName(trainingImageFileName);

  inputReader->Update();
  trainingReader->Update();

//  Software Guide : BeginLatex
//
//  Once the \doxygen{otb}{SVMImageModelEstimator} is instanciated,
//  it is possible to add the new kernel and its parameters.
//
//  Then in addition to the initial code:
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  EstimatorType::Pointer svmEstimator = EstimatorType::New();

  svmEstimator->SetSVMType(C_SVC);
  svmEstimator->SetInputImage(inputReader->GetOutput());
  svmEstimator->SetTrainingImage(trainingReader->GetOutput());

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The instanciation of the kernel is to be implemented. The kernel which is
//  used here is a linear combination of a polynomial kernel and an RBF one.
//  It is written as $$\mu k_1(x, y) + (1-\mu) k_2(x, y)$$ with
//  $k_1(x, y)=\left( \gamma_1 x\cdot y + c_0 \right) ^d$ and
//  $k_2(x, y) = \exp\left( - \gamma_2 \| x-y\|^2 \right)$. Then, the specific
//  parameters of this kernel are:
//  \begin{itemize}
//  \item \code{Mixture} ($\mu$),
//  \item \code{GammaPoly} ($\gamma_1$),
//  \item \code{CoefPoly} ($c_0$),
//  \item \code{DegreePoly} ($d$),
//  \item \code{GammaRBF} ($\gamma_2$).
//  \end{itemize}
//  Their instanciations are achieved through the use of the \code{SetValue}
//  function.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
  otb::MixturePolyRBFKernelFunctor myKernel;
  myKernel.SetValue("Mixture", 0.5);
  myKernel.SetValue("GammaPoly", 1.0);
  myKernel.SetValue("CoefPoly", 0.0);
  myKernel.SetValue("DegreePoly", 1);
  myKernel.SetValue("GammaRBF", 1.5);
  myKernel.Update();
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  Once the kernel's parameters are affected and the kernel updated,
//  the connection to \doxygen{otb}{SVMImageModelEstimator} takes place here.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
  svmEstimator->SetKernelFunctor(&myKernel);
  svmEstimator->SetKernelType(GENERIC);
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The model estimation procedure is triggered by calling the
//  estimator's \code{Update} method.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  svmEstimator->Update();

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The rest of the code remains unchanged...
//
//  Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  svmEstimator->SaveModel(outputModelFileName);
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  In the file \code{outputModelFileName} a specific line will appear when using a
//  generic kernel. It gives the name of the kernel and its parameters name and value.
//
//  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
