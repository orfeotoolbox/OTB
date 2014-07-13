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

// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{otb}{MorphologicalSegmentationFilter}. This filter
// performs a segmentation of the details \code{supFilter} and
// \code{infFilter} extracted with the morphological pyramid. The
// segmentation algorithm used is based on seeds extraction using the
// \doxygen{otb}{ImageToPointSetFilter}, followed by a connected
// threshold segmentation using the
// \doxygen{itk}{ConnectedThresholdImageFilter}. The threshold for seeds
// extraction and segmentation are computed using quantiles. A pre
// processing step is applied by multiplying the full resolution
// brighter details (resp. darker details) with the original image
// (resp. the inverted original image). This perfoms an enhancement of
// the regions contour precision. The details from the pyramid are set
// via the \code{SetBrighterDetails()} and \code{SetDarkerDetails()}
// methods. The brighter and darker details depend on the filter used
// in the pyramid analysis. If the
// \doxygen{otb}{OpeningClosingMorphologicalFilter} filter is used,
// then the brighter details are those from the \code{supFilter} image
// list, whereas if the
// \doxygen{otb}{ClosingOpeningMorphologicalFilter} filter is used,
// the brighter details are those from the \code{infFilter} list. The
// output of the segmentation filter is a single segmentation images
// list, containing first the brighter details segmentation from
// higher scale to lower, and then the darker details in the same
// order. The attention of the user is drawn to the fact that since
// the label filter used internally will deal with a large number of
// labels, the \code{OutputPixelType} is required to be sufficiently
// precise. Unsigned short or Unsigned long would be a good choice,
// unless the user has a very good reason to think that a less precise
// type will be sufficient. The first step to use this filter is to
// include its header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMorphologicalPyramidSegmentationFilter.h"
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The mathematical morphology filters to be used have also to be
// included here, as well as the morphological pyramid analysis filter.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbMorphologicalPyramidAnalysisFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkMacro.h"

int main(int itkNotUsed(argc), char * argv[])
{

  const char*        inputFilename = argv[1];
  const char*        outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];
  const unsigned int numberOfLevels = atoi(argv[4]);
  const double       decimationRatio = atof(argv[5]);
  const float        seedsQuantile = atof(argv[6]);
  const float        segmentationQuantile = atof(argv[7]);
  const unsigned int minObjectSize = atoi(argv[8]);

// Software Guide : BeginLatex
//
// As usual, we start by defining the types for the pixels, the
// images, the reader and the writer. We also define the types needed
// for the morphological pyramid analysis.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef unsigned char  InputPixelType;
  typedef unsigned short OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension>
  StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType,
      InputImageType,
      StructuringElementType>
  OpeningClosingFilterType;
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType,
      InputImageType,
      OpeningClosingFilterType>
  PyramidFilterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now define the type for the
// \doxygen{otb}{MorphologicalPyramidSegmentationFilter} which is
// templated over the input and output image types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::MorphologicalPyramidSegmentationFilter<InputImageType,
      OutputImageType>
  SegmentationFilterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Since the output of the segmentation filter is a  list of images,
// we define an iterator type which will be used to access the
// segmented images.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef SegmentationFilterType::OutputImageListIteratorType
  OutputListIteratorType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The following code snippet shows how to read the input image and
// perform the morphological pyramid analysis.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  PyramidFilterType::Pointer pyramid = PyramidFilterType::New();
  pyramid->SetNumberOfLevels(numberOfLevels);
  pyramid->SetDecimationRatio(decimationRatio);
  pyramid->SetInput(reader->GetOutput());
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now instantiate the segmentation filter and set its
// parameters. As one can see, the \code{SetReferenceImage()} is used to
// pass the original image in order to obtain sharp region
// boundaries. Using the \code{SetBrighterDetails()} and
// \code{SetDarkerDetails()} the output of the analysis is passed to the
// filter. Finally, the parameters for the segmentation are set by
// using the \code{SetSeedsQuantile()},
// \code{SetConnectedThresholdQuantile()} and
// \code{SetMinimumObjectSize()} methods.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  SegmentationFilterType::Pointer segmentation = SegmentationFilterType::New();
  segmentation->SetReferenceImage(reader->GetOutput());
  segmentation->SetBrighterDetails(pyramid->GetSupFilter());
  segmentation->SetDarkerDetails(pyramid->GetInfFilter());
  segmentation->SetSeedsQuantile(seedsQuantile);
  segmentation->SetConnectedThresholdQuantile(segmentationQuantile);
  segmentation->SetMinimumObjectSize(minObjectSize);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The pipeline is executed bu calling the \code{Update()} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  segmentation->Update();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Finally, we get an iterator to the list generated as output for the
// segmentation and we use it to iterate through the list and write
// the images to files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  OutputListIteratorType it = segmentation->GetOutput()->Begin();
  WriterType::Pointer    writer;
  int                    index = 1;
  std::stringstream      oss;
  while (it != segmentation->GetOutput()->End())
    {
    oss << outputFilenamePrefix << index << "." << outputFilenameSuffix;
    writer = WriterType::New();
    writer->SetInput(it.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    std::cout << oss.str() << " file written." << std::endl;
    oss.str("");
    ++index;
    ++it;
    }
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The user will pay attention to the fact that the list contains
// first the brighter details segmentation from
// higher scale to lower, and then the darker details in the same
// order.
//
// Software Guide : EndLatex

  return EXIT_SUCCESS;
}
