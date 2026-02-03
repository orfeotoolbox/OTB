#!/usr/bin/env python
#
# Copyright (C) 2005-2025 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Prepares the NISAR product to make it compatible with OTB input system, by extracting the datasets from the HDF5 file to TIF files.

import argparse
import json
import logging
import subprocess
from datetime import datetime
from pathlib import Path

import h5py
from osgeo import gdal

logger = logging.getLogger(__name__)

GRIDS_DATASETS = ("/frequencyA/HH", "/frequencyA/VV", "/frequencyA/HV",
                  "/frequencyA/VH", "/frequencyA/RH", "/frequencyA/RV",
                  "/frequencyB/HH", "/frequencyB/VV", "/frequencyB/HV",
                  "/frequencyB/VH", "/frequencyB/RH", "/frequencyB/RV")
SENSOR_MODEL_DATASETS = ("calibrationInformation/geometry/beta0",
                         "calibrationInformation/geometry/gamma0",
                         "calibrationInformation/geometry/sigma0")


def convert_date(input_date: str) -> str:
    """
    Convert a DateTime string from NISAR format to ISO format.
    :param input_date: The date to convert
    :return: The date converted to ISO format
    """
    return datetime.strptime(input_date, "%Y%m%dT%H%M%S").isoformat('T')


def add_product_metadata(nisar_file: Path, outfile: Path) -> None:
    """
    Add the metadata from the HDF5 file to the TIF file.
    :param nisar_file: Path to the HDF NISAR file
    :param outfile: Path to the output TIF file
    """
    # Open the output file
    ds = gdal.Open(str(outfile))
    metadata = ds.GetMetadata()

    # Read the input file
    with h5py.File(str(nisar_file), 'r') as h5file:
        # Load the metadata
        for k, v in h5file.attrs.items():
            metadata[k] = v

    # Read metadata from filename (see product specification)
    splited = str(nisar_file).split("_")
    # 0 SensorID
    metadata["SensorID"] = splited[0]
    # 1 0 Instrument
    match splited[1][0]:
        case "L":
            metadata["Instrument"] = "L-SAR"
        case "S":
            metadata["Instrument"] = "S-SAR"
        case _:
            logger.warning(f"Unable to read metadata 'Instrument'. Unknown key {splited[1][0]}")
    # 1 1 Level
    metadata["ProductLevel"] = int(splited[1][1])
    # 2 Processing type
    match splited[2]:
        case "PR":
            metadata["ProcessingType"] = "Production"
        case "UR":
            metadata["ProcessingType"] = "Urgent Response"
        case "OD":
            metadata["ProcessingType"] = "Science On-Demand"
        case _:
            logger.warning(f"Unable to read metadata 'ProcessingType'. Unknown Processing Type {splited[2]}")
    # 3 Product Identifier
    if splited[3] not in ["GSLC", "RSLC"]:
        logger.warning(f"Unable to read metadata 'ProductType'. Only processes GSLC and RSLC products. {splited[3]} is not accepted.")
    metadata["ProductType"] = splited[3]
    # 4 Cycle number
    metadata["CycleNumber"] = splited[4]
    # 5 Relative orbit
    metadata["RelativeOrbit"] = splited[5]
    # 6 Orbit Direction
    match splited[6]:
        case "A":
            metadata["OrbitDirection"] = "Ascending"
        case "D":
            metadata["OrbitDirection"] = "Descending"
        case _:
            logger.warning(f"Unable to read metadata 'OrbitDirection'. Unknown direction {splited[6]}")
    # 7 Frame number
    metadata["FrameNumber"] = splited[7]
    # 8 Bandwidth Mode Code
    metadata["BandwidthMode"] = splited[8]
    # 9 Polarization
    metadata["PolarizationCode"] = splited[9]
    # 10 Source of data
    metadata["SourceMode"] = splited[10]
    # 11 StartDateTime
    metadata["StartDateTime"] = convert_date(splited[11])
    # 12 EndDateTime
    metadata["EndDateTime"] = convert_date(splited[12])
    # 13 Composite Release Identifier
    metadata["CompositeReleaseIdentifier"] = splited[13]
    # 14 Product accuracy
    metadata["ProductAccuracy"] = splited[14]
    # 15 Coverage Indicator
    metadata["CoverageIndicator"] = splited[15]
    # 16 Location of the Science Data System
    # 17 Product Counter
    metadata["ProductCounter"] = splited[17].split('.')[0]

    # Polarization
    metadata["Polarization"] = metadata["Band"].split('_')[-1]

    # Write the metadata
    ds.SetMetadata(metadata)


def extract_nisar_product(nisar_file: Path) -> None:
    """
    Create a TIF file for each frequency-polarization combination present in the input HDF5 file.
    :param nisar_file: Path to the input HDF5 file
    """
    # Check input is a file
    if not nisar_file.is_file():
        raise IOError("nisar_file must point to a file")
    # Check input is a HDF5 file
    if not nisar_file.suffix == ".h5":
        raise IOError("nisar_file must point to a HDF5 file")
    # Fetch the datasets present in the nisar file
    datasets_json = json.loads(subprocess.check_output(f"gdalinfo -json {nisar_file}", shell=True))
    frequency_datasets = [(k, v) for k, v in datasets_json["metadata"]['SUBDATASETS'].items() if v.endswith(GRIDS_DATASETS)]
    # Extract the Subdatasets
    for dataset in frequency_datasets:
        # NISAR HDF5 products are actually more similar to NetCDFs
        netcdf_dataset = f"{dataset[1].replace('HDF5', 'NETCDF')}"
        # Open the dataset
        ds = gdal.Open(netcdf_dataset)
        if ds is None:
          continue
        # Check presence of projection
        if ds.GetProjectionRef() == '':
            logger.warning(f"The dataset {netcdf_dataset} doesn't contain a projection")
        # Retrieve name of dataset
        mo = netcdf_dataset.replace('/', '_')
        mo = mo.split(":")[2].replace('__', '')
        # Build name of output file (adding frequency and polarization, and .tif extension)
        outfile = nisar_file.with_stem(nisar_file.stem + '_' + '_'.join(mo.split('_')[-2:])).with_suffix(".tif")
        # Extract the dataset
        subprocess.run(f"gdal_translate -of GTiff -ot Float32 -mo Band={mo} {netcdf_dataset} {str(outfile)}", shell=True)
        # Add the metadata
        add_product_metadata(nisar_file, outfile)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog='ExtractNisarProduct',
        description='Prepares the NISAR product to make it compatible with OTB input system, by extracting the datasets from the HDF5 file to TIF files.')
    parser.add_argument('filename')
    args = parser.parse_args()
    extract_nisar_product(Path(args.filename))
