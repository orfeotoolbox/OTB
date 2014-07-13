/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginCommandLineArgs
//  INPUTS: {multiSpect.tif} , {multiSpectInterp.tif}, {panchro.tif}
//  OUTPUTS: {BayesianFusion_0.9999.tif} , {pretty_BayesianFusion_0.9999.png} , {pretty_multiSpect_0.9999.png} , {pretty_multiSpectInterp_0.9999.png} , {pretty_panchro_0.9999.png}
//  0.9999
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//  INPUTS: {multiSpect.tif} , {multiSpectInterp.tif}, {panchro.tif}
//  OUTPUTS: {BayesianFusion_0.5.tif} , {pretty_BayesianFusion_0.5.png} , {pretty_multiSpect_0.5.png} , {pretty_multiSpectInterp_0.5.png} , {pretty_panchro_0.5.png}
//  0.5
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
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
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbBayesianFusionFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkCastImageFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

int main(int argc, char *argv[])
{
  if (argc < 10)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputMultiSpectralImage inputMultiSpectralInterpolatedImage "
              << "inputPanchromatiqueImage outputImage outputImagePrinted "
              << "msPrinted msiPrinted panchroPrinted lambda"
              << std::endl;
    return 1;
    }

  //  Software Guide : BeginLatex
  //
  //  The image types are now defined using pixel types and particular
  //  dimension. The panchromatic image is defined as an \doxygen{otb}{Image}
  //  and the multispectral one as \doxygen{otb}{VectorImage}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef double InternalPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InternalPixelType, Dimension>       PanchroImageType;
  typedef otb::VectorImage<InternalPixelType, Dimension> MultiSpecImageType;
  // Software Guide : EndCodeSnippet

  typedef double                                       OutputPixelType;
  typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;

  // We instantiate reader and writer types
  //
  typedef  otb::ImageFileReader<MultiSpecImageType> ReaderVectorType;
  typedef  otb::ImageFileReader<PanchroImageType>   ReaderType;
  typedef  otb::ImageFileWriter<OutputImageType>    WriterType;

  ReaderVectorType::Pointer multiSpectReader       = ReaderVectorType::New();
  ReaderVectorType::Pointer multiSpectInterpReader = ReaderVectorType::New();
  ReaderType::Pointer       panchroReader                = ReaderType::New();
  WriterType::Pointer       writer                       = WriterType::New();

  multiSpectReader->SetFileName(argv[1]);
  multiSpectInterpReader->SetFileName(argv[2]);
  panchroReader->SetFileName(argv[3]);
  writer->SetFileName(argv[4]);

  //  Software Guide : BeginLatex
  //
  //  The Bayesian data fusion filter type is instantiated using the images types as
  //  a template parameters.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::BayesianFusionFilter<MultiSpecImageType,
      MultiSpecImageType,
      PanchroImageType,
      OutputImageType>
  BayesianFusionFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Next the filter is created by invoking the \code{New()} method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  BayesianFusionFilterType::Pointer bayesianFilter =
    BayesianFusionFilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Now the multi spectral image, the interpolated multi spectral image and
  //  the panchromatic image are given as inputs to the filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  bayesianFilter->SetMultiSpect(multiSpectReader->GetOutput());
  bayesianFilter->SetMultiSpectInterp(multiSpectInterpReader->GetOutput());
  bayesianFilter->SetPanchro(panchroReader->GetOutput());

  writer->SetInput(bayesianFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  The BayesianFusionFilter requires defining one parameter : $\lambda$.
  //  The $\lambda$ parameter can be used to tune the fusion toward either a high color
  //  consistency or sharp details. Typical $\lambda$ value range in  $[0.5, 1[$,  where higher
  //  values yield sharper details. by default $\lambda$ is set at 0.9999.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  bayesianFilter->SetLambda(atof(argv[9]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The invocation of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.  It is recommended to place update calls in a
  //  \code{try/catch} block in case errors occur and exceptions are thrown.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& excep)
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }
  // Software Guide : EndCodeSnippet

  // Create an 3 band images for the software guide
  typedef unsigned char                                 OutputPixelType2;
  typedef otb::VectorImage<OutputPixelType2, Dimension> OutputVectorImageType;
  typedef otb::ImageFileWriter<OutputVectorImageType>   VectorWriterType;
  typedef otb::VectorRescaleIntensityImageFilter<MultiSpecImageType,
      OutputVectorImageType>
  VectorRescalerType;
  typedef otb::VectorRescaleIntensityImageFilter<OutputImageType,
      OutputVectorImageType>
  VectorRescalerBayesianType;
  typedef otb::ImageToVectorImageCastFilter<PanchroImageType,
      MultiSpecImageType> CasterType;
  typedef otb::MultiChannelExtractROI<OutputPixelType2,
      OutputPixelType2>
  ChannelExtractorType;

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

  ChannelExtractorType::Pointer selecterms = ChannelExtractorType::New();
  ChannelExtractorType::Pointer selectermsi = ChannelExtractorType::New();
  ChannelExtractorType::Pointer selecterf = ChannelExtractorType::New();

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
  //selecterf->SetExtractionRegion(bayesianFilter->GetOutput()->GetLargestPossibleRegion());
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

  //  Software Guide : BeginLatex
  //
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
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
