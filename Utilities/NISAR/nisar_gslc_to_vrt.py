#!/usr/bin/env python3
"""Create a georeferenced VRT for a NISAR GSLC layer."""

from __future__ import annotations

import argparse
import html
from pathlib import Path

import h5py


def build_subdataset_string(h5_path: Path, frequency: str, polarization: str) -> str:
    return (
        f'HDF5:"{h5_path}"://science/LSAR/GSLC/grids/'
        f"frequency{frequency}/{polarization}"
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_h5", type=Path)
    parser.add_argument("output_vrt", type=Path)
    parser.add_argument("--frequency", default="A")
    parser.add_argument("--polarization", default="HH")
    args = parser.parse_args()

    frequency = args.frequency.upper()
    polarization = args.polarization.upper()

    with h5py.File(args.input_h5, "r") as h5:
        grid = h5[f"science/LSAR/GSLC/grids/frequency{frequency}"]
        raster = grid[polarization]
        x_coords = grid["xCoordinates"][:]
        y_coords = grid["yCoordinates"][:]
        epsg = int(grid["projection"][()])
        x_spacing = float(grid["xCoordinateSpacing"][()])
        y_spacing = float(grid["yCoordinateSpacing"][()])
        width = int(raster.shape[1])
        height = int(raster.shape[0])

    x_origin = float(x_coords[0] - 0.5 * x_spacing)
    y_origin = float(y_coords[0] - 0.5 * y_spacing)
    geotransform = f"{x_origin}, {x_spacing}, 0.0, {y_origin}, 0.0, {y_spacing}"
    subdataset = build_subdataset_string(args.input_h5.resolve(), frequency, polarization)

    vrt = f"""<VRTDataset rasterXSize="{width}" rasterYSize="{height}">
  <SRS>EPSG:{epsg}</SRS>
  <GeoTransform>{geotransform}</GeoTransform>
  <Metadata>
    <MDI key="NISAR.ProductType">GSLC</MDI>
    <MDI key="NISAR.Frequency">{html.escape(frequency)}</MDI>
    <MDI key="NISAR.Polarization">{html.escape(polarization)}</MDI>
    <MDI key="NISAR.SourceHDF5">{html.escape(str(args.input_h5.resolve()))}</MDI>
  </Metadata>
  <VRTRasterBand dataType="CFloat32" band="1">
    <SimpleSource>
      <SourceFilename relativeToVRT="0">{html.escape(subdataset)}</SourceFilename>
      <SourceBand>1</SourceBand>
      <SourceProperties RasterXSize="{width}" RasterYSize="{height}" DataType="CFloat32" BlockXSize="{width}" BlockYSize="1"/>
      <SrcRect xOff="0" yOff="0" xSize="{width}" ySize="{height}"/>
      <DstRect xOff="0" yOff="0" xSize="{width}" ySize="{height}"/>
    </SimpleSource>
  </VRTRasterBand>
</VRTDataset>
"""

    args.output_vrt.parent.mkdir(parents=True, exist_ok=True)
    args.output_vrt.write_text(vrt, encoding="utf-8")
    print(args.output_vrt)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
