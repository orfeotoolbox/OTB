/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


//  Start by including some necessary headers and with the
//  usual \code{main} declaration. Apart from the classical header related to
// image input and output. We need the headers related to the fusion and the
// orthorectification. One header is also required to be able to process
// vector images (the XS one) with the orthorectification.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbGenericRSResampleImageFilter.h"

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "otbStandardFilterWatcher.h"

int main(int argc, char* argv[])
{

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

  if (argc != 12)
  {
    std::cout << argv[0] << " <input_pan_filename> <input_xs_filename> ";
    std::cout << "<output_filename> <utm zone> <hemisphere N/S>  ";
    std::cout << "<x_ground_upper_left_corner> <y_ground_upper_left_corner> ";
    std::cout << "<x_Size> <y_Size> ";
    std::cout << "<x_groundSamplingDistance> ";
    std::cout << "<y_groundSamplingDistance "
              << "(negative since origin is upper left)>" << std::endl;

    return EXIT_FAILURE;
  }

  // We declare the different images, readers and writer:

  using ImageType             = otb::Image<unsigned int, 2>;
  using VectorImageType       = otb::VectorImage<unsigned int, 2>;
  using DoubleImageType       = otb::Image<double, 2>;
  using DoubleVectorImageType = otb::VectorImage<double, 2>;
  using ReaderType            = otb::ImageFileReader<ImageType>;
  using VectorReaderType      = otb::ImageFileReader<VectorImageType>;
  using WriterType            = otb::ImageFileWriter<VectorImageType>;

  ReaderType::Pointer       readerPAN = ReaderType::New();
  VectorReaderType::Pointer readerXS  = VectorReaderType::New();
  WriterType::Pointer       writer    = WriterType::New();

  readerPAN->SetFileName(argv[1]);
  readerXS->SetFileName(argv[2]);
  writer->SetFileName(argv[3]);

  // We declare the projection (here we chose the UTM projection, other choices
  // are possible) and retrieve the parameters from the command line:
  // \begin{itemize}
  // \item the UTM zone
  // \item the hemisphere
  // \end{itemize}

  std::string wkt =
      otb::SpatialReference::FromUTM(atoi(argv[4]), argv[5][0] == 'N' ? otb::SpatialReference::hemisphere::north : otb::SpatialReference::hemisphere::south)
          .ToWkt();

  //  We will need to pass several parameters to the orthorectification
  // concerning the desired output region:

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
  origin[0] = strtod(argv[6], nullptr);
  origin[1] = strtod(argv[7], nullptr);

  // We declare the orthorectification filter. And provide the different
  // parameters:

  using OrthoRectifFilterType = otb::GenericRSResampleImageFilter<ImageType, DoubleImageType>;

  OrthoRectifFilterType::Pointer orthoRectifPAN = OrthoRectifFilterType::New();
  orthoRectifPAN->SetOutputProjectionRef(wkt);

  orthoRectifPAN->SetInput(readerPAN->GetOutput());

  orthoRectifPAN->SetOutputStartIndex(start);
  orthoRectifPAN->SetOutputSize(size);
  orthoRectifPAN->SetOutputSpacing(spacing);
  orthoRectifPAN->SetOutputOrigin(origin);

  // Now we are able to have the orthorectified area from the PAN image. We just
  // have to follow a similar process for the XS image.

  using VectorOrthoRectifFilterType = otb::GenericRSResampleImageFilter<VectorImageType, DoubleVectorImageType>;


  VectorOrthoRectifFilterType::Pointer orthoRectifXS = VectorOrthoRectifFilterType::New();

  orthoRectifXS->SetOutputProjectionRef(wkt);

  orthoRectifXS->SetInput(readerXS->GetOutput());

  orthoRectifXS->SetOutputStartIndex(start);
  orthoRectifXS->SetOutputSize(size);
  orthoRectifXS->SetOutputSpacing(spacing);
  orthoRectifXS->SetOutputOrigin(origin);

  //  It's time to declare the fusion filter and set its inputs:

  using FusionFilterType           = otb::SimpleRcsPanSharpeningFusionImageFilter<DoubleImageType, DoubleVectorImageType, VectorImageType>;
  FusionFilterType::Pointer fusion = FusionFilterType::New();
  fusion->SetPanInput(orthoRectifPAN->GetOutput());
  fusion->SetXsInput(orthoRectifXS->GetOutput());

  //  And we can plug it to the writer. To be able to process the images by
  // tiles, we use the \code{SetAutomaticTiledStreaming()} method of the writer.
  // We trigger the pipeline execution with the \code{Update()} method.

  writer->SetInput(fusion->GetOutput());

  otb::StandardFilterWatcher watcher(writer, "OrthoFusion");

  writer->Update();

  return EXIT_SUCCESS;
}
