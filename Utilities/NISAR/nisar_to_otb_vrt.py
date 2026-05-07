#!/usr/bin/env python3
"""
Build an OTB-compatible VRT wrapper for a NISAR RSLC raster layer.

This tool is intentionally geometry-first:
- it extracts the metadata needed by the OTB SAR geometry chain
- it serializes them as OTB metadata entries inside the VRT
- it also preserves a few NISAR-specific extension keys for later use

Current scope:
- NISAR RSLC / SLC products
- non-burst workflow
- geometry/coregistration oriented metadata

Not covered in this first version:
- sensor-specific DiapOTB deramp metadata beyond extension keys
"""

from __future__ import annotations

import argparse
import bisect
import json
import math
import os
import re
import sys
import xml.etree.ElementTree as ET
from collections import OrderedDict
from dataclasses import dataclass
from datetime import datetime, timedelta, timezone
from pathlib import Path
from typing import Any, Iterable, Optional


SPEED_OF_LIGHT = 299_792_458.0
WGS84_GCP_WKT = (
    'GEOGCS["WGS 84",DATUM["WGS_1984",'
    'SPHEROID["WGS 84",6378137,298.257223563,AUTHORITY["EPSG","7030"]],'
    'AUTHORITY["EPSG","6326"]],PRIMEM["Greenwich",0,AUTHORITY["EPSG","8901"]],'
    'UNIT["degree",0.0174532925199433,AUTHORITY["EPSG","9122"]],'
    'AXIS["Latitude",NORTH],AXIS["Longitude",EAST],AUTHORITY["EPSG","4326"]]'
)


class NISARVRTError(RuntimeError):
    pass


@dataclass
class OrbitRecord:
    time_utc: datetime
    position: tuple[float, float, float]
    velocity: tuple[float, float, float]


@dataclass
class InfoSceneCoord:
    reference_row: int
    reference_col: int
    latitude: float
    longitude: float
    azimuth_time: datetime
    range_time: float
    incidence_angle: float


@dataclass
class GCPEntry:
    ident: str
    row: float
    col: float
    x: float
    y: float
    z: float
    azimuth_time: datetime
    slant_range_time: float


@dataclass
class RasterLayer:
    science_root: str
    product_name: str
    frequency: str
    polarization: str
    dataset_path: str
    width: int
    height: int
    gdal_datatype: str


@dataclass
class RadarGrid:
    slant_range_start_m: float
    slant_range_spacing_m: float
    zero_doppler_start: datetime
    zero_doppler_spacing_s: float


@dataclass
class GeoGrid:
    zero_doppler_time: list[float]
    slant_range: list[float]
    lon: list[list[float]]
    lat: list[list[float]]
    incidence: Optional[list[list[float]]]
    ground_track_velocity: Optional[list[list[float]]]
    height_index: int
    height_value: Optional[float]


@dataclass
class TimeAxis:
    datetimes: list[datetime]
    seconds: list[float]
    units: Optional[str]


@dataclass
class CalibrationGrid:
    zero_doppler_time: list[float]
    slant_range: list[float]
    values: list[list[float]]


@dataclass
class CalibrationMetadata:
    scale_factor: float
    scale_factor_slope: float
    beta0: CalibrationGrid
    gamma0: CalibrationGrid
    sigma0: CalibrationGrid
    noise: Optional[CalibrationGrid]


@dataclass
class DopplerCentroidRecord:
    azimuth_time: datetime
    t0: float
    dop_coef: list[float]
    geo_dop_coef: list[float]


def _import_h5py():
    try:
        import h5py  # type: ignore
    except ImportError as exc:  # pragma: no cover - depends on local env
        raise NISARVRTError(
            "This tool requires the 'h5py' package. "
            "Install it in the runtime used to build the VRT."
        ) from exc
    return h5py


def _decode_scalar(value: Any) -> Any:
    if hasattr(value, "tolist"):
        value = value.tolist()
    if isinstance(value, bytes):
        return value.decode("utf-8")
    if isinstance(value, str):
        return value
    if isinstance(value, (int, float)):
        return value
    if isinstance(value, list) and len(value) == 1:
        return _decode_scalar(value[0])
    return value


def _as_str(value: Any) -> str:
    value = _decode_scalar(value)
    if isinstance(value, bytes):
        return value.decode("utf-8")
    if isinstance(value, list):
        if not value:
            return ""
        return _as_str(value[0])
    return str(value)


def _as_float(value: Any) -> float:
    value = _decode_scalar(value)
    if isinstance(value, list):
        if not value:
            raise NISARVRTError("Cannot convert empty list to float")
        value = value[0]
    return float(value)


def _format_float(value: float) -> str:
    if math.isfinite(value):
        text = f"{value:.20f}".rstrip("0").rstrip(".")
        return text if text else "0"
    raise NISARVRTError(f"Non-finite float cannot be serialized: {value}")


def _format_otb_fixed_number(value: int | float) -> str:
    if math.isfinite(float(value)):
        return f"{float(value):.20f}"
    raise NISARVRTError(f"Non-finite numeric value cannot be serialized: {value}")


def _format_time(value: datetime) -> str:
    value = value.astimezone(timezone.utc)
    text = value.strftime("%Y-%m-%dT%H:%M:%S.%f").rstrip("0").rstrip(".")
    return f"{text}Z"


def _format_time_seconds(value: datetime) -> str:
    value = value.astimezone(timezone.utc).replace(microsecond=0)
    return value.strftime("%Y-%m-%dT%H:%M:%SZ")


def _to_unix_seconds(value: datetime) -> float:
    return value.astimezone(timezone.utc).timestamp()


def _to_mjd_days(value: datetime) -> float:
    return _to_unix_seconds(value) / 86400.0 + 40587.0


def _parse_time(value: Any) -> datetime:
    text = _as_str(value).strip()
    if not text:
        raise NISARVRTError("Empty timestamp")
    text = text.replace(" ", "T")
    match = re.match(r"^(.*?\.\d{6})\d+(.*)$", text)
    if match:
        text = match.group(1) + match.group(2)
    if text.endswith("Z"):
        text = text[:-1] + "+00:00"
    dt = datetime.fromisoformat(text)
    if dt.tzinfo is None:
        dt = dt.replace(tzinfo=timezone.utc)
    return dt.astimezone(timezone.utc)


def _parse_time_units_origin(units: str) -> datetime:
    text = units.strip()
    prefix = "seconds since "
    if not text.lower().startswith(prefix):
        raise NISARVRTError(f"Unsupported time units: {units}")
    origin = text[len(prefix):].strip()
    origin = origin.replace(" ", "T")
    match = re.match(r"^(.*?\.\d{6})\d+(.*)$", origin)
    if match:
        origin = match.group(1) + match.group(2)
    if origin.endswith("Z"):
        origin = origin[:-1] + "+00:00"
    dt = datetime.fromisoformat(origin)
    if dt.tzinfo is None:
        dt = dt.replace(tzinfo=timezone.utc)
    return dt.astimezone(timezone.utc)


def _read_time_axis(node: Any) -> TimeAxis:
    raw = node[()]
    units = node.attrs.get("units")
    if isinstance(units, bytes):
        units = units.decode("utf-8")

    if hasattr(raw, "tolist"):
        raw = raw.tolist()

    if isinstance(raw, list):
        if not raw:
            raise NISARVRTError("Empty time axis")
        first = raw[0]
        if isinstance(first, (bytes, str)):
            datetimes = [_parse_time(v) for v in raw]
            origin = datetimes[0]
            seconds = [(dt - origin).total_seconds() for dt in datetimes]
            return TimeAxis(datetimes=datetimes, seconds=seconds, units=units)
        if isinstance(first, (int, float)):
            if not units:
                raise NISARVRTError("Numeric time axis is missing CF-style units")
            origin = _parse_time_units_origin(str(units))
            seconds_abs = [float(v) for v in raw]
            datetimes = [origin + timedelta(seconds=value) for value in seconds_abs]
            return TimeAxis(datetimes=datetimes, seconds=seconds_abs, units=str(units))

    raise NISARVRTError("Unsupported time axis encoding")


def _read_node(h5: Any, path: str) -> Any:
    if path not in h5:
        raise KeyError(path)
    return h5[path]


def _read_scalar_path(h5: Any, path: str) -> Any:
    node = _read_node(h5, path)
    return node[()]


def _read_first_existing_scalar(h5: Any, paths: Iterable[str]) -> tuple[str, Any]:
    for path in paths:
        if path in h5:
            return path, _read_scalar_path(h5, path)
    raise NISARVRTError(f"No path found among candidates: {list(paths)}")


def _read_optional_scalar(h5: Any, paths: Iterable[str]) -> tuple[Optional[str], Optional[Any]]:
    for path in paths:
        if path in h5:
            return path, _read_scalar_path(h5, path)
    return None, None


def _read_vector_as_float_list(h5: Any, path: str) -> list[float]:
    node = _read_node(h5, path)
    return [float(v) for v in node[()].tolist()]


def _read_2d_plane(node: Any, height_index: int) -> list[list[float]]:
    data = node[()]
    if hasattr(data, "tolist"):
        data = data.tolist()
    if not isinstance(data, list):
        raise NISARVRTError("Expected array-like geolocation grid")
    if not data:
        raise NISARVRTError("Empty geolocation grid")
    if isinstance(data[0][0], list):
        return [[float(v) for v in row] for row in data[height_index]]
    return [[float(v) for v in row] for row in data]


def _read_2d_array(node: Any) -> list[list[float]]:
    data = node[()]
    if hasattr(data, "tolist"):
        data = data.tolist()
    if not isinstance(data, list) or not data or not isinstance(data[0], list):
        raise NISARVRTError("Expected a 2D calibration array")
    return [[float(v) for v in row] for row in data]


def _path_candidates(layer: RasterLayer, product_root: str) -> dict[str, list[str]]:
    freq_root = f"{product_root}/swaths/{layer.frequency}"
    meta_root = f"{product_root}/metadata"
    id_root = f"{layer.science_root}/identification"
    proc_root = f"{meta_root}/processingInformation"
    param_root = f"{proc_root}/parameters"
    return {
        "look_direction": [
            f"{id_root}/lookDirection",
        ],
        "orbit_direction": [
            f"{id_root}/orbitPassDirection",
        ],
        "absolute_orbit_number": [
            f"{id_root}/absoluteOrbitNumber",
        ],
        "zero_doppler_start": [
            f"{id_root}/zeroDopplerStartTime",
            f"{freq_root}/zeroDopplerStartTime",
            f"{param_root}/{layer.frequency}/zeroDopplerStartTime",
        ],
        "zero_doppler_end": [
            f"{id_root}/zeroDopplerEndTime",
            f"{freq_root}/zeroDopplerEndTime",
            f"{param_root}/{layer.frequency}/zeroDopplerEndTime",
        ],
        "zero_doppler_spacing": [
            f"{freq_root}/zeroDopplerTimeSpacing",
            f"{param_root}/{layer.frequency}/zeroDopplerTimeSpacing",
        ],
        "zero_doppler_vector": [
            f"{product_root}/swaths/zeroDopplerTime",
            f"{freq_root}/zeroDopplerTime",
        ],
        "slant_range_vector": [
            f"{freq_root}/slantRange",
        ],
        "slant_range_spacing": [
            f"{freq_root}/slantRangeSpacing",
            f"{param_root}/{layer.frequency}/slantRangeSpacing",
        ],
        "slant_range_start": [
            f"{param_root}/{layer.frequency}/slantRangeStart",
            f"{freq_root}/slantRangeStart",
        ],
        "center_frequency": [
            f"{freq_root}/processedCenterFrequency",
            f"{param_root}/{layer.frequency}/processedCenterFrequency",
            f"{param_root}/{layer.frequency}/centerFrequency",
        ],
        "range_bandwidth": [
            f"{freq_root}/processedRangeBandwidth",
            f"{param_root}/{layer.frequency}/rangeBandwidth",
        ],
        "azimuth_bandwidth": [
            f"{freq_root}/processedAzimuthBandwidth",
            f"{param_root}/{layer.frequency}/azimuthBandwidth",
        ],
        "geolocation_grid": [
            f"{meta_root}/geolocationGrid",
        ],
        "orbit_time": [
            f"{meta_root}/orbit/time",
        ],
        "orbit_position": [
            f"{meta_root}/orbit/position",
        ],
        "orbit_velocity": [
            f"{meta_root}/orbit/velocity",
        ],
        "calibration_scale_factor": [
            f"{meta_root}/calibrationInformation/{layer.frequency}/{layer.polarization}/scaleFactor",
        ],
        "calibration_scale_factor_slope": [
            f"{meta_root}/calibrationInformation/{layer.frequency}/{layer.polarization}/scaleFactorSlope",
        ],
        "calibration_beta0": [
            f"{meta_root}/calibrationInformation/geometry/beta0",
        ],
        "calibration_gamma0": [
            f"{meta_root}/calibrationInformation/geometry/gamma0",
        ],
        "calibration_sigma0": [
            f"{meta_root}/calibrationInformation/geometry/sigma0",
        ],
        "calibration_geometry_slant_range": [
            f"{meta_root}/calibrationInformation/geometry/slantRange",
        ],
        "calibration_geometry_zero_doppler_time": [
            f"{meta_root}/calibrationInformation/geometry/zeroDopplerTime",
        ],
        "calibration_noise": [
            f"{meta_root}/calibrationInformation/{layer.frequency}/noiseEquivalentBackscatter/{layer.polarization}",
        ],
        "calibration_noise_slant_range": [
            f"{meta_root}/calibrationInformation/{layer.frequency}/noiseEquivalentBackscatter/slantRange",
        ],
        "calibration_noise_zero_doppler_time": [
            f"{meta_root}/calibrationInformation/{layer.frequency}/noiseEquivalentBackscatter/zeroDopplerTime",
        ],
        "processing_doppler_centroid": [
            f"{param_root}/{layer.frequency}/dopplerCentroid",
        ],
        "processing_doppler_zero_doppler_time": [
            f"{param_root}/{layer.frequency}/zeroDopplerTime",
            f"{param_root}/zeroDopplerTime",
        ],
        "processing_doppler_slant_range": [
            f"{param_root}/{layer.frequency}/slantRange",
        ],
    }


def _guess_gdal_datatype(dtype_name: str) -> str:
    dtype_name = dtype_name.lower()
    if dtype_name == "complex64":
        return "CFloat32"
    if dtype_name == "complex128":
        return "CFloat64"
    if dtype_name == "float32":
        return "Float32"
    if dtype_name == "float64":
        return "Float64"
    if dtype_name == "uint16":
        return "UInt16"
    if dtype_name == "int16":
        return "Int16"
    if dtype_name == "uint32":
        return "UInt32"
    if dtype_name == "int32":
        return "Int32"
    if dtype_name == "uint8":
        return "Byte"
    raise NISARVRTError(f"Unsupported dataset dtype for VRT generation: {dtype_name}")


def discover_layer(h5: Any, requested_frequency: Optional[str], requested_pol: Optional[str]) -> RasterLayer:
    science_root = None
    for root in ("/science/LSAR", "/science/SSAR"):
        if root in h5:
            science_root = root
            break
    if science_root is None:
        raise NISARVRTError("Could not find /science/LSAR or /science/SSAR in the input product")

    product_name = "RSLC" if f"{science_root}/RSLC" in h5 else "SLC" if f"{science_root}/SLC" in h5 else None
    if product_name is None:
        raise NISARVRTError("Could not find an RSLC or SLC group in the input product")

    product_root = f"{science_root}/{product_name}"
    swaths_root = f"{product_root}/swaths"
    swath_group = h5[swaths_root]
    frequencies = sorted(name for name in swath_group.keys() if name.startswith("frequency"))
    if not frequencies:
        raise NISARVRTError("No frequency groups found in RSLC swaths")

    frequency = requested_frequency or frequencies[0]
    if not frequency.startswith("frequency"):
        frequency = f"frequency{frequency}"
    if frequency not in frequencies:
        raise NISARVRTError(f"Frequency '{frequency}' not found. Available: {frequencies}")

    freq_root = f"{swaths_root}/{frequency}"
    freq_group = h5[freq_root]

    polarizations = []
    list_pol_path = f"{freq_root}/listOfPolarizations"
    if list_pol_path in h5:
        polarizations = [_as_str(v) for v in h5[list_pol_path][()].tolist()]
    else:
        for name, obj in freq_group.items():
            if getattr(obj, "shape", None) and len(obj.shape) == 2:
                polarizations.append(name)
        polarizations.sort()

    if not polarizations:
        raise NISARVRTError(f"No polarization datasets found under {freq_root}")

    polarization = requested_pol or polarizations[0]
    if polarization not in polarizations:
        raise NISARVRTError(f"Polarization '{polarization}' not found. Available: {polarizations}")

    dataset_path = f"{freq_root}/{polarization}"
    ds = h5[dataset_path]
    if len(ds.shape) != 2:
        raise NISARVRTError(f"Expected a 2D raster dataset at {dataset_path}, got shape {ds.shape}")

    height, width = int(ds.shape[0]), int(ds.shape[1])
    dtype_name = str(ds.dtype)
    gdal_datatype = _guess_gdal_datatype(dtype_name)
    return RasterLayer(
        science_root=science_root,
        product_name=product_name,
        frequency=frequency,
        polarization=polarization,
        dataset_path=dataset_path,
        width=width,
        height=height,
        gdal_datatype=gdal_datatype,
    )


def extract_radar_grid(h5: Any, layer: RasterLayer, candidates: dict[str, list[str]]) -> RadarGrid:
    zero_vector = None
    for path in candidates["zero_doppler_vector"]:
        if path in h5:
            zero_vector = _read_time_axis(h5[path]).datetimes
            break
    if zero_vector and len(zero_vector) >= 2:
        zero_start = zero_vector[0]
        zero_spacing_s = (zero_vector[1] - zero_vector[0]).total_seconds()
    else:
        _, zero_start_raw = _read_first_existing_scalar(h5, candidates["zero_doppler_start"])
        _, zero_spacing_raw = _read_first_existing_scalar(h5, candidates["zero_doppler_spacing"])
        zero_start = _parse_time(zero_start_raw)
        zero_spacing_s = float(_as_float(zero_spacing_raw))

    slant_range = None
    for path in candidates["slant_range_vector"]:
        if path in h5:
            slant_range = [float(v) for v in h5[path][()].tolist()]
            break
    if slant_range and len(slant_range) >= 2:
        slant_start = slant_range[0]
        slant_spacing_m = slant_range[1] - slant_range[0]
    else:
        _, slant_start_raw = _read_first_existing_scalar(h5, candidates["slant_range_start"])
        _, slant_spacing_raw = _read_first_existing_scalar(h5, candidates["slant_range_spacing"])
        slant_start = float(_as_float(slant_start_raw))
        slant_spacing_m = float(_as_float(slant_spacing_raw))

    return RadarGrid(
        slant_range_start_m=slant_start,
        slant_range_spacing_m=slant_spacing_m,
        zero_doppler_start=zero_start,
        zero_doppler_spacing_s=zero_spacing_s,
    )


def extract_orbits(h5: Any, candidates: dict[str, list[str]]) -> list[OrbitRecord]:
    time_path, _ = _read_first_existing_scalar(h5, candidates["orbit_time"])
    pos_path, pos_raw = _read_first_existing_scalar(h5, candidates["orbit_position"])
    vel_path, vel_raw = _read_first_existing_scalar(h5, candidates["orbit_velocity"])
    del pos_path, vel_path

    if hasattr(pos_raw, "tolist"):
        pos_raw = pos_raw.tolist()
    if hasattr(vel_raw, "tolist"):
        vel_raw = vel_raw.tolist()

    times = _read_time_axis(h5[time_path]).datetimes
    positions = [tuple(float(x) for x in row) for row in pos_raw]
    velocities = [tuple(float(x) for x in row) for row in vel_raw]
    if not (len(times) == len(positions) == len(velocities)):
        raise NISARVRTError("Orbit time, position, and velocity arrays have inconsistent sizes")

    return [
        OrbitRecord(time_utc=t, position=p, velocity=v)
        for t, p, v in zip(times, positions, velocities)
    ]


def _pick_height_index(height_values: Optional[list[float]], requested_height: float) -> tuple[int, Optional[float]]:
    if not height_values:
        return 0, None
    best = min(range(len(height_values)), key=lambda idx: abs(height_values[idx] - requested_height))
    return best, height_values[best]


def extract_geogrid(
    h5: Any,
    candidates: dict[str, list[str]],
    requested_height: float,
    radar: RadarGrid,
) -> Optional[GeoGrid]:
    geo_root = None
    for path in candidates["geolocation_grid"]:
        if path in h5:
            geo_root = path
            break
    if geo_root is None:
        return None

    zero_node = _read_node(h5, f"{geo_root}/zeroDopplerTime")
    slant_node = _read_node(h5, f"{geo_root}/slantRange")
    # NISAR geolocationGrid follows the usual cartographic convention here:
    # - coordinateX = longitude
    # - coordinateY = latitude
    # This also matches a quick sanity check against the sub-satellite track
    # derived from the orbit state vectors for the real RSLC scene used here.
    lon_node = _read_node(h5, f"{geo_root}/coordinateX")
    lat_node = _read_node(h5, f"{geo_root}/coordinateY")

    height_values = None
    if f"{geo_root}/heightAboveEllipsoid" in h5:
        height_values = [float(v) for v in h5[f"{geo_root}/heightAboveEllipsoid"][()].tolist()]
    height_index, height_value = _pick_height_index(height_values, requested_height)

    incidence = None
    if f"{geo_root}/incidenceAngle" in h5:
        incidence = _read_2d_plane(h5[f"{geo_root}/incidenceAngle"], height_index)

    ground_track_velocity = None
    if f"{geo_root}/groundTrackVelocity" in h5:
        ground_track_velocity = _read_2d_plane(h5[f"{geo_root}/groundTrackVelocity"], height_index)

    zero_axis = _read_time_axis(zero_node)
    zero_doppler_time = [
        (dt - radar.zero_doppler_start).total_seconds()
        for dt in zero_axis.datetimes
    ]

    return GeoGrid(
        zero_doppler_time=zero_doppler_time,
        slant_range=[float(v) for v in slant_node[()].tolist()],
        lon=_read_2d_plane(lon_node, height_index),
        lat=_read_2d_plane(lat_node, height_index),
        incidence=incidence,
        ground_track_velocity=ground_track_velocity,
        height_index=height_index,
        height_value=height_value,
    )


def extract_calibration(h5: Any, candidates: dict[str, list[str]]) -> Optional[CalibrationMetadata]:
    scale_path, scale_raw = _read_optional_scalar(h5, candidates["calibration_scale_factor"])
    if scale_path is None or scale_raw is None:
        return None

    _, scale_slope_raw = _read_optional_scalar(h5, candidates["calibration_scale_factor_slope"])

    def read_grid(value_key: str, slant_key: str, azimuth_key: str) -> CalibrationGrid:
        value_path, _ = _read_first_existing_scalar(h5, candidates[value_key])
        slant_path, _ = _read_first_existing_scalar(h5, candidates[slant_key])
        azimuth_path, _ = _read_first_existing_scalar(h5, candidates[azimuth_key])
        return CalibrationGrid(
            zero_doppler_time=_read_time_axis(h5[azimuth_path]).seconds,
            slant_range=[float(v) for v in h5[slant_path][()].tolist()],
            values=_read_2d_array(h5[value_path]),
        )

    noise = None
    noise_path, _ = _read_optional_scalar(h5, candidates["calibration_noise"])
    if noise_path is not None:
        noise = read_grid(
            "calibration_noise",
            "calibration_noise_slant_range",
            "calibration_noise_zero_doppler_time",
        )

    return CalibrationMetadata(
        scale_factor=float(_as_float(scale_raw)),
        scale_factor_slope=0.0 if scale_slope_raw is None else float(_as_float(scale_slope_raw)),
        beta0=read_grid(
            "calibration_beta0",
            "calibration_geometry_slant_range",
            "calibration_geometry_zero_doppler_time",
        ),
        gamma0=read_grid(
            "calibration_gamma0",
            "calibration_geometry_slant_range",
            "calibration_geometry_zero_doppler_time",
        ),
        sigma0=read_grid(
            "calibration_sigma0",
            "calibration_geometry_slant_range",
            "calibration_geometry_zero_doppler_time",
        ),
        noise=noise,
    )


def extract_doppler_centroids(h5: Any, candidates: dict[str, list[str]]) -> list[DopplerCentroidRecord]:
    doppler_path, _ = _read_optional_scalar(h5, candidates["processing_doppler_centroid"])
    if doppler_path is None:
        return []

    az_path, _ = _read_first_existing_scalar(h5, candidates["processing_doppler_zero_doppler_time"])
    slant_path, _ = _read_first_existing_scalar(h5, candidates["processing_doppler_slant_range"])

    az_axis = _read_time_axis(h5[az_path]).datetimes
    slant_ranges_m = [float(v) for v in h5[slant_path][()].tolist()]
    doppler_values = _read_2d_array(h5[doppler_path])

    if len(doppler_values) != len(az_axis):
        raise NISARVRTError("Doppler centroid LUT row count does not match its azimuth axis")
    if any(len(row) != len(slant_ranges_m) for row in doppler_values):
        raise NISARVRTError("Doppler centroid LUT column count does not match its slant-range axis")
    if len(slant_ranges_m) < 2:
        raise NISARVRTError("Doppler centroid LUT requires at least 2 slant-range samples")

    slant_times = [(2.0 * value) / SPEED_OF_LIGHT for value in slant_ranges_m]
    t0 = slant_times[0]
    dt = slant_times[-1] - slant_times[0]
    inv_dt = 0.0 if dt == 0.0 else 1.0 / dt

    records: list[DopplerCentroidRecord] = []
    for az_time, row in zip(az_axis, doppler_values):
        first = row[0]
        last = row[-1]
        slope = 0.0 if dt == 0.0 else (last - first) * inv_dt
        records.append(
            DopplerCentroidRecord(
                azimuth_time=az_time,
                t0=t0,
                dop_coef=[first, slope],
                geo_dop_coef=[first, slope],
            )
        )
    return records


def _clamp(value: float, low: float, high: float) -> float:
    return max(low, min(high, value))


def _bilinear(grid_x: list[float], grid_y: list[float], values: list[list[float]], x: float, y: float) -> float:
    if len(grid_x) < 2 or len(grid_y) < 2:
        raise NISARVRTError("Geolocation grid must have at least 2 samples per axis")

    x = _clamp(x, grid_x[0], grid_x[-1])
    y = _clamp(y, grid_y[0], grid_y[-1])

    ix = max(0, min(len(grid_x) - 2, bisect.bisect_right(grid_x, x) - 1))
    iy = max(0, min(len(grid_y) - 2, bisect.bisect_right(grid_y, y) - 1))

    x0, x1 = grid_x[ix], grid_x[ix + 1]
    y0, y1 = grid_y[iy], grid_y[iy + 1]
    tx = 0.0 if x1 == x0 else (x - x0) / (x1 - x0)
    ty = 0.0 if y1 == y0 else (y - y0) / (y1 - y0)

    v00 = values[iy][ix]
    v01 = values[iy][ix + 1]
    v10 = values[iy + 1][ix]
    v11 = values[iy + 1][ix + 1]

    return (
        (1.0 - tx) * (1.0 - ty) * v00
        + tx * (1.0 - ty) * v01
        + (1.0 - tx) * ty * v10
        + tx * ty * v11
    )


def _interp_geogrid(geogrid: Optional[GeoGrid], az_time_s: float, slant_range_m: float) -> tuple[float, float, float, Optional[float]]:
    if geogrid is None:
        return 0.0, 0.0, 0.0, None

    lon = _bilinear(geogrid.slant_range, geogrid.zero_doppler_time, geogrid.lon, slant_range_m, az_time_s)
    lat = _bilinear(geogrid.slant_range, geogrid.zero_doppler_time, geogrid.lat, slant_range_m, az_time_s)
    inc = 0.0
    gtv = None
    if geogrid.incidence is not None:
        inc = _bilinear(geogrid.slant_range, geogrid.zero_doppler_time, geogrid.incidence, slant_range_m, az_time_s)
    if geogrid.ground_track_velocity is not None:
        gtv = _bilinear(geogrid.slant_range, geogrid.zero_doppler_time, geogrid.ground_track_velocity, slant_range_m, az_time_s)
    return lon, lat, inc, gtv


def build_scene_coord(radar: RadarGrid, geogrid: Optional[GeoGrid], row: int, col: int) -> InfoSceneCoord:
    az_offset_s = row * radar.zero_doppler_spacing_s
    az_time = radar.zero_doppler_start + timedelta(seconds=az_offset_s)
    slant_range_m = radar.slant_range_start_m + col * radar.slant_range_spacing_m
    lon, lat, inc, _ = _interp_geogrid(
        geogrid,
        az_time_s=az_offset_s,
        slant_range_m=slant_range_m,
    )
    return InfoSceneCoord(
        reference_row=row,
        reference_col=col,
        latitude=lat,
        longitude=lon,
        azimuth_time=az_time,
        range_time=(2.0 * slant_range_m) / SPEED_OF_LIGHT,
        incidence_angle=inc,
    )


def build_gcps(layer: RasterLayer, radar: RadarGrid, geogrid: Optional[GeoGrid], step: int, gcp_height: float) -> list[GCPEntry]:
    step = max(1, step)
    rows = sorted(set([0, layer.height - 1] + list(range(0, layer.height, step))))
    cols = sorted(set([0, layer.width - 1] + list(range(0, layer.width, step))))
    gcps: list[GCPEntry] = []
    idx = 0
    for row in rows:
        for col in cols:
            scene = build_scene_coord(radar, geogrid, row, col)
            gcps.append(
                GCPEntry(
                    ident=str(idx),
                    row=float(row),
                    col=float(col),
                    x=scene.longitude,
                    y=scene.latitude,
                    z=gcp_height,
                    azimuth_time=scene.azimuth_time,
                    slant_range_time=scene.range_time,
                )
            )
            idx += 1
    return gcps


def _haversine_m(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
    r = 6378137.0
    p1 = math.radians(lat1)
    p2 = math.radians(lat2)
    dp = math.radians(lat2 - lat1)
    dl = math.radians(lon2 - lon1)
    a = math.sin(dp / 2.0) ** 2 + math.cos(p1) * math.cos(p2) * math.sin(dl / 2.0) ** 2
    return 2.0 * r * math.asin(math.sqrt(a))


def estimate_line_spacing_m(radar: RadarGrid, layer: RasterLayer, geogrid: Optional[GeoGrid]) -> float:
    center_row = layer.height // 2
    center_col = layer.width // 2
    current = build_scene_coord(radar, geogrid, center_row, center_col)
    next_row = build_scene_coord(radar, geogrid, min(layer.height - 1, center_row + 1), center_col)
    if current.latitude == 0.0 and current.longitude == 0.0 and next_row.latitude == 0.0 and next_row.longitude == 0.0:
        return 0.0
    return _haversine_m(current.latitude, current.longitude, next_row.latitude, next_row.longitude)


def _add_vector_keyword_entries(
    kwl: "OrderedDict[str, str]",
    prefix: str,
    entries: list[dict[str, str]],
    *,
    add_integer_index_aliases: bool = False,
) -> None:
    for idx, entry in enumerate(entries):
        base = f"{prefix}_{_format_otb_fixed_number(idx)}."
        for key, value in entry.items():
            kwl[base + key] = value
        if add_integer_index_aliases:
            # Some OTB/GDAL import paths look up vector entries with integer
            # indices (e.g. SAR.Orbits_0.Time) even though OTB serialization
            # normally uses fixed-precision floating indices. Emit both forms
            # for robustness.
            alias_base = f"{prefix}_{idx}."
            for key, value in entry.items():
                kwl[alias_base + key] = value
    kwl[f"{prefix}.number"] = _format_otb_fixed_number(len(entries))


def _flatten_grid(values: list[list[float]]) -> list[float]:
    return [float(value) for row in values for value in row]


def _format_space_separated(values: list[float]) -> str:
    return " ".join(_format_otb_fixed_number(value) for value in values)


def _add_nisar_lookup_data(
    kwl: "OrderedDict[str, str]",
    prefix: str,
    type_id: int,
    image_first_azimuth_time: float,
    image_azimuth_time_spacing: float,
    image_first_slant_range: float,
    image_slant_range_spacing: float,
    grid: CalibrationGrid,
) -> None:
    flat_values = _flatten_grid(grid.values)
    if len(grid.values) != len(grid.zero_doppler_time):
        raise NISARVRTError("Calibration LUT row count does not match its zero-Doppler axis")
    if any(len(row) != len(grid.slant_range) for row in grid.values):
        raise NISARVRTError("Calibration LUT column count does not match its slant-range axis")

    kwl[f"{prefix}CalibrationLookupData_{type_id}_Sensor"] = "NISAR"
    kwl[f"{prefix}CalibrationLookupData_{type_id}_Type"] = str(type_id)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_ImageFirstAzimuthTime"] = _format_otb_fixed_number(image_first_azimuth_time)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_ImageAzimuthTimeSpacing"] = _format_otb_fixed_number(image_azimuth_time_spacing)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_ImageFirstSlantRange"] = _format_otb_fixed_number(image_first_slant_range)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_ImageSlantRangeSpacing"] = _format_otb_fixed_number(image_slant_range_spacing)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_LUTAzimuthTimes"] = _format_space_separated(grid.zero_doppler_time)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_LUTSlantRanges"] = _format_space_separated(grid.slant_range)
    kwl[f"{prefix}CalibrationLookupData_{type_id}_LUTHeight"] = str(len(grid.zero_doppler_time))
    kwl[f"{prefix}CalibrationLookupData_{type_id}_LUTWidth"] = str(len(grid.slant_range))
    kwl[f"{prefix}CalibrationLookupData_{type_id}_LUTValues"] = _format_space_separated(flat_values)


def _make_identity_grid(grid: CalibrationGrid) -> CalibrationGrid:
    return CalibrationGrid(
        zero_doppler_time=list(grid.zero_doppler_time),
        slant_range=list(grid.slant_range),
        values=[[1.0 for _ in row] for row in grid.values],
    )


def _make_otb_factor_grid(grid: CalibrationGrid) -> CalibrationGrid:
    eps = 1e-12
    return CalibrationGrid(
        zero_doppler_time=list(grid.zero_doppler_time),
        slant_range=list(grid.slant_range),
        values=[
            [1.0 / math.sqrt(max(value, eps)) for value in row]
            for row in grid.values
        ],
    )


def _make_otb_noise_grid(grid: CalibrationGrid, scale_power: float) -> CalibrationGrid:
    return CalibrationGrid(
        zero_doppler_time=list(grid.zero_doppler_time),
        slant_range=list(grid.slant_range),
        values=[
            [scale_power * value for value in row]
            for row in grid.values
        ],
    )


def build_otb_keywordlist(
    layer: RasterLayer,
    radar: RadarGrid,
    orbits: list[OrbitRecord],
    center: InfoSceneCoord,
    ul: InfoSceneCoord,
    ur: InfoSceneCoord,
    lr: InfoSceneCoord,
    ll: InfoSceneCoord,
    gcps: list[GCPEntry],
    look_direction: str,
    orbit_direction: str,
    orbit_number: Optional[int],
    center_frequency_hz: Optional[float],
    range_bandwidth_hz: Optional[float],
    azimuth_bandwidth_hz: Optional[float],
    line_spacing_m: float,
    geogrid: Optional[GeoGrid],
    calibration: Optional[CalibrationMetadata],
    doppler_centroids: Optional[list[DopplerCentroidRecord]] = None,
) -> OrderedDict[str, str]:
    kwl: OrderedDict[str, str] = OrderedDict()

    kwl["METADATATYPE"] = "OTB"
    kwl["OTB_VERSION"] = "NISAR_VRT_WORKAROUND"
    kwl["SAR"] = "<SARParam>"
    kwl["SensorID"] = "NISAR"
    kwl["Mission"] = "NISAR"
    kwl["Instrument"] = layer.science_root.split("/")[-1]
    kwl["ProductType"] = layer.product_name
    kwl["Polarization"] = layer.polarization
    kwl["OrbitDirection"] = orbit_direction.upper()
    kwl["NumberOfLines"] = str(layer.height)
    kwl["NumberOfColumns"] = str(layer.width)
    kwl["AcquisitionStartTime"] = _format_time(radar.zero_doppler_start)
    kwl["AcquisitionStopTime"] = _format_time(
        radar.zero_doppler_start + timedelta(seconds=radar.zero_doppler_spacing_s * max(layer.height - 1, 0))
    )
    kwl["AcquisitionDate"] = kwl["AcquisitionStartTime"]
    kwl["PRF"] = _format_float(1.0 / radar.zero_doppler_spacing_s)
    kwl["RSF"] = _format_float(SPEED_OF_LIGHT / (2.0 * radar.slant_range_spacing_m))
    kwl["PixelSpacing"] = _format_float(radar.slant_range_spacing_m)
    kwl["LineSpacing"] = _format_float(line_spacing_m)
    kwl["RangeTimeFirstPixel"] = _format_float((2.0 * radar.slant_range_start_m) / SPEED_OF_LIGHT)
    kwl["RangeTimeLastPixel"] = _format_float(
        (2.0 * (radar.slant_range_start_m + (layer.width - 1) * radar.slant_range_spacing_m)) / SPEED_OF_LIGHT
    )
    kwl["CenterIncidenceAngle"] = _format_float(center.incidence_angle)
    kwl["RedDisplayChannel"] = "0"
    kwl["GreenDisplayChannel"] = "0"
    kwl["BlueDisplayChannel"] = "0"

    if orbit_number is not None:
        kwl["OrbitNumber"] = str(orbit_number)
    if center_frequency_hz is not None:
        kwl["RadarFrequency"] = _format_float(center_frequency_hz)

    # OTB SAR geometry block
    kwl["SAR.AzimuthTimeInterval"] = _format_float(radar.zero_doppler_spacing_s)
    kwl["SAR.NearRangeTime"] = _format_float((2.0 * radar.slant_range_start_m) / SPEED_OF_LIGHT)
    kwl["SAR.RangeSamplingRate"] = _format_float(SPEED_OF_LIGHT / (2.0 * radar.slant_range_spacing_m))
    kwl["SAR.RangeResolution"] = _format_float(radar.slant_range_spacing_m)
    kwl["SAR.NumberOfLinesPerBurst"] = str(layer.height)
    kwl["SAR.NumberOfSamplesPerBurst"] = str(layer.width)
    kwl["SAR.AzimuthBandwidth"] = _format_float(azimuth_bandwidth_hz or 0.0)
    kwl["SAR.RangeBandwidth"] = _format_float(range_bandwidth_hz or 0.0)
    kwl["SAR.AzimuthSteeringRate"] = "0"
    kwl["SAR.RightLookingFlag"] = "1" if look_direction.upper() == "RIGHT" else "0"

    def add_scene(prefix: str, scene: InfoSceneCoord) -> None:
        kwl[prefix + "AzimuthTime"] = _format_time(scene.azimuth_time)
        kwl[prefix + "referenceRow"] = str(scene.reference_row)
        kwl[prefix + "referenceColumn"] = str(scene.reference_col)
        kwl[prefix + "latitude"] = _format_float(scene.latitude)
        kwl[prefix + "longitude"] = _format_float(scene.longitude)
        kwl[prefix + "rangeTime"] = _format_float(scene.range_time)
        kwl[prefix + "incidenceAngle"] = _format_float(scene.incidence_angle)

    add_scene("SAR.CenterSceneCoord", center)
    add_scene("SAR.ulSceneCoord", ul)
    add_scene("SAR.urSceneCoord", ur)
    add_scene("SAR.lrSceneCoord", lr)
    add_scene("SAR.llSceneCoord", ll)

    orbit_entries = []
    for orbit in orbits:
        orbit_entries.append(
            {
                "Time": _format_time(orbit.time_utc),
                "Position": " ".join(_format_float(v) for v in orbit.position),
                "Velocity": " ".join(_format_float(v) for v in orbit.velocity),
            }
        )
    _add_vector_keyword_entries(kwl, "SAR.Orbits", orbit_entries, add_integer_index_aliases=True)

    # OTB SAR geometry expects at least one burst record, even for non-bursted
    # SLC products. For NISAR RSLC, model the whole image as a single burst.
    burst_entries = [
        {
            "AzimuthStartTime": _format_time(radar.zero_doppler_start),
            "AzimuthStopTime": _format_time(
                radar.zero_doppler_start + timedelta(seconds=radar.zero_doppler_spacing_s * max(layer.height - 1, 0))
            ),
            "StartLine": "0",
            "EndLine": str(layer.height - 1),
            "StartSample": "0",
            "EndSample": str(layer.width - 1),
            "AzimuthAnxTime": _format_float(0.0),
        }
    ]
    _add_vector_keyword_entries(kwl, "SAR.BurstRecords", burst_entries, add_integer_index_aliases=True)

    if doppler_centroids is None:
        doppler_centroids = []

    doppler_entries = []
    for record in doppler_centroids:
        doppler_entries.append(
            {
                "AzimuthTime": _format_time(record.azimuth_time),
                "t0": _format_otb_fixed_number(record.t0),
                "DopCoef": _format_space_separated(record.dop_coef),
                "GeoDopCoef": _format_space_separated(record.geo_dop_coef),
            }
        )
    if doppler_entries:
        _add_vector_keyword_entries(kwl, "SAR.DopplerCentroid", doppler_entries, add_integer_index_aliases=True)
    else:
        kwl["SAR.DopplerCentroid.number"] = "0"

    # Keep FM vectors empty in the generic OTB block unless explicit
    # support is available. Preserve raw NISAR extension keys instead.
    kwl["SAR.AzimuthFmRates.number"] = "0"
    kwl["SAR.SlantRangeToGroundRangeRecords.number"] = "0"
    kwl["SAR.GroundRangeToSlantRangeRecords.number"] = "0"

    kwl["SAR.GCPProjection"] = WGS84_GCP_WKT
    gcp_entries = []
    for gcp in gcps:
        gcp_entries.append(
            {
                "Id": gcp.ident,
                "Info": "",
                "Row": _format_float(gcp.row),
                "Col": _format_float(gcp.col),
                "X": _format_float(gcp.x),
                "Y": _format_float(gcp.y),
                "Z": _format_float(gcp.z),
            }
        )
        kwl[f"SAR.GCPTimes_{gcp.ident}"] = f"{_format_time(gcp.azimuth_time)} {_format_float(gcp.slant_range_time)}"
    _add_vector_keyword_entries(kwl, "SAR.GCP", gcp_entries, add_integer_index_aliases=True)

    # NISAR extension keys preserved as ExtraKeys in OTB
    kwl["NISAR.ProductPath"] = layer.dataset_path
    kwl["NISAR.Frequency"] = layer.frequency
    kwl["NISAR.LookDirection"] = look_direction
    kwl["NISAR.OrbitDirection"] = orbit_direction
    kwl["NISAR.SlantRangeStartMeters"] = _format_float(radar.slant_range_start_m)
    kwl["NISAR.SlantRangeSpacingMeters"] = _format_float(radar.slant_range_spacing_m)
    kwl["NISAR.ZeroDopplerStartTime"] = _format_time(radar.zero_doppler_start)
    kwl["NISAR.ZeroDopplerTimeSpacingSeconds"] = _format_float(radar.zero_doppler_spacing_s)
    if geogrid is not None:
        kwl["NISAR.GeoGridHeightIndex"] = str(geogrid.height_index)
        if geogrid.height_value is not None:
            kwl["NISAR.GeoGridHeightValue"] = _format_float(geogrid.height_value)

    if calibration is not None:
        scale_power = calibration.scale_factor * calibration.scale_factor
        image_first_azimuth_time = _to_mjd_days(radar.zero_doppler_start)
        image_azimuth_time_spacing = radar.zero_doppler_spacing_s / 86400.0

        kwl["CalScale"] = _format_float(scale_power)
        kwl["SARCalib"] = "<SARCalib>"
        kwl["SARCalib.CalibrationLookupFlag"] = "1"
        kwl["SARCalib.RescalingFactor"] = "1"
        kwl["SARCalib.CalibrationStartTime"] = _format_time_seconds(radar.zero_doppler_start)
        kwl["SARCalib.CalibrationStopTime"] = _format_time_seconds(
            radar.zero_doppler_start + timedelta(seconds=radar.zero_doppler_spacing_s * max(layer.height - 1, 0))
        )
        kwl["SARCalib.RadiometricCalibrationNoisePolynomialDegree"] = "0 0"
        kwl["SARCalib.RadiometricCalibrationAntennaPatternNewGainPolynomialDegree"] = "0 0"
        kwl["SARCalib.RadiometricCalibrationAntennaPatternOldGainPolynomialDegree"] = "0 0"
        kwl["SARCalib.RadiometricCalibrationIncidenceAnglePolynomialDegree"] = "0 0"
        kwl["SARCalib.RadiometricCalibrationRangeSpreadLossPolynomialDegree"] = "0 0"
        kwl["SARCalib.RadiometricCalibrationNoise"] = "[0, 0] 0;"
        kwl["SARCalib.RadiometricCalibrationAntennaPatternNewGain"] = "[0, 0] 1;"
        kwl["SARCalib.RadiometricCalibrationAntennaPatternOldGain"] = "[0, 0] 1;"
        kwl["SARCalib.RadiometricCalibrationIncidenceAngle"] = "[0, 0] 0;"
        kwl["SARCalib.RadiometricCalibrationRangeSpreadLoss"] = "[0, 0] 1;"

        _add_nisar_lookup_data(
            kwl,
            "SARCalib.",
            0,
            image_first_azimuth_time,
            image_azimuth_time_spacing,
            radar.slant_range_start_m,
            radar.slant_range_spacing_m,
            _make_otb_factor_grid(calibration.sigma0),
        )
        _add_nisar_lookup_data(
            kwl,
            "SARCalib.",
            1,
            image_first_azimuth_time,
            image_azimuth_time_spacing,
            radar.slant_range_start_m,
            radar.slant_range_spacing_m,
            _make_otb_factor_grid(calibration.beta0),
        )
        _add_nisar_lookup_data(
            kwl,
            "SARCalib.",
            2,
            image_first_azimuth_time,
            image_azimuth_time_spacing,
            radar.slant_range_start_m,
            radar.slant_range_spacing_m,
            _make_otb_factor_grid(calibration.gamma0),
        )
        _add_nisar_lookup_data(
            kwl,
            "SARCalib.",
            3,
            image_first_azimuth_time,
            image_azimuth_time_spacing,
            radar.slant_range_start_m,
            radar.slant_range_spacing_m,
            _make_identity_grid(calibration.beta0),
        )
        if calibration.noise is not None:
            _add_nisar_lookup_data(
                kwl,
                "SARCalib.",
                4,
                image_first_azimuth_time,
                image_azimuth_time_spacing,
                radar.slant_range_start_m,
                radar.slant_range_spacing_m,
                _make_otb_noise_grid(calibration.noise, scale_power),
            )

        kwl["NISAR.CalibrationScaleFactor"] = _format_float(calibration.scale_factor)
        kwl["NISAR.CalibrationScaleFactorSlope"] = _format_float(calibration.scale_factor_slope)

    return kwl


def build_vrt_tree(
    input_file: Path,
    output_vrt: Path,
    layer: RasterLayer,
    kwl: OrderedDict[str, str],
    gcps: list[GCPEntry],
) -> ET.ElementTree:
    root = ET.Element("VRTDataset", rasterXSize=str(layer.width), rasterYSize=str(layer.height))

    metadata = ET.SubElement(root, "Metadata")
    for key, value in kwl.items():
        mdi = ET.SubElement(metadata, "MDI", key=key)
        mdi.text = value

    if gcps:
        gcp_list = ET.SubElement(root, "GCPList", Projection=WGS84_GCP_WKT)
        for gcp in gcps:
            ET.SubElement(
                gcp_list,
                "GCP",
                Id=gcp.ident,
                Pixel=_format_float(gcp.col),
                Line=_format_float(gcp.row),
                X=_format_float(gcp.x),
                Y=_format_float(gcp.y),
                Z=_format_float(gcp.z),
            )

    band = ET.SubElement(root, "VRTRasterBand", dataType=layer.gdal_datatype, band="1")
    source = ET.SubElement(band, "SimpleSource")
    source_filename = ET.SubElement(source, "SourceFilename", relativeToVRT="0")
    source_filename.text = f'HDF5:"{input_file.resolve()}"://{layer.dataset_path.lstrip("/")}'
    source_band = ET.SubElement(source, "SourceBand")
    source_band.text = "1"
    ET.SubElement(
        source,
        "SourceProperties",
        RasterXSize=str(layer.width),
        RasterYSize=str(layer.height),
        DataType=layer.gdal_datatype,
    )
    ET.SubElement(source, "SrcRect", xOff="0", yOff="0", xSize=str(layer.width), ySize=str(layer.height))
    ET.SubElement(source, "DstRect", xOff="0", yOff="0", xSize=str(layer.width), ySize=str(layer.height))

    return ET.ElementTree(root)


def _indent_xml(elem: ET.Element, level: int = 0) -> None:
    indent = "\n" + level * "  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = indent + "  "
        for child in elem:
            _indent_xml(child, level + 1)
        if not child.tail or not child.tail.strip():
            child.tail = indent
    elif level and (not elem.tail or not elem.tail.strip()):
        elem.tail = indent


def write_keyword_dump(path: Path, kwl: OrderedDict[str, str]) -> None:
    with path.open("w", encoding="utf-8") as stream:
        for key, value in kwl.items():
            stream.write(f"{key}={value}\n")


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Build an OTB-compatible VRT for a NISAR RSLC layer.")
    parser.add_argument("input_h5", help="Input NISAR HDF5 product")
    parser.add_argument("output_vrt", help="Output VRT file")
    parser.add_argument("--frequency", help="Frequency group, e.g. frequencyA or A")
    parser.add_argument("--polarization", help="Polarization dataset, e.g. HH")
    parser.add_argument(
        "--reference-height",
        type=float,
        default=0.0,
        help="Requested height above ellipsoid in meters for the geolocation grid slice",
    )
    parser.add_argument(
        "--gcp-step",
        type=int,
        default=1024,
        help="Sparse GCP sampling step in pixels",
    )
    parser.add_argument(
        "--gcp-height",
        type=float,
        default=0.0,
        help="Height assigned to generated GCPs in meters",
    )
    parser.add_argument(
        "--keyword-dump",
        help="Optional text dump of all serialized OTB metadata entries",
    )
    parser.add_argument(
        "--json-summary",
        help="Optional debug JSON summary of the selected raster layer",
    )
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)
    input_h5 = Path(args.input_h5)
    output_vrt = Path(args.output_vrt)
    if not input_h5.exists():
        raise NISARVRTError(f"Input file does not exist: {input_h5}")

    h5py = _import_h5py()
    with h5py.File(input_h5, "r") as h5:
        layer = discover_layer(h5, args.frequency, args.polarization)
        product_root = f"{layer.science_root}/{layer.product_name}"
        candidates = _path_candidates(layer, product_root)
        radar = extract_radar_grid(h5, layer, candidates)
        orbits = extract_orbits(h5, candidates)
        geogrid = extract_geogrid(h5, candidates, args.reference_height, radar)
        calibration = extract_calibration(h5, candidates)
        doppler_centroids = extract_doppler_centroids(h5, candidates)

        _, look_direction_raw = _read_optional_scalar(h5, candidates["look_direction"])
        _, orbit_direction_raw = _read_optional_scalar(h5, candidates["orbit_direction"])
        _, orbit_number_raw = _read_optional_scalar(h5, candidates["absolute_orbit_number"])
        _, center_frequency_raw = _read_optional_scalar(h5, candidates["center_frequency"])
        _, range_bandwidth_raw = _read_optional_scalar(h5, candidates["range_bandwidth"])
        _, azimuth_bandwidth_raw = _read_optional_scalar(h5, candidates["azimuth_bandwidth"])

        look_direction = _as_str(look_direction_raw or "RIGHT").upper()
        orbit_direction = _as_str(orbit_direction_raw or "ASCENDING").upper()
        orbit_number = int(_as_float(orbit_number_raw)) if orbit_number_raw is not None else None
        center_frequency_hz = float(_as_float(center_frequency_raw)) if center_frequency_raw is not None else None
        range_bandwidth_hz = float(_as_float(range_bandwidth_raw)) if range_bandwidth_raw is not None else None
        azimuth_bandwidth_hz = float(_as_float(azimuth_bandwidth_raw)) if azimuth_bandwidth_raw is not None else None

        center = build_scene_coord(radar, geogrid, layer.height // 2, layer.width // 2)
        ul = build_scene_coord(radar, geogrid, 0, 0)
        ur = build_scene_coord(radar, geogrid, 0, layer.width - 1)
        lr = build_scene_coord(radar, geogrid, layer.height - 1, layer.width - 1)
        ll = build_scene_coord(radar, geogrid, layer.height - 1, 0)
        gcps = build_gcps(layer, radar, geogrid, args.gcp_step, args.gcp_height)
        line_spacing_m = estimate_line_spacing_m(radar, layer, geogrid)

        kwl = build_otb_keywordlist(
            layer=layer,
            radar=radar,
            orbits=orbits,
            center=center,
            ul=ul,
            ur=ur,
            lr=lr,
            ll=ll,
            gcps=gcps,
            look_direction=look_direction,
            orbit_direction=orbit_direction,
            orbit_number=orbit_number,
            center_frequency_hz=center_frequency_hz,
            range_bandwidth_hz=range_bandwidth_hz,
            azimuth_bandwidth_hz=azimuth_bandwidth_hz,
        line_spacing_m=line_spacing_m,
        geogrid=geogrid,
        calibration=calibration,
        doppler_centroids=doppler_centroids,
    )

        tree = build_vrt_tree(input_h5, output_vrt, layer, kwl, gcps)
        _indent_xml(tree.getroot())
        output_vrt.parent.mkdir(parents=True, exist_ok=True)
        tree.write(output_vrt, encoding="utf-8", xml_declaration=True)

        if args.keyword_dump:
            write_keyword_dump(Path(args.keyword_dump), kwl)

        if args.json_summary:
            summary = {
                "layer": {
                    "science_root": layer.science_root,
                    "product_name": layer.product_name,
                    "frequency": layer.frequency,
                    "polarization": layer.polarization,
                    "dataset_path": layer.dataset_path,
                    "width": layer.width,
                    "height": layer.height,
                    "gdal_datatype": layer.gdal_datatype,
                },
                "radar_grid": {
                    "slant_range_start_m": radar.slant_range_start_m,
                    "slant_range_spacing_m": radar.slant_range_spacing_m,
                    "zero_doppler_start": _format_time(radar.zero_doppler_start),
                    "zero_doppler_spacing_s": radar.zero_doppler_spacing_s,
                },
                "geogrid": None if geogrid is None else {
                    "height_index": geogrid.height_index,
                    "height_value": geogrid.height_value,
                },
                "calibration": None if calibration is None else {
                    "scale_factor": calibration.scale_factor,
                    "scale_factor_slope": calibration.scale_factor_slope,
                    "has_noise_lut": calibration.noise is not None,
                    "geometry_lut_shape": [
                        len(calibration.sigma0.zero_doppler_time),
                        len(calibration.sigma0.slant_range),
                    ],
                },
                "gcp_count": len(gcps),
            }
            Path(args.json_summary).write_text(json.dumps(summary, indent=2), encoding="utf-8")

    print(f"Wrote OTB-compatible VRT: {output_vrt}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main(sys.argv[1:]))
    except NISARVRTError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(2)
