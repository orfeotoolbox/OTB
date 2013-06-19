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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImage.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}
//    OUTPUTS: {ROI_QB_MUL_1_SVN_CLASS.png}
//    ${OTB_DATA_ROOT}/Examples/svm_image_model.svm
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{SVMClassifier} class for performing SVM
// classification on images.
// In this example, we will use an SVM model estimated in the example
// of section \ref{sec:LearningWithImages}
// to separate between water and non-water pixels by using the RGB
// values only. The images used for this example are shown in
// figure~\ref{fig:SVMROIS}.
// The first thing to do is include the header file for the
// class. Since the \doxygen{otb}{SVMClassifier} takes
// \doxygen{itk}{ListSample}s as input, the class
// \doxygen{itk}{PointSetToListAdaptor} is needed.
//
//
//  Software Guide : EndLatex

#include "itkImageToListAdaptor.h"

// Software Guide : BeginCodeSnippet
#include "itkListSample.h"
#include "otbSVMClassifier.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{

  if (argc != 4)
    {
    std::cout << "Usage : " << argv[0] << " inputImage outputImage modelFile "
              << std::endl;
    return EXIT_FAILURE;
    }

  const char * imageFilename  = argv[1];
  const char * modelFilename  = argv[3];
  const char * outputFilename = argv[2];

// Software Guide : BeginLatex
//
// In the framework of supervised learning and classification, we will
// always use feature vectors for the characterization of the
// classes. On the other hand, the class labels are scalar
// values. Here, we start by defining the type of the features as the
// \code{PixelType}, which will be used to define the feature
// \code{VectorType}. We also declare the type for the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef double                 PixelType;
  typedef std::vector<PixelType> VectorType;
  typedef int                    LabelPixelType;
// Software Guide : EndCodeSnippet
  const unsigned int Dimension = 2;

// Software Guide : BeginLatex
//
// We can now proceed to define the image type used for storing the
// features. We also define the reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::Image<itk::FixedArray<PixelType, 3>,
      Dimension>          InputImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now read the image by calling the \code{Update} method of the reader.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(imageFilename);

  reader->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The image has now to be transformed to a sample which
// is compatible with the classification framework. We will use a
// \doxygen{itk}{Statistics::ImageToListAdaptor} for this
// task. This class is templated over the image type used for
// storing the measures.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::Statistics::ImageToListAdaptor<InputImageType> SampleType;
  SampleType::Pointer sample = SampleType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After instantiation, we can set the image as an imput of our
// sample adaptor.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  sample->SetImage(reader->GetOutput());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Now, we need to declare the SVM model which is to be used by the
// classifier. The SVM model is templated over the type of value used
// for the measures and the type of pixel used for the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMModel<PixelType, LabelPixelType> ModelType;

  ModelType::Pointer model = ModelType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After instantiation, we can load a model saved to a file (see
// section \ref{sec:LearningWithImages} for an example of model
// estimation and storage to a file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  model->LoadModel(modelFilename);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We have now all the elements to create a classifier. The classifier
// is templated over the sample type (the type of the data to be
// classified) and the label type (the type of the output of the classifier).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMClassifier<SampleType, LabelPixelType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We set the classifier parameters : number of classes, SVM model,
// the sample data. And we trigger the classification process by
// calling the \code{Update} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  int numberOfClasses = model->GetNumberOfClasses();
  classifier->SetNumberOfClasses(numberOfClasses);
  classifier->SetModel(model);
  classifier->SetSample(sample.GetPointer());
  classifier->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After the classification step, we usually want to get the
// results. The classifier gives an output under the form of a sample
// list. This list supports the classical STL iterators. Therefore, we
// will create an output image and fill it up with the results of the
// classification. The pixel type of the output image is the same as
// the one used for the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef ClassifierType::ClassLabelType         OutputPixelType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  OutputImageType::Pointer outputImage = OutputImageType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We allocate the memory for the output image using the information
// from the input image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::Index<Dimension>       myIndexType;
  typedef itk::Size<Dimension>        mySizeType;
  typedef itk::ImageRegion<Dimension> myRegionType;

  mySizeType size;
  size[0] = reader->GetOutput()->GetRequestedRegion().GetSize()[0];
  size[1] = reader->GetOutput()->GetRequestedRegion().GetSize()[1];

  myIndexType start;
  start[0] = 0;
  start[1] = 0;

  myRegionType region;
  region.SetIndex(start);
  region.SetSize(size);

  outputImage->SetRegions(region);
  outputImage->Allocate();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now declare the interators on the list that we get at the
// output of the classifier as well as the iterator to fill the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassifierType::OutputType* membershipSample =
    classifier->GetOutput();
  ClassifierType::OutputType::ConstIterator m_iter =
    membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last =
    membershipSample->End();

  typedef itk::ImageRegionIterator<OutputImageType> OutputIteratorType;
  OutputIteratorType outIt(outputImage,
                           outputImage->GetBufferedRegion());

  outIt.GoToBegin();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will iterate through the list, get the labels and assign pixel
// values to the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  while (m_iter != m_last && !outIt.IsAtEnd())
    {
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter;
    ++outIt;
    }
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Only for visualization purposes, we choose to rescale the image of
// classes before saving it to a file. We will use the
// \doxygen{itk}{RescaleIntensityImageFilter} for this purpose.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned char, Dimension> FileImageType;

  typedef itk::RescaleIntensityImageFilter<OutputImageType,
      FileImageType> RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(itk::NumericTraits<unsigned char>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<unsigned char>::max());

  rescaler->SetInput(outputImage);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now create an image file writer and save the image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<FileImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outputFilename);
  writer->SetInput(rescaler->GetOutput());

  writer->Update();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
// Figure \ref{fig:SVMCLASS} shows the result of the SVM classification.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1_SVN_CLASS.eps}
// \itkcaption[SVM Image Classification]{Result of the SVM
// classification . Left: RGB image. Right: image of classes.}
// \label{fig:SVMCLASS}
// \end{figure}
//  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
