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


/* Example usage:
./LAIFromNDVIImageTransform Input/verySmallFSATSW.tif \
                            Output/siTvLAIFromNDVIImageTransformExampleTest_verySmallFSATSW.tif \
                            Output/verySmallFSATSW_visu.png \
                            Output/siTvLAIFromNDVIImageTransformExampleTest_verySmallFSATSW_visu.png \
                            1 \
                            4
*/

//
// This example presents a way to generate LAI (Leaf Area Index) image using formula dedicated to Formosat2.
// LAI Image is used as an input in Image Simulation process.
//
// Let's look at the minimal code required to use this algorithm. First, the
// following headers must be included.

#include "itkUnaryFunctorImageFilter.h"

#include "otbVegetationIndicesFunctor.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 7)
  {
    std::cerr << "Wrong Parameters " << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int                            Dimension = 2;
  typedef otb::VectorImage<double, Dimension>   InputImageType;
  typedef otb::Image<double, Dimension>         OutputImageType;
  typedef otb::Image<unsigned char, Dimension>  ImageVisuType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ImageFileWriter<ImageVisuType>   VisuWriterType;
  typedef otb::ImageFileWriter<InputImageType>  InWriterType;
  // Filter type is a generic \doxygen{itk}{UnaryFunctorImageFilter} using Formosat2 specific LAI
  //  \doxygen{otb}{LAIFromNDVIFormosat2Functor}.

  typedef otb::Functor::LAIFromNDVIFormosat2Functor<InputImageType::InternalPixelType, InputImageType::InternalPixelType, OutputImageType::PixelType>
                                                                                                 FunctorType;
  typedef itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, FunctorType> LAIFRomNDVIImageFilterType;

  // Instantiating object

  //  Next the filter is created by invoking the \code{New()}~method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.

  LAIFRomNDVIImageFilterType::Pointer filter = LAIFRomNDVIImageFilterType::New();

  ReaderType::Pointer     reader     = ReaderType::New();
  WriterType::Pointer     writer     = WriterType::New();
  InWriterType::Pointer   inWriter   = InWriterType::New();
  VisuWriterType::Pointer visuWriter = VisuWriterType::New();

  char* InputName   = argv[1];
  char* OutputName1 = argv[2];
  char* OutputName2 = argv[3];
  char* OutputName3 = argv[4];
  reader->SetFileName(InputName);


  // filter input is set with input image
  //

  filter->SetInput(reader->GetOutput());

  // then red and nir channels index are set using \code{SetRedIndex()} and  \code{SetNIRIndex()}
  //
  unsigned int redChannel = static_cast<unsigned int>(atoi(argv[5]));
  unsigned int nirChannel = static_cast<unsigned int>(atoi(argv[6]));
  filter->GetFunctor().SetRedIndex(redChannel);
  filter->GetFunctor().SetNIRIndex(nirChannel);

  //  The invocation of the \code{Update()} method triggers the
  //  execution of the pipeline.

  filter->Update();


  writer->SetFileName(OutputName1);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  // rescale data

  inWriter->SetFileName(OutputName2);
  // typedef itk::RescaleIntensityImageFilter<InputImageType,
  //     OutputImageType> RescalerType;

  // RescalerType::Pointer     rescaler     = RescalerType::New();
  // rescaler->SetInput(reader->GetOutput());
  // rescaler->SetOutputMinimum(0);
  // rescaler->SetOutputMaximum(255);

  inWriter->SetInput(reader->GetOutput());
  inWriter->Update();


  visuWriter->SetFileName(OutputName3);
  typedef itk::RescaleIntensityImageFilter<OutputImageType, ImageVisuType> RescalerTypeOut;

  RescalerTypeOut::Pointer rescaler = RescalerTypeOut::New();
  rescaler->SetInput(filter->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  visuWriter->SetInput(rescaler->GetOutput());
  visuWriter->Update();

  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{verySmallFSATSW_visu.eps}
  // \includegraphics[width=0.44\textwidth]{siTvLAIFromNDVIImageTransformExampleTest_verySmallFSATSW_visu.eps}
  // \itkcaption[LAIFromNDVIImageTransform Filter]{LAI generation \emph{(right)} from NDVI applied on Formosat 2 Image \emph{(left)} .}
  // \label{fig:LAIFromNDVIImageTransform}
  // \end{figure}
  //
  //  Figure \ref{fig:LAIFromNDVIImageTransform} illustrates the LAI generation using Formosat 2 data.
  //


  return EXIT_SUCCESS;
}
