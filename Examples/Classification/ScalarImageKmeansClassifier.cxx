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
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {QB_Suburb_labelled.png}
//    0 4 79.5097 138.136 213.846 25.9395
//    NORMALIZE_EPS_OUTPUT_OF: {QB_Suburb_labelled.png}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example shows how to use the KMeans model for classifying the pixel of
// a scalar image.
//
// The  \subdoxygen{itk}{Statistics}{ScalarImageKmeansImageFilter} is used for taking
// a scalar image and applying the K-Means algorithm in order to define classes
// that represents statistical distributions of intensity values in the pixels.
// The classes are then used in this filter for generating a labeled image where
// every pixel is assigned to one of the classes.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkScalarImageKmeansImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc < 5)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0];
    std::cerr << " inputScalarImage outputLabeledImage contiguousLabels";
    std::cerr << " numberOfClasses mean1 mean2... meanN " << std::endl;
    return EXIT_FAILURE;
    }

  const char * inputImageFileName = argv[1];

// Software Guide : BeginLatex
//
// First we define the pixel type and dimension of the image that we intend to
// classify. With this image type we can also declare the
// \doxygen{otb}{ImageFileReader} needed for reading the input image, create one and
// set its input filename.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef signed short PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageFileName);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// With the \code{ImageType} we instantiate the type of the
// \doxygen{itk}{ScalarImageKmeansImageFilter} that will compute the K-Means model
// and then classify the image pixels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::ScalarImageKmeansImageFilter<ImageType> KMeansFilterType;

  KMeansFilterType::Pointer kmeansFilter = KMeansFilterType::New();

  kmeansFilter->SetInput(reader->GetOutput());

  const unsigned int numberOfInitialClasses = atoi(argv[4]);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// In general the classification will produce as output an image whose pixel
// values are integers associated to the labels of the classes. Since typically
// these integers will be generated in order (0, 1, 2, ...N), the output image
// will tend to look very dark when displayed with naive viewers. It is
// therefore convenient to have the option of spreading the label values over
// the dynamic range of the output image pixel type. When this is done, the
// dynamic range of the pixels is divided by the number of classes in order to
// define the increment between labels. For example, an output image of 8 bits
// will have a dynamic range of [0:255], and when it is used for holding four
// classes, the non-contiguous labels will be (0, 64, 128, 192). The selection of
// the mode to use is done with the method \code{SetUseContiguousLabels()}.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int useNonContiguousLabels = atoi(argv[3]);

  kmeansFilter->SetUseNonContiguousLabels(useNonContiguousLabels);
// Software Guide : EndCodeSnippet

  const unsigned int argoffset = 5;

  if (static_cast<unsigned int>(argc) <
      numberOfInitialClasses + argoffset)
    {
    std::cerr << "Error: " << std::endl;
    std::cerr << numberOfInitialClasses << " classes has been specified ";
    std::cerr << "but no enough means have been provided in the command ";
    std::cerr << "line arguments " << std::endl;
    return EXIT_FAILURE;
    }

// Software Guide : BeginLatex
//
// For each one of the classes we must provide a tentative initial value for
// the mean of the class. Given that this is a scalar image, each one of the
// means is simply a scalar value. Note however that in a general case of
// K-Means, the input image would be a vector image and therefore the means
// will be vectors of the same dimension as the image pixels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  for (unsigned k = 0; k < numberOfInitialClasses; ++k)
    {
    const double userProvidedInitialMean = atof(argv[k + argoffset]);
    kmeansFilter->AddClassWithInitialMean(userProvidedInitialMean);
    }
// Software Guide : EndCodeSnippet

  const char * outputImageFileName = argv[2];

// Software Guide : BeginLatex
//
// The \doxygen{itk}{ScalarImageKmeansImageFilter} is predefined for producing an 8
// bits scalar image as output. This output image contains labels associated
// to each one of the classes in the K-Means algorithm. In the following lines
// we use the \code{OutputImageType} in order to instantiate the type of a
// \doxygen{otb}{ImageFileWriter}. Then create one, and connect it to the output of
// the classification filter.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef KMeansFilterType::OutputImageType OutputImageType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput(kmeansFilter->GetOutput());

  writer->SetFileName(outputImageFileName);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We are now ready for triggering the execution of the pipeline. This is done
// by simply invoking the \code{Update()} method in the writer. This call will
// propagate the update request to the reader and then to the classifier.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& excp)
    {
    std::cerr << "Problem encountered while writing ";
    std::cerr << " image file : " << argv[2] << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// At this point the classification is done, the labeled image is saved in a
// file, and we can take a look at the means that were found as a result of the
// model estimation performed inside the classifier filter.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  KMeansFilterType::ParametersType estimatedMeans =
    kmeansFilter->GetFinalMeans();

  const unsigned int numberOfClasses = estimatedMeans.Size();

  for (unsigned int i = 0; i < numberOfClasses; ++i)
    {
    std::cout << "cluster[" << i << "] ";
    std::cout << "    estimated mean : " << estimatedMeans[i] << std::endl;
    }
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// \begin{figure} \center
// \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
// \includegraphics[width=0.44\textwidth]{QB_Suburb_labelled.eps}
// \itkcaption[Output of the KMeans classifier]{Effect of the
// KMeans classifier. Left: original image. Right: image of classes.}
// \label{fig:ScalarImageKMeansClassifierOutput}
// \end{figure}
//
//  Figure \ref{fig:ScalarImageKMeansClassifierOutput}
//  illustrates the effect of this filter with three classes.
//  The means can be estimated by ScalarImageKmeansModelEstimator.cxx.
//
//  Software Guide : EndLatex

  return EXIT_SUCCESS;

}
