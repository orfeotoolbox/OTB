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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {amst.png}
//    OUTPUTS: {EdgeDensityOutput.png}, {PrettyEdgeDensityOutput.png}
//    3 30 10 1.0 0.01
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{EdgeDensityImageFilter}. 
// This filter computes a local density of edges on an image and can
// be useful to detect man made objects or urban areas, for
// instance. The filter has been implemented in a generic way, so that
// the way the edges are detected and the way their density is
// computed can be chosen by the user.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbEdgeDensityImageFilter.h"
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We will also include the header files for the edge detector (a
// Canny filter) and the density estimation (a simple count on a
// binary image).
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

#include "itkCannyEdgeDetectionImageFilter.h"
#include "otbBinaryImageDensityFunction.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[] )
{

  const  char *     infname       = argv[1];
  const  char *     outfname      = argv[2];
  const  char *     prettyfilename      = argv[3];
  
  const unsigned int radius       = atoi(argv[4]);


  /*--*/
    
  const   unsigned int        Dimension = 2;
  typedef float               PixelType;

  /** Variables for the canny detector*/
  const PixelType    upperThreshold   = static_cast<PixelType>(atof(argv[5]));
  const PixelType    lowerThreshold   = static_cast<PixelType>(atof(argv[6]));
  const double    variance         = atof(argv[7]);
  const double    maximumError     = atof(argv[8]);


  // Software Guide : BeginLatex
  //
  // As usual, we start by defining the types for the images, the reader
  // and the writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image< PixelType, Dimension >    ImageType;
  typedef otb::ImageFileReader<ImageType>       ReaderType;
  typedef otb::ImageFileWriter<ImageType>       WriterType;
  // Software Guide : BeginCodeSnippet
  // Software Guide : BeginLatex
  //
  // We define now the type for the function which will be used by the
  // edge density filter to estimate this density. Here we choose a
  // function which counts the number of non null pixels per area. The
  // fucntion takes as template the type of the image to be processed.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::BinaryImageDensityFunction<ImageType> CountFunctionType;
    // Software Guide : BeginCodeSnippet
  // Software Guide : BeginLatex
  //
  // These {\em non null pixels} will be the result of an edge
  // detector. We use here the classical Canny edge detector, which is
  // templated over the input and output image types.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>
                                                     CannyDetectorType;

  // Software Guide : BeginCodeSnippet
  // Software Guide : BeginLatex
  //
  // Finally, we can define the type for the edge density filter which
  // takes as template the input image type, the edge detector type,
  // the count fucntion type and the output image type.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::EdgeDensityImageFilter<ImageType, CannyDetectorType,
                     CountFunctionType, ImageType> EdgeDensityFilterType;

  /**Instancitation of an object*/
  EdgeDensityFilterType::Pointer    filter =      EdgeDensityFilterType::New();
  ReaderType::Pointer               reader =      ReaderType::New();
  CannyDetectorType::Pointer        CannyFilter = CannyDetectorType::New();

  /** Set The input*/
  reader->SetFileName(infname);
  filter->SetInput(reader->GetOutput());
  
  /** Update the Canny Filter Information*/
  CannyFilter->SetUpperThreshold(upperThreshold); 
  CannyFilter->SetLowerThreshold(lowerThreshold);
  CannyFilter->SetVariance(variance); 
  CannyFilter->SetMaximumError(maximumError); 

  filter->SetDetector(CannyFilter);

  /** Write the output*/
  WriterType::Pointer          writer = WriterType::New();
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();


  /************* Image for printing **************/

  typedef otb::Image< unsigned char, 2 > OutputImageType;

  typedef itk::RescaleIntensityImageFilter< ImageType, OutputImageType >
    RescalerType;

  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  rescaler->SetInput( filter->GetOutput() );

  typedef otb::ImageFileWriter< OutputImageType > OutputWriterType;
  OutputWriterType::Pointer outwriter = OutputWriterType::New();

  outwriter->SetFileName( prettyfilename );
  outwriter->SetInput( rescaler->GetOutput() );
  outwriter->Update();
  
  
  
  return EXIT_SUCCESS;
}

