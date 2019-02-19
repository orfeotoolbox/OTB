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
./SOMExample Input/ROI_QB_MUL_1.png Output/ROI_QB_MUL_SOM.png Output/ROI_QB_MUL_SOMACT.png 4 4 4 4 20 1.0 0.1 128
*/


// This example illustrates the use of the
// \doxygen{otb}{SOM} class for building Kohonen's Self Organizing
// Maps.
//
// We will use the SOM in order to build a color table from an input
// image. Our input image is coded with $3\times 8$ bits and we would
// like to code it with only 16 levels. We will use the SOM in order
// to learn which are the 16 most representative RGB values of the
// input image and we will assume that this is the optimal color table
// for the image.
//
// The first thing to do is include the header file for the
// class. We will also need the header files for the map itself and
// the activation map builder whose utility will be explained at the
// end of the example.
//

#include "otbSOMMap.h"
#include "otbSOM.h"
#include "otbSOMActivationBuilder.h"

#include "itkMacro.h"

#include "itkVectorExpandImageFilter.h"
#include "itkVectorNearestNeighborInterpolateImageFunction.h"

#include "itkExpandImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbPrintableImageFilter.h"

// Since the \doxygen{otb}{SOM} class uses a distance, we will need to
// include the header file for the one we want to use
//

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkListSample.h"

int main(int itkNotUsed(argc), char* argv[])
{
  const char*  inputFileName  = argv[1];
  const char*  outputFileName = argv[2];
  const char*  actMapFileName = argv[3];
  unsigned int sizeX          = atoi(argv[4]);
  unsigned int sizeY          = atoi(argv[5]);
  unsigned int neighInitX     = atoi(argv[6]);
  unsigned int neighInitY     = atoi(argv[7]);
  unsigned int nbIterations   = atoi(argv[8]);
  double       betaInit       = atof(argv[9]);
  double       betaEnd        = atof(argv[10]);
  double       initValue      = atof(argv[11]);

  // The Self Organizing Map itself is actually an N-dimensional image
  // where each pixel contains a neuron. In our case, we decide to build
  // a 2-dimensional SOM, where the neurons store RGB values with
  // floating point precision.

  const unsigned int                             Dimension = 2;
  typedef double                                 PixelType;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef ImageType::PixelType                   VectorType;
  // The distance that we want to apply between the RGB values is the
  // Euclidean one. Of course we could choose to use other type of
  // distance, as for instance, a distance defined in any other color space.

  typedef itk::Statistics::EuclideanDistanceMetric<VectorType> DistanceType;
  //
  // We can now define the type for the map. The \doxygen{otb}{SOMMap}
  // class is templated over the neuron type -- \code{PixelType} here
  // --, the distance type and the number of dimensions. Note that the
  // number of dimensions of the map could be different from the one of
  // the images to be processed.

  typedef otb::SOMMap<VectorType, DistanceType, Dimension> MapType;
  //
  // We are going to perform the learning directly on the pixels of the
  // input image. Therefore, the image type is defined using the same
  // pixel type as we used for the map. We also define the type for the
  // imge file reader.

  typedef otb::ImageFileReader<ImageType> ReaderType;
  //
  // Since the \doxygen{otb}{SOM} class works on lists of samples, it
  // will need to access the input image through an adaptor. Its type is
  // defined as follows:

  typedef itk::Statistics::ListSample<VectorType> SampleListType;
  //
  // We can now define the type for the SOM, which is templated over the
  // input sample list and the type of the map to be produced and the two
  // functors that hold the training behavior.

  typedef otb::Functor::CzihoSOMLearningBehaviorFunctor                                                   LearningBehaviorFunctorType;
  typedef otb::Functor::CzihoSOMNeighborhoodBehaviorFunctor                                               NeighborhoodBehaviorFunctorType;
  typedef otb::SOM<SampleListType, MapType, LearningBehaviorFunctorType, NeighborhoodBehaviorFunctorType> SOMType;
  //
  // As an alternative to standard \code{SOMType}, one can decide to use
  // an \doxygen{otb}{PeriodicSOM}, which behaves like \doxygen{otb}{SOM} but
  // is to be considered to as a torus instead of a simple map. Hence, the
  // neighborhood behavior of the winning neuron does not depend on its location
  // on the map...
  // \doxygen{otb}{PeriodicSOM} is defined in otbPeriodicSOM.h.
  //
  // We can now start building the pipeline. The first step is to
  // instantiate the reader and pass its output to the adaptor.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  SampleListType::Pointer sampleList = SampleListType::New();
  sampleList->SetMeasurementVectorSize(reader->GetOutput()->GetVectorLength());

  itk::ImageRegionIterator<ImageType> imgIter(reader->GetOutput(), reader->GetOutput()->GetBufferedRegion());
  imgIter.GoToBegin();

  itk::ImageRegionIterator<ImageType> imgIterEnd(reader->GetOutput(), reader->GetOutput()->GetBufferedRegion());
  imgIterEnd.GoToEnd();

  do
  {
    sampleList->PushBack(imgIter.Get());
    ++imgIter;
  } while (imgIter != imgIterEnd);
  //
  // We can now instantiate the SOM algorithm and set the sample list as input.

  SOMType::Pointer som = SOMType::New();
  som->SetListSample(sampleList);
  //
  // We use a \code{SOMType::SizeType} array in order to set the sizes
  // of the map.

  SOMType::SizeType size;
  size[0] = sizeX;
  size[1] = sizeY;
  som->SetMapSize(size);
  //
  // The initial size of the neighborhood of each neuron is set in the
  // same way.

  SOMType::SizeType radius;
  radius[0] = neighInitX;
  radius[1] = neighInitY;
  som->SetNeighborhoodSizeInit(radius);
  //
  // The other parameters are the number of iterations, the initial and
  // the final values for the learning rate -- $\beta$ -- and the
  // maximum initial value for the neurons (the map will be randomly
  // initialized).

  som->SetNumberOfIterations(nbIterations);
  som->SetBetaInit(betaInit);
  som->SetBetaEnd(betaEnd);
  som->SetMaxWeight(static_cast<PixelType>(initValue));
  //
  //  Now comes the initialization of the functors.

  LearningBehaviorFunctorType learningFunctor;
  learningFunctor.SetIterationThreshold(radius, nbIterations);
  som->SetBetaFunctor(learningFunctor);

  NeighborhoodBehaviorFunctorType neighborFunctor;
  som->SetNeighborhoodSizeFunctor(neighborFunctor);
  som->Update();
  //
  // Finally, we set up the las part of the pipeline where the plug the
  // output of the SOM into the writer. The learning procedure is
  // triggered by calling the \code{Update()} method on the writer.
  // Since the map is itself an image, we can write it to disk with an
  // \doxygen{otb}{ImageFileWriter}.

  // Just for visualization purposes, we zoom the image, and pass it to the printable image filter
  typedef otb::Image<PixelType, 2>                                              SingleImageType;
  typedef itk::ExpandImageFilter<SingleImageType, SingleImageType>              ExpandType;
  typedef otb::PerBandVectorImageFilter<MapType, MapType, ExpandType>           VectorExpandType;
  typedef itk::NearestNeighborInterpolateImageFunction<SingleImageType, double> InterpolatorType;
  typedef otb::PrintableImageFilter<MapType>                                    PrintableFilterType;
  typedef otb::ImageFileWriter<PrintableFilterType::OutputImageType>            PrintableWriterType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  VectorExpandType::Pointer expand       = VectorExpandType::New();
  ExpandType::Pointer       scalarExpand = ExpandType::New();

  scalarExpand->SetExpandFactors(40);
  scalarExpand->SetInterpolator(interpolator);
  // scalarExpand->SetEdgePaddingValue(255);

  expand->SetFilter(scalarExpand);

  expand->SetInput(som->GetOutput());

  expand->UpdateOutputInformation();

  PrintableFilterType::Pointer printFilter = PrintableFilterType::New();
  printFilter->SetInput(expand->GetOutput());

  printFilter->SetChannel(1);
  printFilter->SetChannel(2);
  printFilter->SetChannel(3);

  PrintableWriterType::Pointer printWriter = PrintableWriterType::New();

  printWriter->SetInput(printFilter->GetOutput());
  printWriter->SetFileName(outputFileName);

  printWriter->Update();

  // Figure \ref{fig:SOMMAP} shows the result of the SOM learning. Since
  // we have performed a learning on RGB pixel values, the produced SOM
  // can be interpreted as an optimal color table for the input
  // image. It can be observed that the obtained colors are
  // topologically organised, so similar colors are also close in the
  // map. This topological organisation can be exploited to further
  // reduce the number of coding levels of the pixels without
  // performing a new learning: we can subsample the map to get a new
  // color table. Also, a bilinear interpolation between the neurons can
  // be used to increase the number of coding levels.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
  // \includegraphics[width=0.2\textwidth]{ROI_QB_MUL_SOM.eps}
  // \includegraphics[width=0.2\textwidth]{ROI_QB_MUL_SOMACT.eps}
  // \itkcaption[SOM Image Classification]{Result of the SOM
  // learning. Left: RGB image. Center: SOM. Right: Activation map}
  // \label{fig:SOMMAP}
  // \end{figure}

  // We can now compute the activation map for the input image. The
  // activation map tells us how many times a given neuron is activated
  // for the set of examples given to the map. The activation map is
  // stored as a scalar image and an integer pixel type is usually enough.

  typedef unsigned char OutputPixelType;

  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType>  ActivationWriterType;
  // In a similar way to the \doxygen{otb}{SOM} class the
  // \doxygen{otb}{SOMActivationBuilder} is templated over the sample
  // list given as input, the SOM map type and the activation map to be
  // built as output.

  typedef otb::SOMActivationBuilder<SampleListType, MapType, OutputImageType> SOMActivationBuilderType;
  // We instantiate the activation map builder and set as input the SOM
  // map build before and the image (using the adaptor).

  SOMActivationBuilderType::Pointer somAct = SOMActivationBuilderType::New();
  somAct->SetInput(som->GetOutput());
  somAct->SetListSample(sampleList);
  somAct->Update();
  // The final step is to write the activation map to a file.

  if (actMapFileName != nullptr)
  {
    ActivationWriterType::Pointer actWriter = ActivationWriterType::New();
    actWriter->SetFileName(actMapFileName);

    // The righthand side of figure \ref{fig:SOMMAP} shows the activation
    // map obtained.

    // Just for visualization purposes, we zoom the image.
    typedef itk::ExpandImageFilter<OutputImageType, OutputImageType>              ExpandType2;
    typedef itk::NearestNeighborInterpolateImageFunction<OutputImageType, double> InterpolatorType2;

    InterpolatorType2::Pointer interpolator2 = InterpolatorType2::New();
    ExpandType2::Pointer       expand2       = ExpandType2::New();
    expand2->SetInput(somAct->GetOutput());
    expand2->SetExpandFactors(20);
    expand2->SetInterpolator(interpolator2);
    // expand2->SetEdgePaddingValue(255);
    expand2->UpdateOutputInformation();

    actWriter->SetInput(expand2->GetOutput());
    actWriter->Update();
  }
  else
  {
    std::cerr << "The activation map file name is null" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
