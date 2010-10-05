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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {qb_RoadExtract.tif}
//    OUTPUTS: {RoadExtractBandMath.tif}, {RoadExtractBandMath-pretty.jpg}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example demonstrates the use of the
//  \doxygen{otb}{BandMathImageFilter} class which allows
//  to perform mathematical operation on the input images.
//  This filter is based on the mathematical parser library muParser.
//  The built in functions and operators list is available at:
//  \ref{http://muparser.sourceforge.net/mup_features.html} \\
//  In order to use this filter, at least one input image is to be
// set. An associated variable name can be specified or not by using
// the corresponding SetNthInput method. For the nth input image, if
// no associated variable name has been spefified, a default variable
// name is given by concatenating the letter "b" (for band) and the
// corresponding input index. \\
// Next step is to set the expression according to the variable
// names. For example, in the default case with three input images the
// following expression is valid : "(b1+b2)*b3".
//
//  Software Guide : EndLatex

#include "itkExceptionObject.h"
#include <iostream>

#include "otbImage.h"
#include "otbObjectList.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "itkCastImageFilter.h"
//  Software Guide : BeginLatex
//
// We start by including the needed header file.
// The aim of this example is to compute the NDVI indice
// from a multispecral image and perform a threshold on this 
// indice.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbBandMathImageFilter.h"
// Software Guide : EndCodeSnippet

int main( int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile ";
    std::cerr << " outputPrettyImageFile" << std::endl;
    return EXIT_FAILURE;
    }

//  Software Guide : BeginLatex
//
//  We start by the classical \code{typedef}s needed for reading and
//  writing the images.The \doxygen{otb}{BandMathImageFilter} class
// works with \doxygen{otb}{Image} as input so we need to define additional 
// filters to extract each layer of the multispectral image
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef double                                                        PixelType;
  typedef otb::VectorImage<PixelType, 2>                                InputImageType;
  typedef otb::Image<PixelType, 2>                                      OutputImageType;
  typedef OutputImageType::PixelType                                    VPixelType;
  typedef otb::MultiToMonoChannelExtractROI<VPixelType, VPixelType>     ExtractROIFilterType;
  typedef otb::ObjectList<ExtractROIFilterType>                         ExtractROIFilterListType;
  typedef otb::ImageFileReader<InputImageType>                          ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                         WriterType;
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We can now define the type for the filter:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::BandMathImageFilter<OutputImageType>   FilterType;
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We instantiate the filter, the reader, and the writer:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  FilterType::Pointer filter = FilterType::New();

  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
// Software Guide : EndCodeSnippet

  reader->UpdateOutputInformation();

  
//  Software Guide : BeginLatex
//
//  We need now to extract bands from the \doxygen{otb}{VectorImage}, 
//  it illustrates the use of the \doxygen{otb}{MultiToMonoChannelExtractROI}.
//  Each extracted layer are inputs of the \doxygen{otb}{BandMathImageFilter}:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  unsigned int bandId = 0;
  ExtractROIFilterListType::Pointer ChannelExtractorList = ExtractROIFilterListType::New();
  
  for(unsigned int j = 0; j < reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++j)
    {
    std::ostringstream tmpParserVarName;
    tmpParserVarName << "b" << j+1;

    ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
    extractROIFilter->SetInput(reader->GetOutput());
    extractROIFilter->SetChannel(j+1);
    extractROIFilter->GetOutput()->UpdateOutputInformation();
    ChannelExtractorList->PushBack(extractROIFilter);
    filter->SetNthInput(bandId, ChannelExtractorList->Back()->GetOutput(), tmpParserVarName.str());
    ++bandId;
    }
// Software Guide : EndCodeSnippet


  filter->SetExpression("sign(ndvi(b3,b4) - 0.4)");
  
//  Software Guide : BeginLatex
//
//  Now we can define the mathematical expression to perform on the layers (b1, b2,b3,b4).
//  The filter takes advantage of the parsing capabilities of the muParser library and
//  allows to set the expression as on a digital calculator. \\
//  The expression below returns the sign of the ratio $(NIR-RED)/(NIR+RED) - 0,4$.
//  It uses the built-in function sign(x)  which return -1 if x<0; 1 if x>0:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter->SetExpression("sign((b4-b3)/(b4+b3) - 0.4)");
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// The muParser library offers also the possibility to extended existing built-in functions. For example,
// you can use the OTB expression "ndvi(b3,b4)" with the filter. It will return the same result. 
//
//  Software Guide : EndLatex

//  Software Guide : BeginLatex
//
//  We can now plug the pipeline and run it.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  writer->Update();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// Figure~\ref{fig:BandMathImageFilter} shows the result of the threshold over the NDVI indice
// to a Quickbird image.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{qb_RoadExtract.eps}
// \includegraphics[width=0.45\textwidth]{RoadExtractBandMath-pretty.eps}
// \itkcaption[Band Math]{From left to right:
// Original image, thresholded NDVI indice.}
// \label{fig:BandMathImageFilter}
// \end{figure}
//
// Software Guide : EndLatex

  typedef otb::Image<unsigned char, 2>                                      OutputPrettyImageType;
  typedef otb::ImageFileWriter<OutputPrettyImageType>            StreamingImageFileWriterType;
  typedef itk::CastImageFilter<OutputImageType, OutputPrettyImageType> CastImageFilterType;
  
  StreamingImageFileWriterType::Pointer prettyWriter = StreamingImageFileWriterType::New();
  CastImageFilterType::Pointer caster = CastImageFilterType::New();
  caster->SetInput(filter->GetOutput());
  prettyWriter->SetInput(caster->GetOutput());
  prettyWriter->SetFileName(argv[3]);
  
  prettyWriter->Update();
  
  return EXIT_SUCCESS;
}
