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


// Instead of using \doxygen{otb}{VectorData} to apply projections as
// explained in \ref{sec:VectorDataProjection}, we can also \emph{directly} work
// on OGR data types thanks to \doxygen{otb}{GeometriesProjectionFilter}.
//
// This example demonstrates how to proceed with this alternative set of vector
// data types.

#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"

#include "otbImage.h"
#include "otbImageFileReader.h"

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << argv[0] << " <input vector filename> <input image name> <output vector filename>\n";
    return EXIT_FAILURE;
  }

  // Declare the geometries type that you would like to use in your
  // application. Unlike \doxygen{otb}{VectorData}, \doxygen{otb}{GeometriesSet}
  // is a single type for any kind of geometries set (OGR data source, or OGR
  // layer).

  using InputGeometriesType  = otb::GeometriesSet;
  using OutputGeometriesType = otb::GeometriesSet;

  // First, declare and instantiate the data source
  // \subdoxygen{otb}{ogr}{DataSource}. Then, encapsulate this data source into
  // a \doxygen{otb}{GeometriesSet}.

  otb::ogr::DataSource::Pointer input  = otb::ogr::DataSource::New(argv[1], otb::ogr::DataSource::Modes::Read);
  InputGeometriesType::Pointer  in_set = InputGeometriesType::New(input);

  // We need the image only to retrieve its projection information,
  // i.e. map projection or sensor model parameters. Hence, the image
  // pixels won't be read, only the header information using the
  // \code{UpdateOutputInformation()} method.

  using ImageType                      = otb::Image<unsigned short int, 2>;
  using ImageReaderType                = otb::ImageFileReader<ImageType>;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(argv[2]);
  imageReader->UpdateOutputInformation();

  // The \doxygen{otb}{GeometriesProjectionFilter} will do the work of
  // converting the geometries coordinates. It is usually a good idea
  // to use it when you design applications reading or saving vector
  // data.

  using GeometriesFilterType           = otb::GeometriesProjectionFilter;
  GeometriesFilterType::Pointer filter = GeometriesFilterType::New();

  // Information concerning the original projection of the vector data
  // will be automatically retrieved from the metadata. Nothing else
  // is needed from you:

  filter->SetInput(in_set);

  // Information about the target projection is retrieved directly from
  // the image:

  // necessary for sensors
  filter->SetOutputImageMetadata(imageReader->GetOutput()->GetImageMetadata());
  // necessary for sensors
  filter->SetOutputOrigin(imageReader->GetOutput()->GetOrigin());
  // necessary for sensors
  filter->SetOutputSpacing(imageReader->GetOutput()->GetSignedSpacing());
  // ~ wkt
  filter->SetOutputProjectionRef(imageReader->GetOutput()->GetProjectionRef());

  // Finally, the result is saved into a new vector file.
  // Unlike other OTB filters, \doxygen{otb}{GeometriesProjectionFilter} expects
  // to be given a valid output geometries set where to store the result of its
  // processing -- otherwise the result will be an in-memory data source, and
  // not stored in a file nor a data base.
  //
  // Then, the processing is started by calling \code{Update()}. The actual
  // serialization of the results is guaranteed to be completed when the output
  // geometries set object goes out of scope, or when \code{SyncToDisk} is
  // called.

  otb::ogr::DataSource::Pointer output  = otb::ogr::DataSource::New(argv[3], otb::ogr::DataSource::Modes::Update_LayerCreateOnly);
  OutputGeometriesType::Pointer out_set = OutputGeometriesType::New(output);

  filter->SetOutput(out_set);
  filter->Update();

  // Once again, it is worth noting that none of this code is specific to the
  // vector data format. Whether you pass a shapefile, or a KML file, the
  // correct driver will be automatically instantiated.

  return EXIT_SUCCESS;
}
