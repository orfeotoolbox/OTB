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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_4.tif}
//    OUTPUTS: {SEMClassif.png}
//    4 40 5
//  Software Guide : EndCommandLineArgs
//
//  Software Guide : BeginLatex
//
// In this example, we present OTB's implementation of SEM, through the class
// \doxygen{otb}{SEMClassifier}. This class performs a stochastic version
// of the EM algorithm, but instead of inheriting from
// \doxygen{itk}{ExpectationMaximizationMixtureModelEstimator}, we choosed to
// inherit from \subdoxygen{itk}{Statistics}{ListSample< TSample >},
// in the same way as \doxygen{otb}{SVMClassifier}.
//
// The program begins with \doxygen{otb}{VectorImage} and outputs
// \doxygen{itb}{Image}. Then appropriate header files have to be included:
//
// Software Guide : EndLatex

#include <iostream>

#include "itkVector.h"
#include "itkVariableLengthVector.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
//  Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// \doxygen{otb}{SEMClassifier} performs estimation of mixture to fit the
// initial histogram. Actually, mixture of Gaussian pdf can be performed.
// Those generic pdf are treated in
// \subdoxygen{otb}{Statistics}{ModelComponentBase}. The Gaussian model
// is taken in charge with the class
// \subdoxygen{otb}{Statistics}{GaussianModelComponent}.
//
// Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
#include "otbSEMClassifier.h"
//  Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  try
    {

    if (argc != 6)
      {
      std::cerr << "Unsupervised Image Segmentation with SEM approach\n";
      std::cerr << argv[0] << " imageIn imgClassif num_of_class ";
      std::cerr << "nbIteration size_of_the_neighborhood\n";
      return EXIT_FAILURE;
      }

// Software Guide : BeginLatex
//
// Input/Output images type are define in a classical way.
// In fact, a \doxygen{itk}{VariableLengthVector} is to be
// considered for the templated \code{MeasurementVectorType}, which
// will be used in the \code{ListSample} interface.
//
// Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    typedef double PixelType;

    typedef otb::VectorImage<PixelType, 2>  ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;

    typedef otb::Image<unsigned char, 2>          OutputImageType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;
//  Software Guide : EndCodeSnippet

    char * fileNameIn = argv[1];
    char * fileNameImgInit = NULL;
    char * fileNameOut = argv[2];
    int    numberOfClasses = atoi(argv[3]);
    int    numberOfIteration = atoi(argv[4]);
    int    neighborhood = atoi(argv[5]);
    double terminationThreshold = 1e-5;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(fileNameIn);
    reader->Update();

//  Software Guide : BeginLatex
//
// Once the input image is opened, the classifier may be initialised by
// \code{SmartPointer}.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    typedef otb::SEMClassifier<ImageType, OutputImageType> ClassifType;
    ClassifType::Pointer classifier = ClassifType::New();
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  Then, it follows, classical initializations of the pipeline.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    classifier->SetNumberOfClasses(numberOfClasses);
    classifier->SetMaximumIteration(numberOfIteration);
    classifier->SetNeighborhood(neighborhood);
    classifier->SetTerminationThreshold(terminationThreshold);
    classifier->SetSample(reader->GetOutput());
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// When an initial segmentation is available, the classifier may use it
// as image (of type \code{OutputImageType}) or as a
// \doxygen{itk}{SampleClassifier} result (of type
// \subdoxygen{itk}{Statistics}{MembershipSample< SampleType >}).
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    if (fileNameImgInit != NULL)
      {
      typedef otb::ImageFileReader<OutputImageType> ImgInitReaderType;
      ImgInitReaderType::Pointer segReader = ImgInitReaderType::New();
      segReader->SetFileName(fileNameImgInit);
      segReader->Update();
      classifier->SetClassLabels(segReader->GetOutput());
      }
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  By default, \doxygen{otb}{SEMClassifier} performs initialization of
// \code{ModelComponentBase} by as many instanciation of
// \subdoxygen{otb}{Statistics}{GaussianModelComponent} as the number of
// classes to estimate in the mixture. Nevertheless, the user may add specific
// distribution into the mixture estimation. It is permitted by the use of
// \code{AddComponent} for the given class number and the specific distribution.
//  Software Guide : EndLatex

    std::cerr << "Explicit component initialization\n";

//  Software Guide : BeginCodeSnippet
    typedef ClassifType::ClassSampleType ClassSampleType;
    typedef otb::Statistics::GaussianModelComponent<ClassSampleType>
    GaussianType;

    for (int i = 0; i < numberOfClasses; ++i)
      {
      GaussianType::Pointer model = GaussianType::New();
      classifier->AddComponent(i, model);
      }
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  Once the pipeline is instanciated. The segmentation by itself may be
// launched by using the \code{Update} function.
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    try
      {
      classifier->Update();
      }
//  Software Guide : EndCodeSnippet

    catch (itk::ExceptionObject& err)
      {
      std::cerr << "ExceptionObject caught in " << argv[0] << "!\n";
      std::cerr << err << std::endl;
      return -1;
      }

//  Software Guide : BeginLatex
//
//  The segmentation may outputs a result of type
// \subdoxygen{itk}{Statistics}{MembershipSample< SampleType >} as it is the
// case for the \doxygen{otb}{SVMClassifier}. But when using
// \code{GetOutputImage} the output is directly an Image.
//
// Only for visualization purposes, we choose to rescale the image of
// classes before saving it to a file. We will use the
// \doxygen{itk}{RescaleIntensityImageFilter} for this purpose.
//
// Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    typedef itk::RescaleIntensityImageFilter<OutputImageType,
        OutputImageType> RescalerType;
    RescalerType::Pointer rescaler = RescalerType::New();

    rescaler->SetOutputMinimum(itk::NumericTraits<unsigned char>::min());
    rescaler->SetOutputMaximum(itk::NumericTraits<unsigned char>::max());

    rescaler->SetInput(classifier->GetOutputImage());

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(fileNameOut);
    writer->SetInput(rescaler->GetOutput());
    writer->Update();
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// Figure \ref{fig:RESSEMCLASSIF} shows the result of the SEM segmentation
// with 4 different classes and a contextual neighborhood of 3 pixels.
// \begin{figure}
//  \center
//  \includegraphics[width=0.6\textwidth]{SEMClassif.eps}
//  \itkcaption[SEM Classification results]{SEM Classification results.}
//  \label{fig:RESSEMCLASSIF}
// \end{figure}
//
// As soon as the segmentation is performed by an iterative stochastic
// process, it is worth verifying the output status: does the segmentation
// ends when it has converged or just at the limit of the iteration numbers.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    std::cerr << "Program terminated with a ";
    if (classifier->GetTerminationCode() ==
        ClassifType::CONVERGED) std::cerr << "converged ";
    else std::cerr << "not-converged ";
    std::cerr << "code...\n";
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The text output gives for each class the parameters of the pdf (e.g. mean
// of each component of the class and there covariance matrix, in the case of a
// Gaussian mixture model).
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
    classifier->Print(std::cerr);
//  Software Guide : EndCodeSnippet
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "Exception itk::ExceptionObject thrown !\n";
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }
  catch (...)
    {
    std::cerr << "Unknown exception thrown !\n";
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
