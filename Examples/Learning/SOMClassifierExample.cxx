/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include <fstream>
#include "otbImage.h"
#include "otbSOMMap.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkListSample.h"

/* Example usage:
./SOMClassifierExample Input/ROI_QB_MUL_1.png Input/ROI_QB_MUL_SOM.png Output/ROI_QB_MUL_SOMCLASS.png
*/


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

#include "otbSOMClassifier.h"

int main(int argc, char* argv[])
{

  if (argc != 4)
  {
    std::cout << "Usage : " << argv[0] << " inputImage modelFile outputImage" << std::endl;
    return EXIT_FAILURE;
  }

  const char* imageFilename  = argv[1];
  const char* mapFilename    = argv[2];
  const char* outputFilename = argv[3];

  using InputPixelType         = double;
  using LabelPixelType         = unsigned char;
  const unsigned int Dimension = 2;

  using PixelType = itk::VariableLengthVector<InputPixelType>;

  // As for the SOM learning step, we must define the types for the
  // \code{otb::SOMMap}, and therefore, also for the distance to be
  // used. We will also define the type for the SOM reader, which is
  // actually an \doxygen{otb}{ImageFileReader} which the appropriate
  // image type.

  using DistanceType  = itk::Statistics::EuclideanDistanceMetric<PixelType>;
  using SOMMapType    = otb::SOMMap<PixelType, DistanceType, Dimension>;
  using SOMReaderType = otb::ImageFileReader<SOMMapType>;

  using InputImageType = otb::VectorImage<InputPixelType, Dimension>;
  using ReaderType     = otb::ImageFileReader<InputImageType>;

  //  The classification will be performed by the
  //  \doxygen{otb}{SOMClassifier}, which, as most of the
  //  classifiers, works on
  //  \subdoxygen{itk}{Statistics}{ListSample}s. In order to be able
  //  to perform an image classification, we will need to use the
  //  \subdoxygen{itk}{Statistics}{ImageToListAdaptor} which is
  //  templated over the type of image to be adapted. The
  //  \code{SOMClassifier} is templated over the sample type, the SOMMap
  //  type and the pixel type for the labels.

  using SampleType     = itk::Statistics::ListSample<PixelType>;
  using ClassifierType = otb::SOMClassifier<SampleType, SOMMapType, LabelPixelType>;
  //
  //  The result of the classification will be stored on an image and
  //  saved to a file. Therefore, we define the types needed for this step.

  using OutputImageType = otb::Image<LabelPixelType, Dimension>;
  using WriterType      = otb::ImageFileWriter<OutputImageType>;
  //
  //  We can now start reading the input image and the SOM given as
  //  inputs to the program. We instantiate the readers as usual.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->Update();

  SOMReaderType::Pointer somreader = SOMReaderType::New();
  somreader->SetFileName(mapFilename);
  somreader->Update();
  //
  //  The conversion of the input data from image to list sample is
  //  easily done using the adaptor.

  SampleType::Pointer sample = SampleType::New();

  itk::ImageRegionIterator<InputImageType> it(reader->GetOutput(), reader->GetOutput()->GetLargestPossibleRegion());

  sample->SetMeasurementVectorSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  it.GoToBegin();

  while (!it.IsAtEnd())
  {
    sample->PushBack(it.Get());
    ++it;
  }
  //
  //  The classifier can now be instantiated. The input data is set by
  //  using the \code{SetSample()} method and the SOM si set using the
  //  \code{SetMap()} method. The classification is triggered by using
  //  the \code{Update()} method.

  ClassifierType::Pointer classifier = ClassifierType::New();
  classifier->SetSample(sample.GetPointer());
  classifier->SetMap(somreader->GetOutput());
  classifier->Update();
  //
  //  Once the classification has been performed, the sample list
  //  obtained at the output of the classifier must be converted into an
  //  image. We create the image as follows :

  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  //
  //  We can  now get a pointer to the classification result.

  ClassifierType::OutputType* membershipSample = classifier->GetOutput();
  //
  //  And we can declare the iterators pointing to the front and the
  //  back of the sample list.

  ClassifierType::OutputType::ConstIterator m_iter = membershipSample->Begin();
  ClassifierType::OutputType::ConstIterator m_last = membershipSample->End();
  //
  //  We also declare an \doxygen{itk}{ImageRegionIterator} in order
  //  to fill the output image with the class labels.

  using OutputIteratorType = itk::ImageRegionIterator<OutputImageType>;

  OutputIteratorType outIt(outputImage, outputImage->GetLargestPossibleRegion());
  //
  //  We iterate through the sample list and the output image and assign
  //  the label values to the image pixels.

  outIt.GoToBegin();

  while (m_iter != m_last && !outIt.IsAtEnd())
  {
    outIt.Set(m_iter.GetClassLabel());
    ++m_iter;
    ++outIt;
  }
  //
  //  Finally, we write the classified image to a file.

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(outputImage);
  writer->Update();
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

  return EXIT_SUCCESS;
}
