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
//    INPUTS: {MSLabeledOutput.tif}
//    OUTPUTS: {OBIAI2L2IOutput.tif}, {OBIAI2L2IInputPretty.png}, {OBIAI2L2IOutputPretty.png}
//    1 1108 0
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example shows the basic approach for the transformation of a
//  segmented (labeled) image into a LabelObjectMap and then back to
//  an image. For this matter we will need the following header files
//  which contain the basic classes.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
// Software Guide : EndCodeSnippet
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char * argv[])
{

  if (argc != 8)
    {
    std::cerr << "usage: " << argv[0] <<
    " input output inPretty outPretty conn fg bg" << std::endl;
    exit(1);
    }

  //  Software Guide : BeginLatex
  //
  // The image types are defined using pixel types and
  // dimension. The input image is defined as an \doxygen{otb}{Image}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const int dim                              = 2;
  typedef unsigned short             PixelType;
  typedef otb::Image<PixelType, dim> ImageType;

  typedef itk::LabelObject<PixelType, dim> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>   LabelMapType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // As usual, the reader is instantiated and
  // the input image is set.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Then the binary image is transformed to a collection
  // of label objects. Arguments are:
  // \begin{itemize}
  // \item \code{FullyConnected}: Set whether the connected
  // components are defined strictly by face connectivity or by
  // face+edge+vertex connectivity. Default is FullyConnectedOff.
  // \item \code{InputForegroundValue/OutputBackgroundValue}: Specify the
  // pixel value of input/output of the foreground/background.
  // \end{itemize}
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::BinaryImageToLabelMapFilter<ImageType, LabelMapType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput(reader->GetOutput());
  i2l->SetFullyConnected(atoi(argv[5]));
  i2l->SetInputForegroundValue(atoi(argv[6]));
  i2l->SetOutputBackgroundValue(atoi(argv[7]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Then the inverse process is used to recreate a image of labels.
  // The \doxygen{itk}{LabelMapToLabelImageFilter} converts a
  // LabelMap to a labeled image.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::LabelMapToLabelImageFilter<LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput(i2l->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The output can be passed to a writer. The invocation
  //  of the \code{Update()} method on the writer triggers the
  //  execution of the pipeline.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(l2i->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();
  // Software Guide : EndCodeSnippet

  // Pretty image creation for the printing
  typedef otb::Image<unsigned char,
      dim>
  OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>
  WriterPrettyType;
  typedef itk::RescaleIntensityImageFilter<ImageType,
      OutputPrettyImageType> RescalerType;

  RescalerType::Pointer     rescaler     = RescalerType::New();
  WriterPrettyType::Pointer prettyWriter = WriterPrettyType::New();
  rescaler->SetInput(reader->GetOutput());
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);
  prettyWriter->SetFileName(argv[3]);
  prettyWriter->SetInput(rescaler->GetOutput());
  prettyWriter->Update();

  rescaler->SetInput(l2i->GetOutput());
  prettyWriter->SetFileName(argv[4]);
  prettyWriter->SetInput(rescaler->GetOutput());
  prettyWriter->Update();

  // Software Guide : BeginLatex
  //
  // Figure~\ref{fig:ImageToLabel} shows the effect of transforming an
  // image into a label object map and back to an image
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{OBIAI2L2IInputPretty.eps}
  // \includegraphics[width=0.44\textwidth]{OBIAI2L2IOutputPretty.eps}
  // \itkcaption[Image to Label Object Map]{Transforming an
  // image (left) into a label object map and back to an image (right).}
  // \label{fig:ImageToLabel}
  // \end{figure}
  //  Software Guide : EndLatex

  return 0;
}
