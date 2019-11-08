/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/* Example usage:
./SEMModelEstimatorExample Input/ROI_QB_MUL_4.tif Output/SEMClassif.png 4 40 5
*/

//
// In this example, we present OTB's implementation of SEM, through the class
// \doxygen{otb}{SEMClassifier}. This class performs a stochastic version
// of the EM algorithm, but instead of inheriting from
// \doxygen{itk}{ExpectationMaximizationMixtureModelEstimator}, we chose to
// inherit from \subdoxygen{itk}{Statistics}{ListSample},
// in the same way as \doxygen{otb}{SVMClassifier}.
//
// The program begins with \doxygen{otb}{VectorImage} and outputs
// \doxygen{otb}{Image}. Then appropriate header files have to be included:

#include <iostream>

#include "itkVector.h"
#include "itkVariableLengthVector.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

// \doxygen{otb}{SEMClassifier} performs estimation of mixture to fit the
// initial histogram. Actually, mixture of Gaussian pdf can be performed.
// Those generic pdf are treated in
// \subdoxygen{otb}{Statistics}{ModelComponentBase}. The Gaussian model
// is taken in charge with the class
// \subdoxygen{otb}{Statistics}{GaussianModelComponent}.

#include "otbSEMClassifier.h"

int main(int argc, char* argv[])
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

    // Input/Output images type are define in a classical way.
    // In fact, a \doxygen{itk}{VariableLengthVector} is to be
    // considered for the templated \code{MeasurementVectorType}, which
    // will be used in the \code{ListSample} interface.

    using PixelType = double;

    using ImageType  = otb::VectorImage<PixelType, 2>;
    using ReaderType = otb::ImageFileReader<ImageType>;

    using OutputImageType = otb::Image<unsigned char, 2>;
    using WriterType      = otb::ImageFileWriter<OutputImageType>;

    char*  fileNameIn           = argv[1];
    char*  fileNameImgInit      = nullptr;
    char*  fileNameOut          = argv[2];
    int    numberOfClasses      = atoi(argv[3]);
    int    numberOfIteration    = atoi(argv[4]);
    int    neighborhood         = atoi(argv[5]);
    double terminationThreshold = 1e-5;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(fileNameIn);
    reader->Update();

    // Once the input image is opened, the classifier may be initialised by
    // \code{SmartPointer}.

    using ClassifType               = otb::SEMClassifier<ImageType, OutputImageType>;
    ClassifType::Pointer classifier = ClassifType::New();

    //  Then, it follows, classical initializations of the pipeline.

    classifier->SetNumberOfClasses(numberOfClasses);
    classifier->SetMaximumIteration(numberOfIteration);
    classifier->SetNeighborhood(neighborhood);
    classifier->SetTerminationThreshold(terminationThreshold);
    classifier->SetSample(reader->GetOutput());

    // When an initial segmentation is available, the classifier may use it
    // as image (of type \code{OutputImageType}) or as a
    // \doxygen{itk}{SampleClassifier} result (of type
    // \subdoxygen{itk}{Statistics}{MembershipSample}).

    if (fileNameImgInit != nullptr)
    {
      using ImgInitReaderType              = otb::ImageFileReader<OutputImageType>;
      ImgInitReaderType::Pointer segReader = ImgInitReaderType::New();
      segReader->SetFileName(fileNameImgInit);
      segReader->Update();
      classifier->SetClassLabels(segReader->GetOutput());
    }

    //  By default, \doxygen{otb}{SEMClassifier} performs initialization of
    // \code{ModelComponentBase} by as many instantiation of
    // \subdoxygen{otb}{Statistics}{GaussianModelComponent} as the number of
    // classes to estimate in the mixture. Nevertheless, the user may add specific
    // distribution into the mixture estimation. It is permitted by the use of
    // \code{AddComponent} for the given class number and the specific distribution.

    std::cerr << "Explicit component initialization\n";

    using ClassSampleType = ClassifType::ClassSampleType;
    using GaussianType    = otb::Statistics::GaussianModelComponent<ClassSampleType>;

    for (int i = 0; i < numberOfClasses; ++i)
    {
      GaussianType::Pointer model = GaussianType::New();
      classifier->AddComponent(i, model);
    }

    //  Once the pipeline is instantiated. The segmentation by itself may be
    // launched by using the \code{Update} function.

    try
    {
      classifier->Update();
    }

    catch (itk::ExceptionObject& err)
    {
      std::cerr << "ExceptionObject caught in " << argv[0] << "!\n";
      std::cerr << err << std::endl;
      return -1;
    }

    //  The segmentation may outputs a result of type
    // \subdoxygen{itk}{Statistics}{MembershipSample} as it is the
    // case for the \doxygen{otb}{SVMClassifier}. But when using
    // \code{GetOutputImage} the output is directly an Image.
    //
    // Only for visualization purposes, we choose to rescale the image of
    // classes before saving it to a file. We will use the
    // \doxygen{itk}{RescaleIntensityImageFilter} for this purpose.

    using RescalerType             = itk::RescaleIntensityImageFilter<OutputImageType, OutputImageType>;
    RescalerType::Pointer rescaler = RescalerType::New();

    rescaler->SetOutputMinimum(itk::NumericTraits<unsigned char>::min());
    rescaler->SetOutputMaximum(itk::NumericTraits<unsigned char>::max());

    rescaler->SetInput(classifier->GetOutputImage());

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(fileNameOut);
    writer->SetInput(rescaler->GetOutput());
    writer->Update();

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

    std::cerr << "Program terminated with a ";
    if (classifier->GetTerminationCode() == ClassifType::CONVERGED)
      std::cerr << "converged ";
    else
      std::cerr << "not-converged ";
    std::cerr << "code...\n";

    //  The text output gives for each class the parameters of the pdf (e.g. mean
    // of each component of the class and there covariance matrix, in the case of a
    // Gaussian mixture model).

    classifier->Print(std::cerr);
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
