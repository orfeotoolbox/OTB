/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMetaDataEnum_h
#define otbMetaDataEnum_h

namespace otb
{
/** Metadata as double*/
enum class MDNum
{
// generic
  TileHintX,
  TileHintY,
  DataType,
  NoData,
  OrbitNumber,
  NumberOfLines,
  NumberOfColumns,
  AverageSceneHeight,
// Display
  RedDisplayChannel,
  GreenDisplayChannel,
  BlueDisplayChannel,
// optical section
  PhysicalGain,
  PhysicalBias,
  SolarIrradiance,
  SunElevation,
  SunAzimuth,
  SatElevation,
  SatAzimuth,
  SpectralStep,
  SpectralMin,
  SpectralMax,
// SAR section
  CalScale,
  CalFactor,
  PRF,
  RSF,
  RadarFrequency,
  CenterIncidenceAngle,
  RescalingFactor,
  LineSpacing,
  PixelSpacing,
  RangeTimeFirstPixel,
  RangeTimeLastPixel,
  END
};

/** Metadata as std::string */
enum class MDStr
{
  SensorID,
  Mission,
  Instrument,
  InstrumentIndex,
  BandName,
  EnhancedBandName,
  ProductType,
  GeometricLevel,
  RadiometricLevel,
  Polarization,
  Mode,
  Swath,
  OrbitDirection,
  BeamMode,
  BeamSwath,
  AreaOrPoint,
  LayerType,
  MetadataType,
  OtbVersion,
  END
};

/** Metadata as LUT 1D */
enum class MDL1D
{
  SpectralSensitivity,
  END
};

/** Metadata as LUT 2D */
enum class MDL2D
{
  // Sar calibration lut ...
  END
};

/** Metadata as Time */
enum class MDTime
{
  AcquisitionDate,
  ProductionDate,
  AcquisitionStartTime,
  AcquisitionStopTime,
  END
};

enum class MDGeom
{
  ProjectionWKT,  // -> string
  ProjectionEPSG, // -> int
  ProjectionProj, // -> string
  RPC,            // -> RPCParam
  SAR,            // -> SARParam
  SARCalib,       // -> SARCalib
  SensorGeometry, // -> boost::any
  GCP,            // -> GCPParam
  Adjustment,     // -> ?
  END
};
}

#endif
