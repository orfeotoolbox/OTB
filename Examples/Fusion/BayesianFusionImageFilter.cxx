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
./BayesianFusionImageFilter Input/multiSpect.tif \
                            Input/multiSpectInterp.tif \
                            Input/panchro.tif \
                            Output/BayesianFusion_0.9999.tif \
                            Output/pretty_BayesianFusion_0.9999.png \
                            Output/pretty_multiSpect_0.9999.png \
                            Output/pretty_multiSpectInterp_0.9999.png \
                            Output/pretty_panchro_0.9999.png \
                            0.9999
*/


/* Example usage:
./BayesianFusionImageFilter Input/multiSpect.tif \
                            Input/multiSpectInterp.tif \
                            Input/panchro.tif \
                            Output/BayesianFusion_0.5.tif \
                            Output/pretty_BayesianFusion_0.5.png \
                            Output/pretty_multiSpect_0.5.png \
                            Output/pretty_multiSpectInterp_0.5.png \
                            Output/pretty_panchro_0.5.png \
                            0.5
*/


// \index{otb::BayesianFusionFilter}
// \index{otb::BayesianFusionFilter!header}
//
// The following example illustrates the use of the
// \doxygen{otb}{BayesianFusionFilter}. The Bayesian data fusion
// relies on the idea that variables of interest, denoted as vector $\mathbf{Z}$,
// cannot be directly observed. They are linked to the observable variables
// $\mathbf{Y}$ through the following error-like model.
//
// \begin{equation}
// \mathbf{Y} = g(\mathbf{Z}) + \mathbf{E}
// \end{equation}
//
// where g($\mathbf{Z}$) is a set of functionals and $\mathbf{E}$ is a
// vector of random errors that are stochastically independent from $\mathbf{Z}$.
// This algorithm uses elementary probability calculus, and several assumptions to compute
// the data fusion. For more explication see Fasbender, Radoux and Bogaert's
// publication \cite{JRadoux}.
// Three images are used :
// \begin{itemize}
// \item a panchromatic image,
// \item a multispectral image resampled at the panchromatic image spatial resolution,
// \item a multispectral image resampled at the panchromatic image spatial resolution,
// using, e.g. a cubic interpolator.
// \item a float : $\lambda$, the meaning of the weight to be given to the panchromatic
// image compared to the multispectral one.
// \end{itemize}
//
// Let's look at the minimal code required to use this algorithm. First, the following header
// defining the otb::BayesianFusionFilter class must be included.

#include "otbBayesianFusionFilter.h"

#include "otbImage.h"
#include "itkCastImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc < 10)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputMultiSpectralImage inputMultiSpectralInterpolatedImage "
              << "inputPanchromatiqueImage outputImage outputImagePrinted "
              << "msPrinted msiPrinted panchroPrinted lambda" << std::endl;
    return 1;
  }

  //  The image types are now defined using pixel types and particular
  //  dimension. The panchromatic image is defined as an \doxygen{otb}{Image}
  //  and the multispectral one as \doxygen{otb}{VectorImage}.

  using InternalPixelType      = double;
  const unsigned int Dimension = 2;
  using PanchroImageType       = otb::Image<InternalPixelType, Dimension>;
  using MultiSpecImageType     = otb::VectorImage<InternalPixelType, Dimension>;

  using OutputPixelType = double;
  using OutputImageType = otb::VectorImage<OutputPixelType, Dimension>;

  // We instantiate reader and writer types
  //
  using ReaderVectorType = otb::ImageFileReader<MultiSpecImageType>;
  using ReaderType       = otb::ImageFileReader<PanchroImageType>;
  using WriterType       = otb::ImageFileWriter<OutputImageType>;

  ReaderVectorType::Pointer multiSpectReader       = ReaderVectorType::New();
  ReaderVectorType::Pointer multiSpectInterpReader = ReaderVectorType::New();
  ReaderType::Pointer       panchroReader          = ReaderType::New();
  WriterType::Pointer       writer                 = WriterType::New();

  multiSpectReader->SetFileName(argv[1]);
  multiSpectInterpReader->SetFileName(argv[2]);
  panchroReader->SetFileName(argv[3]);
  writer->SetFileName(argv[4]);

  //  The Bayesian data fusion filter type is instantiated using the images types as
  //  a template parameters.

  using BayesianFusionFilterType = otb::BayesianFusionFilter<MultiSpecImageType, MultiSpecImageType, PanchroImageType, OutputImageType>;

  //  Next the filter is created by invoking the \code{New()} method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.

  BayesianFusionFilterType::Pointer bayesianFilter = BayesianFusionFilterType::New();

  //  Now the multi spectral image, the interpolated multi spectral image and
  //  the panchromatic image are given as inputs to the filter.

  bayesianFilter->SetMultiSpect(multiSpectReader->GetOutput());
  bayesianFilter->SetMultiSpectInterp(multiSpectInterpReader->GetOutput());
  bayesianFilter->SetPanchro(panchroReader->GetOutput());

  writer->SetInput(bayesianFilter->GetOutput());

  //  The BayesianFusionFilter requires defining one parameter : $\lambda$.
  //  The $\lambda$ parameter can be used to tune the fusion toward either a high color
  //  consistency or sharp details. Typical $\lambda$ value range in  $[0.5, 1[$,  where higher
  //  values yield sharper details. by default $\lambda$ is set at 0.9999.

  bayesianFilter->SetLambda(atof(argv[9]));

  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }

  // Create an 3 band images for the software guide
  using OutputPixelType2           = unsigned char;
  using OutputVectorImageType      = otb::VectorImage<OutputPixelType2, Dimension>;
  using VectorWriterType           = otb::ImageFileWriter<OutputVectorImageType>;
  using VectorRescalerType         = otb::VectorRescaleIntensityImageFilter<MultiSpecImageType, OutputVectorImageType>;
  using VectorRescalerBayesianType = otb::VectorRescaleIntensityImageFilter<OutputImageType, OutputVectorImageType>;
  using CasterType                 = otb::ImageToVectorImageCastFilter<PanchroImageType, MultiSpecImageType>;
  using ChannelExtractorType       = otb::MultiChannelExtractROI<OutputPixelType2, OutputPixelType2>;

  multiSpectReader->GenerateOutputInformation();
  multiSpectInterpReader->GenerateOutputInformation();

  CasterType::Pointer cast = CasterType::New();
  cast->SetInput(panchroReader->GetOutput());

  OutputVectorImageType::PixelType minimum, maximum;
  minimum.SetSize(multiSpectReader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(multiSpectReader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);

  VectorRescalerType::Pointer         vrms  = VectorRescalerType::New();
  VectorRescalerType::Pointer         vrmsi = VectorRescalerType::New();
  VectorRescalerBayesianType::Pointer vrb   = VectorRescalerBayesianType::New();

  vrms->SetInput(multiSpectReader->GetOutput());
  vrms->SetOutputMinimum(minimum);
  vrms->SetOutputMaximum(maximum);
  vrms->SetClampThreshold(0.01);

  vrmsi->SetInput(multiSpectInterpReader->GetOutput());
  vrmsi->SetOutputMinimum(minimum);
  vrmsi->SetOutputMaximum(maximum);
  vrmsi->SetClampThreshold(0.01);

  vrb->SetInput(bayesianFilter->GetOutput());
  vrb->SetOutputMinimum(minimum);
  vrb->SetOutputMaximum(maximum);
  vrb->SetClampThreshold(0.01);

  VectorRescalerType::Pointer rp = VectorRescalerType::New();
  rp->SetInput(cast->GetOutput());
  minimum.SetSize(1);
  maximum.SetSize(1);
  minimum.Fill(0);
  maximum.Fill(255);
  rp->SetOutputMinimum(minimum);
  rp->SetOutputMaximum(maximum);
  rp->SetClampThreshold(0.01);

  ChannelExtractorType::Pointer selecterms  = ChannelExtractorType::New();
  ChannelExtractorType::Pointer selectermsi = ChannelExtractorType::New();
  ChannelExtractorType::Pointer selecterf   = ChannelExtractorType::New();

  selecterms->SetInput(vrms->GetOutput());
  // selecterms->SetExtractionRegion(multiSpectReader->GetOutput()->GetLargestPossibleRegion());
  selecterms->SetChannel(2);
  selecterms->SetChannel(3);
  selecterms->SetChannel(4);

  selectermsi->SetInput(vrmsi->GetOutput());
  // selectermsi->SetExtractionRegion(multiSpectInterpReader->GetOutput()->GetLargestPossibleRegion());
  selectermsi->SetChannel(2);
  selectermsi->SetChannel(3);
  selectermsi->SetChannel(4);

  selecterf->SetInput(vrb->GetOutput());
  // selecterf->SetExtractionRegion(bayesianFilter->GetOutput()->GetLargestPossibleRegion());
  selecterf->SetChannel(2);
  selecterf->SetChannel(3);
  selecterf->SetChannel(4);

  VectorWriterType::Pointer vectWriterms  = VectorWriterType::New();
  VectorWriterType::Pointer vectWritermsi = VectorWriterType::New();
  VectorWriterType::Pointer vectWriterf   = VectorWriterType::New();
  VectorWriterType::Pointer vectWriterp   = VectorWriterType::New();

  vectWriterf->SetFileName(argv[5]);
  vectWriterf->SetInput(selecterf->GetOutput());
  vectWriterms->SetFileName(argv[6]);
  vectWriterms->SetInput(selecterms->GetOutput());
  vectWritermsi->SetFileName(argv[7]);
  vectWritermsi->SetInput(selectermsi->GetOutput());
  vectWriterp->SetFileName(argv[8]);
  vectWriterp->SetInput(rp->GetOutput());

  try
  {
    vectWriterms->Update();
    vectWritermsi->Update();
    vectWriterf->Update();
    vectWriterp->Update();
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

  //  Let's now run this example using as input the images
  //  \code{multiSpect.tif} , \code{multiSpectInterp.tif} and \code{panchro.tif}
  //  provided in the directory \code{Examples/Data}. The results
  //  obtained for 2 different values for $\lambda$ are shown in figure
  //  \ref{fig:BayesianImageFusionFilterInput}.
  //
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_multiSpect_0.5.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_multiSpectInterp_0.5.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_panchro_0.5.eps}
  // \itkcaption[Bayesian Data Fusion Example inputs]{Input
  // images used for this example (\copyright European Space Imaging).}
  // \label{fig:BayesianImageFusionFilterInput}
  // \end{figure}

  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_BayesianFusion_0.5.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_BayesianFusion_0.9999.eps}
  // \itkcaption[Bayesian Data Fusion results]{Fusion results
  // for the Bayesian Data Fusion filter for $\lambda = 0.5$ on the left and $\lambda = 0.9999$ on the right.}
  // \label{fig:BayesianImageFusionFilterOutput}
  // \end{figure}
  //

  return EXIT_SUCCESS;
}
