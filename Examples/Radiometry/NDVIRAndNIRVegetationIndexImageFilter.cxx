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
./NDVIRAndNIRVegetationIndexImageFilter Input/NDVI_2.hdr \
                                        Input/NDVI_3.hdr \
                                        Output/NDVIRAndNIRVegetationIndex.tif \
                                        Output/pretty_Red.png \
                                        Output/pretty_NIR.png \
                                        Output/pretty_NDVIRAndNIRVegetationIndex.png
*/


// \index{otb::RAndNIRIndexImageFilter}
// \index{otb::VegetationIndicesFunctor}
// \index{otb::VegetationIndicesFunctor!header}
//
// The following example illustrates the use of the
// \doxygen{otb}{RAndNIRIndexImageFilter} with the use of the Normalized
// Difference Vegatation Index (NDVI).
// NDVI computes the difference between the NIR channel, noted $L_{NIR}$, and the red channel,
// noted $L_{r}$ radiances reflected from the surface and transmitted through the atmosphere:
//
// \begin{equation}
// \mathbf{NDVI} = \frac{L_{NIR}-L_{r}}{L_{NIR}+L_{r}}
// \end{equation}
//
//  \relatedClasses
//  \begin{itemize}
//  \item \subdoxygen{otb}{Functor}{RVI}
//  \item \subdoxygen{otb}{Functor}{PVI}
//  \item \subdoxygen{otb}{Functor}{SAVI}
//  \item \subdoxygen{otb}{Functor}{TSAVI}
//  \item \subdoxygen{otb}{Functor}{MSAVI}
//  \item \subdoxygen{otb}{Functor}{GEMI}
//  \item \subdoxygen{otb}{Functor}{WDVI}
//  \item \subdoxygen{otb}{Functor}{IPVI}
//  \item \subdoxygen{otb}{Functor}{TNDVI}
//  \end{itemize}

// With the \doxygen{otb}{RAndNIRIndexImageFilter} class the filter
// inputs are one channel images: one inmage represents the NIR channel, the
// the other the NIR channel.
//
// Let's look at the minimal code required to use this algorithm. First, the following header
// defining the \doxygen{otb}{RAndNIRIndexImageFilter}
// class must be included.

#include "otbRAndNIRIndexImageFilter.h"

#include "itkMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc < 6)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage1 , inputImage2 , outputImage , prettyinputImage1 , prettyinputImage2 , prettyOutput" << std::endl;
    return 1;
  }

  // The image types are now defined using pixel types the
  // dimension. Input and output images are defined as \doxygen{otb}{Image}.

  const unsigned int                             Dimension = 2;
  typedef double                                 InputPixelType;
  typedef float                                  OutputPixelType;
  typedef otb::Image<InputPixelType, Dimension>  InputRImageType;
  typedef otb::Image<InputPixelType, Dimension>  InputNIRImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  // We instantiate reader and writer types
  typedef otb::ImageFileReader<InputRImageType>   RReaderType;
  typedef otb::ImageFileReader<InputNIRImageType> NIRReaderType;
  typedef otb::ImageFileWriter<OutputImageType>   WriterType;

  // The NDVI (Normalized Difference Vegetation Index) is instantiated using
  // the images pixel type as template parameters. It is
  // implemented as a functor class which will be passed as a
  // parameter to an \doxygen{otb}{RAndNIRIndexImageFilter}.

  typedef otb::Functor::NDVI<InputPixelType, InputPixelType, OutputPixelType> FunctorType;

  // The \doxygen{otb}{RAndNIRIndexImageFilter} type is instantiated using the images
  // types and the NDVI functor as template parameters.

  typedef otb::RAndNIRIndexImageFilter<InputRImageType, InputNIRImageType, OutputImageType, FunctorType> RAndNIRIndexImageFilterType;

  // Instantiating object
  RAndNIRIndexImageFilterType::Pointer filter    = RAndNIRIndexImageFilterType::New();
  RReaderType::Pointer                 readerR   = RReaderType::New();
  NIRReaderType::Pointer               readerNIR = NIRReaderType::New();
  WriterType::Pointer                  writer    = WriterType::New();

  //  Now the input images are set and a name is given to the output image.

  readerR->SetFileName(argv[1]);
  readerNIR->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);

  // We set the processing pipeline: filter inputs are linked to
  // the reader output and the filter output is linked to the writer
  // input.

  filter->SetInputR(readerR->GetOutput());
  filter->SetInputNIR(readerNIR->GetOutput());

  writer->SetInput(filter->GetOutput());

  // Invocation of the \code{Update()} method on the writer triggers the
  // execution of the pipeline.  It is recommended to place \code{update()} calls in a
  // \code{try/catch} block in case errors occur and exceptions are thrown.

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
  catch (...)
  {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
  }

  // Pretty image creation for the printing
  typedef otb::Image<unsigned char, Dimension>                                       OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>                                WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter<OutputImageType, OutputPrettyImageType>   RescalerType;
  typedef itk::RescaleIntensityImageFilter<InputRImageType, OutputPrettyImageType>   RescalerRType;
  typedef itk::RescaleIntensityImageFilter<InputNIRImageType, OutputPrettyImageType> RescalerNIRType;

  RescalerType::Pointer     rescaler     = RescalerType::New();
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();
  rescaler->SetInput(filter->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName(argv[6]);
  prettyWriter->SetInput(rescaler->GetOutput());

  RescalerRType::Pointer    rescalerR       = RescalerRType::New();
  RescalerNIRType::Pointer  rescalerNIR     = RescalerNIRType::New();
  WriterPrettyType::Pointer prettyWriterR   = WriterPrettyType::New();
  WriterPrettyType::Pointer prettyWriterNIR = WriterPrettyType::New();
  rescalerR->SetInput(readerR->GetOutput());
  rescalerR->SetOutputMinimum(0);
  rescalerR->SetOutputMaximum(255);
  prettyWriterR->SetFileName(argv[4]);
  prettyWriterR->SetInput(rescalerR->GetOutput());

  rescalerNIR->SetInput(readerNIR->GetOutput());
  rescalerNIR->SetOutputMinimum(0);
  rescalerNIR->SetOutputMaximum(255);
  prettyWriterNIR->SetFileName(argv[5]);
  prettyWriterNIR->SetInput(rescalerNIR->GetOutput());

  try
  {
    prettyWriter->Update();
    prettyWriterNIR->Update();
    prettyWriterR->Update();
  }
  catch (itk::ExceptionObject& excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
  catch (...)
  {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
  }

  // Let's now run this example using as input the images
  // \code{NDVI\_3.hdr} and  \code{NDVI\_4.hdr} (images kindly and free of charge given by SISA and CNES)
  // provided in the directory \code{Examples/Data}.
  //
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_Red.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_NIR.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_NDVIRAndNIRVegetationIndex.eps}
  // \itkcaption[ARVI Example]{NDVI input images on the left (Red channel and NIR channel), on the right the result of the algorithm.}
  // \label{fig:NDVIRAndNIRIndex}
  // \end{figure}

  return EXIT_SUCCESS;
}
