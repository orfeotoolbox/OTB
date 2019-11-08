/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbVectorDataExtractROI.h"

#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"

int otbVectorDataExtractROI(int itkNotUsed(argc), char* argv[])
{
  const char* infname  = argv[1];
  const char* outfname = argv[2];

  const long int startX = atoi(argv[3]);
  const long int startY = atoi(argv[4]);
  const long int sizeX  = atoi(argv[5]);
  const long int sizeY  = atoi(argv[6]);

  typedef double Type;

  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataExtractROI<VectorDataType> FilterType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;

  typedef otb::RemoteSensingRegion<Type> TypedRegion;

  /** Instantiation of pointer objects*/
  FilterType::Pointer               filter = FilterType::New();
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  VectorDataWriterType::Pointer     writer = VectorDataWriterType::New();

  /** Edit The cartoRegion*/
  TypedRegion            region;
  TypedRegion::SizeType  size;
  TypedRegion::IndexType index;

  size[0]  = sizeX;
  size[1]  = sizeY;
  index[0] = startX;
  index[1] = startY;

  /*Set the projection */
  std::string projectionRefWkt =
      "PROJCS[\"UTM Zone 31, Northern Hemisphere\", GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563, "
      "AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0], AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]], "
      "UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]], AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST], AUTHORITY[\"EPSG\",\"4326\"]], "
      "PROJECTION[\"Transverse_Mercator\"], PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3], PARAMETER[\"scale_factor\", 0.9996], "
      "PARAMETER[\"false_easting\", 500000], PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";

  region.SetSize(size);
  region.SetOrigin(index);
  region.SetRegionProjection(projectionRefWkt);

  /** */
  reader->SetFileName(infname);

  filter->SetInput(reader->GetOutput());
  filter->SetRegion(region);

  /** Write the shapefile*/
  writer->SetFileName(outfname);
  writer->SetInput(filter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
