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

#include "otbVectorDataProjectionFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbVectorDataProjectionFilter(int argc, char* argv[])
{

  if (argc < 3)
  {
    std::cout << argv[0] << " <input filename> <output filename> [<output text>]" << std::endl;

    return EXIT_FAILURE;
  }

  typedef otb::VectorData<double> InputVectorDataType;
  typedef otb::VectorData<double> OutputVectorDataType;

  typedef otb::VectorDataFileReader<InputVectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer                      reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  typedef otb::VectorDataProjectionFilter<InputVectorDataType, OutputVectorDataType> VectorDataFilterType;

  VectorDataFilterType::Pointer vectorDataProjection = VectorDataFilterType::New();

  vectorDataProjection->SetInput(reader->GetOutput());

  std::string projectionRefWkt =
      "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, "
      "AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], "
      "UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], "
      "PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], "
      "PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  vectorDataProjection->SetOutputProjectionRef(projectionRefWkt);

  otb::StandardFilterWatcher watcher(vectorDataProjection, "VectorDataProjection");

  typedef otb::VectorDataFileWriter<OutputVectorDataType> VectorDataFileWriterType;
  VectorDataFileWriterType::Pointer                       writer = VectorDataFileWriterType::New();
  writer->SetFileName(argv[2]);
  writer->SetInput(vectorDataProjection->GetOutput());
  writer->Update();

  // Output the tree in a text file
  if (argc >= 4)
  {
    const char*   outfile = argv[3];
    std::ofstream file;
    file.open(outfile);

    InputVectorDataType::Pointer data;
    data = reader->GetOutput();
    file << "Original data" << std::endl;
    file << data;
    file << std::endl << std::endl;

    OutputVectorDataType::Pointer data2;
    data2 = vectorDataProjection->GetOutput();
    file << "Reprojected data" << std::endl;
    file << data2;
    file << std::endl << std::endl;
    file.close();
  }

  return EXIT_SUCCESS;
}
