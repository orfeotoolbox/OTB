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
//    INPUTS: {GomaAvant.png}, {GomaApres.png}
//    OUTPUTS: {RatioChDet.tif}
//    3
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the class
// \doxygen{otb}{MeanRatioImageFilter} for detecting changes
// between pairs of images. This filter computes the mean intensity in
// the neighborhood of each pixel of the pair of images to be compared
// and uses the ratio of means as a change indicator. This change
// indicator is then normalized between 0 and 1 by using the classical
// \begin{equation}
// r = 1 - min\{\frac{\mu_A}{\mu_B},\frac{\mu_B}{\mu_A} \},
// \end{equation}
// where $\mu_A$ and $\mu_B$ are the local means. This
// example will use the images shown in
// figure ~\ref{fig:RATCHDETINIM}. These correspond to 2 Radarsat fine
// mode acquisitions before and after a lava flow resulting from a
// volcanic eruption.
// \begin{figure}
// \center
// \includegraphics[width=0.35\textwidth]{GomaAvant.eps}
// \includegraphics[width=0.35\textwidth]{GomaApres.eps}
// \itkcaption[Radarsat Images for Change Detection]{Images used for the
// change detection. Left: Before the eruption. Right: after the eruption.}
// \label{fig:RATCHDETINIM}
// \end{figure}
//
// We start by including the corresponding header file.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
#include "otbMeanRatioImageFilter.h"
//  Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkShiftScaleImageFilter.h"
#include "otbCommandProgressUpdate.h"

int main(int argc, char* argv[])
{

  if (argc < 5)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] <<
    " inputImageFile1 inputImageFile2  outputImageFile radius" << std::endl;
    return -1;
    }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  //  Software Guide : BeginLatex
  // We start by declaring the types for the two input images, the
  // change image and the image to be stored in a file for visualization.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef float                                    InternalPixelType;
  typedef unsigned char                            OutputPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType1;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType2;
  typedef otb::Image<InternalPixelType, Dimension> ChangeImageType;
  typedef otb::Image<OutputPixelType, Dimension>   OutputImageType;
  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We can now declare the types for the readers. Since the images
  //  can be vey large, we will force the pipeline to use
  //  streaming. For this purpose, the file writer will be
  //  streamed. This is achieved by using the
  //  \doxygen{otb}{ImageFileWriter} class.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType1>          ReaderType1;
  typedef otb::ImageFileReader<InputImageType2>          ReaderType2;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The change detector will give a normalized result between 0 and
  //  1. In order to store the result in PNG format we will
  //  rescale the results of the change detection in order to use all
  //  the output pixel type range of values.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef itk::ShiftScaleImageFilter<ChangeImageType,
      OutputImageType> RescalerType;

  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The \doxygen{otb}{MeanRatioImageFilter} is templated over
  //  the types of the two input images and the type of the generated change
  //  image.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef otb::MeanRatioImageFilter<
      InputImageType1,
      InputImageType2,
      ChangeImageType>       FilterType;

  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The different elements of the pipeline can now be instantiated.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  ReaderType1::Pointer  reader1 = ReaderType1::New();
  ReaderType2::Pointer  reader2 = ReaderType2::New();
  WriterType::Pointer   writer = WriterType::New();
  FilterType::Pointer   filter = FilterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();
  //  Software Guide : EndCodeSnippet

  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * outputFilename = argv[3];
  //  Software Guide : BeginLatex
  //
  //  We set the parameters of the different elements of the pipeline.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);
  float scale = itk::NumericTraits<OutputPixelType>::max();
  rescaler->SetScale(scale);
  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The only parameter for this change detector is the radius of
  //  the window used for computing the mean of the intensities.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  filter->SetRadius(atoi(argv[4]));
  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We build the pipeline by plugging all the elements together.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());

  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  //  Software Guide : EndCodeSnippet

  typedef otb::CommandProgressUpdate<FilterType> CommandType;

  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver(itk::ProgressEvent(), observer);

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
    }

//  Software Guide : BeginLatex
// Figure \ref{fig:RESRATCHDET} shows the result of the change
// detection by ratio of local means.
// \begin{figure}
// \center
// \includegraphics[width=0.35\textwidth]{RatioChDet.eps}
// \itkcaption[Ratio Change Detection Results]{Result of the
// ratio of means change detector}
// \label{fig:RESRATCHDET}
// \end{figure}
//  Software Guide : EndLatex

  return EXIT_SUCCESS;

}
