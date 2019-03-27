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

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
/* Example usage:
./HyperspectralUnmixingExample Input/AVIRIS/Indian_pines_corrected.tif \
                               Output/Indian_pines_Unmixing_Output.tif \
                               Output/UnmixingbandOutput1.png \
                               Output/UnmixingbandOutput2.png \
                               Output/UnmixingbandOutput3.png \
                               16
*/


// This example illustrates the use of the
// \doxygen{otb}{VcaImageFilter} and \doxygen{otb}{UnConstrainedLeastSquareImageFilter}.
// The VCA filter computes endmembers using the Vertex Component Analysis
// and UCLS performs unmixing on the input image using these endmembers.
//
// The first step required to use these filters is to include its header files.

#include "otbVcaImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 7)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "inputFileName outputFileName outputPrettyFilename1 outputPrettyFilename2 outputPrettyFilename3 EstimatenumberOfEndmembers" << std::endl;
    return EXIT_FAILURE;
  }

  const char*        infname                    = argv[1];
  const char*        outfname                   = argv[2];
  const unsigned int estimateNumberOfEndmembers = atoi(argv[6]);
  const unsigned int Dimension                  = 2;

  typedef double PixelType;
  // We start by defining the types for the images and the reader and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the principal
  // components) from a multi-channel input image.

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;

  typedef otb::VectorRescaleIntensityImageFilter<ImageType, ImageType> RescalerType;
  typedef otb::VectorImageToMatrixImageFilter<ImageType>               VectorImageToMatrixImageFilterType;

  // We instantiate now the image reader and we set the image file name.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  reader->UpdateOutputInformation();


  // For now we need to rescale the input image between 0 and 1 to
  // perform the unmixing algorithm. We use the
  // \doxygen{otb}{VectorRescaleIntensityImageFilter}.

  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());

  ImageType::PixelType minPixel, maxPixel;
  minPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  maxPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  minPixel.Fill(0.);
  maxPixel.Fill(1.);

  rescaler->SetOutputMinimum(minPixel);
  rescaler->SetOutputMaximum(maxPixel);

  // We define the type for the VCA filter. It is templated over the input
  // image type. The only parameter is the number of endmembers which
  // needs to be estimated.
  // We can now the instantiate the filter.

  typedef otb::VCAImageFilter<ImageType> VCAFilterType;
  VCAFilterType::Pointer                 vca = VCAFilterType::New();

  vca->SetNumberOfEndmembers(estimateNumberOfEndmembers);
  vca->SetInput(rescaler->GetOutput());


  // We transform the output estimate endmembers to a matrix representation

  VectorImageToMatrixImageFilterType::Pointer endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(vca->GetOutput());
  endMember2Matrix->Update();

  // We can now procedd to the unmixing algorithm.Parameters
  // needed are the input image and the endmembers matrix.

  typedef otb::UnConstrainedLeastSquareImageFilter<ImageType, ImageType, double> UCLSUnmixingFilterType;
  UCLSUnmixingFilterType::Pointer                                                unmixer = UCLSUnmixingFilterType::New();
  unmixer->SetInput(rescaler->GetOutput());
  unmixer->GetModifiableFunctor().SetMatrix(endMember2Matrix->GetMatrix());

  unmixer->SetNumberOfThreads(1); // FIXME : currently buggy

  // We now instantiate the writer and set the file name for the
  // output image and trigger the unmixing computation with the \code{Update()} of the writer.

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(unmixer->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  // Figure~\ref{fig:UNMIXING_FILTER} shows the result of applying unmixing
  // to an AVIRIS image (220 bands). This dataset is freely available
  // at \url{http://www.ehu.es/ccwintco/index.php/Hyperspectral_Remote_Sensing_Scenes#Indian_Pines}
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{UnmixingbandOutput1.eps}
  // \includegraphics[width=0.25\textwidth]{UnmixingbandOutput2.eps}
  // \includegraphics[width=0.25\textwidth]{UnmixingbandOutput3.eps}
  // \itkcaption[Unmixing Filter]{Result of applying the
  // \doxygen{otb}{VcaImageFilter} and \doxygen{otb}{UnConstrainedLeastSquareImageFilter} to an image. From left
  // to right and top to bottom:
  // first abundance map band, second abundance map band, third abundance map band.}
  // \label{fig:UNMIXING_FILTER}
  // \end{figure}

  typedef otb::Image<PixelType, Dimension>                                 MonoImageType;
  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType>          ExtractROIFilterType;
  typedef otb::Image<unsigned char, 2>                                     OutputImageType;
  typedef itk::RescaleIntensityImageFilter<MonoImageType, OutputImageType> PrettyRescalerType;
  typedef otb::ImageFileWriter<OutputImageType>                            WriterType2;

  for (unsigned int cpt = 0; cpt < 3; ++cpt)
  {
    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    PrettyRescalerType::Pointer   prettyRescaler   = PrettyRescalerType::New();
    WriterType2::Pointer          writer2          = WriterType2::New();

    extractROIFilter->SetInput(unmixer->GetOutput());
    extractROIFilter->SetChannel(cpt + 1);

    prettyRescaler->SetInput(extractROIFilter->GetOutput());
    prettyRescaler->SetOutputMinimum(0);
    prettyRescaler->SetOutputMaximum(255);

    writer2->SetInput(prettyRescaler->GetOutput());
    writer2->SetFileName(argv[cpt + 3]);
    writer2->Update();
  }

  return EXIT_SUCCESS;
}
