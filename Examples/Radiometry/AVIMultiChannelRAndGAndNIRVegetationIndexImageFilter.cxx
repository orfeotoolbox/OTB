/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
./AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter Input/verySmallFSATSW.tif \
                                                       Output/AVIMultiChannelRAndGAndNIRVegetationIndex.tif \
                                                       Output/pretty_FSATSW.png \
                                                       Output/pretty_AVIMultiChannelRAndGAndNIRVegetationIndex.png \
                                                       3 \
                                                       2 \
                                                       4
*/


// \index{otb::VegetationIndex}
// \index{otb::VegetationIndex!header}
//
//
// The following example illustrates the use of the
// itk::UnaryFunctorImageFilter with the
// use of the Angular Vegetation Index (AVI).
// The equation for the Angular Vegetation Index involves the gren, red
// and near infra-red bands. $\lambda_1$, $\lambda_2$ and $\lambda_3$ are the mid-band
// wavelengths for the green, red and NIR bands and $\tan^{-1}$ is the arctangent function.
//
// The AVI expression is
//
// \begin{equation}
// \mathbf{A_1} = \frac{\lambda_3-\lambda_2}{\lambda_2}
// \end{equation}
// \begin{equation}
// \mathbf{A_2} = \frac{\lambda_2-\lambda_1}{\lambda_2}
// \end{equation}
//
// \begin{equation}
// \mathbf{AVI} = \tan^{-1}\left({\frac{A_1}{NIR-R}}\right) + \tan^{-1}\left({\frac{A_2}{G-R}}\right)
// \end{equation}
//
// For more details, refer to Plummer work \cite{AVI}.
//
//
// Let's look at the minimal code required to use this
// algorithm.

#include "otbVegetationIndicesFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "itkThresholdImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc < 8)
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage , outputImage , prettyInput , prettyOutput , redChannel , greenChannel , nirChannel ," << std::endl;
    return 1;
  }

  // The image types are now defined using pixel types and
  // dimension. The input image is defined as an \doxygen{otb}{VectorImage},
  // the output is a \doxygen{otb}{Image}.

  const unsigned int Dimension = 2;
  using InputPixelType         = double;
  using OutputPixelType        = float;
  using InputImageType         = otb::VectorImage<InputPixelType, Dimension>;
  using OutputImageType        = otb::Image<OutputPixelType, Dimension>;

  // We instantiate reader and writer types
  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<OutputImageType>;

  // The AVI (Angular Vegetation Index) is
  // instantiated using the image pixel types as template parameters.

  using FunctorType = otb::Functor::AVI<InputPixelType, OutputPixelType>;

  // The
  // \doxygen{itk}{UnaryFunctorImageFilter}
  // type is defined using the image types and the AVI functor as
  // template parameters. We then instantiate the filter itself.

  using AVIImageFilterTypeType = itk::UnaryFunctorImageFilter<InputImageType, OutputImageType, FunctorType>;

  AVIImageFilterTypeType::Pointer filter = AVIImageFilterTypeType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //  Now the input image is set and a name is given to the output image.

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  // The three used index bands (red, green and NIR) are declared.

  filter->GetFunctor().SetBandIndex(CommonBandNames::RED, ::atoi(argv[5]));
  filter->GetFunctor().SetBandIndex(CommonBandNames::GREEN, ::atoi(argv[6]));
  filter->GetFunctor().SetBandIndex(CommonBandNames::NIR, ::atoi(argv[7]));

  // The filter input is linked to the reader output and
  // the filter output is linked to the writer input.

  filter->SetInput(reader->GetOutput());

  writer->SetInput(filter->GetOutput());

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
  catch (...)
  {
    std::cout << "Unknown exception !" << std::endl;
    return EXIT_FAILURE;
  }

  // Pretty image creation for the printing
  using OutputPrettyImageType       = otb::Image<unsigned char, Dimension>;
  using OutputVectorPrettyImageType = otb::VectorImage<unsigned char, Dimension>;
  using WriterVectorPrettyType      = otb::ImageFileWriter<OutputVectorPrettyImageType>;
  using WriterPrettyType            = otb::ImageFileWriter<OutputPrettyImageType>;
  using RescalerType                = itk::RescaleIntensityImageFilter<OutputImageType, OutputPrettyImageType>;
  using VectorRescalerType          = otb::VectorRescaleIntensityImageFilter<InputImageType, OutputVectorPrettyImageType>;
  using ChannelExtractorType        = otb::MultiChannelExtractROI<unsigned char, unsigned char>;

  VectorRescalerType::Pointer     vectRescaler     = VectorRescalerType::New();
  ChannelExtractorType::Pointer   selecter         = ChannelExtractorType::New();
  WriterVectorPrettyType::Pointer vectPrettyWriter = WriterVectorPrettyType::New();

  OutputVectorPrettyImageType::PixelType minimum, maximum;
  minimum.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);
  vectRescaler->SetOutputMinimum(minimum);
  vectRescaler->SetOutputMaximum(maximum);
  //  vectRescaler->SetClampThreshold(1);
  vectRescaler->SetInput(reader->GetOutput());

  selecter->SetInput(vectRescaler->GetOutput());
  selecter->SetChannel(3);
  selecter->SetChannel(2);
  selecter->SetChannel(1);

  vectPrettyWriter->SetFileName(argv[3]);
  vectPrettyWriter->SetInput(selecter->GetOutput());

  using ThresholderType = itk::ThresholdImageFilter<OutputImageType>;

  ThresholderType::Pointer thresholder = ThresholderType::New();
  thresholder->SetInput(filter->GetOutput());
  thresholder->SetOutsideValue(1.0);
  thresholder->ThresholdOutside(-1.0, 0.05);
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

  // Let's now run this example using as input the image
  // \code{verySmallFSATSW.tif} provided in the
  // directory \code{Examples/Data}.
  //
  //
  // \begin{figure} \center
  // \includegraphics[width=0.24\textwidth]{pretty_FSATSW.eps}
  // \includegraphics[width=0.24\textwidth]{pretty_AVIMultiChannelRAndGAndNIRVegetationIndex.eps}
  // \itkcaption[AVI Example]{AVI result on the right with the left image in input.}
  // \label{fig:AVIMultiChannelRAndGAndNIRIndexImageFilter}
  // \end{figure}
}
