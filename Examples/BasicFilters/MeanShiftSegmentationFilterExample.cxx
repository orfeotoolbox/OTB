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
//    INPUTS: {ROI_QB_MUL_1.png}
//    OUTPUTS: {MSLabeledOutput.tif}, {MSClusteredOutput.tif}, {MSLabeledOutput-pretty.png}, {MSClusteredOutput-pretty.png}
//    16 16 100 100 0.1
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  This example demonstrates the use of the
//  \doxygen{otb}{MeanShiftSegmentationFilter} class which implements
//  filtering and clustering using the mean shift algorithm
//  \cite{Comaniciu2002}. For a given pixel, the mean shift will
//  build a set of neighboring pixels within a given spatial radius
//  and a color range. The spatial and color center of this set is
//  then computed and the algorithm iterates with this new spatial and
//  color center. The Mean Shift can be used for edge-preserving
//  smoothing, or for clustering.
//
//  Software Guide : EndLatex

#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"

#include "itkRGBPixel.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

//  Software Guide : BeginLatex
//
//  We start by including the needed header file.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMeanShiftSegmentationFilter.h"
// Software Guide : EndCodeSnippet
int main(int argc, char * argv[])
{
  if (argc != 11)
    {
    std::cerr << "Usage: " << argv[0] << " infname labeledfname clusteredfname labeledpretty clusteredpretty "
        << "spatialRadius rangeRadius minRegionSize maxiter thres" << std::endl;
    return EXIT_FAILURE;
    }

  const char * infname             = argv[1];
  const char * labeledfname        = argv[2];
  const char * clusteredfname      = argv[3];
  const char * labeledpretty       = argv[4];
  const char * clusteredpretty     = argv[5];
  const unsigned int spatialRadius = atoi(argv[6]);
  const double rangeRadius         = atof(argv[7]);
  const unsigned int minRegionSize = atoi(argv[8]);
  const unsigned int maxiter       = atoi(argv[9]);
  const double thres               = atof(argv[10]);

  //  Software Guide : BeginLatex
  //
  //  We start by the classical \code{typedef}s needed for reading and
  //  writing the images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;

  typedef float                        PixelType;
  typedef unsigned int                 LabelPixelType;
  typedef itk::RGBPixel<unsigned char> ColorPixelType;

  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  typedef otb::Image<LabelPixelType, Dimension>  LabelImageType;
  typedef otb::Image<ColorPixelType, Dimension>  RGBImageType;

  typedef otb::ImageFileReader<ImageType>               ReaderType;
  typedef otb::StreamingImageFileWriter<ImageType>      WriterType;
  typedef otb::StreamingImageFileWriter<LabelImageType> LabelWriterType;

  typedef otb::MeanShiftSegmentationFilter<ImageType, LabelImageType, ImageType> FilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We instantiate the filter, the reader, and 2 writers (for the
  //  labeled and clustered images).
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer1 = WriterType::New();
  LabelWriterType::Pointer writer2 = LabelWriterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We set the file names for the reader and the writers:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(infname);
  writer1->SetFileName(clusteredfname);
  writer2->SetFileName(labeledfname);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We can now set the parameters for the filter. There are 3 main
  //  parameters: the spatial radius used for defining the neighborhood,
  //  the range radius used for defining the interval in the color space
  //  and the minimum size for the regions to be kept after clustering.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetSpatialBandwidth(spatialRadius);
  filter->SetRangeBandwidth(rangeRadius);
  filter->SetMinRegionSize(minRegionSize);
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  //
  // Two another parameters can be set  : the maximum iteration number, which defines maximum number of iteration until convergence.
  //  Algorithm iterative scheme will stop if convergence hasn't been reached after the maximum number of iterations.
  //  Threshold parameter defines mean-shift vector convergence value. Algorithm iterative scheme will stop if mean-shift vector is below this threshold or if iteration number reached maximum number of iterations.
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetMaxIterationNumber(maxiter);
  filter->SetThreshold(thres);
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  //
  //  We can now plug the pipeline and run it.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  writer1->SetInput(filter->GetClusteredOutput());
  writer2->SetInput(filter->GetLabelOutput());

  writer1->Update();
  writer2->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Figure~\ref{fig:MeanShiftSegmentationFilter} shows the result of applying the mean shift
  // to a Quickbird image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.40\textwidth]{ROI_QB_MUL_1.eps}
  // \includegraphics[width=0.40\textwidth]{MSClusteredOutput-pretty.eps}
  // \includegraphics[width=0.40\textwidth]{MSLabeledOutput-pretty.eps}
  // \itkcaption[Mean Shift]{From top to bottom and left to right:
  // Original image, image filtered by
  // mean shift after clustering , and labeled image.}
  // \label{fig:MeanShiftSegmentationFilter}
  // \end{figure}
  // Software Guide : EndLatex

  typedef otb::PrintableImageFilter<ImageType> PrintableFilterType;
  PrintableFilterType::Pointer printableImageFilter = PrintableFilterType::New();

  printableImageFilter->SetChannel(1);
  printableImageFilter->SetChannel(2);
  printableImageFilter->SetChannel(3);

  typedef PrintableFilterType::OutputImageType OutputImageType;
  typedef otb::ImageFileWriter<OutputImageType> PrettyWriterType;

  PrettyWriterType::Pointer prettyWriter = PrettyWriterType::New();

  printableImageFilter->SetInput(filter->GetClusteredOutput());
  prettyWriter->SetFileName(clusteredpretty);
  prettyWriter->SetInput(printableImageFilter->GetOutput());
  prettyWriter->Update();

  typedef otb::ImageFileWriter<RGBImageType> LabelRGBWriterType;

  LabelRGBWriterType::Pointer labelRGBWriter = LabelRGBWriterType::New();

  // Label to RGB image
  typedef itk::Functor::ScalarToRGBPixelFunctor<LabelPixelType> FunctorType;
  typedef itk::UnaryFunctorImageFilter<LabelImageType, RGBImageType, FunctorType> ColorLabelFilterType;
  ColorLabelFilterType::Pointer labelToRGB = ColorLabelFilterType::New();

  labelToRGB->SetInput(filter->GetLabelOutput());

  labelRGBWriter->SetFileName(labeledpretty);
  labelRGBWriter->SetInput(labelToRGB->GetOutput());
  labelRGBWriter->Update();

  return EXIT_SUCCESS;
}
