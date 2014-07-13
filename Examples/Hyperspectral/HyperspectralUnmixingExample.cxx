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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbVectorImageToMatrixImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {Indian_pines_corrected.tif}
//    OUTPUTS: {Indian_pines_Unmixing_Output.tif}, {UnmixingbandOutput1.png}, {UnmixingbandOutput2.png}, {UnmixingbandOutput3.png}
//    16
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{VcaImageFilter} and \doxygen{otb}{UnConstrainedLeastSquareImageFilter}.
// The VCA filter computes endmembers using the Vertex Component Analysis
// and UCLS performs unmixing on the input image using these endmembers.
//
// The first step required to use these filters is to include its header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVcaImageFilter.h"
#include "otbUnConstrainedLeastSquareImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{
  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "inputFileName outputFileName outputPrettyFilename1 outputPrettyFilename2 outputPrettyFilename3 EstimatenumberOfEndmembers" << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const unsigned int estimateNumberOfEndmembers = atoi(argv[6]);
  const unsigned int Dimension = 2;

  typedef double                                                      PixelType;
  // Software Guide : BeginLatex
  //
  // We start by defining the types for the images and the reader and
  // the writer. We choose to work with a \doxygen{otb}{VectorImage},
  // since we will produce a multi-channel image (the principal
  // components) from a multi-channel input image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType, Dimension>  ImageType;
  typedef otb::ImageFileReader<ImageType>         ReaderType;
  typedef otb::ImageFileWriter<ImageType>         WriterType;
  // Software Guide : EndCodeSnippet

  typedef otb::VectorRescaleIntensityImageFilter<ImageType, ImageType> RescalerType;
  typedef otb::VectorImageToMatrixImageFilter<ImageType>              VectorImageToMatrixImageFilterType;

  // Software Guide : BeginLatex
  //
  // We instantiate now the image reader and we set the image file name.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  // Software Guide : EndCodeSnippet

  reader->UpdateOutputInformation();


  // Software Guide : BeginLatex
  //
  // For now we need to rescale the input image between 0 and 1 to
  // perform the unmixing algorithm. We use the
  // \doxygen{otb}{VectorRescaleIntensityImageFilter}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  RescalerType::Pointer         rescaler = RescalerType::New();
  rescaler->SetInput(reader->GetOutput());

  ImageType::PixelType minPixel, maxPixel;
  minPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  maxPixel.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  minPixel.Fill(0.);
  maxPixel.Fill(1.);

  rescaler->SetOutputMinimum(minPixel);
  rescaler->SetOutputMaximum(maxPixel);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We define the type for the VCA filter. It is templated over the input
  // image type. The only parameter is the number of endmembers which
  // needs to be estimated.
  // We can now the instantiate the filter.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::VCAImageFilter<ImageType>                       VCAFilterType;
  VCAFilterType::Pointer vca = VCAFilterType::New();

  vca->SetNumberOfEndmembers(estimateNumberOfEndmembers);
  vca->SetInput(rescaler->GetOutput());
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // We transform the output estimate endmembers to a matrix representation
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  VectorImageToMatrixImageFilterType::Pointer
    endMember2Matrix = VectorImageToMatrixImageFilterType::New();
  endMember2Matrix->SetInput(vca->GetOutput());
  endMember2Matrix->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now procedd to the unmixing algorithm.Parameters
  // needed are the input image and the endmembers matrix.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::UnConstrainedLeastSquareImageFilter<ImageType, ImageType, double> UCLSUnmixingFilterType;
  UCLSUnmixingFilterType::Pointer
    unmixer = UCLSUnmixingFilterType::New();
  unmixer->SetInput(rescaler->GetOutput());
  unmixer->SetMatrix(endMember2Matrix->GetMatrix());
  // Software Guide : EndCodeSnippet

  unmixer->SetNumberOfThreads(1); // FIXME : currently buggy

  // Software Guide : BeginLatex
  //
  // We now instantiate the writer and set the file name for the
  // output image and trigger the unmixing computation with the \code{Update()} of the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  WriterType::Pointer  writer = WriterType::New();
  writer->SetInput(unmixer->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
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
  //
  //  Software Guide : EndLatex

  typedef otb::Image<PixelType, Dimension>                        MonoImageType;
  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType> ExtractROIFilterType;
  typedef otb::Image<unsigned char, 2>                            OutputImageType;
  typedef itk::RescaleIntensityImageFilter<MonoImageType,
                                           OutputImageType>       PrettyRescalerType;
  typedef otb::ImageFileWriter<OutputImageType>                   WriterType2;

  for (unsigned int cpt = 0; cpt < 3; ++cpt)
    {
    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    PrettyRescalerType::Pointer   prettyRescaler = PrettyRescalerType::New();
    WriterType2::Pointer          writer2 = WriterType2::New();

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
