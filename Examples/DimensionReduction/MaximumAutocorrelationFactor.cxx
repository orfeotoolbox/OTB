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
#include "otbStreamingImageFileWriter.h"
#include "otbPrintableImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {wv2_cannes_8bands.tif}
//    OUTPUTS: {MAFOutput.tif}, {maf-input.png}, {maf-output.png}
//
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex 
// This example illustrates the class
// \doxygen{otb}{MaximumAutocorrelationFactorImageFilter}, which
// performs a Maximum Autocorrelation Factor transform \cite{nielsen2011kernel}. Like
// PCA, MAF tries to find a set of orthogonal linear transform, but
// the criterion to maximize is the auto-correlation rather than the
// variance.
//
// Auto-correlation is the correlation between the component and a
// unitary shifted version of the component.
// 
// Please note that the inverse transform is not implemented yet.
//
// We start by including the corresponding header file.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
#include "otbMaximumAutocorrelationFactorImageFilter.h"
//  Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{
  char * infname   = argv[1];
  char * outfname  = argv[2];
  char * inpretty  = argv[3];
  char * outpretty = argv[4];
  
  //  Software Guide : BeginLatex
  //  We then define the types for the input image and the
  //  output image.
  //  Software Guide : EndLatex
  
  //  Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<unsigned short, 2> InputImageType;
  typedef otb::VectorImage<double, 2>         OutputImageType;
  //  Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  We can now declare the types for the reader. Since the images
  //  can be vey large, we will force the pipeline to use
  //  streaming. For this purpose, the file writer will be
  //  streamed. This is achieved by using the
  //  \doxygen{otb}{StreamingImageFileWriter} class.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType>    ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType> WriterType;
  //  Software Guide : EndCodeSnippet

  // SoftwareGuide : BeginLatex
  //  The \doxygen{otb}{MultivariateAlterationDetectorImageFilter} is templated over
  //  the type of the input images and the type of the generated change
  //  image.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef otb::MaximumAutocorrelationFactorImageFilter<InputImageType,
                                          OutputImageType> FilterType;
  //  Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  The different elements of the pipeline can now be instantiated.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  FilterType::Pointer filter = FilterType::New();
  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We set the parameters of the different elements of the pipeline.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  reader->SetFileName(infname);
  writer->SetFileName(outfname);
  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We build the pipeline by plugging all the elements together.
  //
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  //  Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  And then we can trigger the pipeline update, as usual.
  //
  //  Software Guide : EndLatex
    
  //  Software Guide : BeginCodeSnippet
  writer->Update();
  //  Software Guide : EndCodeSnippet

  // This is for rendering in software guide
  typedef otb::PrintableImageFilter<InputImageType,InputImageType>   InputPrintFilterType;
  typedef otb::PrintableImageFilter<OutputImageType,OutputImageType> OutputPrintFilterType;
  typedef InputPrintFilterType::OutputImageType                      VisuImageType;
  typedef otb::StreamingImageFileWriter<VisuImageType>               VisuWriterType;

  InputPrintFilterType::Pointer inputPrintFilter = InputPrintFilterType::New();
  OutputPrintFilterType::Pointer outputPrintFilter = OutputPrintFilterType::New();
  VisuWriterType::Pointer inputVisuWriter = VisuWriterType::New();
  VisuWriterType::Pointer outputVisuWriter = VisuWriterType::New();

  inputPrintFilter->SetInput(reader->GetOutput());
  inputPrintFilter->SetChannel(5);
  inputPrintFilter->SetChannel(3);
  inputPrintFilter->SetChannel(2);
  outputPrintFilter->SetInput(filter->GetOutput());
  outputPrintFilter->SetChannel(1);
  outputPrintFilter->SetChannel(2);
  outputPrintFilter->SetChannel(3);

  inputVisuWriter->SetInput(inputPrintFilter->GetOutput());
  outputVisuWriter->SetInput(outputPrintFilter->GetOutput());

  inputVisuWriter->SetFileName(inpretty);
  outputVisuWriter->SetFileName(outpretty);

  inputVisuWriter->Update();
  outputVisuWriter->Update();

  //  Software Guide : BeginLatex 
  // Figure \ref{fig:MAFFIG} shows the
  // results of Maximum Autocorrelation Factor applied to an 8 bands
  // Worldview2 image. 
  // \begin{figure}
  // \center \includegraphics[width=0.32\textwidth]{maf-input.eps}
  // \includegraphics[width=0.32\textwidth]{maf-output.eps}
  // \itkcaption[Maximum Autocorrelation Factor results]{Results of the
  // Maximum Autocorrelation Factor algorithm applied to a 8 bands
  // Worldview2 image (3 first components).}  \label{fig:MAFFIG}
  // \end{figure} 
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
