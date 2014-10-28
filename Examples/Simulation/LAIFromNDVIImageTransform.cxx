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
//
// INPUTS:  {verySmallFSATSW.tif}
// OUTPUTS: {siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW.tif}, {verySmallFSATSW_visu.png}, {siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW_visu.png}
// 1 4
//  Software Guide : EndCommandLineArgs
//
// Software Guide : BeginLatex
//
// This example presents a way to generate LAI (Leaf Area Index) image using formula dedicated to Formosat2.
// LAI Image is used as an input in Image Simulation process.
//
// Let's look at the minimal code required to use this algorithm. First, the
// following headers must be included.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMultiChannelRAndNIRIndexImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbCommandLineArgumentParser.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char *argv[])
{
  if (argc != 7)
    {
    std::cerr << "Wrong Parameters " << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension> InputImageType;
  typedef otb::Image<double, Dimension> OutputImageType;
  typedef otb::Image<unsigned char, Dimension> ImageVisuType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef otb::ImageFileWriter<ImageVisuType> VisuWriterType;
  typedef otb::ImageFileWriter<InputImageType> InWriterType;
  // Software Guide : BeginLatex
  //
  // Filter type is a generic \doxygen{otb}{MultiChannelRAndNIRIndexImageFilter} using Formosat2 specific LAI
  //  \doxygen{otb}{LAIFromNDVIFormosat2Functor}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Functor::LAIFromNDVIFormosat2Functor
      <InputImageType::InternalPixelType,
      InputImageType::InternalPixelType, OutputImageType::PixelType> FunctorType;
  typedef otb::MultiChannelRAndNIRIndexImageFilter
      <InputImageType, OutputImageType, FunctorType>
      MultiChannelRAndNIRIndexImageFilterType;
  // Software Guide : EndCodeSnippet

  // Instantiating object

  //  Software Guide : BeginLatex
  //
  //  Next the filter is created by invoking the \code{New()}~method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MultiChannelRAndNIRIndexImageFilterType::Pointer filter
  = MultiChannelRAndNIRIndexImageFilterType::New();
  // Software Guide : EndCodeSnippet

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  InWriterType::Pointer inWriter = InWriterType::New();
  VisuWriterType::Pointer visuWriter = VisuWriterType::New();

  char * InputName = argv[1];
  char * OutputName1 = argv[2];
  char * OutputName2 = argv[3];
  char * OutputName3 = argv[4];
  reader->SetFileName(InputName);


  //  Software Guide : BeginLatex
  //
  // filter input is set with input image
  //
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // then red and nir channels index are set using \code{SetRedIndex()} and  \code{SetNIRIndex()}
  //
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  unsigned int redChannel = static_cast<unsigned int> (atoi(argv[5]));
  unsigned int nirChannel = static_cast<unsigned int> (atoi(argv[6]));
  filter->SetRedIndex(redChannel);
  filter->SetNIRIndex(nirChannel);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //  The invocation of the \code{Update()} method triggers the
  //  execution of the pipeline.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->Update();
  // Software Guide : EndCodeSnippet


  writer->SetFileName(OutputName1);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  //rescale data

  inWriter->SetFileName(OutputName2);
  //typedef itk::RescaleIntensityImageFilter<InputImageType,
  //     OutputImageType> RescalerType;

   //RescalerType::Pointer     rescaler     = RescalerType::New();
   //rescaler->SetInput(reader->GetOutput());
   //rescaler->SetOutputMinimum(0);
   //rescaler->SetOutputMaximum(255);

  inWriter->SetInput(reader->GetOutput());
  inWriter->Update();


  visuWriter->SetFileName(OutputName3);
  typedef itk::RescaleIntensityImageFilter<OutputImageType,
      ImageVisuType> RescalerTypeOut;

   RescalerTypeOut::Pointer     rescaler     = RescalerTypeOut::New();
   rescaler->SetInput(filter->GetOutput());
   rescaler->SetOutputMinimum(0);
   rescaler->SetOutputMaximum(255);

  visuWriter->SetInput(rescaler->GetOutput());
  visuWriter->Update();

   //  Software Guide : BeginLatex
   // \begin{figure}
   // \center
   // \includegraphics[width=0.44\textwidth]{verySmallFSATSW_visu.eps}
   // \includegraphics[width=0.44\textwidth]{siTvLAIFromNDVIImageTransfromExampleTest_verySmallFSATSW_visu.eps}
   // \itkcaption[LAIFromNDVIImageTransform Filter]{LAI generation \emph{(right)} from NDVI applied on Formosat 2 Image \emph{(left)} .}
   // \label{fig:LAIFromNDVIImageTransform}
   // \end{figure}
   //
   //  Figure \ref{fig:LAIFromNDVIImageTransform} illustrates the LAI generation using Formosat 2 data.
   //
   //
   //  Software Guide : EndLatex


  return EXIT_SUCCESS;
}


