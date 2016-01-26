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
//    INPUTS: {qb_RoadExtract.tif}
//    OUTPUTS: {qb_blue.tif}, {qb_shiftscale.tif}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  First, we are going to use \doxygen{otb}{VectorImage} instead of the now
// traditionnal \doxygen{otb}{Image}. So we include the required header:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
// We also include some other header which will be useful later. Note that we
// are still using the \doxygen{otb}{Image} in this example for some of the
// output.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "itkShiftScaleImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

int main(int argc, char * argv[])
{
  if (argc != 4)
    {
    std::cerr << "Usage: "
        << argv[0]
        << " <input_filename> <output_extract> <output_shifted_scaled>"
        << std::endl;
    }
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We want to read a multispectral image so we declare the image type and the
  // reader. As we have done in the previous example we get the filename from
  // the command line.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned short int             PixelType;
  typedef otb::VectorImage<PixelType, 2> VectorImageType;

  typedef otb::ImageFileReader<VectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Sometime, you need to process only one spectral band of the image. To get
  // only one of the spectral band we use the
  // \doxygen{otb}{MultiToMonoChannelExtractROI}. The declaration is as usual:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::MultiToMonoChannelExtractROI<PixelType, PixelType>
  ExtractChannelType;
  ExtractChannelType::Pointer extractChannel = ExtractChannelType::New();
  // Software Guide : EndCodeSnippet
  //  Software Guide : BeginLatex
  //
  //  We need to pass the parameters to the filter for the extraction. This
  // filter also allow extracting only a spatial subset of the image. However,
  // we will extract the whole channel in this case.
  //
  // To do that, we need to pass the desired region using the
  // \code{SetExtractionRegion()} (method such as \code{SetStartX},
  // \code{SetSizeX} are also available). We get the region from the reader with
  // the \code{GetLargestPossibleRegion()} method. Before doing that we need to
  // read the metadata from the file: this is done by calling the
  // \code{UpdateOutputInformation()} on the reader's output. The difference with the
  // \code{Update()} is that the pixel array is not allocated (yet !) and reduce
  // the memory usage.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->UpdateOutputInformation();
  extractChannel->SetExtractionRegion(
    reader->GetOutput()->GetLargestPossibleRegion());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We chose the channel number to extract (starting from 1) and we plug the
  // pipeline.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  extractChannel->SetChannel(3);
  extractChannel->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  To output this image, we need a writer. As the output of the
  // \doxygen{otb}{MultiToMonoChannelExtractROI} is a \doxygen{otb}{Image}, we
  // need to template the writer with this type.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::Image<PixelType, 2>                 ImageType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(extractChannel->GetOutput());

  writer->Update();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // After this, we have a one band image that we can process with most OTB
  // filters.
  //
  // In some situation, you may want to apply the same process to all bands of
  // the image. You don't have to extract each band and process them separately.
  // There is several situations:
  //
  // \begin{itemize}
  // \item the filter (or the combination of filters) you want to use are doing
  // operations that are well defined for \doxygen{itk}{VariableLengthVector}
  // (which is the pixel type), then you don't have to do anything special.
  // \item if this is not working, you can look for the equivalent filter
  // specially designed for vector images.
  // \item some of the filter you need to use applies operations undefined for
  // \doxygen{itk}{VariableLengthVector}, then you can use the
  // \doxygen{otb}{PerBandVectorImageFilter} specially designed for this
  // purpose.
  // \end{itemize}
  //
  // Let's see how this filter is working. We chose to apply the
  // \doxygen{itk}{ShiftScaleImageFilter} to each of the spectral band. We start
  // by declaring the filter on a normal \doxygen{otb}{Image}. Note that we
  // don't need to specify any input for this filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::ShiftScaleImageFilter<ImageType, ImageType> ShiftScaleType;
  ShiftScaleType::Pointer shiftScale = ShiftScaleType::New();
  shiftScale->SetScale(0.5);
  shiftScale->SetShift(10);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We declare the \doxygen{otb}{PerBandVectorImageFilter} which has three
  // template: the input image type, the output image type and the filter type
  // to apply to each band.
  //
  // The filter is selected using the \code{SetFilter()} method and the input
  // by the usual \code{SetInput()} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::PerBandVectorImageFilter
  <VectorImageType, VectorImageType, ShiftScaleType> VectorFilterType;
  VectorFilterType::Pointer vectorFilter = VectorFilterType::New();
  vectorFilter->SetFilter(shiftScale);

  vectorFilter->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // Now, we just have to save the image using a writer templated over an
  // \doxygen{otb}{VectorImage}:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<VectorImageType> VectorWriterType;
  VectorWriterType::Pointer writerVector = VectorWriterType::New();

  writerVector->SetFileName(argv[3]);
  writerVector->SetInput(vectorFilter->GetOutput());

  writerVector->Update();

  return EXIT_SUCCESS;
}
// Software Guide : EndCodeSnippet
