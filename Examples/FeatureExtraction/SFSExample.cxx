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

#include "itkMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

/* Example usage:
./SFSExample Input/suburb2.jpeg \
             Output/SFSLengthOutput.tif \
             Output/SFSWidthOutput.tif \
             Output/SFSMeanOutput.tif \
             Output/SFSRatioOutput.tif \
             Output/SFSSDOutput.tif \
             Output/SFSPsiOutput.tif \
             Output/SFSLengthPrettyOutput.tif \
             Output/SFSWidthPrettyOutput.tif \
             Output/SFSMeanPrettyOutput.tif \
             Output/SFSRatioPrettyOutput.tif \
             Output/SFSSDPrettyOutput.tif \
             Output/SFSPsiPrettyOutput.tif \
             20 \
             50 \
             8 \
             4 \
             0.6
*/


// This example illustrates the use of the
// \doxygen{otb}{SFSTexturesImageFilter}.
// This filter computes the Structural Feature Set as described in
// \cite{SFS}. These features are textural parameters which give
// information about the structure of lines passing through each pixel
// of the image.
//
// The first step required to use this filter is to include its header file.

#include "otbSFSTexturesImageFilter.h"

int main(int itkNotUsed(argc), char* argv[])
{
  using PixelType              = double;
  const unsigned int Dimension = 2;

  std::string  inName            = argv[1];
  std::string  outNameLength     = argv[2];
  std::string  outNameWidth      = argv[3];
  std::string  outNameWMean      = argv[4];
  std::string  outNameRatio      = argv[5];
  std::string  outNameSD         = argv[6];
  std::string  outNamePsi        = argv[7];
  std::string  lengthprettyfname = argv[8];
  std::string  widthprettyfname  = argv[9];
  std::string  wmeanprettyfname  = argv[10];
  std::string  ratioprettyfname  = argv[11];
  std::string  sdprettyfname     = argv[12];
  std::string  psiprettyfname    = argv[13];
  PixelType    spectThresh       = atof(argv[14]);
  unsigned int spatialThresh     = atoi(argv[15]);
  unsigned int dirNb             = atoi(argv[16]);
  unsigned int maxConsideration  = atoi(argv[17]);
  double       alpha             = atof(argv[18]);

  // As with every OTB program, we start by defining the types for the
  // images, the readers and the writers.

  using ImageType  = otb::Image<PixelType, Dimension>;
  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;
  // The we can instantiate the type for the SFS filter, which is
  // templated over the input and output pixel types.

  using SFSFilterType = otb::SFSTexturesImageFilter<ImageType, ImageType>;
  // After that, we can instantiate the filter. We will also instantiate
  // the reader and one writer for each output image, since the SFS
  // filter generates 6 different features.

  SFSFilterType::Pointer filter       = SFSFilterType::New();
  ReaderType::Pointer    reader       = ReaderType::New();
  WriterType::Pointer    writerLength = WriterType::New();
  WriterType::Pointer    writerWidth  = WriterType::New();
  WriterType::Pointer    writerWMean  = WriterType::New();
  WriterType::Pointer    writerRatio  = WriterType::New();
  WriterType::Pointer    writerSD     = WriterType::New();
  WriterType::Pointer    writerPsi    = WriterType::New();

  reader->SetFileName(inName);

  // The SFS filter has several parameters which have to be
  // selected. They are:
  // \begin{enumerate}
  // \item a spectral threshold to decide if 2 neighboring pixels are
  // connected;
  //\item a spatial threshold defining the maximum length for an
  // extracted line;
  //\item the number of directions which will be analyzed (the first
  // one is to the right and they are equally distributed between 0 and
  // $2\pi$);
  // \item the $\alpha$ parameter fort the $\omega-mean$ feature;
  // \item the RatioMax parameter fort the $\omega-mean$ feature.
  // \end{enumerate}

  filter->SetSpectralThreshold(spectThresh);
  filter->SetSpatialThreshold(spatialThresh);
  filter->SetNumberOfDirections(dirNb);
  filter->SetRatioMaxConsiderationNumber(maxConsideration);
  filter->SetAlpha(alpha);
  // In order to disable the computation of a feature, the
  // \code{SetFeatureStatus} parameter can be used. The $true$ value
  // enables the feature (default behavior) and the $false$ value
  // disables the computation. Therefore, the following line is useless,
  // but is given here as an example.

  filter->SetFeatureStatus(SFSFilterType::PSI, true);
  // Now, we plug the pipeline using all the writers.

  filter->SetInput(reader->GetOutput());

  writerLength->SetFileName(outNameLength);
  writerLength->SetInput(filter->GetLengthOutput());
  writerLength->Update();

  writerWidth->SetFileName(outNameWidth);
  writerWidth->SetInput(filter->GetWidthOutput());
  writerWidth->Update();

  writerWMean->SetFileName(outNameWMean);
  writerWMean->SetInput(filter->GetWMeanOutput());
  writerWMean->Update();

  writerRatio->SetFileName(outNameRatio);
  writerRatio->SetInput(filter->GetRatioOutput());
  writerRatio->Update();

  writerSD->SetFileName(outNameSD);
  writerSD->SetInput(filter->GetSDOutput());
  writerSD->Update();

  writerPsi->SetFileName(outNamePsi);
  writerPsi->SetInput(filter->GetPSIOutput());
  writerPsi->Update();
  // Figure~\ref{fig:SFS_FILTER} shows the result of applying
  // the SFS computation to an image
  // \begin{figure}
  // \center
  // \includegraphics[width=0.25\textwidth]{suburb2.eps}
  // \includegraphics[width=0.25\textwidth]{SFSLengthPrettyOutput.eps}
  // \includegraphics[width=0.25\textwidth]{SFSWidthPrettyOutput.eps}
  // \includegraphics[width=0.25\textwidth]{SFSMeanPrettyOutput.eps}
  // \includegraphics[width=0.25\textwidth]{SFSRatioPrettyOutput.eps}
  // \includegraphics[width=0.25\textwidth]{SFSSDPrettyOutput.eps}
  // \includegraphics[width=0.25\textwidth]{SFSPsiPrettyOutput.eps}
  // \itkcaption[Right Angle Detection Filter]{Result of applying the
  // \doxygen{otb}{SFSTexturesImageFilter} to an image. From left to
  // right and top to bottom: original image, length, width,
  // $\omega$-mean, ratio, SD and Psi structural features.
  // original image, .}
  // \label{fig:SFS_FILTER}
  // \end{figure}

  /************** pretty images for printing *********/
  using OutputImageType  = otb::Image<unsigned char, 2>;
  using RescalerType     = itk::RescaleIntensityImageFilter<ImageType, OutputImageType>;
  using OutputWriterType = otb::ImageFileWriter<OutputImageType>;

  RescalerType::Pointer rescaler = RescalerType::New();
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  OutputWriterType::Pointer outWriter = OutputWriterType::New();
  outWriter->SetInput(rescaler->GetOutput());

  rescaler->SetInput(filter->GetLengthOutput());
  outWriter->SetFileName(lengthprettyfname);
  outWriter->Update();

  rescaler->SetInput(filter->GetWidthOutput());
  outWriter->SetFileName(widthprettyfname);
  outWriter->Update();

  rescaler->SetInput(filter->GetWMeanOutput());
  outWriter->SetFileName(wmeanprettyfname);
  outWriter->Update();

  rescaler->SetInput(filter->GetRatioOutput());
  outWriter->SetFileName(ratioprettyfname);
  outWriter->Update();

  rescaler->SetInput(filter->GetSDOutput());
  outWriter->SetFileName(sdprettyfname);
  outWriter->Update();

  rescaler->SetInput(filter->GetPSIOutput());
  outWriter->SetFileName(psiprettyfname);
  outWriter->Update();

  return EXIT_SUCCESS;
}
