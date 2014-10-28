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


#include <fstream>
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkListSample.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}, {ROI_QB_MUL_SOM.png}
//    OUTPUTS: {ROI_QB_MUL_SOMCLASS.png}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{SOMClassifier} class for performing a classification
// using an existing Kohonen's Self Organizing. Actually, the SOM
// classification consists only in the attribution of the winner
// neuron index to a given feature vector.
//
// We will use the SOM created in section \ref{sec:SOMColorTable} and
// we will assume that each neuron represents a class in the image.
//
// The first thing to do is include the header file for the
// class.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSOMClassifier.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{

  if (argc != 4)
    {
    std::cout << "Usage : " << argv[0] << " inputImage modelFile outputImage"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char * imageFilename  = argv[1];
  const char * mapFilename  = argv[2];
  const char * outputFilename = argv[3];

  typedef double        InputPixelType;
  typedef unsigned char LabelPixelType;
  const unsigned int Dimension = 2;

  typedef itk::VariableLengthVector<InputPixelType> PixelType;

//  Software Guide : BeginLatex
//
// As for the SOM learning step, we must define the types for the
// \code{otb::SOMMap}, and therefore, also for the distance to be
// used. We will also define the type for the SOM reader, which is
// actually an \subdoxygen{otb}{ImageFileReader} which the appropiate
// image type.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::Statistics::EuclideanDistanceMetric<PixelType>   DistanceType;
  typedef otb::SOMMap<PixelType, DistanceType, Dimension> SOMMapType;
  typedef otb::ImageFileReader<SOMMapType>                SOMReaderType;
// Software Guide : EndCodeSnippet

  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>        ReaderType;

//  Software Guide : BeginLatex
//
//  The classification will be performed by the
//  \subdoxygen{otb}{SOMClassifier}, which, as most of the
//  classifiers, works on
//  \subdoxygen{itk}{Statistics}{ListSample}s. In order to be able
//  to perform an image classification, we will need to use the
//  \subdoxygen{itk}{Statistics}{ImageToListAdaptor} which is
//  templated over the type of image to be adapted. The
//  \code{SOMClassifier} is templated over the sample type, the SOMMap
//  type and the pixel type for the labels.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::Statistics::ListSample<PixelType> SampleType;
  typedef otb::SOMClassifier<SampleType, SOMMapType, LabelPixelType>
  ClassifierType;
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  The result of the classification will be stored on an image and
//  saved to a file. Therefore, we define the types needed for this step.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::Image<LabelPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  We can now start reading the input image and the SOM given as
//  inputs to the program. We instantiate the readers as usual.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->Update();

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(mapFilename);
  somreader->Update();
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  The conversion of the input data from image to list sample is
//  easily done using the adaptor.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  SampleType::Pointer sample = SampleType::New();

  itk::ImageRegionIterator<InputImageType> it(reader->GetOutput(),
                                              reader->GetOutput()->
                                              GetLargestPossibleRegion());

  sample->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    sample->PushBack(it.Get());
    ++it;
    }
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  The classifier can now be instantiated. The input data is set by
//  using the \code{SetSample()} method and the SOM si set using the
//  \code{SetMap()} method. The classification is triggered by using
//  the \code{Update()} method.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassifierType::Pointer classifier = ClassifierType::New();
  classifier->SetSample(sample.GetPointer());
  classifier->SetMap(somreader->GetOutput());
  classifier->Update();
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  Once the classification has been performed, the sample list
//  obtained at the output of the classifier must be converted into an
//  image. We create the image as follows :
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  We can  now get a pointer to the classification result.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassifierType::OutputType* membershipSample = classifier->GetOutput();
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  And we can declare the iterators pointing to the front and the
//  back of the sample list.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassifierType::OutputType::ConstIterator m_iter =  membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last =  membershipSample->End();
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  We also declare an \subdoxygen{itk}{ImageRegionIterator} in order
//  to fill the output image with the class labels.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;

  OutputIteratorType outIt(outputImage, outputImage->GetLargestPossibleRegion());
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  We iterate through the sample list and the output image and assign
//  the label values to the image pixels.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  outIt.GoToBegin();

  while (m_iter != m_last && !outIt.IsAtEnd())
    {
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter;
    ++outIt;
    }
// Software Guide : EndCodeSnippet
//
//  Software Guide : BeginLatex
//
//  Finally, we write the classified image to a file.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(outputImage);
  writer->Update();
// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
// Figure \ref{fig:SOMMAPCLASS} shows the result of the SOM classification.
// \begin{figure}
// \center
// \includegraphics[width=0.35\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.2\textwidth]{ROI_QB_MUL_SOM.eps}
// \includegraphics[width=0.35\textwidth]{ROI_QB_MUL_SOMCLASS.eps}
// \itkcaption[SOM Image Classification]{Result of the SOM
// learning. Left: RGB image. Center: SOM. Right: Classified Image}
// \label{fig:SOMMAPCLASS}
// \end{figure}
//  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
