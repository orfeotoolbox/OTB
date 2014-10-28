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


//  Software Guide : BeginLatex
//
//  Start by including some necessary headers and with the
//  usual \code{main} declaration. Apart from the classical header related to
// image input and output. We need the headers related to the fusion and the
// orthorectification. One header is also required to be able to process
// vector images (the XS one) with the orthorectification.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbOrthoRectificationFilter.h"
#include "otbGenericMapProjection.h"

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "otbStandardFilterWatcher.h"

int main(int argc, char* argv[])
{
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We initialize ossim which is required for the orthorectification and we
  // check that all parameters are provided. Basically, we need:
  // \begin{itemize}
  // \item the name of the input PAN image;
  // \item the name of the input XS image;
  // \item the desired name for the output;
  // \item as the coordinates are given in UTM, we need the UTM zone number;
  // \item of course, we need the UTM coordinates of the final image;
  // \item the size in pixels of the final image;
  // \item and the sampling of the final image.
  // \end{itemize}
  //
  // We check that all those parameters are provided.
  //
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  if (argc != 12)
    {
    std::cout << argv[0] << " <input_pan_filename> <input_xs_filename> ";
    std::cout << "<output_filename> <utm zone> <hemisphere N/S>  ";
    std::cout << "<x_ground_upper_left_corner> <y_ground_upper_left_corner> ";
    std::cout << "<x_Size> <y_Size> ";
    std::cout << "<x_groundSamplingDistance> ";
    std::cout << "<y_groundSamplingDistance "
              << "(negative since origin is upper left)>"
              << std::endl;

    return EXIT_FAILURE;
    }
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We declare the different images, readers and writer:
  //
  //  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::Image<unsigned int, 2>                    ImageType;
  typedef otb::VectorImage<unsigned int, 2>              VectorImageType;
  typedef otb::Image<double, 2>                          DoubleImageType;
  typedef otb::VectorImage<double, 2>                    DoubleVectorImageType;
  typedef otb::ImageFileReader<ImageType>                ReaderType;
  typedef otb::ImageFileReader<VectorImageType>          VectorReaderType;
  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  ReaderType::Pointer       readerPAN = ReaderType::New();
  VectorReaderType::Pointer readerXS = VectorReaderType::New();
  WriterType::Pointer       writer = WriterType::New();

  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);
// Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We declare the projection (here we chose the UTM projection, other choices
  // are possible) and retrieve the paremeters from the command line:
  // \begin{itemize}
  // \item the UTM zone
  // \item the hemisphere
  // \end{itemize}
  //
  //  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::GenericMapProjection<otb::TransformDirection::INVERSE> InverseProjectionType;
  InverseProjectionType::Pointer utmMapProjection = InverseProjectionType::New();
  utmMapProjection->SetWkt("Utm");
  utmMapProjection->SetParameter("Zone", argv[4]);
  utmMapProjection->SetParameter("Hemisphere", argv[5]);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We will need to pass several parameters to the orthorectification
  // concerning the desired output region:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = atoi(argv[8]);
  size[1] = atoi(argv[9]);

  ImageType::SpacingType spacing;
  spacing[0] = atof(argv[10]);
  spacing[1] = atof(argv[11]);

  ImageType::PointType origin;
  origin[0] = strtod(argv[6], NULL);
  origin[1] = strtod(argv[7], NULL);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  // We declare the orthorectification filter. And provide the different
  // parameters:
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::OrthoRectificationFilter<ImageType, DoubleImageType,
      InverseProjectionType>
  OrthoRectifFilterType;

  OrthoRectifFilterType::Pointer orthoRectifPAN =
    OrthoRectifFilterType::New();
  orthoRectifPAN->SetMapProjection(utmMapProjection);

  orthoRectifPAN->SetInput(readerPAN->GetOutput());

  orthoRectifPAN->SetOutputStartIndex(start);
  orthoRectifPAN->SetOutputSize(size);
  orthoRectifPAN->SetOutputSpacing(spacing);
  orthoRectifPAN->SetOutputOrigin(origin);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  // Now we are able to have the orthorectified area from the PAN image. We just
  // have to follow a similar process for the XS image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::OrthoRectificationFilter<VectorImageType,
      DoubleVectorImageType, InverseProjectionType>
  VectorOrthoRectifFilterType;


  VectorOrthoRectifFilterType::Pointer orthoRectifXS =
    VectorOrthoRectifFilterType::New();

  orthoRectifXS->SetMapProjection(utmMapProjection);

  orthoRectifXS->SetInput(readerXS->GetOutput());

  orthoRectifXS->SetOutputStartIndex(start);
  orthoRectifXS->SetOutputSize(size);
  orthoRectifXS->SetOutputSpacing(spacing);
  orthoRectifXS->SetOutputOrigin(origin);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  It's time to declare the fusion filter and set its inputs:
  //
  //  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SimpleRcsPanSharpeningFusionImageFilter
  <DoubleImageType, DoubleVectorImageType, VectorImageType> FusionFilterType;
  FusionFilterType::Pointer fusion = FusionFilterType::New();
  fusion->SetPanInput(orthoRectifPAN->GetOutput());
  fusion->SetXsInput(orthoRectifXS->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  And we can plug it to the writer. To be able to process the images by
  // tiles, we use the \code{SetAutomaticTiledStreaming()} method of the writer.
  // We trigger the pipeline execution with the \code{Update()} method.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetInput(fusion->GetOutput());

  writer->SetAutomaticTiledStreaming();

  otb::StandardFilterWatcher watcher(writer, "OrthoFusion");

  writer->Update();

  return EXIT_SUCCESS;

}
// Software Guide : EndCodeSnippet
