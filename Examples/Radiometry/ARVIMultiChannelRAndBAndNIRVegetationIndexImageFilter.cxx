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


//  Software Guide : BeginCommandLineArgs
//  INPUTS: {VegetationIndex.hd}
//  OUTPUTS: {ARVIMultiChannelRAndBAndNIRVegetationIndex.tif} , {pretty_VegetationIndex.png} , {pretty_ARVIMultiChannelRAndBAndNIRVegetationIndex.png}
//  1 3 2 0.6
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// \index{otb::MultiChannelRAndBAndNIRIndexImageFilter}
// \index{otb::MultiChannelRAndBAndNIRIndexImageFilter!header}
// \index{otb::VegetationIndices}
// \index{otb::VegetationIndices!header}
//
//
// The following example illustrates the use of the
//  \doxygen{otb}{MultiChannelRAndBAndNIRIndexImageFilter} with the
// use of the Atmospherically Resistant Vegetation Index (ARVI) \subdoxygen{otb}{Functor}{ARVI}.  ARVI
// is an improved version of the NDVI that is more robust to the
// atmospheric effect.  In addition to the red and NIR channels (used
// in the NDVI), the ARVI takes advantage of the presence of the blue
// channel to accomplish a self-correction process for the atmospheric
// effect on the red channel. For this, it uses the difference in the
// radiance between the blue and the red channels to correct the
// radiance in the red channel.  Let's define $\rho_{NIR}^{*}$,
// $\rho_{r}^{*}$, $\rho_{b}^{*}$ the normalized radiances (that is to
// say the radiance normalized to reflectance units) of red, blue and
// NIR channels respectively.  $\rho_{rb}^{*}$ is defined as
//
// \begin{equation}
// \rho_{rb}^{*} = \rho_{r}^{*} - \gamma*(\rho_{b}^{*} - \rho_{r}^{*})
// \end{equation}
//
// The ARVI expression is
//
// \begin{equation}
// \mathbf{ARVI} = \frac{\rho_{NIR}^{*}-\rho_{rb}^{*}}{\rho_{NIR}^{*}+\rho_{rb}^{*}}
// \end{equation}
//
//
// This formula can be simplified with :
//
// \begin{equation}
// \mathbf{ARVI} = \frac{ L_{NIR}-L_{rb} }{ L_{NIR}+L_{rb} }
// \end{equation}
//
// For more details, refer to Kaufman and Tanre' work \cite{ARVI}.
//
//  \relatedClasses
//  \begin{itemize}
//  \item \subdoxygen{otb}{Functor}{TSARVI}
//  \item \subdoxygen{otb}{Functor}{EVI}
//  \end{itemize}

// With the \doxygen{otb}{MultiChannelRAndBAndNIRIndexImageFilter} class the
// input has to be a multi channel image and the user has to specify index channel
// of the red, blue and NIR channel.
//
// Let's look at the minimal code required to use this algorithm. First, the following header
// defining the \doxygen{otb}{MultiChannelRAndBAndNIRIndexImageFilter}
// class must be included.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMultiChannelRAndBAndNIRIndexImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "itkThresholdImageFilter.h"

int main(int argc, char *argv[])
{
  if (argc < 8)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr <<
    " inputImage , outputImage , prettyInput , prettyOutput , redChannel , blueChannel , nirChannel , gama"
              << std::endl;
    return 1;
    }

  //  Software Guide : BeginLatex
  //
  // The image types are now defined using pixel types and
  // dimension. The input image is defined as an \doxygen{otb}{VectorImage},
  // the output is a \doxygen{otb}{Image}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef double                                      InputPixelType;
  typedef float                                       OutputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>      OutputImageType;
  // Software Guide : EndCodeSnippet

  // We instantiate reader and writer types
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  //  Software Guide : BeginLatex
  //
  // The ARVI (Atmospherically Resistant Vegetation Index) is
  // instantiated using the image pixel types as template parameters.
  // Note that we also can use other functors which operate with the
  // Red, Blue and Nir channels such as EVI, ARVI and TSARVI.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef  otb::Functor::ARVI<InputPixelType,
      InputPixelType,
      InputPixelType,
      OutputPixelType>        FunctorType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The
  // \doxygen{otb}{MultiChannelRAndBAndNIRIndexImageFilter}
  // type is defined using the image types and the ARVI functor as
  // template parameters. We then instantiate the filter itself.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MultiChannelRAndBAndNIRIndexImageFilter
  <InputImageType,
      OutputImageType,
      FunctorType>
  MultiChannelRAndBAndNIRIndexImageFilterType;

  MultiChannelRAndBAndNIRIndexImageFilterType::Pointer
    filter = MultiChannelRAndBAndNIRIndexImageFilterType::New();
  // Software Guide : EndCodeSnippet

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //  Software Guide : BeginLatex
  //
  //  Now the input image is set and a name is given to the output image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The three used index bands (red, blue and NIR) are declared.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetRedIndex(::atoi(argv[5]));
  filter->SetBlueIndex(::atoi(argv[6]));
  filter->SetNIRIndex(::atoi(argv[7]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The $\gamma$ parameter is set. The
  // \doxygen{otb}{MultiChannelRAndBAndNIRIndexImageFilter}
  // class sets the default value of $\gamma$ to $0.5$.  This parameter
  // is used to reduce the atmospheric effect on a global scale.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->GetFunctor().SetGamma(::atof(argv[8]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // The filter input is linked to the reader output and
  // the filter output is linked to the writer input.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());
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
  catch (...)
    {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
    }

  // Pretty image creation for the printing
  typedef otb::Image<unsigned char,
      Dimension>
  OutputPrettyImageType;
  typedef otb::VectorImage<unsigned char,
      Dimension>
  OutputVectorPrettyImageType;
  typedef otb::ImageFileWriter<OutputVectorPrettyImageType>
  WriterVectorPrettyType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>
  WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter<OutputImageType,
      OutputPrettyImageType>
  RescalerType;
  typedef otb::VectorRescaleIntensityImageFilter<InputImageType,
      OutputVectorPrettyImageType>
  VectorRescalerType;
  typedef otb::MultiChannelExtractROI<unsigned char,
      unsigned char>
  ChannelExtractorType;

  VectorRescalerType::Pointer vectRescaler         =
    VectorRescalerType::New();
  ChannelExtractorType::Pointer selecter           =
    ChannelExtractorType::New();
  WriterVectorPrettyType::Pointer vectPrettyWriter =
    WriterVectorPrettyType::New();

  OutputVectorPrettyImageType::PixelType minimum, maximum;
  minimum.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);
  vectRescaler->SetOutputMinimum(minimum);
  vectRescaler->SetOutputMaximum(maximum);
  vectRescaler->SetClampThreshold(0.01);
  vectRescaler->SetInput(reader->GetOutput());

  selecter->SetInput(vectRescaler->GetOutput());
  selecter->SetChannel(1);
  selecter->SetChannel(2);
  selecter->SetChannel(3);

  vectPrettyWriter->SetFileName(argv[3]);
  vectPrettyWriter->SetInput(selecter->GetOutput());

  typedef itk::ThresholdImageFilter<OutputImageType> ThresholderType;

  ThresholderType::Pointer thresholder = ThresholderType::New();
  thresholder->SetInput(filter->GetOutput());
  thresholder->SetOutsideValue(1.0);
  thresholder->ThresholdOutside(0.0, 1.0);
  thresholder->Update();

  RescalerType::Pointer     rescaler     = RescalerType::New();
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();
  rescaler->SetInput(thresholder->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName(argv[4]);
  prettyWriter->SetInput(rescaler->GetOutput());

  try
    {
    prettyWriter->Update();
    vectPrettyWriter->Update();
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

  return EXIT_SUCCESS;

  // Software Guide : BeginLatex
  //
  // Let's now run this example using as input the image
  // \code{IndexVegetation.hd} (image kindly and free of charge given
  // by SISA and CNES) and $\gamma$=0.6 provided in the
  // directory \code{Examples/Data}.
  //
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_VegetationIndex.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_ARVIMultiChannelRAndBAndNIRVegetationIndex.eps}
  // \itkcaption[ARVI Example]{ARVI result on the right with the left image in input.}
  // \label{fig:ARVIMultiChannelRAndBAndNIRIndexImageFilter}
  // \end{figure}
  //
  //  Software Guide : EndLatex

}
