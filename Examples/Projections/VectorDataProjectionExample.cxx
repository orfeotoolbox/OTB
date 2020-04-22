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


// Let's assume that you have a KML file (hence in geographical coordinates)
// that you would like to superpose to some image with a specific map projection.
// Of course, you could use the handy ogr2ogr tool to do that, but it won't
// integrate so seamlessly into your OTB application.
//
// You can also suppose that the image on which you want to superpose
// the data is not in a specific map projection but a raw image from a
// particular sensor.  Thanks to OTB, the same code below will be able
// to do the appropriate conversion.
//
// This example demonstrates the use of the
// \doxygen{otb}{VectorDataProjectionFilter}.

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cout << argv[0] << " <input vector filename> <input image name> <output vector filename> " << std::endl;
    return EXIT_FAILURE;
  }

  // Declare the vector data type that you would like to use in your
  // application.

  using InputVectorDataType  = otb::VectorData<double>;
  using OutputVectorDataType = otb::VectorData<double>;

  // Declare and instantiate the vector data reader:
  // \doxygen{otb}{VectorDataFileReader}. The call to the
  // \code{UpdateOutputInformation()} method fill up the header information.

  using VectorDataFileReaderType           = otb::VectorDataFileReader<InputVectorDataType>;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // We need the image only to retrieve its projection information,
  // i.e. map projection or sensor model parameters. Hence, the image
  // pixels won't be read, only the header information using the
  // \code{UpdateOutputInformation()} method.

  using ImageType                      = otb::Image<unsigned short int, 2>;
  using ImageReaderType                = otb::ImageFileReader<ImageType>;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(argv[2]);
  imageReader->UpdateOutputInformation();

  // The \doxygen{otb}{VectorDataProjectionFilter} will do the work of
  // converting the vector data coordinates. It is usually a good idea
  // to use it when you design applications reading or saving vector
  // data.

  using VectorDataFilterType                         = otb::VectorDataProjectionFilter<InputVectorDataType, OutputVectorDataType>;
  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();

  // Information concerning the original projection of the vector data
  // will be automatically retrieved from the metadata. Nothing else
  // is needed from you:

  vectorDataProjection->SetInput(reader->GetOutput());

  // Information about the target projection is retrieved directly from
  // the image:

  vectorDataProjection->SetOutputKeywordList(imageReader->GetOutput()->GetImageKeywordlist());
  vectorDataProjection->SetOutputOrigin(imageReader->GetOutput()->GetOrigin());
  vectorDataProjection->SetOutputSpacing(imageReader->GetOutput()->GetSignedSpacing());
  vectorDataProjection->SetOutputProjectionRef(imageReader->GetOutput()->GetProjectionRef());

  // Finally, the result is saved into a new vector file.

  using VectorDataFileWriterType           = otb::VectorDataFileWriter<OutputVectorDataType>;
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  // It is worth noting that none of this code is specific to the
  // vector data format. Whether you pass a shapefile, or a KML file,
  // the correct driver will be automatically instantiated.

  return EXIT_SUCCESS;
}
