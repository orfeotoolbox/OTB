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
//    INPUTS: {msPyrMRToMS_IKO_Halles_4_2_sf_full.tif}, {ROI_IKO_PAN_LesHalles.tif}
//    OUTPUTS: {outputPyrSegmented.tif}
//    0 0.9 0.9 10
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \subdoxygen{otb}{MorphologicalPyramid}{Segmenter}. This class performs
// the segmentation of a detail image extracted from a morphological
// pyramid analysis. The Segmentation is perfomed using the
// \doxygen{itk}{ConnectedThresholdImageFilter}. The seeds are
// extracted from the image using the
// \doxygen{otb}{ImageToPointSetFilter}. The thresolds are set by
// using quantiles computed with the HistogramGenerator.
//
// The first step required to use this filter is to include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMorphologicalPyramidSegmenter.h"
// Software Guide : EndCodeSnippet

#include "itkMacro.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"

int main(int itkNotUsed(argc), char * argv[])
{

  const char*        inputFilename = argv[1];
  const char*        originalFilename = argv[2];
  const char*        outputFilename1 = argv[3];
  const bool         segmentDark = atoi(argv[4]);
  const float        seedsQuantile = atof(argv[5]);
  const float        segmentationQuantile = atof(argv[6]);
  const unsigned int minObjectSize = atoi(argv[7]);

// Software Guide : BeginLatex
//
// As usual, we start by defining the types needed for the pixels, the
// images, the image reader and the image writer. Note that, for this
// example, an RGB image will be created to store the results of the
// segmentation.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef double                       InputPixelType;
  typedef unsigned short               LabelPixelType;
  typedef itk::RGBPixel<unsigned char> RGBPixelType;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef otb::Image<RGBPixelType, 2>           RGBImageType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<RGBImageType>   WriterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We define now the segmenter. Please pay attention to the fact that
// this class belongs to the \code{morphologicalPyramid} namespace.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::MorphologicalPyramid::Segmenter<InputImageType,
      LabelImageType>
  SegmenterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We instantiate the readers which will give us access to the image
// of details produced by the morphological pyramid analysis and the
// original image (before analysis) which is used in order to produce
// segmented regions which are sharper than what would have been
// obtained with the detail image only.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(originalFilename);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We instantiate the segmenter and set its parameters as follows. We
// plug the output of the readers for the details image and the
// original image; we set the boolean variable which controls wether
// the segmented details are bright or dark; we set the quantile used
// to threshold the details image in order to obtain the seed points
// for the segmentation; we set the quantile for setting the threshold
// for the region growing segmentation; and finally, we set the
// minimum size for a segmented region to be kept in the final result.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  SegmenterType::Pointer segmenter = SegmenterType::New();
  segmenter->SetDetailsImage(reader->GetOutput());
  segmenter->SetOriginalImage(reader2->GetOutput());
  segmenter->SetSegmentDarkDetailsBool(segmentDark);
  segmenter->SetSeedsQuantile(seedsQuantile);
  segmenter->SetConnectedThresholdQuantile(segmentationQuantile);
  segmenter->SetMinimumObjectSize(minObjectSize);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The output of the segmenter is an image of integer
// labels, where a label denotes membership of a pixel in a particular
// segmented region. This value is usually coded using 16 bits.
// This format is not practical for visualization, so
// for the purposes of this example, we will convert it to RGB pixels.  RGB
// images have the advantage that they can be saved as a simple png file
// and viewed using any standard image viewer software.  The
// \subdoxygen{itk}{Functor}{ScalarToRGBPixelFunctor} class is a special
// function object designed to hash a scalar value into an
// \doxygen{itk}{RGBPixel}. Plugging this functor into the
// \doxygen{itk}{UnaryFunctorImageFilter} creates an image filter for that
// converts scalar images to RGB images.
//
// Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::Functor::ScalarToRGBPixelFunctor<LabelPixelType>
  ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<LabelImageType,
      RGBImageType,
      ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now plug the final segment of the pipeline by using the
  // color mapper and the image file writer.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  colormapper->SetInput(segmenter->GetOutput());
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(colormapper->GetOutput());
  writer->SetFileName(outputFilename1);
  writer->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
// Figure \ref{fig:PYR_SEGDET} shows the results of the segmentation
// of the image of bright details obtained with the morphological
// pyramid analysis.
// \begin{figure}
// \center
// \includegraphics[width=0.30\textwidth]{ROI_IKO_PAN_LesHalles.eps}
// \includegraphics[width=0.30\textwidth]{msPyrMRToMS_IKO_Halles_4_2_sf_full.eps}
// \includegraphics[width=0.30\textwidth]{outputPyrSegmented.eps}
// \itkcaption[Morphological pyramid analysis]{Morphological pyramid
// segmentation. From left to right: original image, image of bright
// details and result of the sementation.}
// \label{fig:PYR_SEGDET}
// \end{figure}
//
// Software Guide : EndLatex

  return EXIT_SUCCESS;
}
