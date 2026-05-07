# NISAR VRT Utilities

This directory contains helper scripts to expose selected NISAR HDF5 products as
OTB-compatible VRT datasets.

These scripts are intended as transitional utilities while NISAR support is added
to the OTB SAR metadata and calibration stack. They are platform-neutral and keep
packaging concerns out of the generated VRT files.

## Dependencies

Use a Python environment with:

- Python 3.9 or newer
- `h5py`

No OTB Python bindings are required to generate the VRT files.

## RSLC / SLC

Create a complex RSLC VRT with embedded OTB SAR metadata:

```bash
python Utilities/NISAR/nisar_to_otb_vrt.py \
  /path/to/NISAR_L1_PR_RSLC_PRODUCT.h5 \
  /tmp/nisar_freqA_HH.vrt \
  --frequency A \
  --polarization HH \
  --keyword-dump /tmp/nisar_freqA_HH.txt \
  --json-summary /tmp/nisar_freqA_HH.json
```

The generated VRT is designed to work with standard OTB applications such as:

```bash
otbcli_ReadImageInfo -in /tmp/nisar_freqA_HH.vrt
otbcli_SARCalibration -in /tmp/nisar_freqA_HH.vrt -out /tmp/nisar_sigma0.tif float -lut sigma -removenoise false
otbcli_OrthoRectification -io.in /tmp/nisar_freqA_HH.vrt -io.out /tmp/nisar_ortho.tif cfloat -map epsg -map.epsg.code 32645 -outputs.mode autosize -elev.dem /path/to/dem
```

## GSLC

Create a georeferenced GSLC VRT:

```bash
python Utilities/NISAR/nisar_gslc_to_vrt.py \
  /path/to/NISAR_L2_GSLC_PRODUCT.h5 \
  /tmp/nisar_gslc_freqA_HH.vrt \
  --frequency A \
  --polarization HH
```

## Notes

- The RSLC VRT serializes OTB metadata with `METADATATYPE=OTB`.
- Calibration lookup tables are written in the `SARCalib.*` namespace and read by
  `otb::NISARCalibrationLookupData`.
- The utilities do not perform phase unwrapping, ionospheric correction, or
  DiapOTB processing.
