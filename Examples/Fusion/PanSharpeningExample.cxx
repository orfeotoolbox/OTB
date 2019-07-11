/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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
./example Input/QB_Toulouse_Ortho_PAN.tif \
          Input/QB_Toulouse_Ortho_XS.tif \
          Output/QB_Toulouse_Ortho_PXS.tif \
          Output/pretty_QB_Toulouse_Ortho_PXS.png \
          Output/pretty_QB_Toulouse_Ortho_PAN.png \
          Output/pretty_QB_Toulouse_Ortho_XS.png
*/

// Here we are illustrating the use of the
// \doxygen{otb}{SimpleRcsPanSharpeningFusionImageFilter} for PAN-sharpening.
// This example takes a PAN and the corresponding XS images as input. These
// images are supposed to be registered.
//
// The fusion operation is defined as
//
// \begin{equation}
// \frac{XS}{\mathrm{Filtered}(PAN)} PAN
// \end{equation}

// Figure~\ref{fig:PANSHARP_FILTER} shows the result of applying
// this PAN sharpening filter to a Quickbird image.
// \begin{figure}
// \center
// \includegraphics[width=0.44\textwidth]{pretty_QB_Toulouse_Ortho_PAN.eps}
// \includegraphics[width=0.44\textwidth]{pretty_QB_Toulouse_Ortho_XS.eps}
// \includegraphics[width=0.44\textwidth]{pretty_QB_Toulouse_Ortho_PXS.eps}
// \itkcaption[Pan sharpening]{Result of applying
// the \doxygen{otb}{SimpleRcsPanSharpeningFusionImageFilter} to
// orthorectified Quickbird
// image. From left to right : original PAN image, original XS image and the
// result of the PAN sharpening}
// \label{fig:PANSHARP_FILTER}
// \end{figure}

// We start by including the required header and declaring the main function:

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

#include "otbPrintableImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
int main(int argc, char* argv[])
{

  if (argc < 7)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputPanchromatiqueImage inputMultiSpectralImage outputImage outputImagePrinted panchroPrinted msPrinted" << std::endl;
    return 1;
  }

  // We declare the different image type used here as well as the image reader.
  // Note that, the reader for the PAN image is templated by an
  // \doxygen{otb}{Image} while the XS reader uses an
  // \doxygen{otb}{VectorImage}.

  using ImageType          = otb::Image<double, 2>;
  using VectorImageType    = otb::VectorImage<double, 2>;
  using ReaderType         = otb::ImageFileReader<ImageType>;
  using ReaderVectorType   = otb::ImageFileReader<VectorImageType>;
  using VectorIntImageType = otb::VectorImage<unsigned short int, 2>;

  ReaderVectorType::Pointer readerXS  = ReaderVectorType::New();
  ReaderType::Pointer       readerPAN = ReaderType::New();

  // We pass the filenames to the readers

  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);

  // We declare the fusion filter an set its inputs using the readers:

  using FusionFilterType           = otb::SimpleRcsPanSharpeningFusionImageFilter<ImageType, VectorImageType, VectorIntImageType>;
  FusionFilterType::Pointer fusion = FusionFilterType::New();
  fusion->SetPanInput(readerPAN->GetOutput());
  fusion->SetXsInput(readerXS->GetOutput());

  // And finally, we declare the writer and call its \code{Update()} method to
  // trigger the full pipeline execution.

  using WriterType           = otb::ImageFileWriter<VectorIntImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(fusion->GetOutput());
  writer->Update();

  using PrintableImageType              = otb::PrintableImageFilter<VectorIntImageType>;
  PrintableImageType::Pointer printable = PrintableImageType::New();

  using VectorCharImageType        = otb::VectorImage<unsigned char, 2>;
  using PNGWriterType              = otb::ImageFileWriter<VectorCharImageType>;
  PNGWriterType::Pointer pngwriter = PNGWriterType::New();

  printable->SetInput(fusion->GetOutput());
  printable->SetChannel(3);
  printable->SetChannel(2);
  printable->SetChannel(1);
  pngwriter->SetFileName(argv[4]);
  pngwriter->SetInput(printable->GetOutput());
  pngwriter->Update();

  using PrintableImageType2               = otb::PrintableImageFilter<VectorImageType>;
  PrintableImageType2::Pointer printable2 = PrintableImageType2::New();
  printable2->SetInput(readerXS->GetOutput());
  printable2->SetChannel(3);
  printable2->SetChannel(2);
  printable2->SetChannel(1);
  pngwriter->SetFileName(argv[6]);
  pngwriter->SetInput(printable2->GetOutput());
  pngwriter->Update();

  using VectorCastFilterType = otb::ImageToVectorImageCastFilter<ImageType, VectorImageType>;

  VectorCastFilterType::Pointer vectorCastFilter = VectorCastFilterType::New();
  PNGWriterType::Pointer        pngwriterPan     = PNGWriterType::New();

  vectorCastFilter->SetInput(readerPAN->GetOutput());
  PrintableImageType2::Pointer printable3 = PrintableImageType2::New();
  printable3->SetInput(vectorCastFilter->GetOutput());
  printable3->SetChannel(1);
  printable3->SetChannel(1);
  printable3->SetChannel(1);

  pngwriterPan->SetFileName(argv[5]);

  pngwriterPan->SetInput(printable3->GetOutput());

  pngwriterPan->Update();

  return EXIT_SUCCESS;
}
