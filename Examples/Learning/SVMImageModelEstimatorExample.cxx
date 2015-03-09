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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}, {ROI_mask.png}
//    OUTPUTS: {svm_image_model.svn}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{SVMImageModelEstimator} class. This class allows the
// estimation of a SVM model (supervised learning) from a feature
// image and an image of labels. In this example, we will train an SVM
// to separate between water and non-water pixels by using the RGB
// values only. The images used for this example are shown in
// figure~\ref{fig:SVMROIS}.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.45\textwidth]{ROI_mask.eps}
// \itkcaption[SVM Image Model Estimation]{Images used for the
// estimation of the SVM model. Left: RGB image. Right: image of labels.}
// \label{fig:SVMROIS}
// \end{figure}
// The first thing to do is include the header file for the class.
//
//  Software Guide : EndLatex

#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

//  Software Guide : BeginCodeSnippet
#include "otbSVMImageModelEstimator.h"
//  Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"

int main(int itkNotUsed(argc), char* argv[])
{

  const char* inputImageFileName = argv[1];
  const char* trainingImageFileName = argv[2];
  const char* outputModelFileName = argv[3];

//  Software Guide : BeginLatex
//
//  We define the types for the input and training images. Even if the
//  input image will be an RGB image, we can read it as a 3 component
//  vector image. This simplifies the interfacing with OTB's SVM
//  framework.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  typedef unsigned char InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;

  typedef otb::Image<InputPixelType,  Dimension> TrainingImageType;
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The \doxygen{otb}{SVMImageModelEstimator} class is templated over
//  the input (features) and the training (labels) images.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  typedef otb::SVMImageModelEstimator<InputImageType,
      TrainingImageType>   EstimatorType;

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  As usual, we define the readers for the images.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType>    InputReaderType;
  typedef otb::ImageFileReader<TrainingImageType> TrainingReaderType;

  InputReaderType::Pointer    inputReader = InputReaderType::New();
  TrainingReaderType::Pointer trainingReader = TrainingReaderType::New();

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We read the images. It is worth to note that, in order to ensure
//  the pipeline coherence, the output of the objects which preceed the
//  model estimator in the pipeline, must be up to date, so we call
//  the corresponding \code{Update} methods.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  inputReader->SetFileName(inputImageFileName);
  trainingReader->SetFileName(trainingImageFileName);

  inputReader->Update();
  trainingReader->Update();

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We can now instantiate the model estimator and set its parameters.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  EstimatorType::Pointer svmEstimator = EstimatorType::New();

  svmEstimator->SetInputImage(inputReader->GetOutput());
  svmEstimator->SetTrainingImage(trainingReader->GetOutput());

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
//  Finally, the estimated model can be saved to a file for later use.
//
//  Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  svmEstimator->SaveModel(outputModelFileName);

//  Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
