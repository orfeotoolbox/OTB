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
#include "itkMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {suburb2.jpeg}
//    OUTPUTS: {SFSLengthOutput.tif}, {SFSWidthOutput.tif}, {SFSMeanOutput.tif}, {SFSRatioOutput.tif}, {SFSSDOutput.tif}, {SFSPsiOutput.tif}, {SFSLengthPrettyOutput.tif}, {SFSWidthPrettyOutput.tif}, {SFSMeanPrettyOutput.tif}, {SFSRatioPrettyOutput.tif}, {SFSSDPrettyOutput.tif}, {SFSPsiPrettyOutput.tif}
//    20 50 8 4 0.6
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{SFSTexturesImageFilter}.
// This filter computes the Structural Feature Set as descibed in
// \cite{SFS}. These features are textural parameters which give
// information about the structure of lines passing through each pixel
// of the image.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSFSTexturesImageFilter.h"
// Software Guide : EndCodeSnippet

int main(int itkNotUsed(argc), char * argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;

  std::string  inName            = argv[1];
  std::string  outNameLength     = argv[2];
  std::string  outNameWidth      = argv[3];
  std::string  outNameWMean      = argv[4];
  std::string  outNameRatio      = argv[5];
  std::string  outNameSD         = argv[6];
  std::string  outNamePsi         = argv[7];
  std::string  lengthprettyfname     = argv[8];
  std::string  widthprettyfname      = argv[9];
  std::string  wmeanprettyfname      = argv[10];
  std::string  ratioprettyfname      = argv[11];
  std::string  sdprettyfname         = argv[12];
  std::string  psiprettyfname         = argv[13];
  PixelType    spectThresh         = atof(argv[14]);
  unsigned int spatialThresh    = atoi(argv[15]);
  unsigned int dirNb            = atoi(argv[16]);
  unsigned int maxConsideration = atoi(argv[17]);
  double       alpha                  = atof(argv[18]);

// Software Guide : BeginLatex
//
// As with every OTB program, we start by defining the types for the
// images, the readers and the writers.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ImageFileReader<ImageType>  ReaderType;
  typedef otb::ImageFileWriter<ImageType>  WriterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The we can instantiate the type for the SFS filter, which is
// templated over the input and output pixel types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SFSTexturesImageFilter<ImageType, ImageType> SFSFilterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// After that, we can instantiate the filter. We will also instantiate
// the reader and one writer for each output image, since the SFS
// filter generates 6 different features.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  SFSFilterType::Pointer filter    = SFSFilterType::New();
  ReaderType::Pointer    reader       = ReaderType::New();
  WriterType::Pointer    writerLength = WriterType::New();
  WriterType::Pointer    writerWidth  = WriterType::New();
  WriterType::Pointer    writerWMean  = WriterType::New();
  WriterType::Pointer    writerRatio  = WriterType::New();
  WriterType::Pointer    writerSD     = WriterType::New();
  WriterType::Pointer    writerPsi    = WriterType::New();
// Software Guide : EndCodeSnippet

  reader->SetFileName(inName);

// Software Guide : BeginLatex
//
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
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter->SetSpectralThreshold(spectThresh);
  filter->SetSpatialThreshold(spatialThresh);
  filter->SetNumberOfDirections(dirNb);
  filter->SetRatioMaxConsiderationNumber(maxConsideration);
  filter->SetAlpha(alpha);
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// In order to disable the computation of a feature, the
// \code{SetFeatureStatus} parameter can be used. The $true$ value
// enables the feature (default behavior) and the $false$ value
// disables the computation. Therefore, the following line is useless,
// but is given here as an example.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter->SetFeatureStatus(SFSFilterType::PSI, true);
  // Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Now, we plug the pipeline using all the writers.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
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
  //
  //  Software Guide : EndLatex

  /************** pretty images for printing *********/
  typedef otb::Image<unsigned char,
      2>                                     OutputImageType;
  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputImageType> RescalerType;
  typedef otb::ImageFileWriter<OutputImageType>
  OutputWriterType;

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
